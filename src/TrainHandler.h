#pragma once
#include "entities/EntityInclude.h"
#include "curves.h"

class BoundingBox;
class Scene;

class GUImanager;



struct trainCarData {
	Entity* entity;
	MeshEntity* trainMesh;
	MeshEntity* hornMesh;
	int carIndex;
	double curvePos;
	int segment;
	Vector3 displacement;
	
};


class TrainHandler
{
private:
	std::vector<trainCarData> trainCarArray;
	int numCars = 0;
	double frontCarPos = 0.0;
	BeizerCurve* activeCurve=NULL;
	double speed = .03;
	bool collidedWithPlayer = false;
	GUImanager* guiManager=NULL;
	int health = 100;
	int maxHealth = 100;
	
	
public:

	static TrainHandler* instance;

	TrainHandler();
	TrainHandler(BeizerCurve* curve);

	
	void setActiveCurve(BeizerCurve* curve);
	void addCar(Entity* entity, MeshEntity* trainMesh= NULL);
	
	Vector3 getCarPosition(int carNum);
	Matrix44 getCarDirection(int carNum); 
	Matrix44 getCarDirPos(int carNum);

	Vector3 getCarDisplacement(int carNum);
	
	std::vector<Matrix44> getTrainDirPos();
	
	void damageTrain(int damage);
	void fixTrain();
	void addToMaxHealth(int quantity);
	int getHealth();
	int getMaxHealth();
	
	
	
	
	void setSpeed(double speed);
	double getSpeed();
	
	void update(double dt);
	

	trainCarData getCarData(int carNum);

	bool getCollidedWithPlayer();

	void renderHealth();
	

	
	
	
	int getCarNum() {
		return numCars;
	}
	
	
	
	
};

