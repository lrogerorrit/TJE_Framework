#include "SpaceShark.h"
#include "TrainHandler.h"
#include "extra/commonItems.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "extra/easing.h"

Vector2 idleTimes(1, 3);//(10, 30);
Vector2 attackTimes(200, 500);//(20, 60);
Vector2 backStageTimes(10, 30);
Vector2 homingTimes(200, 500);//(50, 80);





SpaceShark* SpaceShark::instance = nullptr;

SpaceShark::SpaceShark():trainHandler(TrainHandler::instance)
{
	//TODO: Generate mesh entity for shark
	instance = this;
	Mesh* mesh = Mesh::Get("data/assets/shark/shark.obj");
	Texture* texture = Texture::Get("data/assets/shark/sharkTexture.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	this->meshEntity = new MeshEntity(mesh, texture, shader);
	

	generateNewPosition();
}

void SpaceShark::generateNewPosition() {
	this->rightSide = (bool) randomIntRange(0, 1);
	int sideSeparation = randomIntRange(50, 200);
	this->height = 0;
	float displacement= randomIntRange(-50, 50);
	
	this->rightDisplacement = sideSeparation;
	Matrix44 trainDirPos = trainHandler->getCarDirPos(0);
	Vector3 trainPos = trainDirPos.getTranslation();
	Vector3 right = trainDirPos.rightVector();
	Vector3 front= trainDirPos.frontVector();
	
	Vector2 trainPos2D(trainPos.x, trainPos.z);
	Vector3 pos = trainPos + right * (sideSeparation * (this->rightSide ? 1 : -1)) + front * displacement;
	this->position = Vector2(pos.x, pos.z); //trainPos2D + (Vector2(displacement, sideSeparation * (this->rightSide ? 1 : -1)));
	this->meshEntity->setPosition(Vector3(this->position.x, trainPos.y + this->height, this->position.y));
	this->meshEntity->modifyScale(.03);
	this->train_separation= (this->position - trainPos2D).length();
}



void SpaceShark::updateIdle(double deltaTime)
{
	if (this->inBackstage) {
		this->scared_times = 0;
		this->inBackstage = false;
		//TODO: Set position of shark near train, but far enough;
		generateNewPosition();
	}

	idleDisplacement+= .25 * deltaTime*idleDisplacementDir*(speed*2);
	if (abs(idleDisplacement) >= .99f) {
		
		idleDisplacementDir *= -1;
		changeDir = true;
	}
	std::cout<<"idleDisplacement: "<<((idleDisplacement / 2.0) + .5) << "\n";
	
	auto easingFunction = getEasingFunction(EaseInOutSine);
	double displEasing = ((easingFunction((idleDisplacement / 2.0) + .5)-.5)*2.0);
	
	
	Matrix44 trainDirPos = trainHandler->getCarDirPos(0);
	Vector3 trainPos = trainDirPos.getTranslation();
	Vector3 right = trainDirPos.rightVector();
	Vector3 front= trainDirPos.frontVector();
	Vector3 up = right.cross(front);
	Vector2 trainPos2D(trainPos.x, trainPos.z);
	Vector3 pos = trainPos + right * (rightDisplacement * (this->rightSide ? 1 : -1)) + front * (displEasing *this->maxRightDisplacement);
	
	if (changeDir) {
		sharkAngleDirection += 2 * deltaTime * idleDisplacementDir * (speed *3);
		if (sharkAngleDirection > 1.0||sharkAngleDirection<0.0) {
			changeDir = false;
			if(sharkAngleDirection<=0)
				sharkAngleDirection = 0;
			else
				sharkAngleDirection = 1;
		}
		
	}
	
	
	this->position = Vector2(pos.x, pos.z);
	this->train_separation = (this->position - trainPos2D).length();
	
	Vector3 rot= Vector3(0, sharkAngleDirection, 0);
	Matrix44 rotMatrix = trainDirPos.getRotationOnly();
		
	this->meshEntity->model.m[0] = rotMatrix._11;
	this->meshEntity->model.m[1] = rotMatrix._12;
	this->meshEntity->model.m[2] = rotMatrix._13;

	this->meshEntity->model.m[4] = rotMatrix._21;
	this->meshEntity->model.m[5] = rotMatrix._22;
	this->meshEntity->model.m[6] = rotMatrix._23;

	this->meshEntity->model.m[8] = rotMatrix._31;
	this->meshEntity->model.m[9] = rotMatrix._32;
	this->meshEntity->model.m[10] = rotMatrix._33;
	
	//float angle = remap(0, 1, -60, 120, sharkAngleDirection);
	float angle = remap(0, 1, -60, 120, sharkAngleDirection);
	this->meshEntity->model.setRotation(angle*DEG2RAD,Vector3(0,1,0));
	this->meshEntity->model.setUpAndOrthonormalize(Vector3(0, 1, 0));
	this->meshEntity->model.translateGlobal(pos.x,pos.y,pos.z);
	this->meshEntity->modifyScale(.1);
	

	if ((timeInStage > idleTimes.y) || ((timeInStage > idleTimes.x) && (randomInt() >= 6)))
		this->setState(eSharkState::HOMING);//ATTACK);
}

void SpaceShark::updateAttack(double deltaTime)
{


	navigateToTrain(deltaTime);
	
	//Change stage
	if ((timeInStage > attackTimes.y) || ((timeInStage > attackTimes.x) && (randomInt() >= 7)))
		this->setState(eSharkState::IDLE);//RETREAT);
}

void SpaceShark::updateRetreat(double deltaTime)
{
	
}

void SpaceShark::updateBackstage(double deltaTime)
{
	this->scared_times = 0;
	this->inBackstage = true;

	//Change stage
	if ((timeInStage > backStageTimes.y) || ((timeInStage > backStageTimes.x) && (randomInt() >= 6)))
		this->setState(eSharkState::IDLE);
	
		
}


void SpaceShark::updateHomingTarget(double deltaTime) {
	this->homingAngle += 1.0* deltaTime*10;
	if (homingAngle > 360.0)
		homingAngle -=360.0;
	Vector3 trainPos = this->trainHandler->getCarPosition(0);
	float xDispl = cos(DEG2RAD*homingAngle) * this->homingRadius;
	float yDispl = sin(DEG2RAD*homingAngle) * this->homingRadius;
	this->homingTarget = Vector3(trainPos.x + xDispl, trainPos.y, trainPos.z + yDispl);
	std::cout << homingAngle << "         "<<"\r";
	
}


void SpaceShark::homingGoToRadius(double deltaTime) {
	Vector3 pos = this->meshEntity->getPosition();
	Vector3 target = this->homingTarget;
	Vector3 dir = (target-pos).normalize();
	Vector3 translation = dir *this->homingSpeed;
	this->meshEntity->model.setFrontAndOrthonormalize(dir);
	this->meshEntity->model.translateGlobal(translation.x, translation.y, translation.z);
	this->meshEntity->model.setUpAndOrthonormalize(Vector3(0, 1, 0));
	this->meshEntity->modifyScale(.1);
}

void SpaceShark::homingRotate(double deltaTime) {
	Vector3 pos = this->meshEntity->getPosition();
	Vector3 target = this->homingTarget;
	Vector3 translation = (target-pos);
	Vector3 dir = translation.normalize();

	
	
	this->meshEntity->setPosition(target);
	this->meshEntity->model.setFrontAndOrthonormalize(dir);
	//this->meshEntity->model.setUpAndOrthonormalize(Vector3(0, 1, 0));
	this->meshEntity->modifyScale(.1);
}

void SpaceShark::updateHoming(double deltaTime)
{
	this->updateHomingTarget(deltaTime);
	float dist = abs(this->train_separation - homingRadius);
	//std::cout << dist << std::endl;
	if (dist>6.0) {
		this->homingGoToRadius(deltaTime);
	}
	else {
		this->homingRotate(deltaTime);
	}
	this->train_separation = this->meshEntity->getPosition().distance(this->trainHandler->getCarPosition(0));
	if ((timeInStage > homingTimes.y) || ((timeInStage > homingTimes.x) && (randomInt() >= 6)))
		this->setState(eSharkState::ATTACK);
}


void SpaceShark::Update(double deltaTime)
{
	this->timeInStage += deltaTime;
	
	switch (this->state) {
		case eSharkState::IDLE:
			updateIdle(deltaTime);
			break;
		case eSharkState::ATTACK:
			updateAttack(deltaTime);
			break;
		case eSharkState::RETREAT:
			updateRetreat(deltaTime);
			break;
		case eSharkState::BACKSTAGE:
			updateBackstage(deltaTime);
			break;
		case eSharkState::HOMING:
			updateHoming(deltaTime);
			break;
	}
}

void SpaceShark::Render()
{
	if (this->state == eSharkState::BACKSTAGE) return;
	this->meshEntity->render();
	
}

void SpaceShark::setState(eSharkState state)
{
	this->state = state;
}

double SpaceShark::updateDisplAngle(double deltaTime,float speed) {
	sharkLerpPos += 1 * deltaTime * sharkDisplDirection*(speed*2);
	if (sharkLerpPos > 1) {
		sharkLerpPos = 1;
		sharkDisplDirection = -1;
	}
	else if (sharkLerpPos < 0) {
		sharkLerpPos = 0;
		sharkDisplDirection = 1;
	}
	auto easingFunction = getEasingFunction(EaseInOutQuad);
	double toReturn= easingFunction(sharkLerpPos)*30;
	return toReturn;

	
	
}

void SpaceShark::navigateToTrain(double deltaTime) {
	Vector3 trainPos = this->trainHandler->getCarPosition(0);
	Vector2 trainPos2D(trainPos.x, trainPos.z);
	
	Vector2 difVec2D= (trainPos2D - this->position).normalize();
	Vector3 difVec3D(difVec2D.x, 0, difVec2D.y);
	

	this->meshEntity->model.setUpAndOrthonormalize(Vector3(0, 1, 0));
	this->meshEntity->model.setFrontAndOrthonormalize(difVec3D);
	Vector3 movement= difVec3D * deltaTime * (this->speed/50);
	this->meshEntity->rotate(updateDisplAngle(deltaTime, speed));
	//this->meshEntity->model.translateGlobal(movement.x,movement.y,movement.z);
	this->meshEntity->model.translate(  0, 0,deltaTime*(this->speed*50 ));
	Vector3 pos= this->meshEntity->getPosition();
	this->position= Vector2(pos.x,pos.z);
	this->height= pos.y;
	this->train_separation= (this->position - trainPos2D).length();
	this->meshEntity->modifyScale(.1);
	
	

	

	
}