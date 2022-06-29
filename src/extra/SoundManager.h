#pragma once

#include "../framework.h"
#include <iostream>
#include <string>
#include <map>

struct BASS_3DVECTOR;
class Audio;
class Player;


class SoundManager
{

private:
	Player* player;
	Audio* loadAudio(const char* fileName);
	void playAudio(const char* fileName,float volume=1);
	void stopAudio(const char* fileName);
	BASS_3DVECTOR* pos = nullptr;
	BASS_3DVECTOR* vel = nullptr;
	BASS_3DVECTOR* front = nullptr;
	BASS_3DVECTOR* top = nullptr;
		
public:
	std::map<std::string, Audio*> loadedAudios;
	static SoundManager* instance;

	

	SoundManager();
	
	void initBass3D();

	void playSound(const char* fileName, Vector3 soundPos, float volume=1);
	void UpdatePlayerPos();
	void playSoundFromPos(const char* fileName, Vector3 soundPos, Vector3 camPos, Vector3 playerVel, Vector3 playerFront, Vector3 playerTop);

};

