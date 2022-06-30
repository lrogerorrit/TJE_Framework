#pragma once
#include "framework.h"


struct segmentData {
	Vector3 position;
	float distance;
	double mu;
	
};

//Using pseudocode from http://paulbourke.net/
class BeizerCurve
{
	public:
		std::vector<Vector3> curvePoints;
		std::vector<Vector3> cachedSegments;
		std::vector<float> segmentDistances;
		int numPoints=0;
		int numSegments = 0;
		float speedConstant = 1;

		std::vector<segmentData> segmentArray;

		float arcLength;
		
		
		BeizerCurve(std::vector<Vector3>& points, float increments=.05,bool cacheAtStart=true); //There should be be n+1 points (last point isnt computed though)

		void calculateArcLength(int points);

		void calculateUniformSegments(int segmentNum, float marginError=.025);

		
	
		void addPoint(Vector3 pos, int index = -1);
		inline Vector3 getPoint(int index) { return curvePoints[index]; };
		inline std::vector<Vector3> getPoints() { return curvePoints; };
		
		Vector3 getPosition(double mu);
		Vector3 getFrontDirection(float mu);
		Matrix44 getRotationMatrix(float mu);
		
		void clearSegmentCache() {
			cachedSegments.clear();
			segmentDistances.clear();
		}
		void cacheSegments(float increments=.05);
		
		
		inline Vector3 getSegment(int i) {
			return segmentArray[i].position;
		}
		inline std::vector<Vector3>& getSegments() {
			return cachedSegments;
		}
		

		Vector3 getSegmentDirection(int i);

		float getSegmentDistance(int i);
		
		int getSegmentFromMu(float mu);
	
};

