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
	void UpdateComponent(const float deltaTime);
	virtual void UpdateActor(const float deltaTime);

	void ProcessInput(const bool* const pbKeyState);
	virtual void ActorInput(const bool* const pbKeyState);

	inline const Vector3& GetPosition() const;
	inline void SetPosition(const Vector3& pos);

	inline float GetScale() const;
	inline void SetScale(const float scale);

	inline const Quaternion& GetRotation() const;
	inline void SetRotation(const Quaternion& rotation);

	void ComputeWorldTransform();
	inline const Matrix4& GetWorldTransform() const;

	inline Vector3 GetForward() const;

	inline EState GetState() const;
	inline void SetState(const EState state);

	Game* GetGame();

	void AddComponent(Component* const pComponent);
	void RemoveComponent(Component* const pComponent);

private:
	EState mState;

	Matrix4 mWorldTransform;
	Vector3 mPosition;
	Quaternion mRotation;
	float mScale;

	bool mbRecomputeWorldTransform;

	std::vector<Component*> mComponents;

	Game* mpGame;
};

inline const Vector3& Actor::GetPosition() const
{
	return mPosition;
}

inline void Actor::SetPosition(const Vector3& pos)
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

inline const Quaternion& Actor::GetRotation() const
{
	return mRotation;
}

inline void Actor::SetRotation(const Quaternion& rotation)
{
	mRotation = rotation;
}

inline const Matrix4& Actor::GetWorldTransform() const
{
	return mWorldTransform;
}

inline Vector3 Actor::GetForward() const
{
	return Vector3::Transform(Vector3::UnitX, mRotation);
}

inline Actor::EState Actor::GetState() const
{
	return mState;
}

inline void Actor::SetState(const Actor::EState state)
{
	mState = state;
}

inline Game* Actor::GetGame()
{
	return mpGame;
}