#include "InventoryHandler.h"
#include "game.h"
#include "GUImanager.h"

InventoryHandler* InventoryHandler::instance = NULL;

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

bool InventoryHandler::canAddItem(ePickupType type)
{
	if (this->isFull()) return false;

	for (int i = 0; i < inventory.size(); ++i) {
		auto data = inventory[i];
		if(data.type==ePickupType::empty|| (data.type==type && data.quantity< maxQuantityPerSlot))
			return true;
	}
	return false;
	
}

int InventoryHandler::addToInventory(ePickupType type, int quantity)
{
	int initQuantity = quantity;
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].quantity+quantity>maxQuantityPerSlot && inventory[i].quantity<maxQuantityPerSlot) {
			if (inventory[i].type == type) {
				quantity = maxQuantityPerSlot - inventory[i].quantity;
				inventory[i].quantity = maxQuantityPerSlot;
			}
			else if (inventory[i].type == ePickupType::empty) {
				inventory[i].type = type;
				inventory[i].quantity = maxQuantityPerSlot;
				quantity -= maxQuantityPerSlot;
				switch (type) {
				case ePickupType::coal:
					inventory[i].tex = Texture::Get("data/inventory/coal.png");
					break;
				case ePickupType::iron:
					inventory[i].tex = Texture::Get("data/inventory/iron.png");
					break;
				case ePickupType::wood:
					inventory[i].tex = Texture::Get("data/inventory/wood.png");
					break;
				case ePickupType::stone:
					inventory[i].tex = Texture::Get("data/inventory/rock.jpg");
					break;
				}
			}
		}
		else if(inventory[i].quantity < maxQuantityPerSlot) {
			if (inventory[i].type == type) {
				inventory[i].quantity += quantity;
				return 0;
			}
			else if (inventory[i].type == ePickupType::empty) {
				inventory[i].type = type;
				inventory[i].quantity += quantity;
				switch (type) {
				case ePickupType::coal:
					inventory[i].tex = Texture::Get("data/inventory/coal.png");
					break;
				case ePickupType::iron:
					inventory[i].tex = Texture::Get("data/inventory/iron.png");
					break;
				case ePickupType::wood:
					inventory[i].tex = Texture::Get("data/inventory/wood.png");
					break;
				case ePickupType::stone:
					inventory[i].tex = Texture::Get("data/inventory/rock.jpg");
					break;
				}
				return 0;
			}			
			
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
				inventory[i].tex = Texture::Get("data/assets/cube.png");
				inventory[i].type = ePickupType::empty;

			}
			else {
				inventory[i].quantity -= quantity;
				return initQuantity;
			}
		}
	}
	return quantity;
}

int InventoryHandler::addToInventory(ePickupType type)
{
	return addToInventory(type, 1);
}

void InventoryHandler::removeFromInventory(ePickupType type)
{
	removeFromInventory(type, 1);
}

void InventoryHandler::removeFromSlot(int slot)
{
	auto& data = inventory[slot];
	if (data.quantity == 1) {
		data.quantity = 0;
		data.tex = Texture::Get("data/assets/cube.png");
		data.type = ePickupType::empty;
	}
	else if(data.quantity>1) {
		data.quantity--;
	}
}

void InventoryHandler::emptySlot(int slot)
{
	auto& data = inventory[slot];
	data.quantity = 0;
	data.tex = Texture::Get("data/assets/cube.png");
	data.type = ePickupType::empty;
}

void InventoryHandler::removeAllFromInventory(ePickupType type)
{
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].type = type) {
			inventory[i].quantity = 0;
			inventory[i].tex = Texture::Get("data/assets/cube.png");
			inventory[i].type = ePickupType::empty;
		}
	}
}

