#pragma once

#include <unordered_map>
#include <string>

#include <SDL3/SDL_log.h>

#include "Math.h"

namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};

class Game;
class SoundEvent;

class AudioSystem final
{
public:
	AudioSystem(Game* const pGame);
	virtual ~AudioSystem() = default;

	bool Initialize();

	void Shutdown();

	void Update(const float deltaTime);

	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	void SetListener(const Matrix4& viewMatrix);

	float GetBusVolume(const std::string& name) const;
	void SetBusVolume(const std::string& name, const float volume);

	bool GetBusPaused(const std::string& name) const;
	void SetBusPaused(const std::string& name, const bool bPause);

protected:
	friend SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(const unsigned int id);

private:
	static unsigned int sNextID;

	Game* mpGame;
	FMOD::Studio::System* mpSystem;
	FMOD::System* mpLowLevelSystem;

	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
};