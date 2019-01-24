#pragma once
#include "Drawable.h"
#include "CallbackInterface.h"

class Animation : public CallbackInterface<Drawable>
{
public:
	Animation(Drawable* _drawable, float _startingX, float _startingY, float _targetX, float _targetY, float _totalAnimTime);
	virtual ~Animation() = default;

	//reset the animation params
	void ResetAnimationParams(float _targetX, float _targetY, float _totalAnimTime);
	
	virtual void Update(float deltaSeconds);
	
	//set a callback for when the animation is completed
	void SetCallback(Drawable* owner, Callback finishedCallback);

	//get the drawable this animation moves
	inline const Drawable* const GetDrawable() const { return mDrawable; }
private:
	Callback OnAnimationFinished;

	float startingX, startingY; //start positions for the animation
	float targetX, targetY;		//target positions for the animation
	float currentTime;			//elapsed time since animation start
	float totalAnimTime;		//total time required to reach the target

	Drawable* mDrawable;		//the drawable to animate

public:
	bool markedForDelete;		//should this animation be deleted in the next frame
};