void InventoryHandler::removeAllFromInventory()
{
	for (int i = 0; i < inventory.size(); ++i) {
		inventory[i].quantity = 0;
		inventory[i].tex = Texture::Get("data/assets/cube.png");
		inventory[i].type = ePickupType::empty;
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

void InventoryHandler::renderOptions()
{
	
	guiManager->doFrame(Vector2(game->window_width / 2,game->window_height / 2), Vector2(400, 300), Vector4(.5, .5, .5, 1));
	
	if (guiManager->doTextButton(Vector2(game->window_width / 2, (game->window_height / 2) - 100),Vector2(300,60), "Drop",5) == 2) {
		if (activeSlotOptions != -1) {
			removeFromSlot(activeSlotOptions);
			activeSlotOptions = -1;
			showSlotOptions = false;
			
		}
		
	}

	if (guiManager->doTextButton( Vector2(game->window_width / 2, (game->window_height / 2) + 0),Vector2(300,60), "Drop all",5) == 2) {
		if (activeSlotOptions != -1) {
			emptySlot(activeSlotOptions);
			activeSlotOptions = -1;
			showSlotOptions = false;

		}

	}
	if (guiManager->doTextButton( Vector2(game->window_width / 2, (game->window_height / 2) + 100),Vector2(300,60), "Close",5) == 2) {
		if (activeSlotOptions != -1) {
			
			activeSlotOptions = -1;
			showSlotOptions = false;

		}

	}
	
}

void InventoryHandler::render()
{
	if(showSlotOptions)
		return renderOptions();
	Vector2 topLeft= Vector2((game->window_width/2)-200, (game->window_height/2)-125);
	guiManager->doFrame(Vector2(game->window_width/2, game->window_height/2), Vector2(400, 250), Vector4(.1, .1, .1, 1.0));
	for (int i = 0; i < 8;++i) {
		auto data = inventory[i];
		int xPos = i % 4;
		int yPos = floor(i / 4);
		if (guiManager->doButton(Vector2(topLeft.x + (xPos * 100) + 50, topLeft.y + 50 + yPos * 100), Vector2(70, 70), Vector4(1, 1, 1, 1))==2) {
			if (data.type != ePickupType::empty) {
				showSlotOptions = true;
				activeSlotOptions = i;
			}
		}
		std::string text = data.quantity >= 10 ? std::to_string(data.quantity) : ("0" + std::to_string(data.quantity));
		guiManager->doImage(Vector2(topLeft.x + (xPos * 100) + 50, topLeft.y + 50 + yPos * 100), Vector2(60, 60), data.tex, Vector4(1, 1, 1, 1));
		guiManager->doText(Vector2(topLeft.x + (xPos * 100) + 48, topLeft.y + 55 + yPos * 100), text,3, Vector3(1,1,1));
		
		
		
		
	}
}

void InventoryHandler::renderold()
{
	

	//Render all icons
	for (int i = 0; i < inventory.size(); ++i) {
		inventory[i].renderSlot(i%nCol,floor(i/nCol), slotPixelSize);
	}

	//Render background:
	Vector2 pos(slotPixelSize * (nCol + 1) / 2, slotPixelSize * (nRow + 1) / 2);

	int windowHeight = Game::instance->window_height;
	int windowWidth = Game::instance->window_width;

	Mesh quad;
	quad.createQuad(pos.x, pos.y, slotPixelSize * (nCol + 1), slotPixelSize * (nRow + 1), false);

	Camera cam2D;
	cam2D.setOrthographic(0, windowWidth, windowHeight, 0, -1, 1);

	Shader* a_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	if (!a_shader) return;
	a_shader->enable();
	a_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	a_shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	Texture* tex = Texture::Get("data/inventory/glassPanel_cornerTR.png");
	if (tex != NULL) {
		a_shader->setUniform("u_texture", tex, 0);
	}
	a_shader->setUniform("u_time", time);
	a_shader->setUniform("u_tex_tiling", 1.0f);
	a_shader->setUniform("u_model", Matrix44());

	quad.render(GL_TRIANGLES);

	a_shader->disable();
}

Vector2 slotToWorldCoord(int x, int y) {
	return Vector2(x * slotPixelSize + slotPixelSize, y*slotPixelSize + slotPixelSize);
}

void slotData::renderSlot(int posX, int posY, int size)
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

slotData::slotData(ePickupType type, int quant, const char* path)
{
	this->type = type;
	this->quantity = quant;
	Texture* tex = Texture::Get(path);
	this->tex = tex;
}

InventoryHandler::InventoryHandler() {
	instance = this;
	guiManager = GUImanager::instance;
	game = Game::instance;
	for (int i = 0; i < nCol * nRow; ++i){
		inventory.push_back(slotData(ePickupType::empty, 0, "data/assets/cube.png"));
	}
}
