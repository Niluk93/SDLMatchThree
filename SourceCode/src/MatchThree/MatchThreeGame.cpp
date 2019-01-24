//Game
#include "MatchThreeGame.h"
#include "GameStates.h"

//std
#include <ctime>
#include <random>

MatchThreeGame::MatchThreeGame()
	: mEngine("./assets")
{
	std::srand(static_cast<uint32_t>(std::time(nullptr)));
}

void MatchThreeGame::Start()
{
	//Start game in main menu
	mGameState = new GameStateMainMenu();

	mEngine.Start(*this);
}

void MatchThreeGame::Update()
{
	if (!mGameState || !mGameState->Update(mEngine))
	{
		//From Main Menu always start game
		if (GameStateMainMenu* pMainMenu = dynamic_cast<GameStateMainMenu*>(mGameState))
		{
			mGameState = new GameStateInGame(mEngine);
			delete pMainMenu;
		}
		//From Game always show post game screen
		else if (GameStateInGame* pInGame = dynamic_cast<GameStateInGame*>(mGameState))
		{
			mGameState = new GameStatePostGame(pInGame->GetFinalScore());
			delete pInGame;
		}
		//From post game screen always start game
		else if (GameStatePostGame* pPostGame = dynamic_cast<GameStatePostGame*>(mGameState))
		{
			mGameState = new GameStateInGame(mEngine);
			delete pPostGame;
		}
	}
}