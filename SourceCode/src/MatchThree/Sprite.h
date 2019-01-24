#pragma once
#include "Drawable.h"

class Sprite : public Drawable
{
	typedef SDLEngine::Engine::Texture Texture;

public:
	Sprite();
	Sprite(Texture _Texture, float _x = 0.f, float _y = 0.f, float _rot = 0.f, float _scale = 1.0f);
	~Sprite();

	void SetTexture(const Texture& texture);
	const Texture& GetTexture() const;

	void Update(const SDLEngine::Engine& engine);
	void Render(const SDLEngine::Engine& engine) const;

private:
	Texture mTexture;
};