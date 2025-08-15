#pragma once

#include <string>
#include <vector>

#include "Component.h"

class Actor;
class SoundEvent;

class AudioComponent final : public Component
{
public:
	AudioComponent(Actor* const pActor, const int updateOrder);
	AudioComponent(Actor* const pActor);
	virtual ~AudioComponent();

	void Update(const float deltaTime) override;
	void OnUpdateWorldTransform() override;

	SoundEvent PlayEvent(const std::string& name);

	void StopAllEvent();

private:
	std::vector<SoundEvent> mEvents2D;
	std::vector<SoundEvent> mEvents3D;
};