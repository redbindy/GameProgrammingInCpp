#include "SpriteComponent.h"

#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* const pOwner, const int drawOrder)
	: Component(pOwner)
	, mpTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
{
	Game* const pGame = pOwner->GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	pRenderer->AddSprite(this);
}

SpriteComponent::SpriteComponent(Actor* const pOwner)
	: SpriteComponent(pOwner, 100)
{
}

SpriteComponent::~SpriteComponent()
{
	Game* const pGame = mpOwner->GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	pRenderer->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* const pShader)
{
	if (mpTexture == nullptr)
	{
		return;
	}

	const Matrix4 scaleMat = Matrix4::CreateScale(mTexWidth, mTexHeight, 1.f);
	const Matrix4 world = scaleMat * mpOwner->GetWorldTransform();

	pShader->SetMatrixUniform("uWorldTransform", world);

	mpTexture->SetActive();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void SpriteComponent::SetTexture(Texture* const pTexture)
{
	mpTexture = pTexture;

	mTexWidth = pTexture->GetWidth();
	mTexHeight = pTexture->GetHeight();
}

int SpriteComponent::GetDrawOrder() const
{
	return mDrawOrder;
}

int SpriteComponent::GetTexWidth() const
{
	return mTexWidth;
}

int SpriteComponent::GetTexHeight() const
{
	return mTexHeight;
}
