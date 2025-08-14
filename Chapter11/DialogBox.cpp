#include "DialogBox.h"

#include "Renderer.h"
#include "Game.h"

DialogBox::DialogBox(Game* const pGame, const std::string& text, std::function<void()> onOk)
	: UIScreen(pGame)
{
	mBGPos = Vector2(0.f, 0.f);
	mTitlePos = Vector2(0.f, 100.f);
	mNextButtonPos = Vector2(0.f, 0.f);

	Renderer* const pRenderer = mpGame->GetRenderer();

	mpBackground = pRenderer->GetTexture("Assets/DialogBG.png");

	SetTitle(text, Vector3::Zero, 30);

	AddButton("OKButton",
		[onOk]()
		{
			onOk();
		}
	);

	AddButton(
		"CancelButton",
		[this]()
		{
			Close();
		}
	);
}

DialogBox::~DialogBox()
{
}
