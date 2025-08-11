#include "InputSystem.h"

InputSystem::InputSystem()
	: mState{}
	, mpController(nullptr)
{

}

bool InputSystem::Initialize()
{
	mState.keyboard.mpCurrState = SDL_GetKeyboardState(nullptr);

	int count = 0;
	const SDL_JoystickID* pStickIDs = SDL_GetJoysticks(&count);
	mpController = SDL_OpenGamepad(*pStickIDs);

	mState.controller.mbConnected = (mpController != nullptr);

	return true;
}

void InputSystem::Shutdown()
{

}

void InputSystem::PrepareForUpdate()
{
	memcpy(mState.keyboard.mPrevState, mState.keyboard.mpCurrState, sizeof(bool) * SDL_SCANCODE_COUNT);

	mState.mouse.mPrevButtons = mState.mouse.mCurrButtons;
	mState.mouse.mbRelative = false;
	mState.mouse.mScrollWheel = Vector2::Zero;

	memcpy(mState.controller.mPrevButtons, mState.controller.mCurrButtons, sizeof(bool) * SDL_GAMEPAD_BUTTON_COUNT);
}

void InputSystem::Update()
{
	float mouseX;
	float mouseY;
	if (mState.mouse.mbRelative)
	{
		mState.mouse.mCurrButtons = SDL_GetRelativeMouseState(&mouseX, &mouseY);
	}
	else
	{
		mState.mouse.mCurrButtons = SDL_GetMouseState(&mouseX, &mouseY);
	}
	mState.mouse.mMousePos = Vector2(mouseX, mouseY);

	for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i)
	{
		mState.controller.mCurrButtons[i] = SDL_GetGamepadButton(mpController, SDL_GamepadButton(i));
	}

	const int leftTrigger = SDL_GetGamepadAxis(mpController, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
	mState.controller.mLeftTrigger = filter1D(leftTrigger);

	const int rightTrigger = SDL_GetGamepadAxis(mpController, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
	mState.controller.mRightTrigger = filter1D(rightTrigger);

	const float lStickX = SDL_GetGamepadAxis(mpController, SDL_GAMEPAD_AXIS_LEFTX);
	const float lStickY = -SDL_GetGamepadAxis(mpController, SDL_GAMEPAD_AXIS_LEFTY);

	mState.controller.mLeftStick = filter2D(lStickX, lStickY);

	const float rStickX = SDL_GetGamepadAxis(mpController, SDL_GAMEPAD_AXIS_RIGHTX);
	const float rStickY = -SDL_GetGamepadAxis(mpController, SDL_GAMEPAD_AXIS_RIGHTY);

	mState.controller.mRightStick = filter2D(rStickX, rStickY);
}

void InputSystem::ProcessEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_EVENT_MOUSE_WHEEL:
		mState.mouse.mScrollWheel = Vector2(event.wheel.x, event.wheel.y);
		break;

	default:
		break;
	}
}

void InputSystem::SetRelativeMouseMode(SDL_Window* const pWindow, const bool bRelative)
{
	SDL_SetWindowRelativeMouseMode(pWindow, bRelative);

	mState.mouse.mbRelative = bRelative;
}

float InputSystem::filter1D(const int input)
{
	constexpr int deadZone = 250;
	constexpr int maxValue = 30000;

	float ret = 0.f;

	const int absValue = input > 0 ? input : -input;
	if (absValue > deadZone)
	{
		ret = (absValue - deadZone) / static_cast<float>(maxValue - deadZone);

		if (input < 0)
		{
			ret *= -1.f;
		}

		ret = Math::Clamp(ret, -1.f, 1.f);
	}

	return ret;
}

Vector2 InputSystem::filter2D(const int x, const int y)
{
	constexpr float deadZone = 8000.f;
	constexpr float maxValue = 30000.f;

	Vector2 dir(x, y);

	const float length = dir.Length();

	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		float f = (length - deadZone) / (maxValue - deadZone);

		f = Math::Clamp(f, 0.f, 1.f);

		dir *= f / length;
	}

	return dir;
}

bool KeyboardState::GetKeyValue(const SDL_Scancode keyCode) const
{
	return mpCurrState[keyCode];
}

EButtonState KeyboardState::GetKeyState(const SDL_Scancode keyCode) const
{
	EButtonState state = EButtonState::NONE;
	if (!mPrevState[keyCode])
	{
		if (mpCurrState[keyCode])
		{
			state = EButtonState::PRESSED;
		}
	}
	else
	{
		if (!mpCurrState[keyCode])
		{
			state = EButtonState::RELEASED;
		}
		else
		{
			state = EButtonState::HELD;
		}
	}

	return state;
}

bool MouseState::GetButtonValue(const int button) const
{
	return (mCurrButtons & SDL_BUTTON_MASK(button));
}

EButtonState MouseState::GetButtonState(const int button) const
{
	int mask = SDL_BUTTON_MASK(button);

	EButtonState state = EButtonState::NONE;
	if ((mask & mPrevButtons) == 0)
	{
		if ((mask & mCurrButtons) != 0)
		{
			state = EButtonState::PRESSED;
		}
	}
	else
	{
		if ((mask & mCurrButtons) == 0)
		{
			state = EButtonState::RELEASED;
		}
		else
		{
			state = EButtonState::HELD;
		}
	}

	return state;
}

bool ControllerState::GetButtonValue(const SDL_GamepadButton button) const
{
	return mCurrButtons[button];
}

EButtonState ControllerState::GetButtonState(const SDL_GamepadButton button) const
{
	EButtonState state = EButtonState::NONE;
	if (!mPrevButtons[button])
	{
		if (mCurrButtons[button])
		{
			state = EButtonState::PRESSED;
		}
	}
	else
	{
		if (!mCurrButtons[button])
		{
			state = EButtonState::RELEASED;
		}
		else
		{
			state = EButtonState::HELD;
		}
	}

	return state;
}
