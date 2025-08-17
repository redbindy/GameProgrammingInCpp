#include "AudioSystem.h"

#include <fmod_studio.hpp>
#include <fmod_errors.h>

#include "SoundEvent.h"
#include "Game.h"

constexpr int MAX_PATH_LENGTH = 512;

unsigned int AudioSystem::sNextID;

AudioSystem::AudioSystem(Game* const pGame)
	: mpGame(pGame)
	, mpSystem(nullptr)
	, mpLowLevelSystem(nullptr)
{

}

bool AudioSystem::Initialize()
{
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR,
		FMOD_DEBUG_MODE_TTY
	);

	FMOD_RESULT result = FMOD::Studio::System::create(&mpSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));

		return false;
	}

	result = mpSystem->initialize(
		512,
		FMOD_STUDIO_INIT_NORMAL,
		FMOD_INIT_NORMAL,
		nullptr
	);

	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD System: %s", FMOD_ErrorString(result));

		return false;
	}

	mpSystem->getLowLevelSystem(&mpLowLevelSystem);

	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	UnloadAllBanks();

	if (mpSystem != nullptr)
	{
		mpSystem->release();
	}
}

void AudioSystem::Update(const float deltaTime)
{
	std::vector<unsigned int> done;
	for (std::pair<const unsigned int, FMOD::Studio::EventInstance*>& pair : mEventInstances)
	{
		FMOD::Studio::EventInstance* pE = pair.second;

		FMOD_STUDIO_PLAYBACK_STATE state;
		pE->getPlaybackState(&state);

		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			pE->release();
			done.push_back(pair.first);
		}
	}

	for (const unsigned int id : done)
	{
		mEventInstances.erase(id);
	}

	mpSystem->update();
}

void AudioSystem::LoadBank(const std::string& name)
{
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	FMOD::Studio::Bank* pBank = nullptr;
	FMOD_RESULT result = mpSystem->loadBankFile(
		name.c_str(),
		FMOD_STUDIO_LOAD_BANK_NORMAL,
		&pBank
	);

	if (result == FMOD_OK)
	{
		mBanks.insert({ name, pBank });
		pBank->loadSampleData();

		int numEvents = 0;
		pBank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			pBank->getEventList(events.data(), numEvents, &numEvents);

			char eventName[MAX_PATH_LENGTH];
			for (int i = 0; i < numEvents; ++i)
			{
				FMOD::Studio::EventDescription* pE = events[i];
				pE->getPath(eventName, MAX_PATH_LENGTH, nullptr);

				mEvents.insert({ eventName, pE });
			}
		}

		int numBuses = 0;
		pBank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			pBank->getBusList(buses.data(), numBuses, &numBuses);

			char busName[MAX_PATH_LENGTH];
			for (int i = 0; i < numBuses; ++i)
			{
				FMOD::Studio::Bus* pBus = buses[i];
				pBus->getPath(busName, MAX_PATH_LENGTH, nullptr);

				mBuses.insert({ busName, pBus });
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	std::unordered_map<std::string, FMOD::Studio::Bank*>::iterator iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	FMOD::Studio::Bank* pBank = iter->second;

	int numEvents = 0;
	pBank->getEventCount(&numEvents);

	if (numEvents > 0)
	{
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		pBank->getEventList(events.data(), numEvents, &numEvents);

		char eventName[MAX_PATH_LENGTH];
		for (int i = 0; i < numEvents; ++i)
		{
			FMOD::Studio::EventDescription* const pE = events[i];
			pE->getPath(eventName, MAX_PATH_LENGTH, nullptr);

			std::unordered_map<std::string, FMOD::Studio::EventDescription*>::iterator eventIter = mEvents.find(eventName);
			if (eventIter != mEvents.end())
			{
				mEvents.erase(eventIter);
			}
		}
	}

	int numBuses = 0;
	pBank->getBusCount(&numBuses);

	if (numBuses > 0)
	{
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		pBank->getBusList(buses.data(), numBuses, &numBuses);

		char busName[MAX_PATH_LENGTH];
		for (int i = 0; i < numBuses; ++i)
		{
			FMOD::Studio::Bus* pBus = buses[i];

			pBus->getPath(busName, MAX_PATH_LENGTH, nullptr);

			std::unordered_map<std::string, FMOD::Studio::Bus*>::iterator busIter = mBuses.find(busName);
			if (busIter != mBuses.end())
			{
				mBuses.erase(busIter);
			}
		}
	}

	pBank->unloadSampleData();
	pBank->unload();

	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (std::pair<const std::string, FMOD::Studio::Bank*>& pair : mBanks)
	{
		pair.second->unloadSampleData();
		pair.second->unload();
	}
	mBanks.clear();

	mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	unsigned int retID = 0;

	std::unordered_map<std::string, FMOD::Studio::EventDescription*>::iterator iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		FMOD::Studio::EventInstance* pEvent = nullptr;
		iter->second->createInstance(&pEvent);
		if (pEvent != nullptr)
		{
			pEvent->start();

			++sNextID;
			retID = sNextID;

			mEventInstances.insert({ sNextID, pEvent });
		}
	}

	return SoundEvent(this, retID);
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(const unsigned int id)
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*>::iterator iter = mEventInstances.find(id);
	if (iter != mEventInstances.end())
	{
		pEvent = iter->second;
	}

	return pEvent;
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		FMOD_VECTOR v;
		v.x = in.Y;
		v.y = in.Z;
		v.z = in.X;

		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
	Matrix4 invView = viewMatrix;
	invView.Invert();

	FMOD_3D_ATTRIBUTES listener;
	listener.position = VecToFMOD(invView.GetTranslation());
	listener.forward = VecToFMOD(invView.GetZAxis());
	listener.up = VecToFMOD(invView.GetYAxis());
	listener.velocity = { 0.f, 0.f, 0.f };

	mpSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float ret = 0.f;

	const std::unordered_map<std::string, FMOD::Studio::Bus*>::const_iterator iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getVolume(&ret);
	}

	return ret;
}

void AudioSystem::SetBusVolume(const std::string& name, const float volume)
{
	std::unordered_map<std::string, FMOD::Studio::Bus*>::iterator iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setVolume(volume);
	}
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
	bool bRet = false;
	const std::unordered_map<std::string, FMOD::Studio::Bus*>::const_iterator iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getPaused(&bRet);
	}

	return bRet;
}

void AudioSystem::SetBusPaused(const std::string& name, const bool bPause)
{
	std::unordered_map<std::string, FMOD::Studio::Bus*>::iterator iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setPaused(bPause);
	}
}
