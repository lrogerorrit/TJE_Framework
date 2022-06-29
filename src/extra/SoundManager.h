#pragma once
#include <bass.h>
#include "../framework.h"
#include "Audio.h"

class SoundManager
{
public:
	static SoundManager* instance;

	SoundManager();
	
	void initBass3D();

	void PlaySound(const char* fileName, Vector3 soundPos);
	void UpdatePlayerPos();
	void PlaySoundFromPos(const char* fileName, Vector3 soundPos, Vector3 camPos, Vector3 playerVel, Vector3 playerFront, Vector3 playerTop);

	BASS_3DVECTOR* pos;
	BASS_3DVECTOR* vel;
	BASS_3DVECTOR* front;
	BASS_3DVECTOR* top;

	Audio audios;
	

	

};

