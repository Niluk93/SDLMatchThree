#pragma once
#include "Text.h"

//class to maintain game score and draw it on the screen.
class Score : public Text
{
public:
	Score(int _pointsPerBlock);

	void IncrementScore(int numBlocksDestroyed = 0);
	inline int GetScore() const { return score; }
private:
	int score;
	int pointsPerBlock;
};