#pragma once
#include "entities/EntityInclude.h"
#include "mesh.h"
#include "game.h"

class GUImanager
{
public:
	
	void drawQuad(Mesh* quad, Texture* tex);
	void drawQuad(Mesh* quad, Texture* tex, Vector3 position, Vector3 scale, Vector3 rotation);
	
	
	
	
};

