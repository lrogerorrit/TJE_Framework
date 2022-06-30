#include "ProceduralWorldStage.h"
#include "../Scene.h"
#include "../TrackHandler.h"
#include "../curves.h"
#include "../entities/EntityInclude.h"
#include "../trainHandler.h"
#include "../CubeMap.h"
#include "../SpaceShark.h"
#include "../Player.h"
#include "../InventoryHandler.h"
#include "../input.h"
#include "../extra/SoundManager.h"
#include "../framework.h"
#include "../game.h"
#include "../GUImanager.h"

void ProceduralWorldStage::loadAssets() {
	Mesh::Get("data/assets/rocks/rock1.obj");
	Mesh::Get("data/assets/rocks/rock2.obj");
	Mesh::Get("data/assets/rocks/rock3.obj");
	Mesh::Get("data/assets/rocks/rock4.obj");
	Mesh::Get("data/assets/pickables/plank.obj");
	Mesh::Get("data/assets/pickables/GoldBar.obj");
	Mesh::Get("data/assets/pickables/coal.obj");

}

void ProceduralWorldStage::generateProceduralScenery()
{
	//
	proceduralParent->removeAllChildren();
	this->scenery.clear();

	Vector4 defaultCol = Vector4(1, 1, 1, 1);
	float scales[7] = { .5,.5,.5,.5,10,.5,10 }; //{ .5,.5, .5,30 };
	float chances[7] = { 10,10,10,10,9,7,6 }; //{ .5,.5, .5,30 };
	float separation[7] = { 3,3,3,3,5,6,7 };
	Vector4 colors[7] = { defaultCol,defaultCol,defaultCol,defaultCol,Vector4(0.63,.49,0,1),Vector4(1,.77,.08,1),Vector4(.2,.2,.2,1) };
	bool useRockShader[7] = { true,true,true,true,false,true,true };
	BeizerCurve* trackCurve = this->trackHandler->getActiveCurve();
	this->player = Player::instance;
	if (trackCurve == nullptr) return;

	float maxDistance = 10.0f;

	for (int ii = 0; ii < 7; ++ii) {

		eSceneryType scType = (eSceneryType)ii;
		std::vector<Vector3> positions;
		float len = trackCurve->arcLength;
		for (int i = 0; i <= trackCurve->segmentArray.size(); i += separation[ii]) {
			if (randomIntRange(0, 10) > chances[ii])continue;
			if (i >= trackCurve->segmentArray.size()) continue;
			segmentData& data = trackCurve->segmentArray[i];
			Matrix44 mat = Matrix44::IDENTITY;
			Vector3 pos = data.position;
			std::cout << "pos 1: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
			mat.setTranslation(pos.x, pos.y, pos.z);

			int segmentNum = trackCurve->getSegmentFromMu(data.mu);
			Vector3 dir = trackCurve->getSegmentDirection(segmentNum);

			Vector3 right = dir.cross(Vector3(0, 1, 0));
			mat.setUpAndOrthonormalize(Vector3(0, 1, 0));
			mat.setFrontAndOrthonormalize(dir);



			float xDistance = ((random() - .5) * 2.0) * 15.0;
			float yDistance = ((random() - .5) * 2.0) * 15.0;
			float zDistance = ((random() - .5) * 2.0) * 1.0;

			if (xDistance >= 0) xDistance += 15; else xDistance -= 15;
			//if (yDistance >= 0) yDistance += 15; else yDistance -= 15;

			//mat.rotate(xAngle, Vector3(1, 0, 0));
			//mat.rotate(zAngle, Vector3(0, 0, 1));
			//Vector3 offset mat->r (xAngle, yAngle, 0);
			Vector3 newPos = mat.getTranslation();
			newPos += right * xDistance;
			newPos += pos * zDistance;
			newPos += Vector3(0, yDistance, 0);
			pos = newPos;
			//pos = mat.getTranslation()  mat.rightVector() * distance;
			//print pos to console
			std::cout << "pos 2: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
			positions.push_back(pos);
		}
		sceneryData data = sceneryData(positions, scType);
		data.scenery->setColor(colors[ii]);
		this->scenery.push_back(data);
		data.scenery->groupScale(scales[ii]);
		if (ii > 3)
			data.scenery->ingoreCollision = true;
		proceduralParent->addChild(data.scenery);
	}


	//this->scene->getRoot()->addChild(data.scenery);



}

void ProceduralWorldStage::renderScenery()
{

	for (auto& sceneryEntity : this->scenery) {
		//sceneryEntity.scenery->render();

	}

}

ProceduralWorldStage::ProceduralWorldStage(Scene* scene) :Stage(scene)
{
	initStage();
}

ProceduralWorldStage::ProceduralWorldStage(Scene* scene, TrainHandler* trainHandler) :Stage(scene)
{
	this->trainHandler = trainHandler;
	initStage();


}

