/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "utils.h"

class Entity;
class Scene;
class Stage;
class GUImanager;
class Player;

class Game
{
	
private:
	std::vector<bool> prevMouseState;
	std::vector<bool> mouseState;
public:
	static Game* instance;
	

	//window
	SDL_Window* window;
	int window_width;
	int window_height;
	int keyState = 0;

	//some globals
	long frame;
    float runningtime;
	float elapsed_time;
	int fps;
	bool must_exit;
	bool cameraLocked;
	bool shouldCamBeLocked = false;;
	GUImanager* guiManager;

	Player* player;

	//some vars
	Camera* camera; //our global camera
	bool mouse_locked; //tells if the mouse is locked (not seen)

	Game( int window_width, int window_height, SDL_Window* window );

	//main functions
	void render( void );
	void update( double dt );

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);


	
	
	bool isKeyPressed(SDL_Keycode key);
	bool wasKeyPressed(SDL_Keycode key);
	Vector2 getMousePosition();
	bool isLeftMouseDown();
	bool isRightMouseDown();
	bool isMiddleMouseDown();
	bool wasLeftMouseDown();
	bool wasRightMouseDown();
	bool wasMiddleMouseDown();
	
	//custom vars	
	std::vector<Entity*> destroyQueue;
	std::vector<Scene*> scenes;
	Scene* activeScene=nullptr; //TODO: remove (or refactor since now in stage)
	Stage* activeStage = nullptr;
	

	//custom functions
	void addToDestroyQueue(Entity* ent);
	void setActiveScene(Scene* scene);
	void setActiveStage(Stage* stage);

	void moveToStageNum(int num);
};


#endif 