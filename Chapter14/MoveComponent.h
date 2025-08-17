#pragma once

#include "Component.h"

class Actor;

class MoveComponent : public Component
{
public:
	MoveComponent(Actor* const pOwner);
	virtual ~MoveComponent() = default;

	void Update(const float deltaTime) override;

	float GetAngularSpeed() const;
	void SetAngularSpeed(const float speed);

	float GetForwardSpeed() const;
	void SetForwardSpeed(const float speed);

	float GetStrafeSpeed() const;
	void SetStrafeSpeed(const float speed);

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;

	TypeID GetType() const override
	{
		return TMoveComponent;
	}

private:
	float mAngularSpeed;
	float mForwardSpeed;

	float mStrafeSpeed;
};