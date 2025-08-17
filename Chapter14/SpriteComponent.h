#pragma once

#include "Component.h"

class Actor;
class Shader;
class Texture;

class SpriteComponent final : public Component
{
public:
	SpriteComponent(Actor* const pOwner, const int drawOrder);
	SpriteComponent(Actor* const pOwner);
	virtual ~SpriteComponent();

	virtual void Draw(Shader* const pShader);
	virtual void SetTexture(Texture* const pTexture);

	int GetDrawOrder() const;
	int GetTexWidth() const;
	int GetTexHeight() const;

	bool IsVisible() const
	{
		return mbVisible;
	}

	void SetVisible(const bool bVisible)
	{
		mbVisible = bVisible;
	}

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;

	TypeID GetType() const override
	{
		return TSpriteComponent;
	}

protected:
	Texture* mpTexture;

	int mDrawOrder;

	int mTexWidth;
	int mTexHeight;

	bool mbVisible;
};