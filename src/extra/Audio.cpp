#include "Audio.h"


/*HCHANNEL* Audio::Play(const char* filename)
{
	Audio* aud = find(filename);
	

	HSAMPLE hSample = aud->sample;
	HCHANNEL hSampleChannel;
	hSampleChannel = BASS_SampleGetChannel(hSample, true);

	BASS_ChannelPlay(hSampleChannel, true);

	return &hSampleChannel;
}*/

void Audio::stop()
{
	if(channel)
		BASS_SampleStop(channel);
}

void Audio::play(float volume)
{
	if (BASS_ChannelIsActive(channel) == BASS_ACTIVE_PLAYING) {
		std::cout << "Audio is playing, will ignore request\n";
		return;
	}
	else {
		std::cout << "Channel status: " << BASS_ChannelIsActive(channel) << std::endl;
	}
	
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volume);
	if (!channel) {
		HCHANNEL hSampleChannel;
		hSampleChannel = BASS_SampleGetChannel(loadedAudio, true);
		this->channel = hSampleChannel;
	}

	BASS_ChannelPlay(channel, true);
	
}

HCHANNEL Audio::getChannel()
{
	return channel;
}

HSAMPLE Audio::getSample()
{
	return loadedAudio;
}



Audio::Audio(const char* fileName)
{
	
	HSAMPLE hSample;
	hSample = BASS_SampleLoad(false, fileName, 0, 0, 3, 0);	
	if (hSample == 0) {
		//file not found
		std::cout << "ERROR load " << fileName << std::endl;
		std::cout <<"Error Code "<< BASS_ErrorGetCode() << std::endl;
	}
	this->loadedAudio = hSample;
	std::cout << "+ AUDIO load " << fileName << std::endl;
	
}

Audio::~Audio()
{
	BASS_SampleFree(loadedAudio);

}
/*HCHANNEL Audio::play(float volume)
{
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volume);
	HCHANNEL hSampleChannel;
	hSampleChannel = BASS_SampleGetChannel(sample, true);

	BASS_ChannelPlay(hSampleChannel, true);

	return hSampleChannel;
}*/
/*
void Audio::Stop(HCHANNEL channel)
{
	BASS_SampleStop(channel);
}*/
/*
Audio* Audio::Get(const char* filename)
{
	Audio* sound = find(filename);
	if (sound)
		return sound;
	sound = new Audio();

	if (!sound->loadSample(filename)) {
		delete sound;
		return NULL;
	}

	sLoadedAudios.insert(std::pair<std::string, Audio*>(filename, sound));
	return sound;
}

HSAMPLE Audio::loadSample(const char* fileName)
{
	HSAMPLE hSample;
	hSample = BASS_SampleLoad(false, fileName, 0, 0, 3, 0);
	if (hSample == 0) {
		//file not found
		std::cout << "ERROR load " << fileName << std::endl;
	}
	this->sample = hSample;
	std::cout << "+ AUDIO load " << fileName << std::endl;
	return hSample;
}*/