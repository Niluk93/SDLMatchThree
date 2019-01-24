#pragma once
#include <SDLWrapper/Engine.h>
#include <vector>
#include <string>

class Drawable;

template<typename T>
class CallbackInterface;

//base class for different game states
class GameStateBase
{
public:
	GameStateBase();
	virtual ~GameStateBase();

	virtual bool Update(const SDLWrapper::Engine& engine) = 0;

protected:
	std::vector<Drawable*> mDrawables;
};

//game state for main menu. this will lead to the main game.
class GameStateMainMenu : public GameStateBase
{
public:
	GameStateMainMenu();
	~GameStateMainMenu();

	bool Update(const SDLWrapper::Engine& engine);
private:
	const char* startMessage;
};

//game state for when the player is playing the game.
class GameStateInGame : public GameStateBase
{
public:
	typedef CallbackInterface<GameStateBase> TimerClass;

public:
	GameStateInGame(const SDLWrapper::Engine& engine);
	~GameStateInGame();

	void OnGameTimerFinished(const TimerClass* timer);
	bool Update(const SDLWrapper::Engine& engine);
	int GetFinalScore() const;

private:
	bool isGameRunning;
};

//game state for when the player finishes the game. displays final score, and leads back to the in-game game state.
class GameStatePostGame : public GameStateBase
{
public:
	GameStatePostGame(int _score);
	~GameStatePostGame();

	bool Update(const SDLWrapper::Engine& engine);

private:
	std::string mScoreText;
};