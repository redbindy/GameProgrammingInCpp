#pragma once

#include <SDL3/SDL.h>

#include "Math.h"

class InputSystem;

enum class EButtonState
{
	NONE,
	PRESSED,
	RELEASED,
	HELD
};

class KeyboardState final
{
	friend InputSystem;

public:
	KeyboardState()
		: mpCurrState(nullptr)
		, mPrevState{}
	{

	}
	virtual ~KeyboardState() = default;

	bool GetKeyValue(const SDL_Scancode keyCode) const;

	EButtonState GetKeyState(const SDL_Scancode keyCode) const;

private:
	const bool* mpCurrState;
	bool mPrevState[SDL_SCANCODE_COUNT];
};

class MouseState final
{
	friend InputSystem;

public:
	MouseState()
		: mCurrButtons(0)
		, mPrevButtons(0)
		, mbRelative(false)
	{

	}
	virtual ~MouseState() = default;

	const Vector2& GetPosition() const
	{
		return mMousePos;
	}

	const Vector2& GetScrollWheel() const
	{
		return mScrollWheel;
	}

	bool IsRelative() const
	{
		return mbRelative;
	}

	bool GetButtonValue(const int button) const;

	EButtonState GetButtonState(const int button) const;

private:
	Vector2 mMousePos;
	Vector2 mScrollWheel;

	SDL_MouseButtonFlags mCurrButtons;
	SDL_MouseButtonFlags mPrevButtons;

	bool mbRelative;
};

class ControllerState final
{
	friend InputSystem;

public:
	ControllerState()
		: mCurrButtons{}
		, mPrevButtons{}
		, mbConnected(false)
		, mLeftTrigger(0.f)
		, mRightTrigger(0.f)
		, mLeftStick(Vector2::Zero)
		, mRightStick(Vector2::Zero)
	{

	}

	bool GetButtonValue(const SDL_GamepadButton button) const;

	EButtonState GetButtonState(const SDL_GamepadButton button) const;

	bool IsConnected() const
	{
		return mbConnected;
	}

	float GetLeftTrigger() const
	{
		return mLeftTrigger;
	}

	float GetRightTrigger() const
	{
		return mRightTrigger;
	}

	const Vector2& GetLeftStick() const
	{
		return mLeftStick;
	}

	const Vector2& GetRightStick() const
	{
		return mRightStick;
	}

private:
	bool mCurrButtons[SDL_GAMEPAD_BUTTON_COUNT];
	bool mPrevButtons[SDL_GAMEPAD_BUTTON_COUNT];

	bool mbConnected;

	float mLeftTrigger;
	float mRightTrigger;

	Vector2 mLeftStick;
	Vector2 mRightStick;
};

struct InputState
{
	KeyboardState keyboard;
	MouseState mouse;
	ControllerState controller;
};

class InputSystem final
{
public:
	InputSystem();
	virtual ~InputSystem() = default;

	bool Initialize();

	void Shutdown();

	void PrepareForUpdate();

	void Update();

	void ProcessEvent(const SDL_Event& event);

	inline const InputState& GetState() const;

	void SetRelativeMouseMode(SDL_Window* const pWindow, const bool bRelative);

private:
	InputState mState;

	SDL_Gamepad* mpController;

private:
	float filter1D(const int input);
	Vector2 filter2D(const int x, const int y);
};

inline const InputState& InputSystem::GetState() const
{
	return mState;
}
