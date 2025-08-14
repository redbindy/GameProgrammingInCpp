#pragma once

#include "Component.h"

class TargetComponent final : public Component
{
public:
	TargetComponent(Actor* const pOwner);
	virtual ~TargetComponent();

private:
};