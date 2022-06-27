#include "GUImanager.h"
#include "mesh.h"
#include "game.h"
#include "texture.h"
#include "camera.h"

GUImanager* GUImanager::instance = NULL;

GUImanager::GUImanager()
{
	instance = this;
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
	a_shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	if (tex != NULL) {
		a_shader->setUniform("u_texture", tex, 0);
	}
	a_shader->setUniform("u_time", time);
	a_shader->setUniform("u_text_tiling", 1.0f);

	quad->render(GL_TRIANGLES);

	a_shader->disable();

	
}

int GUImanager::doFrame(Vector2 position, Vector2 size, Vector4 color)
{
	return 0;
}
