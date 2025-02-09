#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include <cmath>

#include "Scene.h"
#include "entities/EntityInclude.h"
#include "curves.h"
#include "stages/StagesInclude.h"
#include "TrackHandler.h"
#include "Player.h"
#include "TrainHandler.h"
#include "CubeMap.h"
#include "extra/SceneParser.h"
#include "stages/DepositionStage.h"
#include "SpaceShark.h"
#include <bass.h>
#include <time.h> 
#include "InventoryHandler.h"
#include"GUImanager.h"
#include "extra/SoundManager.h"



//some globals
Mesh* mesh = NULL;
Mesh* low_poly_mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;
float mouse_speed = 100.0f;
FBO* fbo = NULL;

Mesh* trolleyMesh = NULL;
Texture* trolleyTexture = NULL;
TrainHandler* trainHandler=NULL;

bool playTrack = false;
BeizerCurve* bc;
float trackPos = 0;

//Coses Uri
MeshEntity* ground;
MeshEntity* playerMesh;


Player* player= NULL;


InventoryHandler* inv = NULL;
bool invOpen = false;

//end coses uri
Game* Game::instance = NULL;


Scene* returnTestScene() {
	Scene* testScene= new Scene();
	
	Entity* baseEntity = new Entity();
	testScene->setRoot(baseEntity);
	
	MeshEntity* testMeshEntity = new MeshEntity(mesh,low_poly_mesh, texture, shader);
	testMeshEntity->modifyScale(.5);
	//baseEntity->addChild(testMeshEntity);
	testMeshEntity->model.translateGlobal(10, 30, 10);
	
	
	
	glPointSize(2);

	
	return testScene;
}

ProceduralWorldStage* testStage() {
	ProceduralWorldStage* stage = new ProceduralWorldStage(returnTestScene(),trainHandler);
	
	return stage;
}





void loadTestCar(Game* game) {
	trolleyMesh = Mesh::Get("data/assets/train/test_vehicle.obj");
	trolleyTexture = Texture::Get("data/assets/train/test_vehicle.png");
	Stage* stage = game->activeStage;
	//Entity* positionEntity = new Entity();
	Entity* positionEntity = new Entity();
	MeshEntity* trolleyEntity = new MeshEntity(trolleyMesh,trolleyTexture,Shader::Get("data/shaders/basic.vs","data/shaders/rockShader.fs"));
	
	//trolleyEntity->ingoreCollision = true;
	trolleyEntity->setCollisionMesh(Mesh::Get("data/assets/train/collisionMesh.obj"));
	positionEntity->addChild(trolleyEntity);
	
	stage->getScene()->getRoot()->addChild(positionEntity);
	positionEntity->forceCheckChilds = true;
	trainHandler->addCar(positionEntity,trolleyEntity);
	trolleyEntity->maxRenderDist = 10000000000000;
	trolleyEntity->modifyScale(10);
	
}

