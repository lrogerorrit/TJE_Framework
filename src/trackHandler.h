#pragma once
#include "framework.h"
#include "curves.h"


class Mesh;

struct trackQuad {
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 v4;

	trackQuad(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4);
};

struct MeshData {
	std::vector<Vector3> positions;
	std::vector<Vector2> uvs;
	std::vector<Vector3> normals;
};

struct trackTieData {
	double trackLocation;
	Vector3 center;
	Vector3 leftCenter;
	Vector3 rightCenter;
	std::vector<trackQuad> quads;	
	MeshData* tieData;
	Mesh* tieMesh = NULL;
};

struct trackSectionData {
	Vector3 startPoint;
	Vector3 leftRailStart;
	Vector3 rightRailStart;
	Vector3 leftRailTopLeft;
	Vector3 rightRailTopLeft;
	Vector3 rightVector;
	Vector3 frontVector;
	Vector3 center;
	double length;
	
	std::vector<trackQuad> quads;

	MeshData* leftTrackData;
	MeshData* rightTrackData;
	

	Mesh* leftTrackMesh = NULL;
	Mesh* rightTrackMesh = NULL;
	
};

class TrackHandler
{
private:
	BeizerCurve* activeCurve=NULL;
	double trackPosition = 0.0;
	Matrix44 posRotMatrix= Matrix44::IDENTITY;
	std::vector<trackSectionData> sectionDataArray;
	std::vector<trackTieData> tieDataArray;
	
	
	void generateTrackMesh(); 
	void addTriToMeshData(MeshData& data, trackQuad& quad, std::vector<Vector2> uvs, Vector3 normal, Vector3 tri);
	void addFaceToMeshData(MeshData& data, trackQuad& quad, std::vector<Vector2> uvs, Vector3 normal, Vector3 tri1= Vector3(0, 1, 2), Vector3 tri2= Vector3(0, 2, 3));
	
public:
	static TrackHandler* instance;

	TrackHandler();

	void setActiveCurve(BeizerCurve* curve);
	void updatePosition(double dt);



	void calculateTrack();
	
	void renderTrack(int maxDistance = 200);
	
	double getTrackPosition(){ return trackPosition; }
	BeizerCurve* getActiveCurve(){ return activeCurve; }
	Matrix44 getRenderMatrix(){ return posRotMatrix; }
	
	
};	

