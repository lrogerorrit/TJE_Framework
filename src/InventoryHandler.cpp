#include "InventoryHandler.h"

int InventoryHandler::getTotalQuantity(ePickupType type)
{
	int out = 0;
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].type = type) {
			out += inventory[i].quantity;
		}
	}
	return out;
}

int InventoryHandler::addToInventory(ePickupType type, int quantity)
{
	int initQuantity = quantity;
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].quantity+quantity>maxQuantityPerSlot) {
			if (inventory[i].type == type) {
				quantity = maxQuantityPerSlot - inventory[i].quantity;
				inventory[i].quantity = maxQuantityPerSlot;
			}
			else if (inventory[i].type == ePickupType::empty) {
				inventory[i].type = type;
				inventory[i].quantity = maxQuantityPerSlot;
				quantity -= maxQuantityPerSlot;
			}
		}
		else {
			if (inventory[i].type == type) {
				inventory[i].quantity += quantity;
			}
			else if (inventory[i].type == ePickupType::empty) {
				inventory[i].type = type;
				inventory[i].quantity += quantity;
			}			
			return 0;
			
		}
	}
	return initQuantity-quantity;
}

int InventoryHandler::removeFromInventory(ePickupType type, int quantity) {
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
	return quantity;
}

bool InventoryHandler::addToInventory(ePickupType type)
{
	return addToInventory(type, 1);
}

void InventoryHandler::removeFromInventory(ePickupType type)
{
	removeFromInventory(type, 1);
}

void InventoryHandler::removeAllFromInventory(ePickupType type)
{
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].type = type) {
			inventory[i].quantity = 0;
		}
	}
}

void InventoryHandler::removeAllFromInventory()
{
	for (int i = 0; i < inventory.size(); ++i) {
		inventory[i].quantity = 0;
	}
}

bool InventoryHandler::isFull()
{
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].quantity < maxQuantityPerSlot) {
			return false;
		}
	}
	return true;
}

bool InventoryHandler::isEmpty()
{
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].quantity > 0) {
			return false;
		}
	}
	return true;
}


