#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

#include "Component.h"

class Actor;
class AIState;

class AIComponent final : public Component
{
public:
	AIComponent(Actor* const pOwner);
	virtual ~AIComponent() = default;

	void Update(const float deltaTime) override;
	void ChangeState(const std::string& name);

	void RegisterState(AIState* const state);

private:
	std::unordered_map<std::string, AIState*> mStateMap;

	AIState* mpCurrentState;
};