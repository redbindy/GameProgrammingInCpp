#pragma once

#include <vector>

#include "Math.h"

class Game;
class Component;

class Actor
{
public:
	enum class EState
	{
		ACTIVE,
		PAUSED,
		DEAD
	};

public:
	Actor(Game* const pGame);
	virtual ~Actor();

	void Update(const float deltaTime);
	void UpdateComponents(const float deltaTime);
	virtual void UpdateActor(const float deltaTime);

	void AddComponent(Component* const pComponent);
	void RemoveComponent(Component* const pComponent);

	inline EState GetState() const;
	inline void SetState(const EState state);

	inline Vector2 GetPosition() const;
	inline void SetPosition(const Vector2& pos);

	inline float GetScale() const;
	inline void SetScale(const float scale);

	inline float GetRotation() const;
	inline void SetRotation(const float rotation);

	inline Game* GetGame() const;

private:
	EState mState;

	Vector2 mPosition;
	float mScale;
	float mRotation;

	std::vector<Component*> mComponentPtrs;

	class Game* mpGame;
};

inline Actor::EState Actor::GetState() const
{
	return mState;
}

inline void Actor::SetState(const EState state)
{
	mState = state;
}

inline Vector2 Actor::GetPosition() const
{
	return mPosition;
}

inline void Actor::SetPosition(const Vector2& pos)
{
	mPosition = pos;
}

inline float Actor::GetScale() const
{
	return mScale;
}

inline void Actor::SetScale(const float scale)
{
	mScale = scale;
}

inline float Actor::GetRotation() const
{
	return mRotation;
}

inline void Actor::SetRotation(const float rotation)
{
	mRotation = rotation;
}

inline Game* Actor::GetGame() const
{
	return mpGame;
}