#include "SpriteComponent.h"

#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* const pOwner, const int drawOrder)
	: Component(pOwner)
	, mpTexture(nullptr)
	, mTextureWidth(0.f)
	, mTextureHeight(0.f)
	, mDrawOrder(drawOrder)
{
	Game* const pGame = pOwner->GetGame();

	pGame->AddSprite(this);
}

SpriteComponent::SpriteComponent(Actor* const pOwner)
	: SpriteComponent(pOwner, 100)
{
}

SpriteComponent::~SpriteComponent()
{
	Game* const pGame = mpOwner->GetGame();

	pGame->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* const pShader)
{
	if (mpTexture == nullptr)
	{
		return;
	}

	Matrix4 scaleMat = Matrix4::CreateScale(
		mTextureWidth,
		mTextureHeight,
		1.f
	);

	Matrix4 world = scaleMat * mpOwner->GetWorldTransform();
	pShader->SetMatrixUniform("uWorldTransform", world);

	mpTexture->SetActive();

	glDrawElements(
		GL_TRIANGLES,
		6,
		GL_UNSIGNED_INT,
		nullptr
	);
}

void SpriteComponent::SetTexture(Texture* const pTexture)
{
	mpTexture = pTexture;

	mTextureWidth = pTexture->GetWidth();
	mTextureHeight = pTexture->GetHeight();
}
