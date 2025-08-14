#pragma once

#include <vector>

#include "UIScreen.h"
#include "Collision.h"

class TargetComponent;

class HUD final : public UIScreen
{
public:
	HUD(Game* const pGame);
	virtual ~HUD();

	void Update(const float deltaTime) override;
	void Draw(Shader* const pShader) override;

	void AddTargetComponent(TargetComponent* const pTC);
	void RemoveTargetComponent(TargetComponent* const pTC);

protected:
	Texture* mpCrosshair;
	Texture* mpCrosshairEnemy;

	bool mbTargetEnemy;

	std::vector<TargetComponent*> mTargetComps;

	Texture* mpRadar;
	Texture* mpBlipTex;
	Texture* mpRadarArrow;

	std::vector<Vector2> mBlips;

	float mRadarRange;
	float mRadarRadius;

protected:
	void updateCrosshair(const float deltaTime);
	void updateRadar(const float deltaTime);
};