ProceduralWorldStage::~ProceduralWorldStage()
{
}

void ProceduralWorldStage::initStage()
{
	proceduralParent = new Entity();
	this->guiManager = GUImanager::instance;
	this->inventoryHandler = InventoryHandler::instance;
	this->soundManager = SoundManager::instance;
	stageType = eStageType::PROCEDURAL_WORLD;
	this->cubeMap = CubeMap::instance;
	this->loadAssets();

	this->trackHandler = TrackHandler::instance;

	//TODO: Generate spline randomly
	std::vector<Vector3> positions({ Vector3(0, 0, 0), Vector3(5, 0, 2), Vector3(10, 0, 5), Vector3(12, 0, 10), Vector3(15, 0, 20), Vector3(20,0,30), Vector3(35,0,38), Vector3(40,0,45),
		Vector3(50,0,50),Vector3(80,0,100),Vector3(120,0,150),Vector3(180,0,160),Vector3(200,0,200),Vector3(250,0,300),Vector3(400,0,350),Vector3(500,0,500),Vector3(600,0,600),
		Vector3(700,0,700),Vector3(800,0,800),Vector3(900,0,900),Vector3(1000,0,1000),Vector3(1100,0,1100),Vector3(1200,0,1200),Vector3(1300,0,1300),
		Vector3(1400,0,1400),Vector3(1500,0,1500),Vector3(1600,0,1600),Vector3(1700,0,1700),Vector3(1800,0,1800),Vector3(1900,0,1900),Vector3(2000,0,2000),
		
		});
	BeizerCurve* curve = new BeizerCurve(positions);
	this->trackHandler->setActiveCurve(curve);
	this->getScene()->getRoot()->addChild(proceduralParent);
	generateProceduralScenery();
}

void ProceduralWorldStage::initSpaceShark() {
	if (!SpaceShark::instance)
		new SpaceShark();
	this->spaceShark = SpaceShark::instance;

}

bool ProceduralWorldStage::isPlayerNearResource() {
	return true;
}

Vector4 ProceduralWorldStage::getNearResource() { //(x,y,z,type)

	for (int i = 0; i < 3; ++i) {
		auto& data = this->scenery[i + 4];
		for (int y = 0; y < data.positions.size(); ++y) {
			Vector3 pos = data.positions[y];
			if (pos.distance(player->getPosition()) < 10) {
				return Vector4(pos, i + 4);
			}
		}
	}
	return Vector4(0, 0, 0, -1);
}

ePickupType getPickupTypeFromItem(int type) {
	switch (type) {
	case 4:
		return ePickupType::wood;

	default:
		return ePickupType::empty;
	}
}

void ProceduralWorldStage::getResource(Vector4 data)
{
	//TODO: if inventory has space;
	ePickupType pickupType = getPickupTypeFromItem(data.w);
	if (!inventoryHandler->canAddItem(pickupType)) return; //TODO: Notify inv full;
	auto& scenerydata = this->scenery[data.w];
	for (int i = 0; i < scenerydata.positions.size(); ++i) {
		Vector3 pos = scenerydata.positions[i];
		if (pos.x == data.x && pos.y == data.y && pos.z == data.z) {
			scenerydata.positions.erase(scenerydata.positions.begin() + i);
			scenerydata.scenery->removeObject(i);
			inventoryHandler->addToInventory(pickupType);


			//TODO: Play pickup sound
			return;
		}
	}
}

void ProceduralWorldStage::checkHorn() {
	Vector3 hornPos = this->trainHandler->getCarData(0).hornMesh->getGlobalMatrix().getTranslation();

	Vector3 plPos = this->player->getPosition();
	float dist = hornPos.distance(plPos);
	this->showHornText = (dist < 10);
	if (Input::wasKeyPressed(SDL_SCANCODE_E) && dist < 10) {
		std::cout << "pressed horn\n";
		soundManager->playSound("data/audio/soundEffects/horns.wav", this->trainHandler->getCarData(0).hornMesh->getGlobalMatrix().getTranslation());
		//soundManager->PlaySound("data/audio/soundEffects/horns.wav",hornPos);
		this->spaceShark->scareShark(hornPos);

	}

}



