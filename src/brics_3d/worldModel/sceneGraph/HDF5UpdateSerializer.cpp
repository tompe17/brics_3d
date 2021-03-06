/******************************************************************************
 * BRICS_3D - 3D Perception and Modeling Library
 * Copyright (c) 2014, KU Leuven
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

#include "HDF5UpdateSerializer.h"
#include <fstream>

namespace brics_3d {
namespace rsg {


HDF5UpdateSerializer::HDF5UpdateSerializer(IOutputPort* port) :
		port(port) {

	/* some default values */
	storeMessageBackupsOnFileSystem = false;
	fileImageIncremet = 1*sizeof(char);

	std::stringstream instanceBasedSuffix; //in case multiple HDF5 files  with the same derived file name are created.
	instanceBasedSuffix << this; // pointer as "name"
//	fileSuffix = "-rsg.h5";
	fileSuffix = "-rsg-" + instanceBasedSuffix.str() + ".h5";

}

HDF5UpdateSerializer::~HDF5UpdateSerializer() {

}

bool HDF5UpdateSerializer::addNode(Id parentId, Id& assignedId,
		vector<Attribute> attributes, bool forcedId) {
	LOG(DEBUG) << "HDF5UpdateSerializer: adding a Node-" << assignedId.toString();
	try {
		std::string fileName = "Node-" + assignedId.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::ADD, scene);
		HDF5Typecaster::addNodeIdToHDF5Group(parentId, scene, rsgParentIdName);

		H5::Group group = scene.createGroup("Node-" + assignedId.toString()); // The actual data
		HDF5Typecaster::addNodeTypeInfoToHDF5Group(HDF5Typecaster::NODE, group);
		HDF5Typecaster::addNodeIdToHDF5Group(assignedId, group);
		HDF5Typecaster::addAttributesToHDF5Group(attributes, group);

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();


	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer addNode: Cannot create a HDF serialization.";
		return false;
	}

	return true;
}

bool HDF5UpdateSerializer::addGroup(Id parentId, Id& assignedId,
		vector<Attribute> attributes, bool forcedId) {
	LOG(DEBUG) << "HDF5UpdateSerializer: adding a Group-" << assignedId.toString();
	try {
		std::string fileName = "Group-" + assignedId.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::ADD, scene);
		HDF5Typecaster::addNodeIdToHDF5Group(parentId, scene, rsgParentIdName);

		H5::Group group = scene.createGroup("Group-" + assignedId.toString()); // The actual data
		HDF5Typecaster::addNodeTypeInfoToHDF5Group(HDF5Typecaster::GROUP, group);
		HDF5Typecaster::addNodeIdToHDF5Group(assignedId, group);
		HDF5Typecaster::addAttributesToHDF5Group(attributes, group);

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();
//		doSendMessage(fileName);

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer addGroup: Cannot create a HDF serialization.";
		return false;
	}

	return true;
}

bool HDF5UpdateSerializer::addTransformNode(Id parentId,
		Id& assignedId, vector<Attribute> attributes,
		IHomogeneousMatrix44::IHomogeneousMatrix44Ptr transform,
		TimeStamp timeStamp, bool forcedId) {

	LOG(DEBUG) << "HDF5UpdateSerializer: adding a Transform-" << assignedId.toString();
	try {
		std::string fileName = "Transform-" + assignedId.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::ADD, scene);
		HDF5Typecaster::addNodeIdToHDF5Group(parentId, scene, rsgParentIdName);

		H5::Group group = scene.createGroup("Transform-" + assignedId.toString()); // The actual data
		HDF5Typecaster::addNodeTypeInfoToHDF5Group(HDF5Typecaster::TRANSFORM, group);
		HDF5Typecaster::addNodeIdToHDF5Group(assignedId, group);
		HDF5Typecaster::addAttributesToHDF5Group(attributes, group);
		HDF5Typecaster::addTransformToHDF5Group(transform, timeStamp, group);

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();
//		doSendMessage(fileName);

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer addTransformNode: Cannot create a HDF serialization.";
		return false;
	}
	return true;
}

bool HDF5UpdateSerializer::addUncertainTransformNode(Id parentId,
		Id& assignedId, vector<Attribute> attributes,
		IHomogeneousMatrix44::IHomogeneousMatrix44Ptr transform,
		ITransformUncertainty::ITransformUncertaintyPtr uncertainty,
		TimeStamp timeStamp, bool forcedId) {
	LOG(ERROR) << "HDF5UpdateSerializer: functionality not yet implemented.";
	return false;
}

