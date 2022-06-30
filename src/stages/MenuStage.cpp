#include "MenuStage.h"
#include "../input.h"
#include "../game.h"
#include "../GUImanager.h"


MenuStage::MenuStage():Stage()
{
	initStage();
}

void MenuStage::render()
{
	Vector2 screenCenter = Vector2(gameInstance->window_width / 2, gameInstance->window_height / 2);
	guiManager->doText(Vector2(screenCenter.x - 150, screenCenter.y + 150), "Press SPACE to start!", 4);
}

void MenuStage::update(double elapsed_seconds)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		this->gameInstance->moveToStageNum(1);
	}
}

void MenuStage::initStage()
{
	this->guiManager = GUImanager::instance;
}
