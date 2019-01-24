#pragma once
#include "Drawable.h"
#include "CallbackInterface.h"
#include <SDLWrapper/Updater.h>
#include "GameStates.h"

template<typename T>
class Timer : public Drawable, public CallbackInterface<T>
{
public:
	Timer(float _maxTime, T* _owner = nullptr, Callback _onTimerFinished = nullptr);
	~Timer() = default;

	void Update(const SDLEngine::Engine& engine);
	void Render(const SDLEngine::Engine& engine) const;

private:
	float maxTimeInSeconds;
	float currentTimeInSeconds;

	Callback callback;
};

//template function definitions-----------------------------------------------------

template<typename T>
Timer<T>::Timer(float _maxTime, T* _owner, Callback _onTimerFinished)
	: Drawable()
	, CallbackInterface<T>(_owner)
	, maxTimeInSeconds(_maxTime)
	, currentTimeInSeconds(0.f)
	, callback(_onTimerFinished)
{

}

template<typename T>
void Timer<T>::Update(const SDLEngine::Engine& engine)
{
	currentTimeInSeconds += engine.GetLastFrameSeconds();
	if (currentTimeInSeconds >= maxTimeInSeconds)
	{
		//timer finished. callback time!
		currentTimeInSeconds = maxTimeInSeconds;
		if (mOwner && callback)
		{
			(mOwner->*callback)(this);
		}
	}
}

template<typename T>
void Timer<T>::Render(const SDLEngine::Engine& engine) const
{
	int remainingTime = (int)(maxTimeInSeconds - currentTimeInSeconds);
	int remainingMinutes = remainingTime / 60;
	int remainingSeconds = remainingTime % 60;

	//convert remaining time to text
	std::string textMinutes = std::to_string(remainingMinutes);
	std::string textSeconds = std::to_string(remainingSeconds);
	std::string textTime = textMinutes.append(":").append(textSeconds);

	engine.Write(textTime.c_str(), xPos, yPos, rotation);
}