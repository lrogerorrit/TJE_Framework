#include "GroupEntity.h"
#include "../camera.h"






GroupEntity::GroupEntity(Mesh* mesh, Texture* texture, Shader* shader, std::vector<Matrix44>& matrixList,Vector4 color)
{
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->matrixList = matrixList;
	this->type = EntityType::GROUP;
	this->color = color;
	
}

GroupEntity::GroupEntity(Mesh* mesh, Mesh* lowPoly, Texture* texture, Shader* shader, std::vector<Matrix44>& matrixList,Vector4 color)
{
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->matrixList = matrixList;
	this->setLowPoly(lowPoly);
	this->type = EntityType::GROUP;
	this->color = color;
}

GroupEntity::~GroupEntity()
{
	this->matrixList.clear();
}

void GroupEntity::render()
{
	Camera* cam = Camera::current;
	shader->enable();
	
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_texture", texture,0);
	shader->setUniform("u_color", this->color); //todo change to variable
	
	for (int i = 0; i < matrixList.size(); i++)
	{
		shader->setUniform("u_model", matrixList[i]);
		mesh->render(GL_TRIANGLES);
	}
	
	
	
	shader->disable();
	
	Entity::render();
}

void GroupEntity::update(float dt)
{
	
}

void GroupEntity::setGroupScale(float scale)
{
	for (int i = 0; i < matrixList.size(); i++)
	{
		matrixList[i].setScale(scale,scale,scale);
	}
	this->scaled = scale;
}

void GroupEntity::groupScale(float scale)
{
	for (int i = 0; i < matrixList.size(); i++)
	{
		matrixList[i].scale(scale, scale, scale);
	}
	this->scaled *= scale;
}

bool GroupEntity::getShouldRenderEntity()
{
	return true;
}

void GroupEntity::addObject(Matrix44 objMatrix) 
{
	matrixList.push_back(objMatrix);
}

void GroupEntity::removeObject(int i)
{
	matrixList.erase(matrixList.begin() + i);
}

void GroupEntity::setColor(Vector4 color)
{
	this->color = color;
}

void GroupEntity::setLowPoly(Mesh* mesh)
{
	this->hasLowPolyVersion = true;
	this->lowPolyMesh = mesh;
}

bool GroupEntity::testCollision(Vector3 charCenter, float radius, Vector3& collisionPoint, Vector3& collisionNormal, Matrix44 otherModel, Mesh* otherMesh = NULL)
{
	if (this->ingoreCollision) return false;
	Vector3 position = this->model.getTranslation();
	for (int i=0; i < matrixList.size(); ++i){
		//TODO: OPtimize and do precheck to see if calc colisions
		
		if (this->mesh->testSphereCollision(matrixList[i], charCenter, radius, collisionPoint, collisionNormal)) {
			std::cout << "collision size " << this->model.m[0] << "-" << this->model.m[5] << "-" << this->model.m[10] << std::endl;
			return true;
		}
	}
	return false;
}
