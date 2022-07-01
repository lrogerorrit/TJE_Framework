#pragma once
#include "Stage.h"
#include "../framework.h"

class GUImanager;
class Texture;
class SoundManager;

class WinStage :
    public Stage
{
private:
	GUImanager* guiManager=NULL;
	Texture* texture=NULL;
	SoundManager* soundManager;
	
public:
	
	WinStage();
	

	void render();
	void update(double elapsed_seconds);
	void initStage();
		
};

