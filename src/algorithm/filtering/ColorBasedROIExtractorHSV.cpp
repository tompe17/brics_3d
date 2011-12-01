/******************************************************************************
 * BRICS_3D - 3D Perception and Modeling Library
 * Copyright (c) 2011, GPS GmbH
 *
 * Author: Pinaki Sunil Banerjee
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

#include "ColorBasedROIExtractorHSV.h"

#include "core/ColorSpaceConvertor.h"
#include <stdio.h>

namespace BRICS_3D {

ColorBasedROIExtractorHSV::ColorBasedROIExtractorHSV() {
	this->maxH = 255;
	this->minH	= 0;

	this->maxS = 255;
	this->minS	= 0;

	this->maxV = 255;
	this->minV	= 0;

}

double ColorBasedROIExtractorHSV::getMaxH() const
{
	return maxH;
}

double ColorBasedROIExtractorHSV::getMaxS() const
{
	return maxS;
}

double ColorBasedROIExtractorHSV::getMaxV() const
{
	return maxV;
}

double ColorBasedROIExtractorHSV::getMinH() const
{
	return minH;
}

double ColorBasedROIExtractorHSV::getMinS() const
{
	return minS;
}

double ColorBasedROIExtractorHSV::getMinV() const
{
	return minV;
}

void ColorBasedROIExtractorHSV::setMaxH(double maxH)
{
	this->maxH = maxH;
}

void ColorBasedROIExtractorHSV::setMaxS(double maxS)
{
	this->maxS = maxS;
}

void ColorBasedROIExtractorHSV::setMaxV(double maxV)
{
	this->maxV = maxV;
}

void ColorBasedROIExtractorHSV::setMinH(double minH)
{
	this->minH = minH;
}

void ColorBasedROIExtractorHSV::setMinS(double minS)
{
	this->minS = minS;
}

void ColorBasedROIExtractorHSV::setMinV(double minV)
{
	this->minV = minV;
}

ColorBasedROIExtractorHSV::~ColorBasedROIExtractorHSV() {}

void ColorBasedROIExtractorHSV::extractColorBasedROI(BRICS_3D::ColoredPointCloud3D *in_cloud,
		BRICS_3D::ColoredPointCloud3D *out_cloud){

	if(this->minS == 0 && this->minH == 0 && this->minV == 0 && this->maxH == 255 &&
			this->maxS == 255 && this->maxV == 255) {
		printf("[WARNING] Using maximum limits for HSV based ROI Extraction!!!\n");
	}

	int cloudSize =	in_cloud->getSize();
	double tempH, tempS, tempV;
	int tempR, tempG, tempB;
	uint8_t tempChar;
	bool passed;
	BRICS_3D::ColorSpaceConvertor colorConvertor;
	BRICS_3D::Point3D tempPoint3D;
	out_cloud->getPointCloud()->clear();

		printf("Used H-S Limits for extraction: H:[%f %f] S:[%f %f]\n", minH, maxH, minS, maxS);
	for (unsigned int i = 0; i < static_cast<unsigned int>(cloudSize); i++) {
		passed = false;
		//Getting the HSV values for the RGB points
		tempChar = in_cloud->getPointCloud()->data()[i].red;
		tempR = tempChar << 0;
		tempR = abs(tempR);

		tempChar = in_cloud->getPointCloud()->data()[i].green;
		tempG = tempChar << 0;
		tempG = abs(tempG);

		tempChar = in_cloud->getPointCloud()->data()[i].blue;
		tempB = tempChar << 0;
		tempB = abs(tempB);

		colorConvertor.rgbToHsv(tempR, tempG, tempB, &tempH, &tempS, &tempV);
		if(minH < maxH){


			//Checking the values with the set limits
			if (tempH <= maxH && tempH >= minH) {
				if (tempS >= minS && tempS <= maxS) {
					passed=true;

				}
			}


			//		printf("H-S Limits: [%f %f %f %f]\n", minH, maxH, minS, maxS);
			//		printf("Actual H-S Values: [%d %d %d %f %f]\n", tempR, tempG, tempB, tempH, tempS);
			if(passed){
				BRICS_3D::Point3D *tempPoint3D =  new BRICS_3D::Point3D(in_cloud->getPointCloud()->data()[i].getX(),
						in_cloud->getPointCloud()->data()[i].getY(),
						in_cloud->getPointCloud()->data()[i].getZ());

				BRICS_3D::ColoredPoint3D *tempColoredPoint3D = new BRICS_3D::ColoredPoint3D(tempPoint3D,
						in_cloud->getPointCloud()->data()[i].red,
						in_cloud->getPointCloud()->data()[i].green,
						in_cloud->getPointCloud()->data()[i].blue);

				out_cloud->addPoint(tempColoredPoint3D);
				delete tempPoint3D;
				delete tempColoredPoint3D;
			}


		}else {
			//		passed = false;
			//		//Getting the HSV values for the RGB points
			//		tempChar = in_cloud->getPointCloud()->data()[i].red;
			//		tempR = tempChar << 0;
			//		tempR = abs(tempR);
			//
			//		tempChar = in_cloud->getPointCloud()->data()[i].green;
			//		tempG = tempChar << 0;
			//		tempG = abs(tempG);
			//
			//		tempChar = in_cloud->getPointCloud()->data()[i].blue;
			//		tempB = tempChar << 0;
			//		tempB = abs(tempB);
			//
			//		colorConvertor.rgbToHsv(tempR, tempG, tempB, &tempH, &tempS, &tempV);

			//Checking the values with the set limits
			if ((tempH <= 255 && tempH >= minH) || (tempH >= 0 && tempH <= maxH)) {
				if (tempS >= minS && tempS <= maxS) {
					passed=true;

				}
			}


			//		printf("H-S Limits: [%f %f %f %f]\n", minH, maxH, minS, maxS);
			//		printf("Actual H-S Values: [%d %d %d %f %f]\n", tempR, tempG, tempB, tempH, tempS);
			if(passed){
				BRICS_3D::Point3D *tempPoint3D =  new BRICS_3D::Point3D(in_cloud->getPointCloud()->data()[i].getX(),
						in_cloud->getPointCloud()->data()[i].getY(),
						in_cloud->getPointCloud()->data()[i].getZ());

				BRICS_3D::ColoredPoint3D *tempColoredPoint3D = new BRICS_3D::ColoredPoint3D(tempPoint3D,
						in_cloud->getPointCloud()->data()[i].red,
						in_cloud->getPointCloud()->data()[i].green,
						in_cloud->getPointCloud()->data()[i].blue);

				out_cloud->addPoint(tempColoredPoint3D);
				delete tempPoint3D;
				delete tempColoredPoint3D;
			}
		}
	}
	//	printf("Output cloud size:%d\n", out_cloud->getSize());
}



void ColorBasedROIExtractorHSV::extractColorBasedROI(BRICS_3D::ColoredPointCloud3D *in_cloud,
		BRICS_3D::PointCloud3D *out_cloud){

	if(this->minS == 0 && this->minH == 0 && this->minV == 0 && this->maxH == 255 &&
			this->maxS == 255 && this->maxV == 255) {
		printf("[WARNING] Using maximum limits for HSV based ROI Extraction!!!\n");
	}

	int cloudSize =	in_cloud->getSize();
	double tempH, tempS, tempV;
	int tempR, tempG, tempB;
	uint8_t tempChar;
	bool passed;
	BRICS_3D::ColorSpaceConvertor colorConvertor;
	BRICS_3D::Point3D tempPoint3D;
	out_cloud->getPointCloud()->clear();

		printf("Used H-S Limits for extraction: H:[%f %f] S:[%f %f]\n", minH, maxH, minS, maxS);

	for (unsigned int i = 0; i < static_cast<unsigned int>(cloudSize); i++) {

		passed = false;
		//Getting the HSV values for the RGB points
		tempChar = in_cloud->getPointCloud()->data()[i].red;
		tempR = tempChar << 0;
		tempR = abs(tempR);

		tempChar = in_cloud->getPointCloud()->data()[i].green;
		tempG = tempChar << 0;
		tempG = abs(tempG);

		tempChar = in_cloud->getPointCloud()->data()[i].blue;
		tempB = tempChar << 0;
		tempB = abs(tempB);

		colorConvertor.rgbToHsv(tempR, tempG, tempB, &tempH, &tempS, &tempV);

		//Checking the values with the set limits
		if (tempH <= maxH && tempH >= minH) {
			if (tempS >= minS && tempS <= maxS) {
				passed=true;

			}
		}

		//		printf("H-S Limits: [%f %f %f %f]\n", minH, maxH, minS, maxS);
		//		printf("Actual H-S Values: [%d %d %d %f %f]\n", tempR, tempG, tempB, tempH, tempS);

		if(passed){
			BRICS_3D::Point3D *tempPoint3D =  new BRICS_3D::Point3D(in_cloud->getPointCloud()->data()[i].getX(),
					in_cloud->getPointCloud()->data()[i].getY(),
					in_cloud->getPointCloud()->data()[i].getZ());

			out_cloud->addPoint(tempPoint3D);
			delete tempPoint3D;

		}

	}
	//	printf("Output cloud size:%d\n", out_cloud->getSize());
}


}
