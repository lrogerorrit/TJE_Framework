#pragma once
#include "entities/EntityInclude.h"

//#include "game.h"

class Mesh;
class Camera;
class Shader;
class Game;

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
	Shader* quadShader;
	Texture* whiteTexture;
	Game* game=NULL;

	int windowWidth=0;
	int windowHeight=0;

	bool isGuiOpen = false;
	
public:
	
	static GUImanager* instance;
	
	GUImanager();

	void drawQuad(Mesh* quad, Texture* tex);
	void drawQuad(Mesh* quad, Texture* tex, Vector3 position, Vector3 scale, Vector3 rotation);

	void update();

	inline bool getIsGuiOpen() { return isGuiOpen; };
	void setIsGuiOpen(bool state) { this->isGuiOpen = state; }

	int doFrame(Vector2 position,Vector2 size,Vector4 color);
	int doButton(Vector2 position,Vector2 size,Vector4 color);
	int doTextButton(Vector2 position, Vector2 size, std::string text, int scale=2, Vector3 textColor=Vector3(0,0,0), Vector4 color = Vector4(1, 1, 1, 1));
	int doImage(Vector2 position, Vector2 size, Texture* texture, Vector4 color= Vector4(1,1,1,1));
	int doText(Vector2 position, std::string text, int scale=2, Vector3 color = Vector3(1, 1, 1));
	
	
	
	
	
};

