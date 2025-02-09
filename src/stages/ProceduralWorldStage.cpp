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

void ProceduralWorldStage::loadAssets() {
	Mesh::Get("data/assets/rocks/rock1.obj");
	Mesh::Get("data/assets/rocks/rock2.obj");
	Mesh::Get("data/assets/rocks/rock3.obj");
	Mesh::Get("data/assets/rocks/rock4.obj");
	Mesh::Get("data/assets/pickables/plank.obj");
}

void ProceduralWorldStage::generateProceduralScenery()
{
	Vector4 defaultCol = Vector4(1, 1, 1, 1);
	float scales[5] = { .5,.5,.5,.5,10 }; //{ .5,.5, .5,30 };
	float chances[5] = { 10,10,10,10,9 }; //{ .5,.5, .5,30 };
	float separation[5] = { 3,3,3,3,5 };
	Vector4 colors[5] = { defaultCol,defaultCol,defaultCol,defaultCol,Vector4(0.63,.49,0,1) };
	bool useRockShader[5] = { true,true,true,true,false };
	BeizerCurve* trackCurve = this->trackHandler->getActiveCurve();
	this->player = Player::instance;
	if (trackCurve == nullptr) return;
	
	float maxDistance = 10.0f;
	
	for (int ii=0; ii<5;++ii){
		eSceneryType scType = (eSceneryType)ii;
		std::vector<Vector3> positions;
		float len = trackCurve->arcLength;
		if (randomIntRange(0, 10) > chances[ii])continue;
		for (int i = 0; i <= trackCurve->segmentArray.size(); i+=separation[ii]) {
			segmentData& data= trackCurve->segmentArray[i];
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
		//this->scene->getRoot()->addChild(data.scenery);
	}
	
	
	//this->scene->getRoot()->addChild(data.scenery);
	
		
	
}

void ProceduralWorldStage::renderScenery()
{
	
	for (auto& sceneryEntity : this->scenery) {
		if ((int)sceneryEntity.type < 4) continue;
		sceneryEntity.scenery->render();
		
	}
	
}

ProceduralWorldStage::ProceduralWorldStage(Scene* scene):Stage(scene)
{
	initStage();
}

ProceduralWorldStage::ProceduralWorldStage(Scene* scene, TrainHandler* trainHandler):Stage(scene)
{
	this->trainHandler = trainHandler;
	initStage();
	
	
}

ProceduralWorldStage::~ProceduralWorldStage()
{
}

void ProceduralWorldStage::initStage()
{
	
	this->inventoryHandler = InventoryHandler::instance;
	this->soundManager = SoundManager::instance;
	stageType = eStageType::PROCEDURAL_WORLD;
	this->cubeMap = CubeMap::instance;
	this->loadAssets();

	this->trackHandler = TrackHandler::instance;

	//TODO: Generate spline randomly
	std::vector<Vector3> positions({ Vector3(0, 0, 0), Vector3(5, 0, 2), Vector3(10, 0, 5), Vector3(12, 0, 10), Vector3(15, 0, 20), Vector3(20,0,30), Vector3(35,0,38), Vector3(40,0,45),
		Vector3(50,0,50),Vector3(80,0,100),Vector3(120,0,150),Vector3(180,0,160),Vector3(200,0,200),Vector3(250,0,300),Vector3(400,0,350),Vector3(500,0,500)
		});
	BeizerCurve* curve = new BeizerCurve(positions);
	this->trackHandler->setActiveCurve(curve);
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
	
	for (int i = 4; i < max_scenery_types; ++i) {
		auto& data = this->scenery[i];
		for (int y = 0; y < data.positions.size(); ++y) {
			Vector3 pos= data.positions[y];
			if (pos.distance(player->getPosition()) < 10) {
				return Vector4(pos,i);
			}
		}
	}
	return Vector4(0,0,0,-1);
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
	ePickupType pickupType= getPickupTypeFromItem(data.w);
	if (!inventoryHandler->canAddItem(pickupType)) return; //TODO: Notify inv full;
	auto& scenerydata = this->scenery[data.w];
	for (int i = 0; i < scenerydata.positions.size(); ++i) {
		Vector3 pos= scenerydata.positions[i];
		if(pos.x==data.x && pos.y==data.y && pos.z==data.z) {
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
	float dist= hornPos.distance(plPos);
	
	if(Input::wasKeyPressed(SDL_SCANCODE_E) && dist<20) {
		std::cout << "pressed horn\n";
		soundManager->playSound("data/audio/soundEffects/horns.wav", this->trainHandler->getCarData(0).hornMesh->getGlobalMatrix().getTranslation());
		//soundManager->PlaySound("data/audio/soundEffects/horns.wav",hornPos);
		this->spaceShark->scareShark(hornPos);
		
	}
	
}



void ProceduralWorldStage::update(double deltaTime)
{
	if (trainHandler->getHealth() == 0) {
		//TODO: end game;
	}
	this->trackHandler->updatePosition(deltaTime);
	if (this->trainHandler)
		this->trainHandler->update(deltaTime);
	checkHorn();
	spaceShark->Update(deltaTime);

	Vector4 resourceData= getNearResource();
//	std::cout<<resourceData.x<<", "<<resourceData.y<<", "<<resourceData.z<<", "<<resourceData.w<<std::endl;
	if (resourceData.w != -1 && Input::wasKeyPressed(SDL_SCANCODE_E)) {
		getResource(resourceData);
	}
	
	if (Input::wasKeyPressed(SDL_SCANCODE_I)) {
		inventoryHandler->setOpen(!inventoryHandler->getIsOpen());
	}

	Stage::update(deltaTime);


	
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

}

sceneryData::sceneryData(std::vector<Vector3>& positions, eSceneryType type,bool useRockShader)
{
	std::vector<Matrix44> matrices;
	for (Vector3& pos : positions) {
		Matrix44 mat = Matrix44::IDENTITY;
		mat.setTranslation(pos.x, pos.y, pos.z);
		matrices.push_back(mat);
	}
	this->type = type;
	//TODO: Load Mesh and texture from data
	Mesh* mesh;
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
				
					
	}
	
	
	
	Texture* texture = Texture::Get("data/assets/rocks/texture.png");
	Shader* shader = Shader::Get("data/shaders/basic.vs", useRockShader?"data/shaders/rockShader.fs": "data/shaders/texture.fs");
	GroupEntity* group = new GroupEntity(mesh, texture, shader, matrices);
	this->positions = positions;
	
	this->scenery = group;
}
