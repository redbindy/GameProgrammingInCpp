#include "UIScreen.h"

#include "Game.h"
#include "Font.h"
#include "Texture.h"
#include "Shader.h"
#include "Renderer.h"

UIScreen::UIScreen(Game* const pGame)
	: mpGame(pGame)
	, mpFont(nullptr)
	, mpTitle(nullptr)
	, mTitlePos(0.f, 300.f)
	, mState(EUIState::ACTIVE)
	, mpButtonOn(nullptr)
	, mpButtonOff(nullptr)
	, mNextButtonPos(0.f, 200.f)
	, mButtons()
	, mBGPos(0.f, 250.f)
	, mpBackground(nullptr)
{
	pGame->PushUI(this);
	mpFont = pGame->GetFont("Assets/Carlito-Regular.ttf");

	Renderer* const pRenderer = pGame->GetRenderer();

	mpButtonOn = pRenderer->GetTexture("Assets/ButtonYellow.png");
	mpButtonOff = pRenderer->GetTexture("Assets/ButtonBlue.png");
}

UIScreen::~UIScreen()
{
	if (mpTitle != nullptr)
	{
		mpTitle->Unload();

		delete mpTitle;
	}

	for (Button* const p : mButtons)
	{
		delete p;
	}
	mButtons.clear();
}

void UIScreen::Update(const float deltaTime)
{

}

void UIScreen::Draw(Shader* const pShader)
{
	if (mpBackground != nullptr)
	{
		DrawTexture(pShader, mpBackground, mBGPos);
	}

	if (mpTitle != nullptr)
	{
		DrawTexture(pShader, mpTitle, mTitlePos);
	}

	for (Button* const p : mButtons)
	{
		Texture* const pTex = p->GetHighlighted() ? mpButtonOn : mpButtonOff;

		DrawTexture(pShader, pTex, p->GetPosition());

		DrawTexture(pShader, p->GetNameTex(), p->GetPosition());
	}
}

void UIScreen::ProcessInput(const bool* const pbKeys)
{
	if (!mButtons.empty())
	{
		float x;
		float y;
		SDL_GetMouseState(&x, &y);

		Renderer* const pRenderer = mpGame->GetRenderer();

		const Vector2 mousePos(x - pRenderer->GetScreenWidth() * 0.5f, pRenderer->GetScreenHeight() * 0.5f - y);

		for (Button* const p : mButtons)
		{
			const bool bSel = p->ContainsPoint(mousePos);

			p->SetHighlighted(bSel);
		}
	}
}

void UIScreen::HandleKeyPress(const int key)
{
	switch (key)
	{
	case SDL_BUTTON_LEFT:
		if (!mButtons.empty())
		{
			for (Button* const p : mButtons)
			{
				if (p->GetHighlighted())
				{
					p->OnClick();

					break;
				}
			}
		}
		break;

	default:
		break;
	}
}

void UIScreen::Close()
{
	mState = EUIState::CLOSING;
}

void UIScreen::SetTitle(const std::string& text, const Vector3& color, int pointSize)
{
	if (mpTitle != nullptr)
	{
		mpTitle->Unload();

		delete mpTitle;

		mpTitle = nullptr;
	}
	mpTitle = mpFont->RenderText(text, color, pointSize);
}

void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
{
	const Vector2 dims(mpButtonOn->GetWidth(), mpButtonOn->GetHeight());

	Button* const pButton = new Button(
		name,
		mpFont,
		onClick,
		mNextButtonPos,
		dims
	);

	mButtons.push_back(pButton);

	mNextButtonPos.Y -= mpButtonOff->GetHeight() + 20.f;
}

void UIScreen::DrawTexture(Shader* const pShader, Texture* const pTexture, const Vector2& offset, const float scale, const bool bFlipY)
{
	float yScale = pTexture->GetHeight() * scale;
	if (bFlipY)
	{
		yScale *= -1.f;
	}

	const Matrix4 scaleMat = Matrix4::CreateScale(
		pTexture->GetWidth() * scale,
		yScale,
		1.f
	);

	const Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.X, offset.Y, 0.f));

	const Matrix4 world = scaleMat * transMat;
	pShader->SetMatrixUniform("uWorldTransform", world);

	pTexture->SetActive();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::setRelativeMouseMode(const bool bRelative)
{
	Renderer* const pRenderer = mpGame->GetRenderer();
	SDL_Window* const pWindow = pRenderer->GetSDLWindow();

	SDL_SetWindowRelativeMouseMode(pWindow, bRelative);

	if (bRelative)
	{
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
}

Button::Button(
	const std::string& name,
	Font* const pFont,
	std::function<void()> onClick,
	const Vector2& pos,
	const Vector2& dims
)
	: mOnClickFunc(onClick)
	, mName(name)
	, mpFont(pFont)
	, mPosition(pos)
	, mDimensions(dims)
	, mbHighlighted(false)
{
	SetName(name);
}

Button::~Button()
{
	if (mpNameTex != nullptr)
	{
		mpNameTex->Unload();

		delete mpNameTex;
	}
}

void Button::SetName(const std::string& name)
{
	mName = name;

	if (mpNameTex != nullptr)
	{
		mpNameTex->Unload();

		delete mpNameTex;

		mpNameTex = nullptr;
	}

	mpNameTex = mpFont->RenderText(mName);
}

bool Button::ContainsPoint(const Vector2& pt) const
{
	const Vector2 halfDimensions = mDimensions * 0.5f;

	const bool bNo = pt.X < (mPosition.X - halfDimensions.X)
		|| pt.X >(mPosition.X + halfDimensions.X)
		|| pt.Y < (mPosition.Y - halfDimensions.Y)
		|| pt.Y >(mPosition.Y + halfDimensions.Y);

	return !bNo;
}

void Button::OnClick()
{
	if (mOnClickFunc != nullptr)
	{
		mOnClickFunc();
	}
}
