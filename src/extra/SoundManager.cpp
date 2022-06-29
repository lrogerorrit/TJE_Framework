#include "SoundManager.h"
#include "../game.h"
#include "../Player.h"
#include "Audio.h"



SoundManager* SoundManager::instance = NULL;

void SoundManager::initBass3D()
{
	if (BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		//error abriendo la tarjeta de sonido...
	}
}

SoundManager::SoundManager()
{
	//this->pos = new BASS_3DVECTOR(0, 0, 0);
	initBass3D();
	vel = new BASS_3DVECTOR(0, 0, 0);
	front= new BASS_3DVECTOR(0, 0, 0);
	top= new BASS_3DVECTOR(0, 0, 0);
	instance = this;
	player = Player::instance;
	
}

Audio* SoundManager::loadAudio(const char* fileName)
{
	loadedAudios[fileName] = new Audio(fileName);
}



void SoundManager::playAudio(const char* fileName,float volume)
{
	if(!loadedAudios[fileName])
		{
		loadAudio(fileName);
	}
	loadedAudios[fileName]->play(volume);
}

void SoundManager::stopAudio(const char* fileName)
{
	if(loadedAudios[fileName])
		{
		loadedAudios[fileName]->stop();
	}
}

void SoundManager::UpdatePlayerPos() {

	Vector3 camPos = Game::instance->player->position;
	Vector3 playerVel = Game::instance->player->speedVector;
	Vector3 playerFront = Game::instance->camera->eye;
	Vector3 playerTop = Game::instance->camera->up;

	pos = new BASS_3DVECTOR(camPos.x, camPos.y, camPos.z);
	vel = new BASS_3DVECTOR(playerVel.x, playerVel.y, playerVel.z);
	front = new BASS_3DVECTOR(playerFront.x, playerFront.y, playerFront.z);
	top = new BASS_3DVECTOR(playerTop.x, playerTop.y, playerTop.z);
	BASS_Set3DPosition(pos, vel, front, top);
	BASS_Apply3D();

}

void SoundManager::playSound(const char* fileName, Vector3 soundPos,float volume) {
	Vector3 camPos = Game::instance->player->position;
	double dist = sqrt(dot(soundPos, camPos));
	BASS_Set3DFactors(dist, 1.0, 1.0);
	BASS_Apply3D();

	this->playAudio(fileName,volume);
	//audios.Play(fileName);


}




void SoundManager::playSoundFromPos(const char* fileName, Vector3 soundPos, Vector3 camPos, Vector3 playerVel, Vector3 playerFront, Vector3 playerTop) {
	double dist = sqrt(dot(soundPos, camPos));
	BASS_Set3DFactors(dist, 1.0, 1.0);
	BASS_3DVECTOR* pos = new BASS_3DVECTOR(camPos.x, camPos.y, camPos.z);
	BASS_3DVECTOR* vel = new BASS_3DVECTOR(playerVel.x, playerVel.y, playerVel.z);
	BASS_3DVECTOR* front = new BASS_3DVECTOR(playerFront.x, playerFront.y, playerFront.z);
	BASS_3DVECTOR* top = new BASS_3DVECTOR(playerTop.x, playerTop.y, playerTop.z);
	BASS_Set3DPosition(pos, vel, front, top);
	BASS_Apply3D();


	//audios.Play(fileName);
	playAudio(fileName);
}


