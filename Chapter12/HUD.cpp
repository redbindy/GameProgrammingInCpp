#include "HUD.h"

#include "Game.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "TargetComponent.h"
#include "FollowActor.h"

HUD::HUD(Game* const pGame)
	: UIScreen(pGame)
	, mpCrosshair(nullptr)
	, mpCrosshairEnemy(nullptr)
	, mbTargetEnemy(false)
	, mTargetComps()
	, mpRadar(nullptr)
	, mpBlipTex(nullptr)
	, mpRadarArrow(nullptr)
	, mBlips()
	, mRadarRange(2000.f)
	, mRadarRadius(92.f)
{
	Renderer* const pRenderer = pGame->GetRenderer();

	mpCrosshair = pRenderer->GetTexture("Assets/Crosshair.png");
	mpCrosshairEnemy = pRenderer->GetTexture("Assets/CrosshairRed.png");

	mpRadar = pRenderer->GetTexture("Assets/Radar.png");
	mpBlipTex = pRenderer->GetTexture("Assets/Blip.png");
	mpRadarArrow = pRenderer->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD()
{
}

void HUD::Update(const float deltaTime)
{
	UIScreen::Update(deltaTime);

	updateCrosshair(deltaTime);
	updateRadar(deltaTime);
}

void HUD::Draw(Shader* const pShader)
{
	Texture* const pCross = mbTargetEnemy ? mpCrosshairEnemy : mpCrosshair;
	DrawTexture(pShader, pCross, Vector2::Zero, 2.f);

	const Vector2 cRadarPos(-390.f, 275.f);

	DrawTexture(pShader, mpRadar, cRadarPos, 1.f);

	for (Vector2& blip : mBlips)
	{
		DrawTexture(pShader, mpBlipTex, cRadarPos + blip, 1.f);
	}

	DrawTexture(pShader, mpRadarArrow, cRadarPos);
}

void HUD::AddTargetComponent(TargetComponent* const pTC)
{
	mTargetComps.push_back(pTC);
}

void HUD::RemoveTargetComponent(TargetComponent* const pTC)
{
	std::vector<TargetComponent*>::iterator iter = std::find(mTargetComps.begin(), mTargetComps.end(), pTC);

	mTargetComps.erase(iter);
}

void HUD::updateCrosshair(const float deltaTime)
{
	mbTargetEnemy = false;

	const float cAimDist = 5000.f;

	Renderer* const pRenderer = mpGame->GetRenderer();

	Vector3 start;
	Vector3 dir;
	pRenderer->GetScreenDirection(start, dir);

	LineSegment l(start, start + dir * cAimDist);

	PhysWorld::CollisionInfo info;

	PhysWorld* const pPhysWorld = mpGame->GetPhysWorld();
	if (pPhysWorld->SegmentCast(l, info))
	{
		for (TargetComponent* const p : mTargetComps)
		{
			if (p->GetOwner() == info.pActor)
			{
				mbTargetEnemy = true;

				break;
			}
		}
	}
}

void HUD::updateRadar(const float deltaTime)
{
	mBlips.clear();

	FollowActor* const pPlayer = mpGame->GetPlayer();

	const Vector3 playerPos = pPlayer->GetPosition();
	const Vector2 playerPos2D(playerPos.Y, playerPos.X);

	const Vector3 playerForward = pPlayer->GetForward();
	const Vector2 playerForward2D(playerForward.X, playerForward.Y);

	const float angle = Math::Atan2(playerForward2D.Y, playerForward2D.X);

	const Matrix3 rotMat = Matrix3::CreateRotation(angle);

	for (TargetComponent* const p : mTargetComps)
	{
		Actor* const pOwner = p->GetOwner();

		const Vector3 targetPos = pOwner->GetPosition();
		const Vector2 actorPos2D(targetPos.Y, targetPos.X);

		const Vector2 playerToTarget = actorPos2D - playerPos2D;

		if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
		{
			Vector2 blipPos = playerToTarget;
			blipPos *= mRadarRadius / mRadarRange;

			blipPos = Vector2::Transform(blipPos, rotMat);
			mBlips.push_back(blipPos);
		}
	}
}