void ProceduralWorldStage::update(double deltaTime)
{
	
	if (changeCooldown > 0) {
		changeCooldown -= deltaTime;
		canChangeStage = false;
	}
	else {
		changeCooldown = 0;
		canChangeStage = true;
	}
	
	if (trainHandler->getHealth() == 0) {
		gameInstance->moveToStageNum(4);
	}

	if (canChangeStage && !inventoryHandler->getIsOpen() && Input::wasKeyPressed(SDL_SCANCODE_B)) {
		changeCooldown = 120;
		gameInstance->moveToStageNum(3);
		return;
		
	}
	this->trackHandler->updatePosition(deltaTime);
	if (this->trainHandler)
		this->trainHandler->update(deltaTime);
	checkHorn();
	spaceShark->Update(deltaTime);

	player->testCollisions();
	player->updatePlayer(deltaTime);
	
	Vector4 resourceData = getNearResource();
	
	showPickText = (resourceData.w != -1.0f);
	
	//	std::cout<<resourceData.x<<", "<<resourceData.y<<", "<<resourceData.z<<", "<<resourceData.w<<std::endl;
	if (resourceData.w != -1 && Input::wasKeyPressed(SDL_SCANCODE_E)) {
		getResource(resourceData);
	}



	if (Input::wasKeyPressed(SDL_SCANCODE_I)) {
		inventoryHandler->setOpen(!inventoryHandler->getIsOpen());
	}
	
	if (isPreparingForLoop) {
		if (loopStage == 0) { //Fade
			blockerTransparency -= 1 * deltaTime;
			if (blockerTransparency <= 0) {
				blockerTransparency = 0;
			
				loopStage = 1;
			}
		}
		else if (loopStage == 1) { //Move back and regenerate resources;
			this->generateProceduralScenery();
			this->trainHandler->setCarPosition(0);
			loopStage = 2;
		}
		else if (loopStage == 2) {
			blockerTransparency += 1 * deltaTime;
			std::cout << blockerTransparency << "\n";

			if (blockerTransparency >= 1) {
				blockerTransparency = 1;
				loopStage = 0;
				isPreparingForLoop = false;
			}
		}
	}
	//std::cout << trainHandler->getCurveProgress() << "\n";
	if (trainHandler->getCurveProgress() > .9 && !isPreparingForLoop) {
		std::cout << "Preparing for loop\n";
		isPreparingForLoop = true;
	}

	Stage::update(deltaTime);



}

void ProceduralWorldStage::renderUI() {
	if (this->showPickText && !inventoryHandler->getIsOpen()) {
		guiManager->doText(Vector2(gameInstance->window_width * .6, 35),"E - Pick Item",3);
	}else if(this->showHornText)
		guiManager->doText(Vector2(gameInstance->window_width * .6, 35), "E - Play Horn", 3);
	else if (this->changeCooldown > 0) {
		std::string str = "Teleport Cooldown: " + std::to_string(int(changeCooldown));
		guiManager->doText(Vector2(gameInstance->window_width * .54, 35), str, 3);
	}
	else {
		guiManager->doText(Vector2(gameInstance->window_width * .7, 35), "B - Teleport", 3);
	

	}
		
	if (isPreparingForLoop && blockerTransparency <1) {
		guiManager->doFrame(Vector2(gameInstance->window_width / 2, gameInstance->window_height / 2), Vector2(800, 600), Vector4(0, 0, 0, blockerTransparency));
	}
}

void ProceduralWorldStage::render()
{
	cubeMap->Render();
	renderScenery();
	//trainHandler->render();
	trackHandler->renderTrack();
	spaceShark->Render();
	Stage::render();


	//UI Render
	inventoryHandler->render();
	trainHandler->renderHealth();
	renderUI();

}

sceneryData::sceneryData(std::vector<Vector3>& positions, eSceneryType type, bool useRockShader)
{
	std::vector<Matrix44> matrices;
	for (Vector3& pos : positions) {
		Matrix44 mat = Matrix44::IDENTITY;
		mat.setTranslation(pos.x, pos.y, pos.z);
		matrices.push_back(mat);
	}
	this->type = type;
	//TODO: Load Mesh and texture from data
	Mesh* mesh = nullptr;
	switch (type)
	{
	case eSceneryType::ROCK1:
		mesh = Mesh::Get("data/assets/rocks/rock1.obj");
		break;
	case eSceneryType::ROCK2:
		mesh = Mesh::Get("data/assets/rocks/rock2.obj");
		break;
	case eSceneryType::ROCK3:
		mesh = Mesh::Get("data/assets/rocks/rock3.obj");
		break;
	case eSceneryType::ROCK4:
		mesh = Mesh::Get("data/assets/rocks/rock4.obj");
		break;
	case eSceneryType::PLANK:
		mesh = Mesh::Get("data/assets/pickables/plank.obj");
		break;
	case eSceneryType::GOLD:
		mesh = Mesh::Get("data/assets/pickables/GoldBar.obj");
		break;
	case eSceneryType::COAL:
		mesh = Mesh::Get("data/assets/pickables/coal.obj");
		break;


	}



	Texture* texture = Texture::Get("data/assets/rocks/texture.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", useRockShader ? "data/shaders/rockShader.fs" : "data/shaders/texture.fs");
	GroupEntity* group = new GroupEntity(mesh, texture, shader, matrices);
	this->positions = positions;

	this->scenery = group;
}