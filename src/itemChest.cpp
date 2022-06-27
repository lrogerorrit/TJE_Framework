#include "itemChest.h"
#include "Player.h"
#include "InventoryHandler.h"

itemChest::itemChest(int slotNum,Vector3 pos) {
	init(slotNum,pos);
}

itemChest::itemChest(int slotNum)
{
	init(slotNum,Vector3());
}

void itemChest::init(int slotNum,Vector3 pos)
{
	this->player = Player::instance;
	this->numSlots = slotNum;
	slotData.reserve(slotNum);
	for (int i = 0; i < slotNum; ++i) {
		slotData.push_back(itemSlotData());
	}
}

void itemChest::update(double deltaTime)
{
	
}

void itemChest::renderGUI()
{
}

bool itemChest::isFull()
{
	return false;
}
