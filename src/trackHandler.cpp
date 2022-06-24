#include "TrackHandler.h"
#include "game.h"
#include "framework.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"

const int trackSeparation = 10;
float track_size = 1.0f;
float tie_width = 1.0f;
float tie_height = .25f;
bool calculated = false;

TrackHandler* TrackHandler::instance = NULL;

std::vector<Vector2> defaultUVVector= {
	Vector2(0, 1),
	Vector2(1, 1),
	Vector2(1, 0),
	Vector2(0, 0)
};

TrackHandler::TrackHandler() {
	instance = this;
}

void TrackHandler::setActiveCurve(BeizerCurve* curve)
{
	this->activeCurve = curve;
	this->trackPosition = 0.0;
	
	calculateTrack();
}

void TrackHandler::updatePosition(double dt)
{
	if (!calculated) {

		calculateTrack();
	}
	
}

Mesh* generateMeshFromMeshData(MeshData* data) {
	Mesh* mesh = new Mesh();
	mesh->vertices = data->positions;
	mesh->uvs = data->uvs;
	mesh->normals = data->normals;
	
	return mesh;
}

void TrackHandler::generateTrackMesh() {
	for (auto& data : sectionDataArray) {
		data.leftTrackMesh= generateMeshFromMeshData(data.leftTrackData);
		data.rightTrackMesh= generateMeshFromMeshData(data.rightTrackData);
		
	}
	for (auto& tieData : tieDataArray) {
		tieData.tieMesh = generateMeshFromMeshData(tieData.tieData);
	}
}

void TrackHandler::addTriToMeshData(MeshData& data, trackQuad& quad, std::vector<Vector2>uvs, Vector3 normal, Vector3 tri) {
	for (int pos : tri.v) {
		switch (pos) {
		case 0:
			data.positions.push_back(quad.v1);
			break;
		case 1:
			data.positions.push_back(quad.v2);
			break;
		case 2:
			data.positions.push_back(quad.v3);
			break;
		case 3:
			data.positions.push_back(quad.v4);
			break;
		}
		data.uvs.push_back(uvs[pos]);
		data.normals.push_back(normal);
	}
}

void TrackHandler::addFaceToMeshData(MeshData& data, trackQuad& quad, std::vector<Vector2>uvs,Vector3 normal, Vector3 tri1, Vector3 tri2) {
	addTriToMeshData(data, quad, uvs, normal, tri1);
	addTriToMeshData(data, quad, uvs, normal, tri2);
}

