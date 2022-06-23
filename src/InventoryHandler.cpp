#include "InventoryHandler.h"
#include "game.h"

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

void InventoryHandler::render()
{
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].quantity > 0) {
			inventory[i].renderSlot(i%nCol,floor(i/nCol), slotPixelSize);
		}
	}
}

Vector2 slotToWorldCoord(int x, int y) {
	int xOffset = Game::instance->window_width / 9;
	int yOffset = Game::instance->window_height / 6;
	return Vector2(x * Game::instance->window_width/7 + xOffset, y*Game::instance->window_height/2 + yOffset);
}

void InventoryHandler::slotData::renderSlot(int posX, int posY, int size)
{
	Vector2 pos = slotToWorldCoord(posX, posY);
	
	int windowHeight = Game::instance->window_height;
	int windowWidth = Game::instance->window_width;
	
	Mesh quad;
	quad.createQuad(pos.x, pos.y, size, size, false);
	
	Camera cam2D;
	cam2D.setOrthographic(0, windowWidth, windowHeight, 0, -1, 1);
	
	Shader* a_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	
	if (!a_shader) return;
	a_shader->enable();
	a_shader->setUniform("u_color", Vector4(1,1,1,1));
	a_shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	
	if (this->tex != NULL) {
		a_shader->setUniform("u_texture", this->tex,0);
	}
	a_shader->setUniform("u_time", time);
	a_shader->setUniform("u_tex_tiling", 1.0f);
	a_shader->setUniform("u_model", Matrix44());
	
	quad.render(GL_TRIANGLES);

	a_shader->disable();
}

InventoryHandler::slotData::slotData(ePickupType type, int quant, const char* path)
{
	this->type = type;
	this->quantity = quant;
	Texture* tex = Texture::Get(path);
	this->tex = tex;
}

InventoryHandler::InventoryHandler() {
	for (int i = 0; i < nCol * nRow; ++i){
		inventory.push_back(slotData(ePickupType::empty, 0, "data/assets/cube.png"));
	}
}
