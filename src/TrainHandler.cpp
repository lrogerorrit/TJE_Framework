#include "TrainHandler.h"
#include "mesh.h"
#include "stages/StagesInclude.h"
#include "Scene.h"


TrainHandler* TrainHandler::instance = NULL;

TrainHandler::TrainHandler()
{
	instance = this;
}

TrainHandler::TrainHandler(BeizerCurve* curve)
{
	this->activeCurve = curve;
	instance = this;
}

void TrainHandler::setActiveCurve(BeizerCurve* curve)
{
	this->activeCurve = curve;
}

void TrainHandler::addCar(Entity* entity,MeshEntity* trainEntity)
{
	Mesh* hornMesh = Mesh::Get("data/assets/train/horn.obj");
	Texture* hornTexture = Texture::Get("data/assets/train/horn.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	trainCarData data;
	data.entity = entity;
	data.carIndex = this->numCars;
	data.segment = 0; //TODO: get segment from front position
	data.curvePos = 0;
	data.trainMesh = trainEntity;
	data.hornMesh = new MeshEntity(hornMesh, hornTexture, shader);
	data.hornMesh->setPosition(Vector3(0, .8, -1.65));
	data.hornMesh->setCheckAngle(false);
	this->trainCarArray.push_back(data);
	data.hornMesh->ingoreCollision = true;
	data.trainMesh->addChild(data.hornMesh);

	this->numCars++;
}

Vector3 TrainHandler::getCarPosition(int carNum)
{
	return this->trainCarArray[carNum].entity->getPosition()+Vector3(0,5,0); //TODO: Change displacement to real data
}

Matrix44 TrainHandler::getCarDirection(int carNum) // (1,2,3) Forward (4,5,6) right (7,8,9) up
{
	return this->activeCurve->getRotationMatrix(this->trainCarArray[carNum].curvePos);
	
}

Matrix44 TrainHandler::getCarDirPos(int carNum) //// (1,2,3) Forward (4,5,6) right (7,8,9) up (10,11,12) pos
{
	Matrix44 rotVector = this->getCarDirection(carNum);
	Vector3 pos = this->getCarPosition(carNum);
	rotVector._41 = pos.x;
	rotVector._42 = pos.y;
	rotVector._43 = pos.z;
	
	return rotVector;
}

Vector3 TrainHandler::getCarDisplacement(int carNum)
{
	return this->trainCarArray[carNum].displacement;
}

std::vector<Matrix44> TrainHandler::getTrainDirPos()
{
	std::vector<Matrix44> toReturn;
	for (int i = 0; i < this->trainCarArray.size(); i++)
	{
		toReturn.push_back(this->getCarDirPos(i));
	}
	return toReturn;
}

void TrainHandler::setSpeed(double speed)
{
	this->speed = speed;
}

double TrainHandler::getSpeed()
{
	return this->speed;
}

void TrainHandler::update(double dt)
{
	collidedWithPlayer = false;
	
	for (int i = 0; i < this->trainCarArray.size(); i++)
	{
		trainCarData& data= this->trainCarArray[i];
		
		Vector3 oldPos= data.entity->getPosition();
		
		if (data.curvePos >= 1.0) data.curvePos = 0;
		data.curvePos += dt*this->speed * (1 / activeCurve->getSegmentDistance(data.segment));
		
		data.segment = activeCurve->getSegmentFromMu(data.curvePos);
		data.entity->setPosition(activeCurve->getPosition(data.curvePos));
		Matrix44 rotMatrix= activeCurve->getRotationMatrix(data.curvePos);
		data.entity->model.m[0] = rotMatrix._11;
		data.entity->model.m[1] = rotMatrix._12;
		data.entity->model.m[2] = rotMatrix._13;
		
		data.entity->model.m[4] = rotMatrix._21;
		data.entity->model.m[5] = rotMatrix._22;
		data.entity->model.m[6] = rotMatrix._23;
		
		data.entity->model.m[8] = rotMatrix._31;
		data.entity->model.m[9] = rotMatrix._32;
		data.entity->model.m[10] = rotMatrix._33;
		
		
		data.entity->model.setFrontAndOrthonormalize(Vector3(rotMatrix._31, rotMatrix._32, rotMatrix._33));
		data.displacement= data.entity->getPosition() - oldPos;
		
		if (data.trainMesh->didCollide)
			this->collidedWithPlayer = true;
	}
	
	
}





trainCarData TrainHandler::getCarData(int carNum)
{
	return this->trainCarArray[carNum];
}

bool TrainHandler::getCollidedWithPlayer()
{
	return this->collidedWithPlayer;
}


