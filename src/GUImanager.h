#pragma once
#include "entities/EntityInclude.h"

//#include "game.h"

class Mesh;
class Camera;

enum class Gui_State {
	NONE,
	INSIDE,
	CLICKING
};

class GUImanager
{
private:
	Mesh* quad=NULL;
	Camera* cam2d;
	
public:
	
	static GUImanager* instance;
	
	GUImanager();

	void drawQuad(Mesh* quad, Texture* tex);
	void drawQuad(Mesh* quad, Texture* tex, Vector3 position, Vector3 scale, Vector3 rotation);

	int doFrame(Vector2 position,Vector2 size,Vector4 color);
	int doButton(Vector2 position,Vector2 size,Vector4 color);
	int doTextButton(Vector2 position, Vector2 size, std::string text);
	int doImage(Vector2 position, Vector2 size, std::string text);
	
	
	
	
	
};

