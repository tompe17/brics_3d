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


#ifndef JSONTest_H_
#define JSONTest_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "brics_3d/worldModel/WorldModel.h"

namespace unitTests {



class JSONTest : public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( JSONTest );
#ifdef BRICS_JSON_ENABLE
	CPPUNIT_TEST( testLoopBack );
	CPPUNIT_TEST( testUpdateObersver );
	CPPUNIT_TEST( testInputFilter );
	CPPUNIT_TEST( testQuerys );
	CPPUNIT_TEST( testRootNodeQuerys );
#ifdef BRICS_MICROBLX_ENABLE
	CPPUNIT_TEST( testFunctionBlockQuerys );
#endif /* BRICS_MICROBLX_ENABLE */
	CPPUNIT_TEST( testComplexAttributeValues );
#endif /* BRICS_JSON_ENABLE */
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

	void testLoopBack();
	void testUpdateObersver();
	void testInputFilter();
	void testQuerys();
	void testFunctionBlockQuerys();
	void testRootNodeQuerys();
	void testComplexAttributeValues();
	void threadFunction(brics_3d::WorldModel* wm);

private:
	  /// Maximum deviation for equality check of double variables
	  static const double maxTolerance = 0.00001;

//	brics_3d::WorldModel* wm;
	volatile bool doRun;

	std::string blockRepositoryPath;
};

}  // namespace unitTests
#endif /* JSONTest_H_ */

/* EOF */
