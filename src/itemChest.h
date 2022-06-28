#pragma once
#include "framework.h"
#include "extra/commonItems.h"

class MeshEntity;
class Player;
class InventoryHandler;


struct itemSlotData {
	int pos;
	int quantity=0;
	ePickupType itemType;
	bool isEmpty = true;
};

class itemChest
{
private:
	int numSlots=4;
	int maxItemsPerSlot;
	MeshEntity* mesh=NULL;
	Player* player = NULL;
	std::vector<itemSlotData> slotData;
	InventoryHandler* inventoryHandler=NULL;
public:
	itemChest(int slotNum, Vector3 pos);
	itemChest(int slotNum);
	

	void init(int slotNum,Vector3 pos);
	
	bool isPlayerNear;
	void update(double deltaTime);
	void renderGUI();

	bool isFull();
	
	bool canAddObject(ePickupType type);
	
	void addObject(int slot, ePickupType type,int quantity);

	void removeObject(int slot);

};