void TrackHandler::calculateTrack()
{
	BeizerCurve* bc = this->activeCurve;
	calculated = true;
	//std::cout << bc->numSegments << std::endl;
	for (int i = 0; i < bc->numSegments;++i) {
		Vector3& data = bc->cachedSegments[i];
		trackSectionData sectionData;
		sectionData.startPoint = data;
		sectionData.frontVector= bc->getSegmentDirection(i);
		sectionData.rightVector = sectionData.frontVector.cross(Vector3(0, 1, 0));
		sectionData.leftRailStart= data - sectionData.rightVector * trackSeparation * .5;
		sectionData.rightRailStart= data + sectionData.rightVector * trackSeparation * .5;
		sectionData.length = bc->getSegmentDistance(i);
		sectionData.leftRailTopLeft = sectionData.leftRailStart + Vector3(0, track_size / 2, 0);
		sectionData.leftRailTopLeft+= sectionData.rightVector * (-track_size / 2);
		sectionData.rightRailTopLeft = sectionData.rightRailStart + Vector3(0, track_size / 2, 0);
		sectionData.rightRailTopLeft+= sectionData.rightVector * (-track_size / 2);
		sectionData.center = data + sectionData.length * sectionData.frontVector;
		sectionData.leftTrackData = new MeshData();
		sectionData.rightTrackData = new MeshData();

		sectionDataArray.push_back(sectionData);
	}
	
	for (int i = 0; i < sectionDataArray.size(); ++i) {
		trackSectionData& data = sectionDataArray[i];
		
		//std::cout << data.leftRailTopLeft.x << "," << data.leftRailTopLeft.y << "," << data.leftRailTopLeft.z << std::endl;
			
		if (i == 0 || i == sectionDataArray.size() - 1) {
			trackQuad faceL = trackQuad(data.leftRailTopLeft, data.leftRailTopLeft + data.rightVector * track_size, (data.leftRailTopLeft + data.rightVector * track_size) - Vector3(0, track_size, 0), data.leftRailTopLeft - Vector3(0, track_size, 0));
			trackQuad faceR = trackQuad(data.rightRailTopLeft, data.rightRailTopLeft + data.rightVector * track_size, (data.rightRailTopLeft + data.rightVector * track_size) - Vector3(0, track_size, 0), data.rightRailTopLeft - Vector3(0, track_size, 0));
			Vector3 normal = data.frontVector * -1;
			std::vector<int> positions = { 1,2,3,0 };
			addFaceToMeshData(*data.leftTrackData, faceL, defaultUVVector, normal);
			addFaceToMeshData(*data.rightTrackData, faceR, defaultUVVector, normal);
			
			
			
			data.quads.push_back(faceL);
			data.quads.push_back(faceR);
			
			
			if (i == sectionDataArray.size() - 1) continue;
		}
		
		trackSectionData& nextData = sectionDataArray[i+1];
		
		trackQuad upL = trackQuad(data.leftRailTopLeft, nextData.leftRailTopLeft, nextData.leftRailTopLeft - Vector3(0, track_size, 0), data.leftRailTopLeft - Vector3(0, track_size, 0));
		trackQuad downL = trackQuad(data.leftRailTopLeft - Vector3(0, track_size, 0), nextData.leftRailTopLeft - Vector3(0, track_size, 0), nextData.leftRailTopLeft + nextData.rightVector * track_size - Vector3(0, track_size, 0), data.leftRailTopLeft + data.rightVector * track_size - Vector3(0, track_size, 0));
		trackQuad leftL = trackQuad(data.leftRailTopLeft, nextData.leftRailTopLeft, nextData.leftRailTopLeft + nextData.rightVector * track_size, data.leftRailTopLeft + data.rightVector * track_size);
		trackQuad rightL = trackQuad(data.leftRailTopLeft + data.rightVector * track_size, nextData.leftRailTopLeft + nextData.rightVector * track_size, nextData.leftRailTopLeft + nextData.rightVector * track_size - Vector3(0, track_size, 0), data.leftRailTopLeft + data.rightVector * track_size - Vector3(0, track_size, 0));

		trackQuad upR = trackQuad(data.rightRailTopLeft, nextData.rightRailTopLeft, nextData.rightRailTopLeft - Vector3(0, track_size, 0), data.rightRailTopLeft - Vector3(0, track_size, 0));
		trackQuad downR = trackQuad(data.rightRailTopLeft - Vector3(0, track_size, 0), nextData.rightRailTopLeft - Vector3(0, track_size, 0), nextData.rightRailTopLeft + nextData.rightVector * track_size - Vector3(0, track_size, 0), data.rightRailTopLeft + data.rightVector * track_size - Vector3(0, track_size, 0));
		trackQuad leftR = trackQuad(data.rightRailTopLeft, nextData.rightRailTopLeft, nextData.rightRailTopLeft + nextData.rightVector * track_size, data.rightRailTopLeft + data.rightVector * track_size);
		trackQuad rightR = trackQuad(data.rightRailTopLeft + data.rightVector * track_size, nextData.rightRailTopLeft + nextData.rightVector * track_size, nextData.rightRailTopLeft + nextData.rightVector * track_size - Vector3(0, track_size, 0), data.rightRailTopLeft + data.rightVector * track_size - Vector3(0, track_size, 0));
		
		addFaceToMeshData(*data.leftTrackData, upL, defaultUVVector, Vector3(0,1,0));
		addFaceToMeshData(*data.leftTrackData, downL, defaultUVVector, Vector3(0,-1,0));
		addFaceToMeshData(*data.leftTrackData, leftL, defaultUVVector, data.rightVector);
		addFaceToMeshData(*data.leftTrackData, rightL, defaultUVVector, data.rightVector * -1);
		
		addFaceToMeshData(*data.rightTrackData, upR, defaultUVVector, Vector3(0,1,0));
		addFaceToMeshData(*data.rightTrackData, downR, defaultUVVector, Vector3(0,-1,0));
		addFaceToMeshData(*data.rightTrackData, leftR, defaultUVVector, data.rightVector);
		addFaceToMeshData(*data.rightTrackData, rightR, defaultUVVector, data.rightVector * -1);

		data.quads.push_back(upL);
		data.quads.push_back(rightL);
		data.quads.push_back(leftL);
		data.quads.push_back(downL);
		
		data.quads.push_back(upR);
		data.quads.push_back(rightR);
		data.quads.push_back(leftR);
		data.quads.push_back(downR);
	}

	//Calculate ties (tests)
	float half_width = tie_width / 2.0;
	float half_separation = trackSeparation / 2.0;
	
	Vector3 half_height_vector(0, tie_height / 2.0, 0);
	for (double i = 0; i < 1.0; i += .08) {
		trackTieData data;
		data.trackLocation = i;
		Vector3 pos = bc->getPosition(i);
		data.center = pos;
		int section= bc->getSegmentFromMu(i);
		if (section == bc->numSegments - 1) continue;
		Vector3 dir = bc->getSegmentDirection(section);
		Vector3 right = dir.cross(Vector3(0, 1, 0));
		Vector3 leftC= pos - right * half_separation;
		Vector3 rightC= pos + right * half_separation;
		data.tieData = new MeshData();
		

		data.leftCenter = leftC;
		data.rightCenter = rightC;
		
		trackQuad top = trackQuad((leftC - half_width * dir) + half_height_vector, (leftC + half_width * dir) + half_height_vector, (rightC + half_width * dir) + half_height_vector, (rightC - half_width * dir) + half_height_vector);
		trackQuad bottom = trackQuad((leftC - half_width * dir) - half_height_vector, (leftC + half_width * dir) - half_height_vector, (rightC + half_width * dir) - half_height_vector, (rightC - half_width * dir) - half_height_vector);
		trackQuad front = trackQuad((leftC - half_width * dir) + half_height_vector, (rightC - half_width * dir) + half_height_vector, (rightC - half_width * dir) - half_height_vector, (leftC - half_width * dir) - half_height_vector);
		trackQuad back = trackQuad((leftC + half_width * dir) + half_height_vector, (rightC + half_width * dir) + half_height_vector, (rightC + half_width * dir) - half_height_vector, (leftC + half_width * dir) - half_height_vector);
		
		data.quads.push_back(top);
		data.quads.push_back(bottom);
		data.quads.push_back(front);
		data.quads.push_back(back);

		addFaceToMeshData(*data.tieData, top, defaultUVVector, Vector3(0, 1, 0));
		addFaceToMeshData(*data.tieData, bottom, defaultUVVector, Vector3(0, -1, 0));
		addFaceToMeshData(*data.tieData, front, defaultUVVector, dir*-1);
		addFaceToMeshData(*data.tieData, back, defaultUVVector, dir);
		
		tieDataArray.push_back(data);
		
	}
	std::cout << "Track Calculation done\n";
	std::cout << "Generating track mesh\r";
	this->generateTrackMesh();
	std::cout << "Generating track mesh - DONE\n";
	
}

