/**
 * @file 
 * IOctreeSetup.h
 *
 * @date: Feb 18, 2010
 * @author: sblume
 */

#ifndef IOCTREESETUP_H_
#define IOCTREESETUP_H_

/*
 *
 */
namespace BRICS_3D {

/**
 * @brief Abstract setup interface for the octree component.
 * @ingroup filtering
 */
class IOctreeSetup {
public:

	/**
	 * @brief Standard constructor.
	 */
	IOctreeSetup(){};

	/**
	 * @brief Standard destructor.
	 */
	virtual ~IOctreeSetup(){};

	/**
	 * @brief Set the size of the maximum of smallest voxel in an Octree.
	 *
	 * @param voxelSize The new voxel size.
	 * NOTE: the current implementation interprets voxelSize as the from the center to one of the cube faces.
	 */
	virtual void setVoxelSize(double voxelSize) = 0;

	/**
	 * @brief Get the current size the maximum of the smallest voxel in an Octree.
	 * @return The current size.
	 */
	virtual double getVoxelSize() = 0;
};

}

#endif /* IOCTREESETUP_H_ */

/* EOF */