#include "DepositionStage.h"
#include "../Scene.h"
#include "../mesh.h"
#include "../game.h"
#include "../Player.h"
#include "../trackHandler.h"
#include "../TrainHandler.h"
#include "../GUImanager.h"
#include "../InventoryHandler.h"
#include "../input.h"



DepositionStage::DepositionStage()
{
	this->initStage();
}

DepositionStage::DepositionStage(Scene* scene)
{
	this->scene = scene;
	this->initStage();
}

void DepositionStage::initStage() {
	this->guiManager = GUImanager::instance;
	this->stageType = eStageType::DEPO;
	this->player = Player::instance;
	
	this->trainHandler = TrainHandler::instance;
	this->game = Game::instance;
	this->inventoryHandler= InventoryHandler::instance;
	std::cout << inventoryHandler->inventory.size();
	

	
	loadAssets();

	//TODO: Generate spline randomly
	std::vector<Vector3> positions({ Vector3(0, 0, 0), Vector3(5, 0, 2), Vector3(10, 0, 5), Vector3(12, 0, 10), Vector3(15, 0, 20), Vector3(20,0,30), Vector3(35,0,38), Vector3(40,0,45),
		Vector3(50,0,50),Vector3(80,0,100),Vector3(120,0,150),Vector3(180,0,160),Vector3(200,0,200),Vector3(250,0,300),Vector3(400,0,350),Vector3(500,0,500)
		});
	
	
	
	
	

}

void DepositionStage::update(double seconds_elapsed) {

	if (getIsUpgradeGuiVisible()) {
		;
		if (Input::wasKeyPressed(SDL_SCANCODE_B)) {
			gameInstance->moveToStageNum(2);
		}

		if (this->player->position.distance(Vector3(5, 13, -15)) < 5) {
			this->guiManager->doText(Vector2(gameInstance->window_width * .5, 4), "E - Open Upgrade Menu", 3);
			if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
				this->setIsUpgradeGuiVisible(true);
			}
		}
		//trainHandler->update(seconds_elapsed);
		Stage::update(seconds_elapsed);
	}
}

void DepositionStage::render() {
	
	
	//trainHandler->render();
	Stage::render();

	//Render ui:
	this->renderUpgradeMenu();
	renderConfirmationMenu();
	
	
}



Vector4 getButtonColor(int state) {
	switch (state) {
	case 0:
		return Vector4(0.5, 0.5, 0.5, 1);
	case 1:
		return Vector4(0.8, 0.8, 0.8, 1);
	case 2:
		return Vector4(1, 0.7, 0.7, 1);
	}
	return Vector4();
}

std::string generateRequiresTest(Vector2 data) {
	std::string result = "";
	switch((int) data.x) {
	case 4:
		result += "Plank";
		break;
	case 5:
		result += "Iron";
		break;
	case 6:
		result += "Gold";
		break;
	default:
		result += "N/A";
		break;
	}
	
	result += ": " + std::to_string((int) data.y) + "\n";	
	return result;
}

bool DepositionStage::checkHasEnoughResources() {
	std::cout << "si\n";
	for (int i = 0; i < confirmationData.size(); ++i) {
		auto& data = confirmationData[i];
		std::cout << "DATA: " << data.x << ", " << data.y << std::endl;
		std::cout << inventoryHandler->inventory.size() << std::endl;
		if (inventoryHandler->getTotalQuantity((ePickupType)(data.x - 3)) < data.y) {
			std::cout << "Has not enough\n";
			return false;
		}
	}
	std::cout << "Has enough\n";
	return true;
	
}

