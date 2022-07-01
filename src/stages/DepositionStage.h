#pragma once
#include "../framework.h"
#include "Stage.h"




class TrainHandler;
class TrackHandler;
class GUImanager;
class Game;
class InventoryHandler;
class Player;
class CubeMap;
class SoundManager;

enum class eBlockType {
	Wall1,
	Wall2,
	Floor1,
	Floor2,
	Ramp
};


class DepositionStage :
	public Stage
{	
	private:
		TrainHandler* trainHandler = NULL;
		Player* player;
		InventoryHandler* inventoryHandler = NULL;
		bool upgradeGuiVisible = false;//false;
		bool confirmGuiVisible = false;
		GUImanager* guiManager = NULL;
		Game* game;
		CubeMap* cubeMap = NULL;
		
		bool showScreenText = false;

		std::vector<Vector2> confirmationData;
		
		Vector4 buttonState = Vector4(0, 0, 0,0);
		Vector2 confButtonState = Vector2(0, 0);
		int selectedOption = -1;
		
		void loadAssets();
		SoundManager* soundManager;
		
	public:
		bool isFirst = true;

		DepositionStage();
		DepositionStage(Scene* scene);


		void initStage();
		


		void update(double deltaTime);
		void renderUI();
		void render();
		void onTeleport();
		void renderConfirmationMenu();
		void renderUpgradeMenu();
		bool checkHasEnoughResources();

		bool getIsUpgradeGuiVisible(){ return upgradeGuiVisible; }
		void setIsUpgradeGuiVisible(bool state);

		void reset() {
			isFirst = true;
		}

};

