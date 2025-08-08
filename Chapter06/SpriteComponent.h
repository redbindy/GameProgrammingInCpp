#pragma once

#include <GL/glew.h>

#include "Component.h"

class Actor;
class Shader;
class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(Actor* const pOwner, const int drawOrder);
	SpriteComponent(Actor* const pOwner);
	virtual ~SpriteComponent();

	virtual void Draw(Shader* const pShader);
	virtual void SetTexture(Texture* const pTexture);

	inline int GetDrawOrder() const;
	inline float GetTextureWidth() const;
	inline float GetTextureHeight() const;

protected:
	Texture* mpTexture;

	float mTextureWidth;
	float mTextureHeight;

	int mDrawOrder;
};

inline int SpriteComponent::GetDrawOrder() const
{
	return mDrawOrder;
}

inline float SpriteComponent::GetTextureWidth() const
{
	return mTextureWidth;
}

inline float SpriteComponent::GetTextureHeight() const
{
	return mTextureHeight;
}