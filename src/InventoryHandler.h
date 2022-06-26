#pragma once
#include "texture.h"
#include "mesh.h"
#include "shader.h"

enum ePickupType {
	empty,
	wood,
	iron,
	stone,
	coal
};

const int slotPixelSize = 128;
const int nCol = 5;
const int nRow = 2;
struct slotData {
	ePickupType type;
	int quantity;
	void renderSlot(int posX, int posY, int size);
	Texture* tex;
	slotData(ePickupType type, int quant, const char* path);
};

class InventoryHandler
{
public:
	
	const int maxQuantityPerSlot = 12;
	std::vector<slotData> inventory;

	int getTotalQuantity(ePickupType type);
	int addToInventory(ePickupType type, int quantity);  //returns int to know how many did we succesfully added
	int removeFromInventory(ePickupType type, int quantity); //returns int to know how many did we succesfully remove
	int addToInventory(ePickupType type);
	void removeFromInventory(ePickupType type);
	void removeAllFromInventory(ePickupType type);
	void removeAllFromInventory();
	bool isFull();
	bool isEmpty();

	

	void render();
	
	InventoryHandler();
	
	
};

