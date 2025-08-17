#include "BallActor.h"

#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "BallMove.h"
#include "AudioComponent.h"
#include "LevelLoader.h"

BallActor::BallActor(Game* const pGame)
	: Actor(pGame)
	, mpAudioComp(nullptr)
	, mpMyMove(nullptr)
	, mLifeSpan(2.f)
{
	MeshComponent* mc = new MeshComponent(this);

	Renderer* const pRenderer = pGame->GetRenderer();

	Mesh* mesh = pRenderer->GetMesh("Assets/Sphere.gpmesh");
	mc->SetMesh(mesh);

	mpMyMove = new BallMove(this);
	mpMyMove->SetForwardSpeed(1500.f);

	mpAudioComp = new AudioComponent(this);
}

void BallActor::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0.f)
	{
		SetState(EState::DEAD);
	}
}

void BallActor::SetPlayer(Actor* const pPlayer)
{
	mpMyMove->SetPlayer(pPlayer);
}

void BallActor::HitTarget()
{
	mpAudioComp->PlayEvent("event:/Ding");
}

void BallActor::LoadProperties(const rapidjson::Value& inObj)
{
	Actor::LoadProperties(inObj);

	JsonHelper::GetFloat(inObj, "lifespan", mLifeSpan);
}

void BallActor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Actor::SaveProperties(alloc, inObj);

	JsonHelper::AddFloat(alloc, inObj, "lifespan", mLifeSpan);
}
