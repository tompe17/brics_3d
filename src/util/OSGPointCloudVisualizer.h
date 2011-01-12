/**
 * @file 
 * OSGPointCloudVisualizer.h
 *
 * @date: Nov 23, 2009
 * @author: sblume
 */

#ifndef OSGPOINTCLOUDVISUALIZER_H_
#define OSGPOINTCLOUDVISUALIZER_H_

#include <osg/Group>
#include <osg/Geometry>

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>

#include "core/PointCloud3D.h"
#include "core/ColoredPointCloud3D.h"


namespace BRICS_3D {

/**
 * @class OSGPointCloudVisualizer
 * @brief Displays point clouds in an Open Scene Graph (OSG) viewer
 */
class OSGPointCloudVisualizer {
public:

	/**
	 * @brief Standard constructor
	 */
	OSGPointCloudVisualizer();

	/**
	 * @brief Standard destructor
	 */
	virtual ~OSGPointCloudVisualizer();

	/**
	 * @brief Add an additional point cloud to the scene graph.
	 *
	 * The scene graph will <b>not</b> be started, until visualizePointCloud() or
	 * visualizeColoredPointCloud() is invoked!
	 * So this method only allows to specify more than the one cloud in visualizePointCloud.
	 * It is possible to mix colored with non-colored point clouds.
	 * @param[in] pointCloud Pointer to point cloud that will be added to scene grpah.
	 * @param red Specifies the amount of red for the color of the point cloud. Range is 0.0f to 1.0f
	 * @param green Specifies the amount of green for the color of the point cloud. Range is 0.0f to 1.0f
	 * @param blue Specifies the amount of blue for the color of the point cloud. Range is 0.0f to 1.0f
	 * @param alpha Specifies the amount of the alpha channel of the point cloud. Range is 0.0f to 1.0f
	 */
	void addPointCloud(PointCloud3D* pointCloud, float red=1.0f, float green=1.0f, float blue=1.0f, float alpha=1.0f);

	/**
	 * @brief Add an additional colored point cloud to the scene graph.
	 *
	 * The scene graph will <b>not</b> be started, until visualizePointCloud() or
	 * visualizeColoredPointCloud() is invoked!
	 * So this method only allows to specify more than the one cloud in visualizePointCloud.
	 * It is possible to mix colored with non-colored point clouds.
	 *
	 * @param[in] pointCloud Pointer to colored point cloud that will be added to scene graph.
	 * @param alpha Specifies the amount of the alpha channel of the point cloud. Range is 0.0f to 1.0f
	 */
	void addColoredPointCloud(ColoredPointCloud3D* pointCloud, float alpha=1.0f); /// @deprecated

	//void addColoredPointCloud(PointCloud3D* coloredPointCloud, float alpha=1.0f);

	/**
	 * @brief Visualizes a point cloud within the OSG framework
	 *
	 * This method is the most convenient one, but it does not allow to add more point clouds,
	 * as it will start an (endless) loop to display the OSG window.
	 * @param[in] pointCloud Pointer to point cloud that will be visualized
	 * @param red Specifies the amount of red for the color of the point cloud. Range is 0.0f to 1.0f
	 * @param green Specifies the amount of green for the color of the point cloud. Range is 0.0f to 1.0f
	 * @param blue Specifies the amount of blue for the color of the point cloud. Range is 0.0f to 1.0f
	 * @param alpha Specifies the amount of the alpha channel of the point cloud. Range is 0.0f to 1.0f
	 */
	void visualizePointCloud(PointCloud3D* pointCloud, float red=1.0f, float green=1.0f, float blue=1.0f, float alpha=1.0f);

	/**
	 * @brief Visualizes a colored point cloud within the OSG framework
	 *
	 * This method is the most convenient one, but it does not allow to add more (colored) point clouds,
	 * as it will start an (endless) loop to display the OSG window.
	 * @param[in] pointCloud Pointer to colored point cloud that will be visualized
	 * @param alpha Specifies the amount of the alpha channel of the point cloud. Range is 0.0f to 1.0f
	 */
	void visualizeColoredPointCloud(ColoredPointCloud3D *pointCloud, float alpha=1.0f);

	/**
	 * @brief Creates a "geode" (geometric node) element for OSG out of a point cloud
	 * @param[in] pointCloud Pointer to point cloud that will be transformed into an OSG geode
	 * @param red Specifies the amount of red for the color of the point cloud. Range is 0.0f to 1.0f
	 * @param green Specifies the amount of green for the color of the point cloud. Range is 0.0f to 1.0f
	 * @param blue Specifies the amount of blue for the color of the point cloud. Range is 0.0f to 1.0f
	 * @param alpha Specifies the amount of the alpha channel of the point cloud. Range is 0.0f to 1.0f
	 */
	osg::Node* createPointCloudNode(PointCloud3D* pointCloud, float red=1.0f, float green=1.0f, float blue=1.0f, float alpha=1.0f);

	/**
	 * @brief Creates a "geode" (geometric node) element for OSG out of a colored point cloud
	 * @param[in] pointCloud Pointer to colored point cloud that will be transformed into an OSG geode
	 * @param alpha Specifies the amount of the alpha channel of the point cloud. Range is 0.0f to 1.0f
	 */
	osg::Node* createColoredPointCloudNode(ColoredPointCloud3D* pointCloud, float alpha=1.0f);

	osg::Node* createColoredPointCloudNode(PointCloud3D* coloredPointCloud, float alpha=1.0f);

private:

/// OSG viewer object
osgViewer::Viewer viewer;

/// Root node for scenegraph
osg::Group* rootGeode;

};

}

#endif /* OSGPOINTCLOUDVISUALIZER_H_ */

/* EOF */
