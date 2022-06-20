#include "SpaceShark.h"
#include "TrainHandler.h"
#include "extra/commonItems.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "extra/easing.h"

Vector2 idleTimes(1, 3);//(10, 30);
Vector2 attackTimes(20, 30);//(20, 60);
Vector2 backStageTimes(10, 30);
Vector2 homingTimes(10, 30);//(50, 80);
float SCARE_CONSTANT = 1.23;





SpaceShark* SpaceShark::instance = nullptr;


void updateModelRotFrontRight(Matrix44& model, Vector3 front, Vector3 right,bool setUpDefault=false) {
	front.normalize();
	right.normalize();
	model.m[8] = front.x;
	model.m[9] = front.y;
	model.m[10] = front.z;
	model.m[0] = right.x;
	model.m[1] = right.y;
	model.m[2] = right.z;
	if (setUpDefault) {
		model.m[4] = 0;
		model.m[5] = 1;
		model.m[6] = 0;
	}
	
}

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
	//this->meshEntity->model.setUpAndOrthonormalize(Vector3(0, 1, 0));
	this->meshEntity->model.translateGlobal(pos.x,pos.y,pos.z);
	this->meshEntity->modifyScale(.1);
	

	if ((timeInStage > idleTimes.y) || ((timeInStage > idleTimes.x) && (randomInt() >= 6))) {
		this->from_attack = false;
		//this->setState(eSharkState::ATTACK);
		this->setState(eSharkState::HOMING);//ATTACK);
	}
}

void SpaceShark::scareShark(Vector3 pos) {
	Vector3 sharkPos = this->meshEntity->getPosition();
	float dist = sharkPos.distance(pos);
	float invDist = clamp(this->maxScareDistance - dist, 0, this->maxScareDistance);
	this->scareLevels += SCARE_CONSTANT * invDist;
}

void SpaceShark::updateAttack(double deltaTime)
{


	
	navigateToTrain(deltaTime);
	if (this->scareLevels > 100) {
		this->scareLevels = 0;
		this->attackCount = 0;
		this->setState(eSharkState::RETREAT);
	}
	else if (this->train_separation<=attackDamageDistance) {
		std::cout << "Attack count " << this->attackCount << std::endl;
		//TODO: Remove health from train
		if (this->attackCount >= this->maxAttackTimes) {
			this->attackCount = 0;
			this->scareLevels = 0;
			this->setState(eSharkState::RETREAT);
		}
		else {
			this->attackCount++;
			this->from_attack = true;
			this->setState(eSharkState::HOMING);
		}
		
	}

	//Change stage
	if ((timeInStage > attackTimes.y) || ((timeInStage > attackTimes.x) && (randomInt() >= 7)))
		this->setState(eSharkState::RETREAT);
}

void SpaceShark::updateRetreat(double deltaTime)
{
	Vector3 trainPos = trainHandler->getCarPosition(0);
		Vector3 pos = this->meshEntity->getPosition();
	if (retreatFirst) {
		Vector3 dir = (pos - trainPos).normalize();
		Vector3 right = dir.cross(Vector3(0, 1, 0));
		retreatFirst = false;
		this->retreatDir= dir;
		//updateModelRotFrontRight(this->meshEntity->model, dir, right, true);
		this->meshEntity->rotate(179);
		//this->meshEntity->model.setFrontAndOrthonormalize(this->retreatDir.cross(Vector3(0,1,0)));
		//this->meshEntity->model.scale(.1,.1,.1);
	}
	
	//this->meshEntity->model.setUpAndOrthonormalize(Vector3(0, 1, 0));
	float movementSpeed = 1;
	this->meshEntity->model.translate(0, 0, 1);
	
	//this->meshEntity->model.translateGlobal(movementSpeed*dir.x,movementSpeed*dir.y,movementSpeed*dir.z);

	Vector2 trainPos2D(trainPos.x, trainPos.z);
	
	this->train_separation= (Vector2(pos.x,pos.z)-trainPos2D).length();
	std::cout << "Train Separation: " << this->train_separation << "                 \r";
	
	
	if (this->train_separation > retreatSeparation) {
		this->setState(eSharkState::BACKSTAGE);
	}
	
	
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
	Vector3 right= dir.cross(Vector3(0,1,0));
	Vector3 translation = dir *this->homingSpeed;
	//this->meshEntity->model.setFrontAndOrthonormalize(Vector3(dir.x,0,dir.z));
	
	this->meshEntity->model.translateGlobal(translation.x, translation.y, translation.z);
	
	updateModelRotFrontRight(this->meshEntity->model, dir, right);
	this->meshEntity->model.m[4] = 0;
	this->meshEntity->model.m[5] = 1;
	this->meshEntity->model.m[6] = 0;
	this->meshEntity->modifyScale(Vector3(.1, .1, .1));
}

