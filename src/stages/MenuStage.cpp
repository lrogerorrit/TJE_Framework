#include "MenuStage.h"
#include "../input.h"
#include "../game.h"
#include "../GUImanager.h"
#include "../texture.h"


MenuStage::MenuStage():Stage()
{
	initStage();
}

void MenuStage::render()
{
	Vector2 screenCenter = Vector2(gameInstance->window_width / 2, gameInstance->window_height / 2);
	guiManager->doImage(screenCenter, Vector2(800, 600), this->texture);
	//guiManager->doText(Vector2(screenCenter.x - 150, screenCenter.y + 150), "Press SPACE to start!", 4);
}

void MenuStage::update(double elapsed_seconds)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		this->gameInstance->moveToStageNum(2,true);
	}
}

void MenuStage::initStage()
{
	this->texture = Texture::Get("data/images/menu.png");
	this->guiManager = GUImanager::instance;
	
}
