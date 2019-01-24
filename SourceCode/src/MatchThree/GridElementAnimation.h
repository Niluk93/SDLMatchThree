#pragma once
#include "Animation.h"

class GridElementAnimation : public Animation
{
public:
	GridElementAnimation(Drawable* _drawable, float _startingX, float _startingY, float _targetX, float _targetY, float _totalAnimTime, int _row, int _column)
		: Animation(_drawable, _startingX, _startingY, _targetX, _targetY, _totalAnimTime)
		, row(_row)
		, column(_column)
	{}

	inline void GetGridIndex(int& _row, int& _column) const { _row = row; _column = column; }
private:
	int row, column;
};