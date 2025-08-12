#include "SplineActor.h"

#include "SplineCamera.h"

SplineActor::SplineActor(Game* const pGame)
	: Actor(pGame)
	, mpCameraComp(nullptr)
{
	mpCameraComp = new SplineCamera(this);

	Spline path;
	path.ControlPoints.push_back(Vector3::Zero);
	for (int i = 0; i < 5; ++i)
	{
		if (i % 2 == 0)
		{
			path.ControlPoints.push_back(Vector3(300.f * (i + 1), 300.f, 300.f));
		}
		else
		{
			path.ControlPoints.push_back(Vector3(300.f * (i + 1), 0.f, 0.f));
		}
	}

	mpCameraComp->SetSpline(path);
	mpCameraComp->SetPaused(false);
}

void SplineActor::ActorInput(const bool* const pbkeys)
{
}

void SplineActor::RestartSpline()
{
	mpCameraComp->Restart();
}
