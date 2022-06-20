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
	
	struct slotData {
		ePickupType type;
		int pos;
		int quantity;
	};
	int const maxQuantityPerSlot = 4;
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
	
	InventoryHandler();
	
	
};

