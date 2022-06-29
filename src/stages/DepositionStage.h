#pragma once
#include "../framework.h"
#include "Stage.h"




class TrainHandler;
class TrackHandler;
class GUImanager;
class Game;
class InventoryHandler;


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
		TrackHandler* trackHandler = NULL;
		InventoryHandler* inventoryHandler;
		bool upgradeGuiVisible = true;//false;
		bool confirmGuiVisible = true;
		GUImanager* guiManager = NULL;
		Game* game;
		
		std::vector<Vector2> confirmationData;
		
		Vector4 buttonState = Vector4(0, 0, 0,0);
		Vector2 confButtonState = Vector2(0, 0);
		int selectedOption = -1;
		
		void loadAssets();
		
	public:

		DepositionStage();
		DepositionStage(Scene* scene);


		void initStage();
		


		void update(double deltaTime);
		void render();
		void renderConfirmationMenu();
		void renderUpgradeMenu();
		bool checkHasEnoughResources();

		bool getIsUpgradeGuiVisible(){ return upgradeGuiVisible; }
		void setIsUpgradeGuiVisible(bool state);

};

