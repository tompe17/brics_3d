/******************************************************************************
 * BRICS_3D - 3D Perception and Modeling Library
 * Copyright (c) 2015, KU Leuven
 *
 * Author: Sebastian Blumenthal
 *
 *
 * This software is published under a dual-license: GNU Lesser General Public
 * License LGPL 2.1 and Modified BSD license. The dual-license implies that
 * users of this code may choose which terms they prefer.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License LGPL and the BSD license for
 * more details.
 *
 ******************************************************************************/

#include "JSONQueryRunner.h"

namespace brics_3d {
namespace rsg {

JSONQueryRunner::JSONQueryRunner(WorldModel* wm) :
		wm(wm) {
	updateOperationRunner = new JSONDeserializer(wm);
}

JSONQueryRunner::~JSONQueryRunner() {
	if(updateOperationRunner) {
		delete updateOperationRunner;
		updateOperationRunner = 0;
	}
}

bool JSONQueryRunner::query(std::string& queryAsJson,
		std::string& resultAsJson) {

	try {

		libvariant::Variant query = libvariant:: Deserialize(queryAsJson, libvariant::SERIALIZE_GUESS); // GUESS seems to be more permissive with parsing than JSON
		libvariant::Variant result;
		bool success = this->query(query, result);

		resultAsJson = libvariant::Serialize(result, libvariant::SERIALIZE_JSON); //could also go to heap (?!?)
		return success;

	} catch (std::exception const & e) {
		LOG(ERROR) << "JSONQueryRunner: Parser error at input query: " << e.what() << std::endl << "Omitting this query.";
		resultAsJson = "{}";
		return false;
	}

}

bool JSONQueryRunner::query(libvariant::Variant& query,
		libvariant::Variant& result) {

	result.Clear();
	result.Set("querySuccess", libvariant::Variant(false)); // defult value; to be overriden
	result.Set("@worldmodeltype", libvariant::Variant("RSGQueryResult"));

	try {

		// Start to parse it
		if(query.Contains("@worldmodeltype")) {

			std::string type = query.Get("@worldmodeltype").AsString();
			if(type.compare("RSGQuery") == 0) {
				LOG(DEBUG) << "JSONQueryRunner: Found a model for a Query.";

				/* The actual query */
				if(query.Contains("query"))  {

					std::string queryOperation = query.Get("query").AsString();
					if(queryOperation.compare("GET_NODES") == 0) {
						return handleGetNodes(query, result);

					} else if (queryOperation.compare("GET_NODE_ATTRIBUTES") == 0) {
						return handleGetNodeAttributes(query, result);

					} else if (queryOperation.compare("GET_NODE_PARENTS") == 0) {
						return handleGetNodeParents(query, result);

					} else if (queryOperation.compare("GET_GROUP_CHILDREN") == 0) {
						return handleGetGroupChildren(query, result);

					} else if (queryOperation.compare("GET_ROOT_NODE") == 0) {
						return handleGetRootNode(query, result);

					} else if (queryOperation.compare("GET_REMOTE_ROOT_NODES") == 0) {
						return handleGetRemoteRootNodes(query, result);

					} else if (queryOperation.compare("GET_TRANSFORM") == 0) {
						return handleGetTransform(query, result);

					} else if (queryOperation.compare("GET_GEOMETRY") == 0) {
						return handleGetGeometry(query, result);

					} else if (queryOperation.compare("GET_CONNECTION_SOURCE_IDS") == 0) {
						return handleGetSourceIds(query, result);

					} else if (queryOperation.compare("GET_CONNECTION_TARGET_IDS") == 0) {
						return handleGetTargetIds(query, result);

					} else {
						LOG(ERROR) << "JSONQueryRunner: Mandatory query field has unknown value = " << queryOperation;
						handleError("Syntax error: Mandatory query field has unknown value in RSGQuery", result);
						return false;
					}

				} else {
					handleError("Syntax error: Mandatory query field not set in RSGQuery", result);
					return false;
				}

			} else if (type.compare("RSGUpdate") == 0) {
				LOG(DEBUG) << "JSONQueryRunner: Found a model for an Update.";
				result.Clear();
				result.Set("updateSuccess", libvariant::Variant(false));
				result.Set("@worldmodeltype", libvariant::Variant("RSGUpdateResult"));
				int error = updateOperationRunner->write(query);
				LOG(DEBUG) <<"JSONQueryRunner: error code for update = " << error;
				if(error > 0) {
					result.Set("updateSuccess", libvariant::Variant(true));
				}
				//NOTE: we dont't have this elaborated error messaging (yet) for RSGUpdate here as compared to the queries.
			} else {
				LOG(ERROR) << "JSONQueryRunner: Syntax error: Mandatory @worldmodeltype field not set in RSGQuery. Instead it is = " << type;
				handleError("Syntax error: Mandatory @worldmodeltype field not set in RSGQuery", result);
				return false;
			}

		} else {
			handleError("Syntax error: Top level model type @worldmodeltype does not exist.", result);
			return false;
		}


		return true;

	} catch (std::exception const & e) {
		LOG(ERROR) << "JSONQueryRunner: Generic parser error: " << e.what() << std::endl << "Omitting this query.";
		handleError("Syntax error: Generic parser error.", result);
		return false;
	}

	return false;
}

bool JSONQueryRunner::handleGetNodes(libvariant::Variant& query,
		libvariant::Variant& result) {

	/* prepare query */
	std::vector<rsg::Attribute> attributes = JSONTypecaster::getAttributesFromJSON(query);
	std::vector<rsg::Id> ids;

	/* perform query */
	bool success = wm->scene.getNodes(attributes, ids);

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_NODES"));
	result.Set("querySuccess", libvariant::Variant(success));
	JSONTypecaster::addIdsToJSON(ids, result, "ids");

	return success;
}

bool JSONQueryRunner::handleGetNodeAttributes(libvariant::Variant& query,
		libvariant::Variant& result) {

	/* prepare query */
	rsg::Id id = JSONTypecaster::getIdFromJSON(query, "id");
	if(id.isNil()) {
		handleError("Syntax error: Wrong or missing id.", result);
		return false;
	}
	std::vector<rsg::Attribute> attributes;

	/* perform query */
	bool success = wm->scene.getNodeAttributes(id, attributes);

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_NODE_ATTRIBUTES"));
	result.Set("querySuccess", libvariant::Variant(success));
	if(success) {
		JSONTypecaster::addAttributesToJSON(attributes, result);
	}


	return success;
}

bool JSONQueryRunner::handleGetNodeParents(libvariant::Variant& query,
		libvariant::Variant& result) {

	/* prepare query */
	rsg::Id id = JSONTypecaster::getIdFromJSON(query, "id");
	if(id.isNil()) {
		handleError("Syntax error: Wrong or missing id.", result);
		return false;
	}
	std::vector<rsg::Id> ids;

	/* perform query */
	bool success = wm->scene.getNodeParents(id, ids);

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_NODE_PARENTS"));
	result.Set("querySuccess", libvariant::Variant(success));
	JSONTypecaster::addIdsToJSON(ids, result, "ids");

	return success;
}

bool JSONQueryRunner::handleGetGroupChildren(libvariant::Variant& query,
		libvariant::Variant& result) {

	/* prepare query */
	rsg::Id id = JSONTypecaster::getIdFromJSON(query, "id");
	if(id.isNil()) {
		handleError("Syntax error: Wrong or missing id.", result);
		return false;
	}
	std::vector<rsg::Id> ids;

	/* perform query */
	bool success = wm->scene.getGroupChildren(id, ids);

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_GROUP_CHILDREN"));
	result.Set("querySuccess", libvariant::Variant(success));
	JSONTypecaster::addIdsToJSON(ids, result, "ids");

	return success;
}

bool JSONQueryRunner::handleGetRootNode(libvariant::Variant& query,
		libvariant::Variant& result) {


	/* perform query */
	bool success = true; // There is always a root node.
	Id rootId = wm->scene.getRootId();

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_ROOT_NODE"));
	result.Set("querySuccess", libvariant::Variant(success));
	JSONTypecaster::addIdToJSON(rootId, result, "rootId");

	return success;
}

bool JSONQueryRunner::handleGetRemoteRootNodes(libvariant::Variant& query,
		libvariant::Variant& result) {

	/* prepare query */
	std::vector<rsg::Id> ids;

	/* perform query */
	bool success = wm->scene.getRemoteRootNodes(ids);

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_REMOTE_ROOT_NODES"));
	result.Set("querySuccess", libvariant::Variant(success));
	JSONTypecaster::addIdsToJSON(ids, result, "ids");

	return success;
}

bool JSONQueryRunner::handleGetTransform(libvariant::Variant& query,
		libvariant::Variant& result) {

	/* prepare query */
	rsg::Id id = JSONTypecaster::getIdFromJSON(query, "id");
	if(id.isNil()) {
		handleError("Syntax error: Wrong or missing id.", result);
		return false;
	}
	rsg::Id idReferenceNode = JSONTypecaster::getIdFromJSON(query, "idReferenceNode");
	rsg::TimeStamp timeStamp = JSONTypecaster::getTimeStampFromJSON(query, "timeStamp");
	HomogeneousMatrix44::IHomogeneousMatrix44Ptr transform(new HomogeneousMatrix44());

	/* perform query */
	bool success = wm->scene.getTransformForNode(id, idReferenceNode, timeStamp, transform);

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_TRANSFORM"));
	result.Set("querySuccess", libvariant::Variant(success));
	if (success) {
		JSONTypecaster::addTransformToJSON(transform, result, "transform");
	}

	return success;
}

bool JSONQueryRunner::handleGetGeometry(libvariant::Variant& query,
		libvariant::Variant& result) {

	/* prepare query */
	rsg::Id id = JSONTypecaster::getIdFromJSON(query, "id");
	if(id.isNil()) {
		handleError("Syntax error: Wrong or missing id.", result);
		return false;
	}
	Shape::ShapePtr shape;
	TimeStamp timeStamp;

	/* perform query */
	bool success = wm->scene.getGeometry(id, shape, timeStamp);

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_GEOMETRY"));
	result.Set("querySuccess", libvariant::Variant(success));
	if(success) {
		JSONTypecaster::addShapeToJSON(shape, result, "geometry");
		result.Set("unit", libvariant::Variant("m"));
		JSONTypecaster::addTimeStampToJSON(timeStamp, result, "timeStamp");
	}

	return success;
}

bool JSONQueryRunner::handleGetSourceIds(libvariant::Variant& query,
		libvariant::Variant& result) {

	/* prepare query */
	rsg::Id id = JSONTypecaster::getIdFromJSON(query, "id");
	if(id.isNil()) {
		handleError("Syntax error: Wrong or missing id.", result);
		return false;
	}
	std::vector<rsg::Id> ids;

	/* perform query */
	bool success = wm->scene.getConnectionSourceIds(id, ids);

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_CONNECTION_SOURCE_IDS"));
	result.Set("querySuccess", libvariant::Variant(success));
	JSONTypecaster::addIdsToJSON(ids, result, "ids");

	return success;
}

bool JSONQueryRunner::handleGetTargetIds(libvariant::Variant& query,
		libvariant::Variant& result) {

	/* prepare query */
	rsg::Id id = JSONTypecaster::getIdFromJSON(query, "id");
	if(id.isNil()) {
		handleError("Syntax error: Wrong or missing id.", result);
		return false;
	}
	std::vector<rsg::Id> ids;

	/* perform query */
	bool success = wm->scene.getConnectionTargetIds(id, ids);

	/* set up result message */
	result.Set("query", libvariant::Variant("GET_CONNECTION_TARGET_IDS"));
	result.Set("querySuccess", libvariant::Variant(success));
	JSONTypecaster::addIdsToJSON(ids, result, "ids");

	return success;
}

void JSONQueryRunner::handleError(std::string message, libvariant::Variant& result) {
	LOG(ERROR) << "JSONQueryRunner::handleError: " << message;
	result.Clear();
	libvariant::Variant error;
	error.Set("message", libvariant::Variant(message));
	result.Set("error", error);
}

} /* namespace rsg */
} /* namespace brics_3d */

/* EOF */