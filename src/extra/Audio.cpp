#include "Audio.h"


HCHANNEL* Audio::Play(const char* filename)
{
	Audio* aud = find(filename);
	

	HSAMPLE hSample = aud->sample;
	HCHANNEL hSampleChannel;
	hSampleChannel = BASS_SampleGetChannel(hSample, true);

	BASS_ChannelPlay(hSampleChannel, true);

	return &hSampleChannel;
}

Audio* Audio::find(const char* fileName) {
	auto it = sLoadedAudios.find(fileName);
	if (it != sLoadedAudios.end())
		return it->second;
	return NULL;
}

Audio::Audio()
{
	sample = 0;
}

Audio::~Audio()
{
	BASS_SampleFree(sample);
}

HCHANNEL Audio::play(float volume)
{
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volume);
	HCHANNEL hSampleChannel;
	hSampleChannel = BASS_SampleGetChannel(sample, true);

	BASS_ChannelPlay(hSampleChannel, true);

	return hSampleChannel;
}

void Audio::Stop(HCHANNEL channel)
{
	BASS_SampleStop(channel);
}

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
}