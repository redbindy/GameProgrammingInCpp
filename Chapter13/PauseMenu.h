#pragma once

#include <SDL3/SDL.h>

#include "UIScreen.h"

class PauseMenu final : UIScreen
{
public:
	PauseMenu(Game* const pGame);
	virtual ~PauseMenu();

	void HandleKeyPress(const int key) override;

private:
};