#include "Sprite.h"

Sprite::Sprite()
	: mTexture(SDLEngine::Engine::TEXTURE_MAX)
	, Drawable()
{

}

Sprite::Sprite(Texture _Texture, float _x, float _y, float _rot, float _scale)
	: mTexture(_Texture)
	, Drawable(_x, _y, _rot, _scale)
{

}

Sprite::~Sprite()
{

}

void Sprite::SetTexture(const Texture& Texture)
{
	mTexture = Texture;
}

const Sprite::Texture& Sprite::GetTexture() const
{
	return mTexture;
}

void Sprite::Update(const SDLEngine::Engine& engine)
{

}

void Sprite::Render(const SDLEngine::Engine& engine) const
{
	engine.Render(mTexture, xPos, yPos, rotation);
}