#pragma once
#include "Drawable.h"

//class to maintain game score and draw it on the screen.
class Score : public Drawable
{
public:
	Score(int _pointsPerBlock);
	void Update(const SDLEngine::Engine& engine) { }
	void Render(const SDLEngine::Engine& engine) const;

	void IncrementScore(int numBlocksDestroyed = 0);
	inline int GetScore() const { return score; }
private:
	int score;
	int pointsPerBlock;
};