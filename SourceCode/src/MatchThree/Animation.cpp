#include "Animation.h"

Animation::Animation(Drawable* _drawable, float _startingX, float _startingY, float _targetX, float _targetY, float _totalAnimTime)
	: CallbackInterface<Drawable>(nullptr)
	, OnAnimationFinished(nullptr)
	, mDrawable(_drawable)
	, startingX(_startingX)
	, startingY(_startingY)
	, targetX(_targetX)
	, targetY(_targetY)
	, totalAnimTime(_totalAnimTime)
	, markedForDelete(false)
{

}

void Animation::ResetAnimationParams(float _targetX, float _targetY, float _totalAnimTime)
{
	targetX = _targetX;
	targetY = _targetY;
	currentTime = 0.f;
	totalAnimTime = _totalAnimTime;
	markedForDelete = false;
}

void Animation::Update(float deltaSeconds)
{
	if (!mDrawable)
		return;

	currentTime += deltaSeconds;
	if (currentTime > totalAnimTime)
		currentTime = totalAnimTime;

	//lerp between start and end position to find current position
	float animPercentage = currentTime / totalAnimTime;
	float xPos = startingX + (animPercentage * (targetX - startingX));
	float yPos = startingY + (animPercentage * (targetY - startingY));

	mDrawable->SetPosition(xPos, yPos);

	//callback for animation completed
	if (animPercentage >= 1.0f)
	{
		if(mOwner && OnAnimationFinished)
			(mOwner->*OnAnimationFinished)(this);

		//animation completed. mark for delete.
		markedForDelete = true;
	}
}

void Animation::SetCallback(Drawable* owner, Callback finishedCallback)
{
	if (owner && finishedCallback)
	{
		mOwner = owner;
		OnAnimationFinished = finishedCallback;
	}
}