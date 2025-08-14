#include "PauseMenu.h"

#include "Game.h"
#include "Renderer.h"
#include "DialogBox.h"

PauseMenu::PauseMenu(Game* const pGame)
	: UIScreen(pGame)
{
	pGame->SetState(Game::EGameState::PAUSED);

	setRelativeMouseMode(false);

	SetTitle("PauseTitle");

	AddButton(
		"ResumeButton",
		[this]()
		{
			Close();
		}
	);

	AddButton(
		"QuitButton", [this]()
		{
			new DialogBox(mpGame, "QuitText",
				[this]()
				{
					mpGame->SetState(Game::EGameState::QUIT);
				}
			);
		}
	);
}

PauseMenu::~PauseMenu()
{
	setRelativeMouseMode(true);
	mpGame->SetState(Game::EGameState::GAME_PLAY);
}

void PauseMenu::HandleKeyPress(const int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}