bool HDF5UpdateSerializer::addGeometricNode(Id parentId,
		Id& assignedId, vector<Attribute> attributes, Shape::ShapePtr shape,
		TimeStamp timeStamp, bool forcedId) {

	LOG(DEBUG) << "HDF5UpdateSerializer: adding a GeometricNode-" << assignedId.toString();
	try {
		std::string fileName = "GeometricNode-" + assignedId.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::ADD, scene);
		HDF5Typecaster::addNodeIdToHDF5Group(parentId, scene, rsgParentIdName);

		H5::Group group = scene.createGroup("GeometricNode-" + assignedId.toString()); // The actual data
		HDF5Typecaster::addNodeTypeInfoToHDF5Group(HDF5Typecaster::GEOMETIRC_NODE, group);
		HDF5Typecaster::addNodeIdToHDF5Group(assignedId, group);
		HDF5Typecaster::addAttributesToHDF5Group(attributes, group);
		HDF5Typecaster::addShapeToHDF5Group(shape, group);
		HDF5Typecaster::addTimeStampToHDF5Group(timeStamp, group);

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();
//		doSendMessage(fileName);

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer addTransformNode: Cannot create a HDF serialization.";
		return false;
	}
	return true;
}

bool HDF5UpdateSerializer::addRemoteRootNode(Id rootId, vector<Attribute> attributes) {
	LOG(DEBUG) << "HDF5UpdateSerializer: adding a RemoteNode-" << rootId.toString();
	try {
		std::string fileName = "RemoteRootNode-" + rootId.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::ADD_REMOTE_ROOT_NODE, scene);

		H5::Group group = scene.createGroup("RemoteRootNode-" + rootId.toString()); // The actual data
		HDF5Typecaster::addNodeIdToHDF5Group(rootId, group);
		HDF5Typecaster::addAttributesToHDF5Group(attributes, group);

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer addRemoteRootNode: Cannot create a HDF serialization.";
		return false;
	}

	return true;
}

bool HDF5UpdateSerializer::addConnection(Id parentId, Id& assignedId, vector<Attribute> attributes, vector<Id> sourceIds, vector<Id> targetIds, TimeStamp start, TimeStamp end, bool forcedId) {
	LOG(DEBUG) << "HDF5UpdateSerializer: adding a Connection-" << assignedId.toString();
	try {
		std::string fileName = "Connection-" + assignedId.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::ADD, scene);
		HDF5Typecaster::addNodeIdToHDF5Group(parentId, scene, rsgParentIdName);

		H5::Group group = scene.createGroup("Connection-" + assignedId.toString()); // The actual data
		HDF5Typecaster::addNodeTypeInfoToHDF5Group(HDF5Typecaster::CONNECTION, group);
		HDF5Typecaster::addNodeIdToHDF5Group(assignedId, group);
		HDF5Typecaster::addAttributesToHDF5Group(attributes, group);

		/* Connection specific payload: Id sets and time stamps */
		HDF5Typecaster::addIdsToHDF5Group(sourceIds, group, "sourceIds");
		HDF5Typecaster::addIdsToHDF5Group(targetIds, group, "targetIds");
		HDF5Typecaster::addTimeStampToHDF5Group(start, group, "start");
		HDF5Typecaster::addTimeStampToHDF5Group(end, group, "end");

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer addConnection: Cannot create a HDF serialization.";
		return false;
	}

	return true;

}


bool HDF5UpdateSerializer::setNodeAttributes(Id id,
		vector<Attribute> newAttributes,  TimeStamp timeStamp) {

	LOG(DEBUG) << "HDF5UpdateSerializer: updating Attributes for node " << id.toString();
	try {
		std::string fileName = "Attribute-Update-" + id.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::SET_ATTRIBUTES, scene);

		H5::Group group = scene.createGroup("Attribute-Update-" + id.toString()); // The actual data
		HDF5Typecaster::addNodeIdToHDF5Group(id, group);
		HDF5Typecaster::addAttributesToHDF5Group(newAttributes, group);
		HDF5Typecaster::addTimeStampToHDF5Group(timeStamp, group, "attributesTimeStamp");

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer setNodeAttributes: Cannot create a HDF serialization.";
		return false;
	}

	return true;
}

bool HDF5UpdateSerializer::setTransform(Id id,
		IHomogeneousMatrix44::IHomogeneousMatrix44Ptr transform,
		TimeStamp timeStamp) {

	LOG(DEBUG) << "HDF5UpdateSerializer: updating a Transform-" << id.toString();
	try {
		std::string fileName = "Transform-Update-" + id.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::SET_TRANSFORM, scene);

		H5::Group group = scene.createGroup("Transform-Update-" + id.toString()); // The actual data
		HDF5Typecaster::addNodeTypeInfoToHDF5Group(HDF5Typecaster::TRANSFORM, group);
		HDF5Typecaster::addNodeIdToHDF5Group(id, group);
		HDF5Typecaster::addTransformToHDF5Group(transform, timeStamp, group);

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();
//		doSendMessage(fileName);

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer setTransform: Cannot create a HDF serialization.";
		return false;
	}

	return true;
}

