#pragma once
#include "Stage.h"
#include "../framework.h"

class GUImanager;

class MenuStage :
    public Stage
{
private:
	GUImanager* guiManager=NULL;
	
	
public:
	
	MenuStage();
	

	void render();
	void update(double elapsed_seconds);
	void initStage();
		
};

