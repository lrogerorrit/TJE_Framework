#pragma once
#include <bass.h>
#include "../framework.h"
#include "Audio.h"

class SoundManager
{
public:

	void initBass3D();

	void PlaySound(const char* fileName, Vector3 soundPos, Vector3 camPos, Vector3 playerVel, Vector3 playerFront, Vector3 playerTop);
	void PlaySound(const char* fileName, Vector3 soundPos);

	Audio audios;
	

	

};

