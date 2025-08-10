#include "SoundEvent.h"

#include <fmod_studio.hpp>

#include "AudioSystem.h"

SoundEvent::SoundEvent()
	: mpSystem(nullptr)
	, mID(0)
{
}

SoundEvent::SoundEvent(AudioSystem* const pSystem, const unsigned int id)
	: mpSystem(pSystem)
	, mID(0)
{
}

bool SoundEvent::IsValid()
{
	return mpSystem != nullptr && mpSystem->GetEventInstance(mID) != nullptr;
}

void SoundEvent::Restart()
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	if (pEvent != nullptr)
	{
		pEvent->start();
	}
}

void SoundEvent::Stop(const bool bAllowFadeOut)
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	if (pEvent != nullptr)
	{
		FMOD_STUDIO_STOP_MODE mode;
		if (bAllowFadeOut)
		{
			mode = FMOD_STUDIO_STOP_ALLOWFADEOUT;
		}
		else
		{
			mode = FMOD_STUDIO_STOP_IMMEDIATE;
		}

		pEvent->stop(mode);
	}
}

void SoundEvent::Stop()
{
	Stop(true);
}

void SoundEvent::SetPaused(const bool bPause)
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	if (pEvent != nullptr)
	{
		pEvent->setPaused(bPause);
	}
}

void SoundEvent::SetVolume(const float value)
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	if (pEvent != nullptr)
	{
		pEvent->setVolume(value);
	}
}

void SoundEvent::SetPitch(const float value)
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	if (pEvent != nullptr)
	{
		pEvent->setPitch(value);
	}
}

void SoundEvent::SetParameter(const std::string& name, const float value)
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	if (pEvent != nullptr)
	{
		pEvent->setParameterValue(name.c_str(), value);
	}
}

bool SoundEvent::GetPaused() const
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	bool bRet = false;
	if (pEvent != nullptr)
	{
		pEvent->getPaused(&bRet);
	}

	return bRet;
}

float SoundEvent::GetVolume() const
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	float ret = 0.f;
	if (pEvent != nullptr)
	{
		pEvent->getVolume(&ret);
	}

	return ret;
}

float SoundEvent::GetPitch() const
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	float ret = 0.f;
	if (pEvent != nullptr)
	{
		pEvent->getPitch(&ret);
	}

	return ret;
}

float SoundEvent::GetParameter(const std::string& name)
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	float ret = 0.f;
	if (pEvent != nullptr)
	{
		pEvent->getParameterValue(name.c_str(), &ret);
	}

	return ret;
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		FMOD_VECTOR v;
		v.x = in.Y;
		v.y = in.Z;
		v.z = in.X;

		return v;
	}
}

bool SoundEvent::Is3D() const
{
	bool bRet = false;
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	if (pEvent != nullptr)
	{
		FMOD::Studio::EventDescription* pED = nullptr;
		pEvent->getDescription(&pED);
		if (pED != nullptr)
		{
			pED->is3D(&bRet);
		}
	}

	return bRet;
}

void SoundEvent::Set3DAttributes(const Matrix4& worldTrans)
{
	FMOD::Studio::EventInstance* pEvent = nullptr;
	if (mpSystem != nullptr)
	{
		pEvent = mpSystem->GetEventInstance(mID);
	}

	if (pEvent != nullptr)
	{
		FMOD_3D_ATTRIBUTES attr;
		attr.position = VecToFMOD(worldTrans.GetTranslation());
		attr.forward = VecToFMOD(worldTrans.GetXAxis());
		attr.up = VecToFMOD(worldTrans.GetZAxis());

		attr.velocity = { 0.f, 0.f, 0.f };

		pEvent->set3DAttributes(&attr);
	}
}
