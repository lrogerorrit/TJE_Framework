#include "InventoryHandler.h"

int InventoryHandler::getTotalQuantity(ePickupType type)
{
	int out;
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].type = type) {
			out += inventory[i].quantity;
		}
	}
	return out;
}

bool InventoryHandler::addToInventory(ePickupType type, int quantity)
{
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].quantity+quantity>maxQuantityPerSlot) {
			quantity -= inventory[i].quantity;
			inventory[i].quantity = maxQuantityPerSlot;
			
		}
		else {
			inventory[i].quantity += quantity;
			return true;
			
		}
	}
	return false;
}

int InventoryHandler::removeFromInventory(ePickupType type, int quantity)
{
	int initQuantity = quantity;
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].type = type) {
			if (inventory[i].quantity - quantity < 0) {
				quantity -= inventory[i].quantity;
				inventory[i].quantity = 0;
				
			}
			else {
				inventory[i].quantity -= quantity;
				return initQuantity;
			}
		}
	}
	return initQuantity - quantity;
}

bool InventoryHandler::addToInventory(ePickupType type)
{
	return addToInventory(type, 1);
}

void InventoryHandler::removeFromInventory(ePickupType type)
{
	removeFromInventory(type, 1);
}


