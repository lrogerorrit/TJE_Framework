#include "SoundManager.h"
#include "../framework.h"
#include "../game.h"



void SoundManager::initBass()
{
	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		//error abriendo la tarjeta de sonido...
	}
}




