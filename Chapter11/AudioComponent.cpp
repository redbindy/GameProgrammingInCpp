#include "AudioComponent.h"

#include "Game.h"
#include "Actor.h"
#include "AudioSystem.h"
#include "SoundEvent.h"

AudioComponent::AudioComponent(Actor* const pActor, const int updateOrder)
	: Component(pActor, updateOrder)
{
}

AudioComponent::AudioComponent(Actor* const pActor)
	: AudioComponent(pActor, 200)
{
}

AudioComponent::~AudioComponent()
{
	StopAllEvent();
}

void AudioComponent::Update(const float deltaTime)
{
	Component::Update(deltaTime);

	std::vector<SoundEvent>::iterator iter2D = mEvents2D.begin();
	while (iter2D != mEvents2D.end())
	{
		if (!iter2D->IsValid())
		{
			iter2D = mEvents2D.erase(iter2D);
		}
		else
		{
			++iter2D;
		}
	}

	std::vector<SoundEvent>::iterator iter3D = mEvents3D.begin();
	while (iter3D != mEvents3D.end())
	{
		if (!iter3D->IsValid())
		{
			iter3D = mEvents3D.erase(iter3D);
		}
		else
		{
			++iter3D;
		}
	}
}

void AudioComponent::OnUpdateWorldTransform()
{
	const Matrix4 world = mpOwner->GetWorldTransform();
	for (SoundEvent& e : mEvents3D)
	{
		if (e.IsValid())
		{
			e.Set3DAttributes(world);
		}
	}
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
	Game* const pGame = mpOwner->GetGame();
	AudioSystem* const pAudioSystem = pGame->GetAudioSystem();

	SoundEvent e = pAudioSystem->PlayEvent(name);
	if (e.Is3D())
	{
		e.Set3DAttributes(mpOwner->GetWorldTransform());
		mEvents3D.push_back(e);
	}
	else
	{
		mEvents2D.push_back(e);
	}

	return e;
}

void AudioComponent::StopAllEvent()
{
	for (SoundEvent& e : mEvents2D)
	{
		e.Stop();
	}
	mEvents2D.clear();

	for (SoundEvent& e : mEvents3D)
	{
		e.Stop();
	}
	mEvents3D.clear();
}
