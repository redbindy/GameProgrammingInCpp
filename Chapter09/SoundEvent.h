#pragma once

#include <string>

#include "Math.h"

class AudioSystem;

class SoundEvent final
{
public:
	SoundEvent();
	virtual ~SoundEvent() = default;

	bool IsValid();

	void Restart();

	void Stop(const bool bAllowFadeOut);
	void Stop();

	void SetPaused(const bool bPause);
	void SetVolume(const float value);
	void SetPitch(const float value);
	void SetParameter(const std::string& name, const float value);

	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);

	bool Is3D() const;

	void Set3DAttributes(const Matrix4& worldTrans);

protected:
	friend AudioSystem;
	SoundEvent(AudioSystem* const pSystem, const unsigned int id);

private:
	AudioSystem* mpSystem;
	unsigned int mID;
};