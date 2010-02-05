/**
 * @file 
 * NearestNeighborSTANN.h
 *
 * @date: Jan 11, 2010
 * @author: sblume
 */

#ifndef NEARESTNEIGHBORSTANN_H_
#define NEARESTNEIGHBORSTANN_H_

#include "INearestNeighbor.h"
#include "stann/include/dpoint.hpp"
#include "stann/include/sfcnn.hpp"


namespace BRICS_3D {

const unsigned int STANNDimension = 3u;
typedef reviver::dpoint<double, STANNDimension> STANNPoint;

/**
 * @ingroup nearestNeighbor
 * @brief Implementation for the nearest neighbor search algorithm with the STANN librar<.
 *
 *
 * Further information about the STANN library can be found here: http://sites.google.com/a/compgeom.com/stann/Home
 *
 * <b>IMPORTANT NOTE: Currently this wrapper only supports dimensionality of 3!</b>
 *
 */
class NearestNeighborSTANN: public BRICS_3D::INearestNeighbor {
public:

	/**
	 * @brief Standard constructor
	 */
	NearestNeighborSTANN();

	/**
	 * @brief Standard destructor
	 */
	virtual ~NearestNeighborSTANN();

	void setData(vector< vector<float> >* data);
	void setData(vector< vector<double> >* data);
	void setData(PointCloud3D* data);

	int findNearestNeigbor(vector<float>* query);
	int findNearestNeigbor(vector<double>* query);
	int findNearestNeigbor(Point3D* query);

protected:

	/// Handle to the STANN data repesentation (Morton ordering)
	sfcnn<STANNPoint, STANNDimension, double>* nearestNeigborHandle;

	/// Data in STANN-like format
	vector<STANNPoint> points; // TODO: goes to heap?

	/// Vector with the resulting indices (should be only one index)
	vector<long unsigned int> resultIndices;

	/// Vector with the resulting distances (should be only one distance). FLANN search method returns this as the squared distance.
	vector <double> squaredResultDistances;

	/// Here we are only looking the <b>the</b> nearest neighbor, so k = 1
	const static long int k = 1;

};

}

#endif /* NEARESTNEIGHBORSTANN_H_ */

/* EOF */