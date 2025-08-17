#pragma once

#include <rapidjson/document.h>

class Actor;

class Component
{
public:
	enum TypeID
	{
		TComponent,
		TAudioComponent,
		TBallMove,
		TBoxComponent,
		TCameraComponent,
		TFollowCamera,
		TMeshComponent,
		TMoveComponent,
		TSkeletalMeshComponent,
		TSpriteComponent,
		TMirrorCamera,
		TPointLightComponent,
		TTargetComponent,

		NUM_COMPONENT_TYPES
	};

	static const char* TypeNames[NUM_COMPONENT_TYPES];

public:
	Component(Actor* const pOwner, const int updateOrder);
	Component(Actor* const pOwner);
	virtual ~Component();

	virtual void Update(const float deltaTime);
	virtual void ProcessInput(const bool* pbKeyState);
	virtual void OnUpdateWorldTransform();

	inline int GetUpdateOrder() const;

	Actor* GetOwner()
	{
		return mpOwner;
	}

	virtual TypeID GetType() const = 0;

	virtual void LoadProperties(const rapidjson::Value& inObj);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;

	template<typename T>
	static Component* Create(Actor* pActor, const rapidjson::Value& inObj)
	{
		T* t = new T(pActor);

		t->LoadProperties(inObj);

		return t;
	}

protected:
	Actor* mpOwner;

	int mUpdateOrder;
};

inline int Component::GetUpdateOrder() const
{
	return mUpdateOrder;
}