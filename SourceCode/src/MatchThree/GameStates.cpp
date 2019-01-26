#include "GameStates.h"
#include "Sprite.h"
#include "Grid.h"
#include "Timer.h"

//GameStateBase/////////////////////////////////////////////////
GameStateBase::GameStateBase()
{

}

GameStateBase::~GameStateBase()
{
	for (int i = mDrawables.size() - 1; i >= 0; --i)
	{
		Drawable* currentDrawable = mDrawables[i];
		if (currentDrawable)
		{
			mDrawables[i] = nullptr;
			delete currentDrawable;
		}
	}

	mDrawables.clear();
}

bool GameStateBase::Update(const SDLWrapper::Engine& engine)
{
	for (Drawable* pCurrentDrawable : mDrawables)
	{
		if(pCurrentDrawable->drawFlags & Drawable::FlagUpdate)
			pCurrentDrawable->Update(engine);

		if(pCurrentDrawable->drawFlags & Drawable::FlagShow)
			pCurrentDrawable->Render(engine);
	}

	//base game state will only update and render drawables
	//exit conditions are derived game state's responsibility
	return true;
}

//End GameStateBase/////////////////////////////////////////////////

//GameStateMainMenu/////////////////////////////////////////////////
GameStateMainMenu::GameStateMainMenu()
{
	Text* startMessage = new Text("Click anywhere to start!", SDLWrapper::Engine::GetWidth() / 2.f, SDLWrapper::Engine::GetHeight() / 2.f);
	startMessage->SetAlignment(ETextAlignment::Middle);
	startMessage->SetDrawFlags(Drawable::FlagShow);

	mDrawables.push_back(startMessage);
}

GameStateMainMenu::~GameStateMainMenu()
{

}

bool GameStateMainMenu::Update(const SDLWrapper::Engine& engine)
{
	GameStateBase::Update(engine);

	//Exit from main menu if the player clicks anywhere
	return !engine.GetMouseButtonDown();
}
//End GameStateMainMenu/////////////////////////////////////////////////

//GameStateInGame//////////////////////////////////////////////////////
GameStateInGame::GameStateInGame(const SDLWrapper::Engine& engine)
	: isGameRunning(true)
{
	//Init background
	mDrawables.push_back(new Sprite(SDLWrapper::Engine::TEXTURE_BACKGROUND));
	mDrawables[0]->SetDrawFlags(Drawable::FlagShow);

	//Init game grid
	int GridSizeX = 8;
	int GridSizeY = 10;
	Grid* grid = new Grid(GridSizeX, GridSizeY);
	grid->InitGrid(36.f, 100.f, 8.f);
	grid->PopulateSprites(engine);
	mDrawables.push_back(grid);

	typedef Timer<GameStateBase> GameStateTimer;

	//Init game timer
	GameStateTimer* timer = new GameStateTimer(60.f, this, static_cast<GameStateTimer::Callback>(&GameStateInGame::OnGameTimerFinished));
	timer->SetPosition((engine.GetWidth() / 2.f), 25.f);
	timer->SetAlignment(ETextAlignment::Middle);
	mDrawables.push_back(timer);
}

GameStateInGame::~GameStateInGame()
{

}

void GameStateInGame::OnGameTimerFinished(const TimerClass* timer)
{
	isGameRunning = false;
}

int GameStateInGame::GetFinalScore() const
{
	Grid* grid = dynamic_cast<Grid*>(mDrawables[1]);
	if (grid)
	{
		return grid->GetCurrentScore();
	}

	return -1;
}

bool GameStateInGame::Update(const SDLWrapper::Engine& engine)
{
	GameStateBase::Update(engine);

	//Exit game if the timer runs out
	return isGameRunning;
}
//End GameStateMainMenu/////////////////////////////////////////////////

//GameStatePostGame/////////////////////////////////////////////////////
GameStatePostGame::GameStatePostGame(int _score)
	: waitForRestart(true)
{
	std::string scoreNumber = std::to_string(_score);
	scoreNumber = std::string("Final Score - ").append(scoreNumber);;

	Text* endScreenText = new Text(scoreNumber, SDLWrapper::Engine::GetWidth() / 2.f, SDLWrapper::Engine::GetHeight() / 2.f);
	endScreenText->SetAlignment(ETextAlignment::Middle);
	endScreenText->SetDrawFlags(Drawable::FlagShow);
	mDrawables.push_back(endScreenText);

	typedef Timer<GameStateBase> GameStateTimer;

	//Init wait timer. To make sure we don't accidentally restart as soon as we enter post game
	GameStateTimer* timer = new GameStateTimer(2.f, this, static_cast<GameStateTimer::Callback>(&GameStatePostGame::OnWaitTimerFinished));
	timer->SetDrawFlags(Drawable::FlagUpdate);
	mDrawables.push_back(timer);
}

GameStatePostGame::~GameStatePostGame()
{

}

bool GameStatePostGame::Update(const SDLWrapper::Engine& engine)
{
	GameStateBase::Update(engine);

	//Exit post game screen if player clicks anywhere
	return !engine.GetMouseButtonDown() || waitForRestart;
}

void GameStatePostGame::OnWaitTimerFinished(const TimerClass* timer)
{
	waitForRestart = false;
}

//End GameStatePostGame/////////////////////////////////////////////////