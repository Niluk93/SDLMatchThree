#include "Text.h"

Text::Text()
{

}

Text::Text(const std::string& _text, float _x, float _y, float _rot, float _scale)
	: Drawable(_x, _y, _rot, _scale)
	, mText(_text)
	, mAlignment(ETextAlignment::Left)
{

}

void Text::SetText(const std::string& _text)
{
	mText = _text;
}

const char* Text::GetText() const
{
	return mText.c_str();
}

void Text::SetAlignment(const ETextAlignment& _alignment)
{
	mAlignment = _alignment;
}

void Text::Update(const SDLWrapper::Engine& engine)
{

}

void Text::Render(const SDLWrapper::Engine& engine) const
{
	float actualX = xPos;

	switch (mAlignment)
	{
	case ETextAlignment::Left:
		actualX = xPos; // by default, we are aligned left
		break;
	case ETextAlignment::Middle:
		actualX = xPos - engine.CalculateStringWidth(mText.c_str()) / 2.0f;
		break;
	case ETextAlignment::Right:
		actualX = xPos - engine.CalculateStringWidth(mText.c_str());
		break;
	default:
		break;
	}

	engine.Write(mText.c_str(), actualX, yPos, rotation, scale);
}
