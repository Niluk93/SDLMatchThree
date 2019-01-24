#pragma once
#include <SDLWrapper/Engine.h>

//Base class for anything that renders
class Drawable
{
public:
	Drawable() : xPos(0.f), yPos(0.f), rotation(0.f), scale(1.f) {}
	Drawable(float _x, float _y, float _rot, float _scale) : xPos(_x), yPos(_y), rotation(_rot), scale(_scale) {}
	virtual ~Drawable() {}

	virtual void Update(const SDLEngine::Engine& engine) = 0;
	virtual void Render(const SDLEngine::Engine& engine) const = 0;

	inline void SetPosition(float x, float y) { xPos = x; yPos = y; }
	inline void GetPosition(float& x, float& y) const { x = xPos; y = yPos; }

protected:
	float xPos, yPos;
	float rotation;
	float scale;
};