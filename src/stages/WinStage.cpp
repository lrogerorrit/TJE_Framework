#include "WinStage.h"
#include "../input.h"
#include "../game.h"
#include "../GUImanager.h"
#include "../texture.h"
#include "../extra/SoundManager.h"


WinStage::WinStage():Stage()
{
	initStage();
}

void WinStage::render()
{
	
	Vector2 screenCenter = Vector2(gameInstance->window_width / 2, gameInstance->window_height / 2);
	guiManager->doImage(screenCenter,Vector2(800,600),this->texture);
}

void WinStage::update(double elapsed_seconds)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		this->gameInstance->moveToStageNum(0);
	}
}

void WinStage::initStage()
{
	this->texture= Texture::Get("data/images/victory.png");
	this->guiManager = GUImanager::instance;
	soundManager = SoundManager::instance;
}
