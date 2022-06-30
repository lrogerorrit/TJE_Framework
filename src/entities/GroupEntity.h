#pragma once
#include "Entity.h"
#include "../mesh.h"
#include "../shader.h"
#include "../texture.h"


class GroupEntity :
    public Entity
{

protected:
	std::vector<Matrix44> matrixList;
public:
	

	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	bool collidesWithShark = true;
	bool hasLowPolyVersion = false;
	float minDistanceForLowPoly = 200;
	Mesh* lowPolyMesh;
	float scaled = 1.0f;
	Vector4 color;
	
	GroupEntity(Mesh* mesh, Texture* Texture, Shader* shader, std::vector<Matrix44>& matrixList,Vector4 color=Vector4(1,1,1,1));
	GroupEntity(Mesh* mesh, Mesh* lowPoly, Texture* Texture, Shader* shader, std::vector<Matrix44>& matrixList, Vector4 color = Vector4(1, 1, 1, 1));
	~GroupEntity();

	void render();
	void update(float dt);

	void setGroupScale(float scale);
	void groupScale(float scale);

	bool getShouldRenderEntity();

	void addObject(Matrix44 objMatrix);
	void removeObject(int i);

	void setColor(Vector4 color);
	
	std::vector<Matrix44>& getMatrixList() { return matrixList; };

	void setLowPoly(Mesh* mesh);
	inline void setLowPolyDistance(float dist) { this->minDistanceForLowPoly = dist; }
	float getLowPolyDistance() { return this->minDistanceForLowPoly; }

	bool testCollision(Vector3 charCenter, float radius, Vector3& collisionPoint, Vector3& collisionNormal, Matrix44 otherModel, Mesh* otherMesh);
	
	
	
};

