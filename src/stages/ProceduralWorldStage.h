#pragma once
#include "../framework.h"
#include "Stage.h"



class TrackHandler;
class TrainHandler;
class Scene;
class GroupEntity;
class CubeMap;
class InventoryHandler;

class SpaceShark;
class Player;
class SoundManager;
class GUImanager;


enum class eSceneryType {
	ROCK1,
	ROCK2,
	ROCK3,
	ROCK4,
	PLANK,
	GOLD,
	COAL


};

struct sceneryData {
	GroupEntity* scenery;
	eSceneryType type;
	std::vector<Vector3> positions;

	sceneryData(std::vector<Vector3>& positions, eSceneryType type, bool useRockShader = true);

};

class ProceduralWorldStage :
	public Stage
{
private:
	TrackHandler* trackHandler = NULL;
	TrainHandler* trainHandler = NULL;
	SpaceShark* spaceShark = NULL;
	Player* player;
	InventoryHandler* inventoryHandler;
	SoundManager* soundManager;
	GUImanager* guiManager = NULL;;

	bool showPickText = false;
	bool showHornText = false;
	std::vector<sceneryData> scenery;
	CubeMap* cubeMap = NULL;

	const int max_scenery_types = 7;

	void loadAssets();

	void generateProceduralScenery();

	void renderScenery();
public:
	ProceduralWorldStage(Scene* scene);
	ProceduralWorldStage(Scene* scene, TrainHandler* trainHandler);
	~ProceduralWorldStage();
	void initStage();

	void initSpaceShark();

	bool isPlayerNearResource();

	Vector4 getNearResource();

	void getResource(Vector4 data);

	void checkHorn();

	void update(double deltaTime);
	void renderUI();
	void render();
};