bool HDF5UpdateSerializer::setUncertainTransform(Id id,
		IHomogeneousMatrix44::IHomogeneousMatrix44Ptr transform,
		ITransformUncertainty::ITransformUncertaintyPtr uncertainty,
		TimeStamp timeStamp) {
	LOG(ERROR) << "HDF5UpdateSerializer: functionality not yet implemented.";
	return false;
}

bool HDF5UpdateSerializer::deleteNode(Id id) {
	LOG(DEBUG) << "HDF5UpdateSerializer: deleting Node-" << id.toString();
	try {
		std::string fileName = "Node-Deletion-" + id.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::DELETE, scene);

		H5::Group group = scene.createGroup("Node-" + id.toString()); // The actual data
		HDF5Typecaster::addNodeIdToHDF5Group(id, group);

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();
//		doSendMessage(fileName);

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer setTransform: Cannot create a HDF serialization.";
		return false;
	}

	return true;
}

bool HDF5UpdateSerializer::addParent(Id id, Id parentId) {
	LOG(DEBUG) << "HDF5UpdateSerializer: adding Parent " << parentId.toString() << " to Node " << id.toString();
	try {
		std::string fileName = "Parent-Addition-" + id.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::ADD_PARENT, scene);
		HDF5Typecaster::addNodeIdToHDF5Group(parentId, scene, rsgParentIdName);

		H5::Group group = scene.createGroup("Parent-Child-Relation-" + id.toString()); // The actual data
		HDF5Typecaster::addNodeIdToHDF5Group(id, group);

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer addParent: Cannot create a HDF serialization.";
		return false;
	}

	return true;
}

bool HDF5UpdateSerializer::removeParent(Id id, Id parentId) {
	LOG(DEBUG) << "HDF5UpdateSerializer: removing Parent " << parentId.toString() << " to Node " << id.toString();
	try {
		std::string fileName = "Parent-Deletion-" + id.toString() + fileSuffix;
		H5::FileAccPropList faplCore;
		faplCore.setCore(fileImageIncremet, storeMessageBackupsOnFileSystem); // toggle in-memory behavior
		H5::H5File file(fileName, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, faplCore);

		/* Generic/common entry group with general information */
		H5::Group scene = file.createGroup("Scene");
		HDF5Typecaster::addCommandTypeInfoToHDF5Group(HDF5Typecaster::REMOVE_PARENT, scene);
		HDF5Typecaster::addNodeIdToHDF5Group(parentId, scene, rsgParentIdName);

		H5::Group group = scene.createGroup("Parent-Child-Relation-" + id.toString()); // The actual data
		HDF5Typecaster::addNodeIdToHDF5Group(id, group);

		file.flush(H5F_SCOPE_GLOBAL);
		doSendMessage(file);
		file.close();

	} catch (H5::Exception e) {
		LOG(ERROR) << "HDF5UpdateSerializer addParent: Cannot create a HDF serialization.";
		return false;
	}
	return true;
}

bool HDF5UpdateSerializer::doSendMessage(std::string messageName) {
	std::ifstream inputFile;
	inputFile.open(messageName.c_str(), std::ios::binary);

	if(!inputFile.is_open()) {
		LOG(ERROR) << "Cannot open file" << messageName <<". Aborting.";
		return false;
	}

	/* get length of file */
	inputFile.seekg (0, std::ios::end);
	long length = inputFile.tellg();
	inputFile.seekg (0, std::ios::beg);
	LOG(DEBUG) << "HDF5UpdateSerializer: Sending message " << messageName << " with length " << length;

	char *buffer = new char [length];
	inputFile.read (buffer,length); // a single bunch of data is feed into the buffer
	inputFile.close();
	LOG(DEBUG) << "HDF5UpdateSerializer: prepared byte stream.";

	int transferredBytes;
	int returnValue =  port->write(buffer, length, transferredBytes);
	LOG(DEBUG) << "HDF5UpdateSerializer: \t" << transferredBytes << " bytes transferred.";

	delete buffer;
	return true;
}

bool HDF5UpdateSerializer::doSendMessage(H5::H5File message) {
	ssize_t fileSize = -1;
//	ssize_t numberOfBytesRead;

//	fileSize = message.getFileSize();
    fileSize = H5Fget_file_image(message.getId(), NULL, 0);
	LOG(DEBUG) << "HDF5UpdateSerializer (VFD): Sending message with length " << fileSize;

	char *buffer = new char [fileSize];
    H5Fget_file_image(message.getId(), buffer, fileSize);
	LOG(DEBUG) << "HDF5UpdateSerializer (VFD): prepared byte stream with length " << fileSize;

	int transferredBytes;
	int returnValue =  port->write(buffer, fileSize, transferredBytes);
	LOG(DEBUG) << "HDF5UpdateSerializer (VFD): \t" << transferredBytes << " bytes transferred.";

	delete buffer;
	return true;
}

} /* namespace rsg */
} /* namespace brics_3d */
/* EOF */