void SpaceShark::homingRotate(double deltaTime) {
	Vector3 pos = this->meshEntity->getPosition();
	Vector3 target = this->homingTarget;
	Vector3 translation = (target-pos);
	Vector3 dir = translation.normalize();
	Vector3 right= dir.cross(Vector3(0,1,0));

	
	
	this->meshEntity->setPosition(target);

	updateModelRotFrontRight(this->meshEntity->model, dir, right);

	/*this->meshEntity->model.m[8] = dir.x;
	this->meshEntity->model.m[9] = dir.y;
	this->meshEntity->model.m[10] = dir.z;
	this->meshEntity->model.m[0] = right.x;
	this->meshEntity->model.m[1] = right.y;
	this->meshEntity->model.m[2] = right.z;*/
	
	//this->meshEntity->model.setFrontAndOrthonormalize(Vector3(dir.x,0.0f,dir.z));
	//this->meshEntity->model.setUpAndOrthonormalize(Vector3(0, 1, 0));
	this->meshEntity->modifyScale(.1);
	if ((timeInStage > homingTimes.y) || ((timeInStage > homingTimes.x) && (randomInt() >= 6)))
		this->setState(eSharkState::ATTACK);
}

void SpaceShark::updateHoming(double deltaTime)
{
	this->updateHomingTarget(deltaTime);
	float dist = abs(this->train_separation - homingRadius);
	this->train_separation = this->meshEntity->getPosition().distance(this->trainHandler->getCarPosition(0));
	if (dist> homingSnapDistance) {
		this->homingGoToRadius(deltaTime);
	}
	else {
		this->homingRotate(deltaTime);
	}
	
}


void SpaceShark::Update(double deltaTime)
{
	if(this->prevState!= this->state)
	{
		std::cout << "changed state to ";
		switch (this->state) {
			case eSharkState::IDLE:
				std::cout << "IDLE\n";
				break;
			case eSharkState::ATTACK:
				std::cout << "ATTACK\n";
				break;
			case eSharkState::RETREAT:
				std::cout << "RETREAT\n";
				break;
			case eSharkState::BACKSTAGE:
				std::cout << "BACKSTAGE\n";
				break;
			case eSharkState::HOMING:
				std::cout << "HOMING\n";
				break;
		}
		this->timeInStage = 0;
	}
	this->prevState = this->state;
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
	//TODO: REMOVE THIS IS DEBUG;
	Vector3 pos = this->meshEntity->getPosition();
	glColor3f(1, 0, 0);
	glPointSize(20);
	glBegin(GL_POINTS);
	glVertex3f(pos.x,pos.y,pos.z);
	
	glEnd();
	
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
	Vector3 right = difVec3D.cross(Vector3(0, 1, 0));
	Vector3 pos= this->meshEntity->getPosition();
	
	
	//this->meshEntity->model.setUpAndOrthonormalize(Vector3(0, 1, 0));
	//this->meshEntity->model.setFrontAndOrthonormalize(difVec3D);
	
	updateModelRotFrontRight(this->meshEntity->model, difVec3D, right,true);
	
	Vector3 movement= difVec3D * deltaTime * (this->speed/50);
	this->meshEntity->rotate(updateDisplAngle(deltaTime, speed));
	//this->meshEntity->model.translateGlobal(movement.x,movement.y,movement.z);
	this->meshEntity->model.translate(  0, 0,deltaTime*(this->speed*50 ));
	this->position= Vector2(pos.x,pos.z);
	this->height= pos.y;
	this->train_separation= (this->position - trainPos2D).length();
	this->meshEntity->modifyScale(.1);
	std::cout << "Train Separation: " << this->train_separation << "           \r";
	

	

	
}