/**
 * @file 
 * RigidTransformationEstimationTest.h
 *
 * @date: Dec 7, 2009
 * @author: sblume
 */

#ifndef RIGIDTRANSFORMATIONESTIMATIONTEST_H_
#define RIGIDTRANSFORMATIONESTIMATIONTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "core/PointCloud3D.h"
#include "core/HomogeneousMatrix44.h"
#include "algorithm/registration/PointCorrespondenceKDTree.h"
#include "algorithm/registration/RigidTransformationEstimationSVD.h"
#include "algorithm/registration/IterativeClosestPoint.h"

#include <Eigen/Geometry>
#include <iostream>

using namespace std;
using namespace BRICS_3D;

namespace unitTests {

class IterativeClosestPointTest : public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( IterativeClosestPointTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testSimpleAlignment );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testConstructor();
	void testSimpleAlignment();

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW //Required by Eigen2

private:

	IterativeClosestPoint* icp;
	IIterativeClosestPoint* abstractIcp;

	PointCloud3D *pointCloudCube;
	PointCloud3D *pointCloudCubeCopy;

	/* points of a simple cube */
	Point3D *point000;
	Point3D *point001;
	Point3D *point011;
	Point3D *point010;
	Point3D *point100;
	Point3D *point101;
	Point3D *point111;
	Point3D *point110;

	static const double maxTolerance = 0.00001;
};

}  // namespace unitTests
#endif /* RIGIDTRANSFORMATIONESTIMATIONTEST_H_ */

/* EOF */