DepositionStage* loadTestDepo() {
	SceneParser* parser = SceneParser::instance;
	Scene* sc= parser->parseFile("data/export.scene");
	DepositionStage* stage = new DepositionStage(sc);
	return stage;
}

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	prevMouseState.reserve(3);
	prevMouseState.push_back(false);
	prevMouseState.push_back(false);
	prevMouseState.push_back(false);
	mouseState.reserve(3);
	mouseState.push_back(false);
	mouseState.push_back(false);
	mouseState.push_back(false);
	
	
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	runningtime = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	srand(time(NULL));

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
	
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	new GUImanager();
	new InventoryHandler();
	inv = InventoryHandler::instance;
	
	new SoundManager();
	new TrackHandler();
	new CubeMap();
	new SceneParser();
	guiManager = GUImanager::instance;
	
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	texture = new Texture();
 	texture->load("data/texture.tga");
	trainHandler = new TrainHandler();

	// example of loading Mesh from Mesh Manager
	mesh = Mesh::Get("data/sphere.obj");
	low_poly_mesh = Mesh::Get("data/sphereLow.obj");

	// example of shader loading using the shaders manager
	//this->setActiveScene(returnTestScene());
	
	

	//Coses Uri																					///////////
	cameraLocked = FALSE;

	
	player = new Player();


	inv->addToInventory(ePickupType::coal, 3);
	inv->addToInventory(ePickupType::wood, 13);
	inv->addToInventory(ePickupType::iron, 1);
	inv->addToInventory(ePickupType::stone, 3);
	inv->addToInventory(ePickupType::wood, 1);
	inv->addToInventory(ePickupType::iron, 14);
	
	//End coses uri																				//////////

	this->setActiveStage(loadTestDepo());

	loadTestCar(this);
	trainHandler->setActiveCurve(TrackHandler::instance->getActiveCurve());
	
	//this->setActiveScene(returnTestScene());
	
	ProceduralWorldStage* st = (ProceduralWorldStage*)this->activeStage;
	//st->initSpaceShark();

	

	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	//create model matrix for cube
	Matrix44 m;
	m.rotate(angle*DEG2RAD, Vector3(0, 1, 0));

	


	/*if (shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1,1,1,1));
		
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", m);
		shader->setUniform("u_time", runningtime);

		//do the draw call
		mesh->render( GL_TRIANGLES );

		//disable shader
		shader->disable();
	}*/
	//this->activeScene->render();

	//Coses URI										///
	


	//ground->render();

	



	//End coses URI									///


	this->activeStage->render();
	player->renderPlayer();
	
	
	//Draw the floor grid
	//drawGrid();


	//Draw inventory GUI
	//if (invOpen) inv->render();

	//guiManager->doTextButton(Vector2(this->window_width/2.0, this->window_height/2.0), Vector2(300, 300),"hi", Vector4(1, 0, 0, 1));

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	
	guiManager->update();
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	float playerSpeed = 5.0f * seconds_elapsed;
	float rotSpeed = 10.0f * seconds_elapsed;
	//example
	angle += (float)seconds_elapsed * 10.0f;

	//this->activeScene->update(seconds_elapsed);
	this->activeStage->update(seconds_elapsed);
	
	
	
	
	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	for (int i = 0; i < 3; ++i) {
		prevMouseState[i] = (bool) mouseState[i];
		
	}
	mouseState[0] = Input::mouse_state & SDL_BUTTON_LEFT;
	mouseState[1] = Input::mouse_state & SDL_BUTTON_MIDDLE;
	mouseState[2] = Input::mouse_state & SDL_BUTTON_RIGHT;
		
	
	/*
	if (playTrack) {
		//add constant speed taking into count size of segment
		
		int segmentNum = bc->getSegmentFromMu(trackPos);
		float speedMultiplier = (bc->speedConstant/bc->getSegmentDistance(segmentNum));
		std::cout << "(" << segmentNum << ") - " << speedMultiplier << std::endl;
		trackPos += 1 * seconds_elapsed*speedMultiplier;
		if(trackPos>1)
			trackPos = 0;
	}
	else
		trackPos = 0;
		*/


	//Coses URI
	if (guiManager->getIsGuiOpen())
		cameraLocked = false;
	else
		cameraLocked = shouldCamBeLocked;
	

	SDL_ShowCursor(!cameraLocked);
	
	
	
	/*
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);*/


	player->testCollisions();
	player->updatePlayer(seconds_elapsed);

	if (Input::wasKeyPressed(SDL_SCANCODE_O))
	{
		cameraLocked = !cameraLocked;
		shouldCamBeLocked = !shouldCamBeLocked;
		Input::centerMouse();
	};

	

	// end Coses URI
	
	//async input to move the camera around
	
	
	//to navigate with the mouse fixed in the middle
	/*if (mouse_locked)
		Input::centerMouse();*/
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	/*if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}*/
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

bool Game::isKeyPressed(SDL_Keycode key)
{
	return Input::isKeyPressed(key);
}

bool Game::wasKeyPressed(SDL_Keycode key)
{
	return Input::wasKeyPressed(key);
}

Vector2 Game::getMousePosition()
{
	return Input::mouse_position;
}

bool Game::isLeftMouseDown()
{
	return Input::isMousePressed(1);
}

bool Game::isRightMouseDown()
{
	return Input::isMousePressed(3);
}

bool Game::isMiddleMouseDown()
{
	return Input::isMousePressed(2);
}

bool Game::wasLeftMouseDown()
{
	bool state= this->mouseState[0]==1 && this->prevMouseState[0]==0;
	
	return state;
}

bool Game::wasRightMouseDown()
{
	return this->mouseState[2] && !this->prevMouseState[2];
}

bool Game::wasMiddleMouseDown()
{
	return this->mouseState[1] && !this->prevMouseState[1];
}

void Game::addToDestroyQueue(Entity* ent)
{
	this->destroyQueue.push_back(ent);
}

void Game::setActiveScene(Scene* scene)
{
	this->activeScene = scene;
}

void Game::setActiveStage(Stage* stage)
{
	this->activeStage = stage;
}

void Game::moveToStageNum(int num)
{
	switch (num) {
		case 0:
		//this->setActiveStage(this->stage0); //Menu Stage
			break;
		case 1:
			//this->setActiveStage(this->stage1); //Intro Stage
			break;
		case 2:
			//this->setActiveStage(this->stage2); //Procedural Stage
			break;
		case 3:
			//this->setActiveStage(this->stage3); //Depo Stage
			break;
		case 4:
			//this->setActiveStage(this->stage4); //Death Stage
			break;
		
	}
}

