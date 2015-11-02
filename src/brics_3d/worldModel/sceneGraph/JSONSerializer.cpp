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

#include "JSONSerializer.h"
#include "brics_3d/util/JSONTypecaster.h"

namespace brics_3d {
namespace rsg {

JSONSerializer::JSONSerializer(IOutputPort* port) {
	this->port = port;

	/* some default values */
	storeMessageBackupsOnFileSystem = false;

	std::stringstream instanceBasedSuffix; //in case multiple JSON files  with the same derived file name are created.
	instanceBasedSuffix << this; // pointer as "name"
	fileSuffix = "-rsg-" + instanceBasedSuffix.str() + ".json";
}

JSONSerializer::~JSONSerializer() {
}

bool JSONSerializer::addNode(Id parentId, Id& assignedId,
		vector<Attribute> attributes, bool forcedId) {


	LOG(DEBUG) << "JSONSerializer: adding a Node-" << assignedId.toString();
	try {
		std::string fileName = "Node-" + assignedId.toString() + fileSuffix;

		/* header */
		libvariant::Variant graphUpdate;
		graphUpdate.Set("@worldmodeltype", libvariant::Variant("RSGUpdate"));
		graphUpdate.Set("operation", libvariant::Variant("CREATE"));
		JSONTypecaster::addIdToJSON(parentId, graphUpdate, "parentId");

		/* the actual graph primitive */
		libvariant::Variant node;
		node.Set("@graphtype", libvariant::Variant("Node"));
		JSONTypecaster::addIdToJSON(assignedId, node, "id");
		JSONTypecaster::addAttributesToJSON(attributes, node);

		/* assebmle it */
		graphUpdate.Set("node", node);

		/* send it */
		return doSendMessage(graphUpdate);

	} catch (std::exception e) {
		LOG(ERROR) << "JSONSerializer addGroup: Cannot create a JSON serialization. Exception = " << std::endl << e.what();
		return false;
	}

	return false;
}

bool JSONSerializer::addGroup(Id parentId, Id& assignedId,
		vector<Attribute> attributes, bool forcedId) {

	LOG(DEBUG) << "JSONSerializer: adding a Group-" << assignedId.toString();
	try {
		std::string fileName = "Group-" + assignedId.toString() + fileSuffix;

		/* header */
		libvariant::Variant graphUpdate;
		graphUpdate.Set("@worldmodeltype", libvariant::Variant("RSGUpdate"));
		graphUpdate.Set("operation", libvariant::Variant("CREATE"));
		JSONTypecaster::addIdToJSON(parentId, graphUpdate, "parentId");

		/* the actual graph primitive */
		libvariant::Variant node;
		node.Set("@graphtype", libvariant::Variant("Group"));
		JSONTypecaster::addIdToJSON(assignedId, node, "id");
		JSONTypecaster::addAttributesToJSON(attributes, node);

		/* assebmle it */
		graphUpdate.Set("node", node);

		/* send it */
		return doSendMessage(graphUpdate);

	} catch (std::exception e) {
		LOG(ERROR) << "JSONSerializer addGroup: Cannot create a JSON serialization. Exception = " << std::endl << e.what();
		return false;
	}

	return false;
}

bool JSONSerializer::addTransformNode(Id parentId,
		Id& assignedId, vector<Attribute> attributes,
		IHomogeneousMatrix44::IHomogeneousMatrix44Ptr transform,
		TimeStamp timeStamp, bool forcedId) {

	return false;
}

bool JSONSerializer::addUncertainTransformNode(Id parentId,
		Id& assignedId, vector<Attribute> attributes,
		IHomogeneousMatrix44::IHomogeneousMatrix44Ptr transform,
		ITransformUncertainty::ITransformUncertaintyPtr uncertainty,
		TimeStamp timeStamp, bool forcedId) {


	return false;
}

bool JSONSerializer::addGeometricNode(Id parentId,
		Id& assignedId, vector<Attribute> attributes, Shape::ShapePtr shape,
		TimeStamp timeStamp, bool forcedId) {

	return false;
}

bool JSONSerializer::addRemoteRootNode(Id rootId,
		vector<Attribute> attributes) {

	return false;
}

bool JSONSerializer::addConnection(Id parentId, Id& assignedId,
		vector<Attribute> attributes, vector<Id> sourceIds,
		vector<Id> targetIds, TimeStamp start, TimeStamp end, bool forcedId) {


	return false;
}

bool JSONSerializer::setNodeAttributes(Id id,
		vector<Attribute> newAttributes, TimeStamp timeStamp) {

	LOG(DEBUG) << "JSONSerializer: : updating Attributes for node " << id.toString();
	try {
		std::string fileName = "Attribute-Update-" + id.toString() + fileSuffix;

		/* header */
		libvariant::Variant graphUpdate;
		graphUpdate.Set("@worldmodeltype", libvariant::Variant("RSGUpdate"));
		graphUpdate.Set("operation", libvariant::Variant("UPDATE_ATTRIBUTES"));

		/* the actual graph primitive */
		libvariant::Variant node;
		node.Set("@graphtype", libvariant::Variant("Node")); // this is actually a dummy, thoug reqired for correce validation
		JSONTypecaster::addIdToJSON(id, node, "id");
		JSONTypecaster::addAttributesToJSON(newAttributes, node);
		// TODO implement addJSONTypecaster::addTimeStampToJSON(timeStamp, node, "attributesTimeStamp");

		/* assebmle it */
		graphUpdate.Set("node", node);

		/* send it */
		return doSendMessage(graphUpdate);

	} catch (std::exception e) {
		LOG(ERROR) << "JSONSerializer setNodeAttributes: Cannot create a JSON serialization. Exception = " << std::endl << e.what();
		return false;
	}

	return false;
}

bool JSONSerializer::setTransform(Id id,
		IHomogeneousMatrix44::IHomogeneousMatrix44Ptr transform,
		TimeStamp timeStamp) {


	return false;
}

bool JSONSerializer::setUncertainTransform(Id id,
		IHomogeneousMatrix44::IHomogeneousMatrix44Ptr transform,
		ITransformUncertainty::ITransformUncertaintyPtr uncertainty,
		TimeStamp timeStamp) {


	return false;
}

bool JSONSerializer::deleteNode(Id id) {


	return false;
}

bool JSONSerializer::addParent(Id id, Id parentId) {


	return false;
}

bool JSONSerializer::removeParent(Id id, Id parentId) {


	return false;
}

bool JSONSerializer::doSendMessage(libvariant::Variant& message) {

	string messageAsString = libvariant::Serialize(message, libvariant::SERIALIZE_JSON); //could also go to heap (?!?)
	return doSendMessage(messageAsString);
}

bool JSONSerializer::doSendMessage(std::string& message) {
	int transferredBytes = 0;
	int returnValue = -1;

	if(port != 0) {
		returnValue = port->write(message.c_str(), message.size(), transferredBytes);
	} else {
		LOG(WARNING) << "JSONSerializer: message could not be send as the output port is not specified: " << std::endl << message;
	}
	LOG(DEBUG) << "JSONSerializer: \t" << transferredBytes << " bytes transferred.";

	return (returnValue >= 0);
}

} /* namespace rsg */
} /* namespace brics_3d */

/* EOF */