void DepositionStage::renderConfirmationMenu()
{
	if (!confirmGuiVisible) return;
	Vector2 screenCenter = Vector2(game->window_width / 2, game->window_height / 2);
	guiManager->doFrame(screenCenter, Vector2(400, 500), Vector4(.2, .2, .2, 1));
	guiManager->doText(Vector2(screenCenter.x - 135, screenCenter.y - 240), "Attention", 6);
	guiManager->doText(Vector2(screenCenter.x - 175, screenCenter.y - 190), "This upgrade requires:", 3);
	for (int i = 0; i < confirmationData.size(); ++i) {
		guiManager->doText(Vector2(screenCenter.x - 185, screenCenter.y - 150 + i * 40), generateRequiresTest(confirmationData[i]), 3);
	}

	int state1 = guiManager->doTextButton(Vector2(game->window_width /3, screenCenter.y + 170), Vector2(110, 80), "Buy", 3, Vector3(0, 0, 0), getButtonColor(confButtonState.x));
	int state2 = guiManager->doTextButton(Vector2(2*game->window_width /3, screenCenter.y + 170), Vector2(110, 80), "Close", 3, Vector3(0, 0, 0), getButtonColor(confButtonState.y));


	if (state1 == 2) {
		if (checkHasEnoughResources()) {
			confirmGuiVisible = false;
			upgradeGuiVisible = false;
			buttonState = Vector4(0, 0, 0, 0);
			confButtonState = Vector2(0, 0);
			selectedOption = -1;
			for (int i = 0; i < confirmationData.size(); ++i) {
				auto& data = confirmationData[i];
				
				inventoryHandler->removeFromInventory((ePickupType)data.x, (int)data.y);
				
			}
			
			switch (selectedOption){
				case(0):
					trainHandler->fixTrain();
					break;
				case(1):
					trainHandler->addToMaxHealth(10);
					break;
				case(2):
					game->keyState += 1;
					break;
				
			}
			
			
		}
		else {
			confButtonState.x = 2;
		}
	}
	
	if (state2 == 2) {
		confirmGuiVisible = false;
		buttonState = Vector4(0,0,0,0);
		confButtonState = Vector2(0,0);
		selectedOption = -1;
		return;
	}
	
	confButtonState = Vector2(state1, state2);
	

	
}

void DepositionStage::renderUpgradeMenu()
{
	if (!upgradeGuiVisible || confirmGuiVisible) return;
	Vector2 screenCenter = Vector2(game->window_width / 2, game->window_height / 2);
	guiManager->doFrame(screenCenter, Vector2(400, 500), Vector4(.2, .2, .2, 1));
	guiManager->doText(Vector2(screenCenter.x - 125, screenCenter.y - 240), "Upgrades", 6);
	int state1=guiManager->doTextButton(Vector2(screenCenter.x - 125, screenCenter.y - 110), Vector2(100, 100), " Buy",4,Vector3(0,.8,0),getButtonColor(buttonState.x));
	guiManager->doText(Vector2(screenCenter.x - 50, screenCenter.y - 140), "Repair Train", 3);
	int state2=guiManager->doTextButton(Vector2(screenCenter.x - 125, screenCenter.y + 0), Vector2(100, 100), " Buy",4,Vector3(0,.8,0),getButtonColor(buttonState.y));
	guiManager->doText(Vector2(screenCenter.x - 50, screenCenter.y -30), "Add Max Health\n(+20)", 3);	
	
	std::string str = "Buy Key\n(" + (std::to_string(game->keyState+1)) + "/6)";
	int state3 = guiManager->doTextButton(Vector2(screenCenter.x - 125, screenCenter.y + 110), Vector2(100, 100), " Buy", 4, Vector3(0, .8, 0), getButtonColor(buttonState.z));
	guiManager->doText(Vector2(screenCenter.x - 50, screenCenter.y + 100), str, 3);
	
	int state4 = guiManager->doTextButton(Vector2(screenCenter.x, screenCenter.y + 200), Vector2(250, 50), "     Close", 3, Vector3(0, 0, 0), getButtonColor(buttonState.w));
	if (state4==2){	
		upgradeGuiVisible = false;
		buttonState = Vector4(0, 0, 0, 0);
		return;
	}
	
	if (state1==2) {
		this->confirmationData.clear();
		this->confirmationData.push_back(Vector2(4, 8));
		this->confirmationData.push_back(Vector2(5, 2));
		this->confirmationData.push_back(Vector2(6, 1));
		confirmGuiVisible = true;
		selectedOption = 0;
	}
	else if (state2 == 2) {
		this->confirmationData.clear();
		this->confirmationData.push_back(Vector2(4, 10));
		this->confirmationData.push_back(Vector2(5, 10));
		this->confirmationData.push_back(Vector2(6, 10));
		confirmGuiVisible = true;
		selectedOption = 1;
	}
	else if (state3 == 2) {
		this->confirmationData.clear();
		this->confirmationData.push_back(Vector2(4, 15));
		this->confirmationData.push_back(Vector2(5, 15));
		this->confirmationData.push_back(Vector2(6, 15));
		confirmGuiVisible = true;
		selectedOption = 2;
	}
	buttonState = Vector4(state1, state2, state3,state4);
}

void DepositionStage::setIsUpgradeGuiVisible(bool state)
{
	this->upgradeGuiVisible = state;
}





void DepositionStage::loadAssets() {
	//Get slope and cube meshes
	Mesh::Get("data/assets/cube.obj");
	Mesh::Get("data/assets/slope.obj");
		

}