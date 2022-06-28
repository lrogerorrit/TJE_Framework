#include "curves.h"
#include <assert.h>
#include <iostream>


BeizerCurve::BeizerCurve(std::vector<Vector3>& points, float increments, bool cacheAtStart)
{
	this->curvePoints = points;
	this->numPoints = points.size()-1;
	this->calculateArcLength(1000);
	//calculate cached segments
	if (cacheAtStart) {
		this->calculateUniformSegments(150);
		this->cacheSegments(1.0 / 150.0);
	}
		//this->cacheSegments(increments);
}

void BeizerCurve::calculateArcLength(int points) {
	float length = 0.0f;
	float increments = 1.0 / points;
	Vector3 prevPoint = this->getPosition(0);
	for (float i = increments; i <= 1.0; i += increments) {
		Vector3 actualPoint = this->getPosition(i);
		length += prevPoint.distance(actualPoint);
		prevPoint = actualPoint;
	}
	std::cout << "Arc length= " << length << std::endl;
	this->arcLength = length;
}

void BeizerCurve::calculateUniformSegments(int segmentNum,float marginError) {
	float segmentSize = this->arcLength / (float) segmentNum;
	double acumulatedDistance = 0;
	double segmentDistance = 0;
	
	this->numSegments = segmentNum;
	
	double increments = .00001;
	double mu = 0;
	for (int i = 0; i < segmentNum; ++i) {
		segmentData data;
		double segmentPos = segmentSize * i;
		std::cout << "Segment "<<i<<" pos " << segmentPos<< std::endl;
		Vector3 pos = this->getPosition(mu);
		while (abs(acumulatedDistance - segmentPos) > marginError) {
			mu += increments;
			
			Vector3 newPos = this->getPosition(mu);
			float change= pos.distance(newPos);
			acumulatedDistance += change;
			//std::cout << mu <<"-" <<acumulatedDistance<< std::endl;
			segmentDistance += change;
			pos = newPos;
		}
		data.mu = mu;
		data.position = pos;
		std::cout << "Segment " << i << " at dist: " << acumulatedDistance << std::endl;
		if (i > 0) 
			this->segmentArray[i - 1].distance = segmentDistance;

		//this->cachedSegments.push_back(pos);
		this->segmentArray.push_back(data);
		segmentDistance = 0;
	}
}

void BeizerCurve::addPoint(Vector3 pos, int index)
{
	if (index == -1)
		this->curvePoints.push_back(pos);
	else 
		this->curvePoints.insert(this->curvePoints.begin() + index, pos);
	this->numPoints++;
}

Vector3 BeizerCurve::getPosition(double mu) //Extracted from http://paulbourke.net/geometry/bezier/
{
	assert(mu >= 0 && mu < 1);
	int k, kn, nn, nkn;
	double blend, muk, munk;
	Vector3 position= Vector3(0,0,0);
	
	muk = 1;
	munk = pow(1 - mu, (double)this->numPoints);
	
	for (int k = 0; k <= this->numPoints; ++k) {
		
		nn = this->numPoints;
		kn = k;
		nkn = (nn - k);
		blend = muk * munk;
		muk *= mu;
		munk /= (1 - mu);
		while (nn >= 1) {
			blend *= nn;
			nn--;
			if (kn > 1) {
				blend /= (double)kn;
				kn--;
			}
			if (nkn > 1) {
				blend /= (double)nkn;
				nkn--;
			}
		}
		position.x+= this->curvePoints[k].x * blend;
		position.y+= this->curvePoints[k].y * blend;
		position.z+= this->curvePoints[k].z * blend;		
	}
	return position;
	
}

Vector3 BeizerCurve::getFrontDirection(float mu)
{
	return (getPosition(mu) - getPosition((mu) + .0001)).normalize();
}

Matrix44 BeizerCurve::getRotationMatrix(float mu)  // (1,2,3) right (4,5,6) up (7,8,9) forward
{
	Matrix44 toReturn;
	Vector3 forward = getFrontDirection(mu);
	Vector3 right = Vector3(0,1,0).cross(forward).normalize();
	toReturn._11 = right.x;
	toReturn._12 = right.y;
	toReturn._13 = right.z;
	toReturn._21 = 0;
	toReturn._22 = 1;
	toReturn._23 = 0;
	toReturn._31 = forward.x;
	toReturn._32 = forward.y;
	toReturn._33 = forward.z;
	return toReturn;
	
	
}

void BeizerCurve::cacheSegments(float increments)
{
	this->numSegments = floor(1.0 / increments);
	
	this->cachedSegments.reserve(this->numSegments);
	for (float i = 0.0f; i <= 1.0f; i += increments)
		this->cachedSegments.push_back(this->getPosition(i));
	for (int i = 0; i < this->cachedSegments.size()-1; i++) {
		this->segmentDistances.push_back(this->cachedSegments[i].distance(this->cachedSegments[i + 1]));
		
	}
	
}

Vector3 BeizerCurve::getSegmentDirection(int i)
{
	segmentData& data = this->segmentArray[i];
	return (data.position - (this->getPosition(data.mu + .0001))).normalize();
	
}

float BeizerCurve::getSegmentDistance(int i)
{
	return this->segmentDistances[i];
	//return this->segmentArray[i].distance;
	float dist = (this->segmentArray[i].position.distance(this->segmentArray[i + 1].position));
	std::cout << "segment " << i << " dist is " << dist << std::endl;
	return dist;
}

int BeizerCurve::getSegmentFromMu(float mu)
{
	/*if (mu == 0) return 0;
	else if (mu == 1) return this->numSegments;
	for(int i=0;i<this->numSegments-1;++i){
		auto &data = this->segmentArray[i];
		auto& nextData = this->segmentArray[i + 1];

		if (mu >= data.mu && mu < nextData.mu) return i;

		
		
	}*/
	float segmentSize = 1.0f / (float)numSegments;
	int segment = (int)floor(mu / segmentSize);
	return segment;
}

