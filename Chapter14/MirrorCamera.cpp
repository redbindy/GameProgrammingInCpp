#include "MirrorCamera.h"

#include "Actor.h"
#include "Renderer.h"
#include "Game.h"
#include "LevelLoader.h"

MirrorCamera::MirrorCamera(Actor* const pOwner)
	: CameraComponent(pOwner)
	, mHorzDist(150.f)
	, mVertDist(200.f)
	, mTargetDist(400.f)
{
}

void MirrorCamera::Update(const float deltaTime)
{
	CameraComponent::Update(deltaTime);

	const Vector3 idealPos = computeCameraPos();

	const Vector3 target = mpOwner->GetPosition() - mpOwner->GetForward() * mTargetDist;

	const Matrix4 view = Matrix4::CreateLookAt(idealPos, target, Vector3::UnitZ);

	Game* const pGame = mpOwner->GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	pRenderer->SetMirrorView(view);
}

void MirrorCamera::SnapToIdeal()
{
	const Vector3 idealPos = computeCameraPos();
	const Vector3 target = mpOwner->GetPosition() - mpOwner->GetForward() * mTargetDist;

	const Matrix4 view = Matrix4::CreateLookAt(idealPos, target, Vector3::UnitZ);

	Game* const pGame = mpOwner->GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	pRenderer->SetMirrorView(view);
}

void MirrorCamera::LoadProperties(const rapidjson::Value& inObj)
{
	CameraComponent::LoadProperties(inObj);

	JsonHelper::GetFloat(inObj, "horzDist", mHorzDist);
	JsonHelper::GetFloat(inObj, "vertDist", mVertDist);
	JsonHelper::GetFloat(inObj, "targetDist", mTargetDist);
}

void MirrorCamera::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	CameraComponent::SaveProperties(alloc, inObj);

	JsonHelper::AddFloat(alloc, inObj, "horzDist", mHorzDist);
	JsonHelper::AddFloat(alloc, inObj, "vertDist", mVertDist);
	JsonHelper::AddFloat(alloc, inObj, "targetDist", mTargetDist);
}

Vector3 MirrorCamera::computeCameraPos() const
{
	Vector3 cameraPos = mpOwner->GetPosition();
	cameraPos += mpOwner->GetForward() * mHorzDist;
	cameraPos += Vector3::UnitZ * mVertDist;

	return cameraPos;
}
