#pragma once
#include "Text.h"
#include "CallbackInterface.h"
#include <SDLWrapper/Updater.h>
#include "GameStates.h"

template<typename T>
class Timer : public Text, public CallbackInterface<T>
{
public:
	Timer(float _maxTime, T* _owner = nullptr, Callback _onTimerFinished = nullptr);
	~Timer() = default;

	void Update(const SDLWrapper::Engine& engine) override;

private:
	float maxTimeInSeconds;
	float currentTimeInSeconds;

	Callback callback;
};

//template function definitions-----------------------------------------------------

template<typename T>
Timer<T>::Timer(float _maxTime, T* _owner, Callback _onTimerFinished)
	: Text("")
	, CallbackInterface<T>(_owner)
	, maxTimeInSeconds(_maxTime)
	, currentTimeInSeconds(0.f)
	, callback(_onTimerFinished)
{

}

template<typename T>
void Timer<T>::Update(const SDLWrapper::Engine& engine)
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

	int remainingTime = static_cast<int>(maxTimeInSeconds - currentTimeInSeconds);
	int remainingMinutes = remainingTime / 60;
	int remainingSeconds = remainingTime % 60;

	//convert remaining time to text
	std::string textMinutes = std::to_string(remainingMinutes);
	std::string textSeconds = std::to_string(remainingSeconds);
	std::string textTime = textMinutes.append(":").append(textSeconds);

	SetText(textTime);
}
