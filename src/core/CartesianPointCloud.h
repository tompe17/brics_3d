/**
 * @file
 * brics_3dpm 
 * PointCloud.h
 * 
 * @author: Sebastian Blumenthal
 * @date: Aug 24, 2009
 * @version: 0.1
 */

#ifndef CARTESIANPOINTCLOUD_H_
#define CARTESIANPOINTCLOUD_H_

#include <vector>
#include <string>

#include "core/Point3D.h"

using namespace std;

/**
 * @namespace brics
 */
namespace brics {

/**
 * @class PointCloud3D
 * @brief Class to represent a Cartesian 3D point cloud
 * @author Sebastian Blumenthal
 * @date Aug 25, 2009
 */
class PointCloud3D {
public:

	/**
	 * @brief Standard constuctor
	 */
	PointCloud3D();

	/**
	 * @brief Standard destructor
	 */
	virtual ~PointCloud3D();

	/**
	 * @brief Add a point to the poin cloud
	 * @param point Point that will be added
	 */
	void addPoint(Point3D point);

	/**
	 * @brief Get the reference to the point cloud
	 * @return Pointer to point cloud
	 */
    vector<Point3D> *getPointCloud();

	/**
	 * @brief Set the reference to the point cloud
	 * @param pointCloud Pointer to new point cloud
	 */
    void setPointCloud(vector<Point3D> *pointCloud);

    /**
     * @brief Get the number of points in the point cloud
     * @return Size of point cloud (= number of stored points)
     */
    int getSize();

    /**
     * @brief Strores the point cloud into a ply file (Stanford polygon file format)
     * @param filename Specifiess the name of the file .e.g. point_cloud.ply
     */
    void storeToPlyFile(string filename);

    /**
     * @brief Strores the point cloud into a sipmle text file (x y z)
     * @param filename Specifiess the name of the file .e.g. point_cloud.txt
     */
    void storeToTxtFile(string filename);

private:

	///Pointer to vector which represents a Cartesian point cloud
	vector<Point3D> *pointCloud;
};

}

#endif /* CARTESIANPOINTCLOUD_H_ */

/* EOF */

