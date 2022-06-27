#include "GUImanager.h"
#include "mesh.h"
#include "game.h"
#include "texture.h"
#include "camera.h"
#include "shader.h"
#include "framework.h"
#include "utils.h"

GUImanager* GUImanager::instance = NULL;

GUImanager::GUImanager()
{
	instance = this;
	quadShader= Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	quad = new Mesh();
	whiteTexture = Texture::getWhiteTexture();
	game= Game::instance;
	windowHeight= game->window_height;
	windowWidth= game->window_width;
	cam2d = new Camera();
	
}

void GUImanager::drawQuad(Mesh* quad, Texture* tex)
{
	int windowWidth = Game::instance->window_width;
	int windowHeight = Game::instance->window_height;
	
	Camera cam2D;
	cam2D.setOrthographic(0, windowWidth, windowHeight,0, -1, 1);
	
	Shader* a_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	if (!a_shader) return;
	a_shader->enable();

	a_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	a_shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	if (tex != NULL) {
		a_shader->setUniform("u_texture", tex, 0);
	}
	a_shader->setUniform("u_time", time);
	a_shader->setUniform("u_text_tiling", 1.0f);
	
	quad->render(GL_TRIANGLES);
	
	a_shader->disable();
	
}

void GUImanager::drawQuad(Mesh* quad,Texture* tex, Vector3 position, Vector3 scale, Vector3 rotation)
{	
	int windowWidth = Game::instance->window_width;
	int windowHeight = Game::instance->window_height;

	
	cam2d->setOrthographic(0, windowWidth, windowHeight, 0, -1, 1);

	Shader* a_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	if (!a_shader) return;
	a_shader->enable();

	a_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	a_shader->setUniform("u_viewprojection", cam2d->viewprojection_matrix);
	if (tex != NULL) {
		a_shader->setUniform("u_texture", tex, 0);
	}
	a_shader->setUniform("u_time", time);
	a_shader->setUniform("u_text_tiling", 1.0f);

	quad->render(GL_TRIANGLES);

	a_shader->disable();

	
}

void GUImanager::update()
{
	windowWidth = Game::instance->window_width;
	windowHeight = Game::instance->window_height;
	cam2d->setOrthographic(0, windowWidth, windowHeight, 0, -1, 1);
}

int GUImanager::doFrame(Vector2 position, Vector2 size, Vector4 color)
{
	
	quad->createQuad(position.x, position.y ,size.x,size.y,false);
	Camera* old = Camera::current;
	cam2d->enable();
	Matrix44 mat = Matrix44::IDENTITY;
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	quadShader->enable();
	quadShader->setUniform("u_iRes", Vector2(1.0 / (float)this->windowWidth, 1.0 / (float)this->windowHeight));
	quadShader->setUniform("u_color", color);
	quadShader->setUniform("u_texture", whiteTexture, 0);
	quadShader->setUniform("u_viewprojection", cam2d->viewprojection_matrix);
	quadShader->setUniform("u_model", mat);	
	
	quad->render(GL_TRIANGLES);
	
	quadShader->disable();
	glEnable(GL_DEPTH_TEST);
	old->enable();
	return (int)Gui_State::NONE;
}

int GUImanager::doButton(Vector2 position, Vector2 size, Vector4 color)
{
	
	Vector2 mousePos = game->getMousePosition();
	Vector2 topLeft = Vector2(position.x - (size.x / 2.0), position.y - (size.y / 2.0));
	
	this->doFrame(position, size, color);
	if (mousePos.x > topLeft.x && mousePos.x < topLeft.x + size.x && mousePos.y > topLeft.y && mousePos.y < topLeft.y + size.y)
	{
		return game->isLeftMouseDown()?(int)Gui_State::CLICKING:(int)Gui_State::INSIDE;
	}
	else
	{
		return (int)Gui_State::NONE;
	}
	
}

int GUImanager::doTextButton(Vector2 position, Vector2 size, std::string text, Vector4 color)
{
	int state= doButton(position, size, color);
	drawText(position.x- (size.x / 2.2), position.y ,text,Vector3(1,1,1),4.0);
	return state;
}

int GUImanager::doImage(Vector2 position, Vector2 size, Texture* texture, Vector4 color)
{
	quad->createQuad(position.x, position.y, size.x, size.y, false);
	Camera* old = Camera::current;
	cam2d->enable();
	Matrix44 mat = Matrix44::IDENTITY;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	quadShader->enable();
	quadShader->setUniform("u_iRes", Vector2(1.0 / (float)this->windowWidth, 1.0 / (float)this->windowHeight));
	quadShader->setUniform("u_color", color);
	quadShader->setUniform("u_texture", texture, 0);
	quadShader->setUniform("u_viewprojection", cam2d->viewprojection_matrix);
	quadShader->setUniform("u_model", mat);

	quad->render(GL_TRIANGLES);

	quadShader->disable();
	glEnable(GL_DEPTH_TEST);
	old->enable();
	return (int)Gui_State::NONE;
}
