#pragma once
#include <SDLWrapper/Engine.h>

//Base class for anything that renders
class Drawable
{
public:
	friend class GameStateBase;

	enum EDrawableFlags
	{
		FlagNone	= 0,
		FlagUpdate	= 1 << FlagNone,
		FlagShow	= 1 << FlagUpdate,
		FlagAll		= FlagUpdate | FlagShow
	};

public:
	Drawable() : drawFlags(FlagAll), xPos(0.f), yPos(0.f), rotation(0.f), scale(1.f) {}
	Drawable(float _x, float _y, float _rot, float _scale) : drawFlags(FlagAll), xPos(_x), yPos(_y), rotation(_rot), scale(_scale) {}
	virtual ~Drawable() {}

	virtual void	Update(const SDLWrapper::Engine& engine) = 0;
	virtual void	Render(const SDLWrapper::Engine& engine) const = 0;

	inline void		SetPosition(float _x, float _y)				{ xPos = _x; yPos = _y; }
	inline void		GetPosition(float& _x, float& _y) const		{ _x = xPos; _y = yPos; }
	
	inline void		SetRotation(float _rot)						{ rotation = _rot; }
	inline void		Rotate(float _rot)							{ rotation += _rot; }
	inline float	GetRotation() const							{ return rotation; }

	inline void		SetScale(float _scale)						{ scale = _scale; }
	inline void		Scale(float _scale)							{ scale *= _scale; }
	inline float	GetScale() const							{ return scale; }

	inline void		SetDrawFlags(uint8_t _drawFlags)			{ drawFlags = _drawFlags; }
	inline void		SetDrawFlags(EDrawableFlags _drawFlags)		{ drawFlags = _drawFlags; }
protected:
	uint8_t drawFlags;
	float xPos, yPos;
	float rotation;
	float scale;
};