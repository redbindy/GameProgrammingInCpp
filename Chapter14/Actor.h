#pragma once

#include <vector>
#include <rapidjson/document.h>

#include "Math.h"
#include "Component.h"

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

	enum TypeID
	{
		TActor = 0,
		TBallActor,
		TFollowActor,
		TPlaneActor,
		TTargetActor,

		NUM_ACTOR_TYPES
	};

	static const char* TypeNames[NUM_ACTOR_TYPES];

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
	inline Vector3 GetRight() const;

	inline EState GetState() const;
	inline void SetState(const EState state);

	Game* GetGame();

	void AddComponent(Component* const pComponent);
	void RemoveComponent(Component* const pComponent);

	void RotateToNewForward(const Vector3& forward);

	virtual void LoadProperties(const rapidjson::Value& inObj);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;

	template<typename T>
	static Actor* Create(Game* const pGame, const rapidjson::Value& inObj)
	{
		T* t = new T(pGame);

		t->LoadProperties(inObj);

		return t;
	}

	Component* GetComponentOfType(const Component::TypeID type)
	{
		Component* pComp = nullptr;
		for (Component* const p : mComponents)
		{
			if (p->GetType() == type)
			{
				pComp = p;

				break;
			}
		}

		return pComp;
	}

	virtual TypeID GetType() const
	{
		return TActor;
	}

	const std::vector<Component*> GetComponents() const
	{
		return mComponents;
	}

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
	mbRecomputeWorldTransform = true;
}

inline float Actor::GetScale() const
{
	return mScale;
}

inline void Actor::SetScale(const float scale)
{
	mScale = scale;
	mbRecomputeWorldTransform = true;
}

inline const Quaternion& Actor::GetRotation() const
{
	return mRotation;
}

inline void Actor::SetRotation(const Quaternion& rotation)
{
	mRotation = rotation;
	mbRecomputeWorldTransform = true;
}

inline const Matrix4& Actor::GetWorldTransform() const
{
	return mWorldTransform;
}

inline Vector3 Actor::GetForward() const
{
	return Vector3::Transform(Vector3::UnitX, mRotation);
}

inline Vector3 Actor::GetRight() const
{
	return Vector3::Transform(Vector3::UnitY, mRotation);
}

inline Actor::EState Actor::GetState() const
{
	return mState;
}

inline void Actor::SetState(const Actor::EState state)
{
	mState = state;
	mbRecomputeWorldTransform = true;
}

inline Game* Actor::GetGame()
{
	return mpGame;
}