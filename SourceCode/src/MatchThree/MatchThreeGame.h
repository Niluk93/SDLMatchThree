#pragma once
#include <SDLWrapper/Updater.h>
#include <SDLWrapper/Engine.h>

#include <vector>

class GameStateBase;

class MatchThreeGame : public SDLWrapper::Updater
{
public:
	MatchThreeGame();

	void Start();
	void Update();

private:
	SDLWrapper::Engine mEngine;

	GameStateBase* mGameState;
};