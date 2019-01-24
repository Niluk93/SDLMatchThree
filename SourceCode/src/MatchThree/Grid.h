#pragma once
#include "Drawable.h"

#include <vector>

class Sprite;
class Animation;

template<typename T>
class CallbackInterface;

class Score;

struct GridElement
{
public:
	GridElement(int _row, int _col, Sprite* _sprite) : row(_row), col(_col), sprite(_sprite) { }
public:
	int row, col;
	Sprite* sprite;
};

class Grid : public Drawable
{
public:
	typedef CallbackInterface<Drawable> DrawablePtr;
	typedef void (Grid::*GridAnimCallback)(const DrawablePtr*);

public:
	Grid(int sizeX, int sizeY);
	~Grid();

	void InitGrid(float x, float y, float spacing);
	void PopulateSprites(const SDLEngine::Engine& Engine);

	void Update(const SDLEngine::Engine& Engine);
	void Render(const SDLEngine::Engine& Engine) const;
	
	int GetCurrentScore() const;

	//Animation callbacks///////////////////////////////////////////////////

	//called when the animation to move elements down upon destroying an existing element is finished
	void OnDropDownAnimFinished(const DrawablePtr* anim); 
	//called when the animation to fill out empty spots is finished
	void OnResetEmptySpotsAnimFinished(const DrawablePtr* anim);
	//called when two elements have finished swapping
	void OnSwapComplete(const DrawablePtr* anim);
	////////////////////////////////////////////////////////////////////////

private:
	//sets a texture for the provided row and column in a way that it doesn't form a possible match immediately
	void SetSafeTextureForSprite(int row, int col);
	
	//calculates a position for the provided element, starts an animation and assigns a callback for when it is finished if any
	void CalculatePosition(int row, int col, bool isNewElement = false, GridAnimCallback onMoveFinishedCallback = nullptr);
	
	//Swap 2 elements. Optionally choose whether we want the input to be blocked as we wait for swap to finish.
	void Swap(int row, int col, bool waitForSwap = true);

	//check if there is a potential match around the provided row and column.
	bool CheckForMatch(int row, int col);

	//gets the first/last index of texture type provided in the row/column based on the row/column step
	int GetExtremesForTextureType(const SDLEngine::Engine::Texture& texture, int rowStart, int columnStart, int rowStep, int columnStep);

	//move the removed element to a temporary vector, and start moving elements above it to its position. 
	//Optionally provide a callback for when the objects above it have reached the target
	void ClearRemovedElements(int col, int startingRow, int endingRow, GridAnimCallback clearFinishedCallback = nullptr);

	
private:
	Sprite*** mGridElements;					//array of grid elements
	Score* mScore;								//current game score
	
	int sizeX, sizeY;							//size of the grid

	float averageTexWidth, averageTexHeight;	//average width/height of the texture. used for positioning a sprite.
	float gridSpacing;							//spacing between two sprites.

	int selectedX, selectedY;					//currently selected grid element.

	bool waitingForSwap;						//should we wait for the swap animation to complete before allowing input.
	bool mouseDownLastFrame;					//was the mouse down last frame

	std::vector<Animation*> mAnimations;		//list of animations currently playing on the grid
	std::vector<GridElement> mElementsToRemove;	//list of elements that need to be reset 
};