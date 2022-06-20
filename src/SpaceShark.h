#pragma once
#include "framework.h"
#include "entities/EntityInclude.h"

enum class eSharkState {
	IDLE,
	ATTACK,
	RETREAT,
	BACKSTAGE,
	HOMING
};


class TrainHandler;


class SpaceShark
{
private:
	
	
	TrainHandler* trainHandler=NULL;
	
	eSharkState state = eSharkState::IDLE;


	
	
	float lastAttackTime = 0;
	float timeInStage = 0.0;
	float scareLevels = 0.0;
	int scared_times = 0;
	
	float speed = .3;

	bool canAttack = false;
	bool inBackstage = false;
	
	bool rightSide = true;
	float train_separation = 40.0f;

	

	float rightDisplacement = 0;
	
	float maxRightDisplacement = 40;
	
	
	//Idle
	float idleDisplacement = 0;
	float idleDisplacementDir = 1;
	float sharkLerpPos = 0;
	int sharkDisplDirection = 1;
	float sharkAngleDirection = 1;

	bool changeDir = false;

	//Homing Data
	float homingRadius = 50;
	float homingAngle = 0;
	float homingSpeed = .1;
	Vector3 homingTarget;
	

	void generateNewPosition();
	void navigateToTrain(double deltaTime);
	void updateIdle(double deltaTime);
	void updateAttack(double deltaTime);
	void updateRetreat(double deltaTime);
	void updateBackstage(double deltaTime);

	void updateHomingTarget(double deltaTime);
	
	void homingGoToRadius(double deltaTime);
	void homingRotate(double deltaTime);
	void updateHoming(double deltaTime);

	Vector2 position;
	float height = 0.0f; //Relative to train

	MeshEntity* meshEntity = NULL;
	
	
	double updateDisplAngle(double deltaTime, float speed);

public:
	static SpaceShark* instance;
	SpaceShark();
	

	void Update(double deltaTime);
	void Render();
	void setState(eSharkState state);
	void setCanAttack(bool state){
		canAttack = state;
	}
	
};

