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
	Actor(Game* pGame);
	virtual ~Actor();

	void Update(const float deltaTime);
	void UpdateComponents(const float deltaTime);
	virtual void UpdateActor(const float deltaTime);

	void ProcessInput(const bool* pbKeyStates);
	virtual void ActorInput(const bool* pbKeyStates);

	void AddComponent(Component* const pComponent);
	void RemoveComponent(Component* const pComponent);

	void ComputeWorldTransform();
	inline const Matrix4& GetWorldTransform() const;

	inline EState GetState() const;
	inline void SetState(EState state);

	inline const Vector2& GetPosition() const;
	inline void SetPosition(const Vector2& pos);

	inline float GetScale() const;
	inline void SetScale(const float scale);

	inline float GetRotation() const;
	inline void SetRotation(const float rotation);

	inline Vector2 GetForward() const;

	inline Game* GetGame();

private:
	EState mState;

	Vector2 mPosition;
	float mScale;
	float mRotation;

	std::vector<Component*> mComponentPtrs;

	Game* mpGame;

	Matrix4 mWorldTransform;
	bool mbReComputeWorldTransform;
};

inline Actor::EState Actor::GetState() const
{
	return mState;
}

inline void Actor::SetState(EState state)
{
	mState = state;
}

inline const Vector2& Actor::GetPosition() const
{
	return mPosition;
}

inline void Actor::SetPosition(const Vector2& pos)
{
	mPosition = pos;
	mbReComputeWorldTransform = true;
}

inline float Actor::GetScale() const
{
	return mScale;
}

inline void Actor::SetScale(const float scale)
{
	mScale = scale;
	mbReComputeWorldTransform = true;
}

inline float Actor::GetRotation() const
{
	return mRotation;
}

inline void Actor::SetRotation(const float rotation)
{
	mRotation = rotation;
	mbReComputeWorldTransform = true;
}

inline Vector2 Actor::GetForward() const
{
	return Vector2(Math::Cos(mRotation), Math::Sin(mRotation));
}

inline Game* Actor::GetGame()
{
	return mpGame;
}

inline const Matrix4& Actor::GetWorldTransform() const
{
	return mWorldTransform;
}