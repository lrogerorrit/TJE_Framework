#pragma once
#include "Stage.h"
#include "../framework.h"

class TrainHandler;

class ItemPlacementStage :
    public Stage
{
	private:
		TrainHandler* trainHandler = NULL;
		void placeItem(Vector2 pos);
	
	
	public:
		ItemPlacementStage(Scene* scene);
		
		void initStage();

		void update(double deltaTime);
		void render();
		
	
};

