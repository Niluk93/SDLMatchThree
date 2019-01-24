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
//End GameStateBase/////////////////////////////////////////////////

//GameStateMainMenu/////////////////////////////////////////////////
GameStateMainMenu::GameStateMainMenu()
	: startMessage("Click anywhere to start!")
{

}

GameStateMainMenu::~GameStateMainMenu()
{

}

bool GameStateMainMenu::Update(const SDLEngine::Engine& engine)
{
	float stringWidth = engine.CalculateStringWidth(startMessage);
	float xPos = (engine.GetWidth() / 2.f) - (stringWidth / 2.f);
	float yPos = engine.GetHeight() / 2.f;

	engine.Write(startMessage, xPos, yPos);

	//Exit from main menu if the player clicks anywhere
	return !engine.GetMouseButtonDown();
}
//End GameStateMainMenu/////////////////////////////////////////////////

//GameStateInGame//////////////////////////////////////////////////////
GameStateInGame::GameStateInGame(const SDLEngine::Engine& engine)
	: isGameRunning(true)
{
	//Init background
	mDrawables.push_back(new Sprite(SDLEngine::Engine::TEXTURE_BACKGROUND));

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
	timer->SetPosition((engine.GetWidth() / 2.f) - (engine.CalculateStringWidth("0:00") / 2.f), 25.f);
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

bool GameStateInGame::Update(const SDLEngine::Engine& engine)
{
	for (Drawable* pCurrentDrawable : mDrawables)
	{
		pCurrentDrawable->Update(engine);
		pCurrentDrawable->Render(engine);
	}

	//Exit game if the timer runs out
	return isGameRunning;
}
//End GameStateMainMenu/////////////////////////////////////////////////

//GameStatePostGame/////////////////////////////////////////////////////
GameStatePostGame::GameStatePostGame(int _score)
{
	std::string scoreNumber = std::to_string(_score);
	mScoreText = std::string("Final Score - ").append(scoreNumber);;
}

GameStatePostGame::~GameStatePostGame()
{

}

bool GameStatePostGame::Update(const SDLEngine::Engine& engine)
{
	float stringWidth = engine.CalculateStringWidth(mScoreText.c_str());
	float xPos = (engine.GetWidth() / 2.f) - (stringWidth / 2.f);
	float yPos = engine.GetHeight() / 2.f;

	engine.Write(mScoreText.c_str(), xPos, yPos);
	
	//Exit post game screen if player clicks anywhere
	return !engine.GetMouseButtonDown();
}
//End GameStatePostGame/////////////////////////////////////////////////