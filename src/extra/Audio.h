#pragma once
#include <bass.h>
#include "../framework.h"
#include "../game.h"
#include <map>
#include <string>
class Audio
{	
private:
	HSAMPLE loadedAudio;
	HCHANNEL channel=NULL;
public:
	

	Audio(const char* fileName); //importante poner sample a cero aqui
	~Audio(); //aqui deberiamos liberar el sample con BASS_SampleFree

	//HCHANNEL play(float volume); //lanza el audio y retorna el channel donde suena

	//static void Stop(HCHANNEL channel); //para parar un audio necesitamos su channel
	//static Audio* Get(const char* filename); //manager de audios 
	//HSAMPLE loadSample(const char* fileName);
	//static HCHANNEL* Play(const char* filename); //version estática para ir mas rapido
	
	

	void stop();
	void play(float volume);
	HCHANNEL getChannel();
	HSAMPLE getSample();
	
	
};

