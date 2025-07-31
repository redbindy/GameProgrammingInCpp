#pragma once

#include "Actor.h"

class Actor;

class Component
{
public:
	Component(Actor* const pOwner, const int updateOrder);
	Component(Actor* const pOwner);
	virtual ~Component();

	virtual void Update(const float deltaTime);

	int GetUpdateOrder() const;

protected:
	Actor* mpOwner;

	int mUpdateOrder;
};

inline int Component::GetUpdateOrder() const
{
	return mUpdateOrder;
}