void TrackHandler::renderTrack(int maxDistance)
{
	
	/*
	glBegin(GL_QUADS);
	
	//go through the vector sectionDataArray
	//std::cout << sectionDataArray.size() << std::endl;
	for (int i = 0; i < sectionDataArray.size(); ++i) {
		trackSectionData& data = sectionDataArray[i];
		//if (data.length > maxDistance) continue;
		for (int j = 0; j < data.quads.size(); ++j) {
			trackQuad& quad = data.quads[j];
			glColor3f(1,0,0);
			
			glVertex3f(quad.v1.x, quad.v1.y, quad.v1.z);
			glColor3f(0, 1, 0);
			glVertex3f(quad.v2.x, quad.v2.y, quad.v2.z);
			glColor3f(0, 0, 1);
			glVertex3f(quad.v3.x, quad.v3.y, quad.v3.z);
			glColor3f(1, 1, 0);
			glVertex3f(quad.v4.x, quad.v4.y, quad.v4.z);
			
			//print quad.v1 to console
			/*std::cout << quad.v1.x << " " << quad.v1.y << " " << quad.v1.z << std::endl;
			std::cout << quad.v2.x << " " << quad.v2.y << " " << quad.v2.z << std::endl;
			std::cout << quad.v3.x << " " << quad.v3.y << " " << quad.v3.z << std::endl;
			std::cout << quad.v4.x << " " << quad.v4.y << " " << quad.v4.z << std::endl;*-/
			
		}
	}

	for (int i = 0; i < tieDataArray.size(); ++i) {
		trackTieData& data= tieDataArray[i];
		for (int j = 0; j < data.quads.size(); ++j) {
			trackQuad& quad = data.quads[j];
			glColor3f(1, 0, 0);

			glVertex3f(quad.v1.x, quad.v1.y, quad.v1.z);
			glColor3f(0, 1, 0);
			glVertex3f(quad.v2.x, quad.v2.y, quad.v2.z);
			glColor3f(0, 0, 1);
			glVertex3f(quad.v3.x, quad.v3.y, quad.v3.z);
			glColor3f(1, 1, 0);
			glVertex3f(quad.v4.x, quad.v4.y, quad.v4.z);
		}
	}
	
	


	glEnd();

	glPointSize(5);
	glBegin(GL_POINTS);
	BeizerCurve* bc = this->activeCurve;
	/*if (bc->numSegments > 0)
		for (int i = 0; i < bc->cachedSegments.size(); ++i) {
			Vector3& data = bc->cachedSegments[i];
			Vector3 dir = bc->getSegmentDirection(i);
			Vector3 right = dir.cross(Vector3(0, 1, 0));

			Vector3 sideA = data + right * trackSeparation*.5;
			Vector3 sideB = data - right * trackSeparation*.5;
			//print data
			//std::cout << "x: " << data.x << " y: " << data.y << " z: " << data.z << std::endl;
			glColor3f(1, 1, 1);
			glVertex3f(data.x, data.y, data.z);
			glColor3f(1, 1, 0);
			glVertex3f(sideA.x, sideA.y, sideA.z);
			glColor3f(1, 0, 1);
			glVertex3f(sideB.x, sideB.y, sideB.z);

		}*-/
	glColor3f(1, 0, 0);
	glPointSize(10);
	if (bc->numPoints > 0)
		for (int i = 0; i < bc->curvePoints.size(); ++i) {
			Vector3& data = bc->curvePoints[i];
			glVertex3f(data.x, data.y, data.z);
		}
	/*if (playTrack)
	{
		glColor3f(0, 1, 0);
		glPointSize(15);
		Vector3& data = bc->getPosition(trackPos);
		glVertex3f(data.x, data.y, data.z);
	}*-/
	glEnd();
	glPointSize(1);
	glColor3f(1, 1, 1);
	*/
	Camera* cam = Camera::current;
	Vector3 camPos = cam->eye;
	
	BeizerCurve* bc = this->activeCurve;
	Shader* trackShader= Shader::Get("data/shaders/basic.vs", "data/shaders/track.fs");
	trackShader->enable();
	trackShader->setUniform("u_model", Matrix44::IDENTITY);
	trackShader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	trackShader->setUniform("u_color", Vector4(1, 1, 1, 1));
	trackShader->setUniform("u_rail_texture",Texture::Get("data/assets/rails/railTexture.png"), 0); //TODO
	trackShader->setUniform("u_tie_texture", Texture::Get("data/assets/rails/plankTexture.png"),1);  //TODO
	trackShader->setUniform("isTie", false);	
	for (auto data : sectionDataArray) {
		Vector3 segmentPos= data.center; //TODO
		if ((camPos.distance(segmentPos)) > maxDistance) continue;
		data.leftTrackMesh->render(GL_TRIANGLES);
		data.rightTrackMesh->render(GL_TRIANGLES);
		//data.tieMesh->render(GL_TRIANGLES);
		
	}
	trackShader->setUniform("isTie", true);
	for (auto data : tieDataArray) {
		Vector3 segmentPos = data.center; //TODO
		if ((camPos.distance(segmentPos)) > maxDistance) continue;
		data.tieMesh->render(GL_TRIANGLES);
	}
	trackShader->disable();
}

trackQuad::trackQuad(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4)
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->v4 = v4;
}
