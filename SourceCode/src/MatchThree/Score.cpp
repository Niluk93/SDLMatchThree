#include "Score.h"
#include <string>

Score::Score(int _pointsPerBlock)
	: score(0)
	, pointsPerBlock(_pointsPerBlock)
{

}

void Score::IncrementScore(int numBlocksDestroyed)
{
	score += numBlocksDestroyed * pointsPerBlock;
}

void Score::Render(const SDLEngine::Engine& engine) const
{
	char scoreBuff[8];
	std::snprintf(scoreBuff, sizeof(scoreBuff), "%04d", score);
	engine.Write(scoreBuff, xPos, yPos, rotation);
}