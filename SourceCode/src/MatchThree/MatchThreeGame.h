#pragma once
#include <SDLWrapper/Updater.h>
#include <SDLWrapper/Engine.h>

#include <vector>

class GameStateBase;

class MatchThreeGame : public SDLEngine::Updater
{
public:
	MatchThreeGame();

	void Start();
	void Update();

private:
	SDLEngine::Engine mEngine;

	GameStateBase* mGameState;
};