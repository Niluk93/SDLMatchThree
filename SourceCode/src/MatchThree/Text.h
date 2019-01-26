#pragma once
#include "Drawable.h"
#include <string>

enum class ETextAlignment
{
	Left,
	Middle,
	Right
};

class Text : public Drawable
{
	typedef SDLWrapper::Engine::Texture Texture;

public:
	Text();
	Text(const std::string& _Text, float _x = 0.f, float _y = 0.f, float _rot = 0.f, float _scale = 1.0f);
	~Text() = default;

	void SetText(const std::string& text);
	const char* GetText() const;

	void SetAlignment(const ETextAlignment& _alignment);

	void Update(const SDLWrapper::Engine& engine);
	void Render(const SDLWrapper::Engine& engine) const;

private:
	std::string mText;
	ETextAlignment mAlignment;
};