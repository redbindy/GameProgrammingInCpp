#pragma once

#include "Component.h"

class TargetComponent final : public Component
{
public:
	TargetComponent(Actor* const pOwner);
	virtual ~TargetComponent();

	TypeID GetType() const override
	{
		return TTargetComponent;
	}

private:
};