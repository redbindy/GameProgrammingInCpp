#pragma once

#include "Component.h"
#include "Collision.h"

class Actor;

class BoxComponent final : public Component
{
public:
	BoxComponent(Actor* const pOwner, const int updateOrder);
	BoxComponent(Actor* const pOwner);
	virtual ~BoxComponent();

	void OnUpdateWorldTransform() override;

	void SetObjectBox(const AABB& model)
	{
		mObjectBox = model;
	}

	const AABB& GetWorldBox() const
	{
		return mWorldBox;
	}

	void SetShouldRotate(const bool bValue)
	{
		mbShouldRotate = bValue;
	}

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;

	TypeID GetType() const override
	{
		return TBoxComponent;
	}

private:
	AABB mObjectBox;
	AABB mWorldBox;

	bool mbShouldRotate;
};