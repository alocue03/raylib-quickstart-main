#include "AudioManager.h"
AudioManager* AudioManager::instance = nullptr;

AudioManager* AudioManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new AudioManager();
		instance->Init();
	}
	return instance;
}

void AudioManager::Init()
{
	InitAudioDevice();
}

void AudioManager::LoadBackgroundMusic(const char* filename)
{
	backGroundMusic = LoadMusicStream(filename);
}

void AudioManager::PlayBGM()
{
	PlayMusicStream(backGroundMusic);
}

void AudioManager::Update()
{
	UpdateMusicStream(backGroundMusic);
}
AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}
