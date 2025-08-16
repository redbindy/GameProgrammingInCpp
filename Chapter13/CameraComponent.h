#pragma once

#include "Component.h"
#include "Math.h"

class Actor;

class CameraComponent : public Component
{
public:
	CameraComponent(Actor* const pOwner, const int updateOrder);
	CameraComponent(Actor* const pOwner);
	virtual ~CameraComponent() = default;

protected:
	void SetViewMatrix(const Matrix4& view);
};