#pragma once
#include "game.h"
#include "Player.h"

class InventoryHandler
{
	enum ePickupType {
		empty,
		wood,
		iron,
		stone,
		coal
	};
	
	const int slotPixelSize = 32;
	struct slotData {
		ePickupType type;
		int pos;
		int quantity;
		void renderSlot(int posX, int posY, int size);
		Texture* tex;
	};
	const int maxQuantityPerSlot = 12;
	std::vector<slotData> inventory;

	int getTotalQuantity(ePickupType type);
	int addToInventory(ePickupType type, int quantity);
	int removeFromInventory(ePickupType type, int quantity); //returns int to know how many did we succesfully remove
	bool addToInventory(ePickupType type);
	void removeFromInventory(ePickupType type);
	void removeAllFromInventory(ePickupType type);
	void removeAllFromInventory();
	bool isFull();
	bool isEmpty();
	
	void render();
	
	InventoryHandler();
	
	
};

