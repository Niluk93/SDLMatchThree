#include "Score.h"

Score::Score(int _pointsPerBlock)
	: Text("0")
	, score(0)
	, pointsPerBlock(_pointsPerBlock)
{

}

void Score::IncrementScore(int numBlocksDestroyed)
{
	score += numBlocksDestroyed * pointsPerBlock;

	char scoreBuff[8];
	std::snprintf(scoreBuff, sizeof(scoreBuff), "%d", score);
	SetText(scoreBuff);
}
