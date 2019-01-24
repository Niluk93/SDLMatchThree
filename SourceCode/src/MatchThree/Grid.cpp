//Game
#include "Grid.h"
#include "Sprite.h"
#include "Score.h"
#include "GridElementAnimation.h"

//Std
#include <random>

const int index_none = -1;
const int step_positive = 1;
const int step_negative = -1;
const int number_of_consecutive_matches = 3;
const int points_per_block = 100;

Grid::Grid(int _SizeX, int _SizeY)
	: Drawable()
	, mScore(new Score(points_per_block))
	, sizeX(_SizeX)
	, sizeY(_SizeY)
	, gridSpacing(0.f)
	, averageTexWidth(0.f)
	, averageTexHeight(0.f)
	, selectedX(-1)
	, selectedY(-1)
{
	//init columns array
	mGridElements = new Sprite**[sizeY];
	for (int i = 0; i < sizeY; ++i)
	{
		//init rows array
		mGridElements[i] = new Sprite*[sizeX];
	}

	for (int row = 0; row < sizeY; ++row)
	{
		for (int col = 0; col < sizeX; ++col)
		{
			//init all sprites
			mGridElements[row][col] = new Sprite();
		}
	}

	mScore->SetPosition(SDLEngine::Engine::GetWidth() / 2.f - SDLEngine::Engine::CalculateStringWidth("0000") / 2.f,
						SDLEngine::Engine::GetHeight() - 70.f);
}

Grid::~Grid()
{
	//delete all running animations
	for (int i = mAnimations.size() -1; i >= 0; --i)
	{
		Animation* currAnimation = mAnimations[i];
		if (currAnimation)
		{
			mAnimations[i] = nullptr;
			delete currAnimation;
		}
	}
	mAnimations.clear();

	//delete the grid
	for (int row = 0; row < sizeY; ++row)
	{
		//delete the row array
		for (int col = 0; col < sizeX; ++col)
		{
			//delete all the sprites
			Sprite* currSprite = mGridElements[row][col];
			mGridElements[row][col] = nullptr;
			delete currSprite;
		}
		delete[] mGridElements[row];
	}
	delete[] mGridElements;

	//delete the score
	delete mScore;
}

void Grid::InitGrid(float x, float y, float spacing)
{
	xPos = x;
	yPos = y;
	gridSpacing = spacing;
}

void Grid::SetSafeTextureForSprite(int row, int col)
{
	Sprite* currSprite = mGridElements[row][col];
	if (!currSprite)
		return;

	SDLEngine::Engine::Texture randomTexture = (SDLEngine::Engine::Texture)(SDLEngine::Engine::TEXTURE_GEMS_START + (std::rand() % SDLEngine::Engine::TEXTURE_GEMS_END));
	currSprite->SetTexture(randomTexture);

	//find the first and last row for consecutive textures of current type adjacent to provided row and columns
	int startingRow = GetExtremesForTextureType(randomTexture, row, col, step_negative, 0);
	int endingRow = GetExtremesForTextureType(randomTexture, row, col, step_positive, 0);

	//find the first and last columns for consecutive textures of current type adajcent to provided row and columns
	int startingCol = GetExtremesForTextureType(randomTexture, row, col, 0, step_negative);
	int endingCol = GetExtremesForTextureType(randomTexture, row, col, 0, step_positive);

	//If the texture makes it so that a new match will be immediately formed, choose a new texture;
	if (endingRow - startingRow >= number_of_consecutive_matches - 1
		|| endingCol - startingCol >= number_of_consecutive_matches - 1)
	{
		do
		{
			randomTexture = (SDLEngine::Engine::Texture)(SDLEngine::Engine::TEXTURE_GEMS_START + (std::rand() % SDLEngine::Engine::TEXTURE_GEMS_END));
		} while (randomTexture == currSprite->GetTexture());
	}
	currSprite->SetTexture(randomTexture);
}

void Grid::PopulateSprites(const SDLEngine::Engine& engine)
{
	if (!mGridElements)
		return;

	//All gem textures have slightly different width/height. 
	//Getting the average width/height helps avoid issues where x and y spacings don't appropriately sync up
	int numTextures = 0;
	for (int i = SDLEngine::Engine::TEXTURE_GEMS_START; i <= SDLEngine::Engine::TEXTURE_GEMS_END; ++i)
	{
		averageTexWidth += engine.GetTextureWidth((SDLEngine::Engine::Texture)i);
		averageTexHeight += engine.GetTextureHeight((SDLEngine::Engine::Texture)i);
		++numTextures;
	}

	averageTexWidth /= (float)numTextures;
	averageTexHeight /= (float)numTextures;

	for (int row = 0; row < sizeY; ++row)
	{
		for (int col = 0; col < sizeX; ++col)
		{
			//Sets a random texture making sure no matches occur at game start
			SetSafeTextureForSprite(row, col);
			CalculatePosition(row, col, true);
		}
	}
}

void Grid::CalculatePosition(int row, int col, bool isNewElement, GridAnimCallback onMoveFinishedCallback)
{
	Sprite* sprite = mGridElements[row][col];
	if (sprite)
	{
		float targetXPos = xPos + ((float)col * (averageTexWidth + gridSpacing));
		float targetYPos = yPos + ((float)row * (averageTexHeight + gridSpacing));

		float startXPos, startYPos;

		//new elements spawn from top of the screen
		if (isNewElement)
		{
			startXPos = targetXPos;
			startYPos = ((float)row * (averageTexHeight + gridSpacing)) - yPos;
		}
		//existing elements just move from their current position
		else
		{
			sprite->GetPosition(startXPos, startYPos);
		}

		//If the current sprite has any animations running, mark it for delete
		for (Animation* currAnimation : mAnimations)
		{
			const Drawable * const animSprite = currAnimation->GetDrawable();
			if (animSprite == sprite)
			{
				currAnimation->markedForDelete = true;
			}
		}

		//Create a move animation to move the sprite and add it to the animations list.
		Animation* moveAnimation = new GridElementAnimation(sprite, startXPos, startYPos, targetXPos, targetYPos, 0.5f, row, col);
		mAnimations.push_back(moveAnimation);

		if (onMoveFinishedCallback)
		{
			moveAnimation->SetCallback(this, static_cast<CallbackInterface<Drawable>::Callback>(onMoveFinishedCallback));
		}
	}
}

void Grid::Swap(int row, int col, bool waitForSwap)
{
	//make sure the selection is valid
	if (selectedX == index_none && selectedY == index_none)
		return;

	Sprite* swap = mGridElements[selectedY][selectedX];
	mGridElements[selectedY][selectedX] = mGridElements[row][col];
	mGridElements[row][col] = swap;

	//wait for swap animation to complete before player is allowed to input again?
	waitingForSwap = waitForSwap;

	//start swap animations
	CalculatePosition(selectedY, selectedX, false);
	CalculatePosition(row, col, false, &Grid::OnSwapComplete);
}

void Grid::OnSwapComplete(const DrawablePtr* anim)
{
	if (selectedX == index_none || selectedY == index_none)
		return;

	const GridElementAnimation* gridAnim = static_cast<const GridElementAnimation*>(anim);
	if (gridAnim && waitingForSwap)
	{
		int row, col;
		gridAnim->GetGridIndex(row, col);

		waitingForSwap = false;

		//check if there are any matches
		bool matchFound = CheckForMatch(row, col);
		if(!matchFound)
			matchFound = CheckForMatch(selectedY, selectedX);

		//If no match was found, swap the elements back
		if (!matchFound)
			Swap(row, col, false);

		//reset selection
		selectedX = index_none;
		selectedY = index_none;
	}
}

bool Grid::CheckForMatch(int row, int col)
{
	Sprite* currSprite = mGridElements[row][col];
	if (!currSprite)
		return false;

	SDLEngine::Engine::Texture targetTexture = currSprite->GetTexture();

	//find the first and last row for consecutive textures of current type adjacent to provided row and columns
	int startingRow = GetExtremesForTextureType(targetTexture, row, col, step_negative, 0);
	int endingRow = GetExtremesForTextureType(targetTexture, row, col, step_positive, 0);
	
	//find the first and last columns for consecutive textures of current type adajcent to provided row and columns
	int startingCol = GetExtremesForTextureType(targetTexture, row, col, 0, step_negative);
	int endingCol = GetExtremesForTextureType(targetTexture, row, col, 0, step_positive);

	//check if elements can be destroyed vertically in the current column
	if (endingRow - startingRow >= number_of_consecutive_matches - 1)
	{
		ClearRemovedElements(col, startingRow, endingRow, &Grid::OnDropDownAnimFinished);
		return true;
	}

	//check if elements can be destroyed horizontally in the current row
	if (endingCol - startingCol >= number_of_consecutive_matches - 1)
	{
		for (int i = startingCol; i <= endingCol; ++i)
		{
			//set the callback to reset elements only on the last element, so it is triggered just once
			GridAnimCallback callback = nullptr;
			if (i == endingCol)
				callback = &Grid::OnDropDownAnimFinished;

			ClearRemovedElements(i, row, row, callback);
		}
		return true;
	}

	return false;
}

void Grid::ClearRemovedElements(int col, int startingRow, int endingRow, GridAnimCallback clearFinishedCallback)
{
	int numDestroyed = endingRow - startingRow + 1;

	//temporarily store destroyed elements in a vector
	for (int i = startingRow; i <= endingRow; ++i)
	{
		if (mGridElements[i][col])
		{
			mElementsToRemove.push_back(GridElement(endingRow - i, col, mGridElements[i][col]));
			mGridElements[i][col] = nullptr;
		}
	}

	//Push all elements above starting from the element right above our last destroyed row
	//to the element on the topmost row to the free-est row below and then recalculate position
	for (int i = startingRow - 1; i >= 0; --i)
	{
		Sprite* currentSprite = mGridElements[i][col];
		if (currentSprite)
		{
			int targetRow = endingRow + (i + 1 - startingRow);
			mGridElements[targetRow][col] = currentSprite;
			mGridElements[i][col] = nullptr;

			CalculatePosition(targetRow, col, false, clearFinishedCallback);

			waitingForSwap = true;
		}
	}

	//if the element was destroyed in the topmost row, skip straight to creating new elements
	if (startingRow == 0)
	{
		OnDropDownAnimFinished(nullptr);
	}

	//increment score for the number of blocks that were destroyed
	if (mScore)
		mScore->IncrementScore(numDestroyed);
}

int Grid::GetExtremesForTextureType(const SDLEngine::Engine::Texture& texture, int rowStart, int columnStart, int rowStep, int columnStep)
{
	//invalid step count. early out instead.
	if (rowStep == 0 && columnStep == 0)
		return -1;

	int resultRow = rowStart, resultColumn = columnStart;
	while (mGridElements[rowStart][columnStart] && mGridElements[rowStart][columnStart]->GetTexture() == texture)
	{
		//texture type at current index is the same as texture type we're checking against. safe to update the out values
		resultRow = rowStart;
		resultColumn = columnStart;

		//handling exiting the loop in case we're on the edges of the grid
		if (rowStep < 0 && rowStart == 0)
			break;

		if (columnStep < 0 && columnStart == 0)
			break;

		if (rowStep > 0 && rowStart == sizeY - 1)
			break;

		if (columnStep > 0 && columnStart == sizeX - 1)
			break;

		rowStart += rowStep;
		columnStart += columnStep;
	}

	if (rowStep != 0)
		return resultRow;

	if (columnStep != 0)
		return resultColumn;

	return -1;
}

void Grid::OnDropDownAnimFinished(const DrawablePtr* anim)
{
	for (int i = mElementsToRemove.size() - 1; i >= 0; --i)
	{
		GridElement& elementToReset = mElementsToRemove[i]; 
		mGridElements[elementToReset.row][elementToReset.col] = elementToReset.sprite;

		//Reset the texture on the element
		SetSafeTextureForSprite(elementToReset.row, elementToReset.col);

		//if it is the last element in our list to reset, set a callback so it is triggered only once
		GridAnimCallback callback = nullptr;
		if (i == 0)
			callback = &Grid::OnResetEmptySpotsAnimFinished;
		CalculatePosition(elementToReset.row, elementToReset.col, true, callback);
	}

	if(mElementsToRemove.size() > 0)
		waitingForSwap = true;
}

void Grid::OnResetEmptySpotsAnimFinished(const DrawablePtr* anim)
{
	waitingForSwap = false;

	if (mElementsToRemove.size() == 0)
		return;

	mElementsToRemove.clear();

	//Sweep the grid to make sure no new matches have formed
	bool matchFound = false;
	for (int row = 0; row < sizeY; ++row)
	{
		for (int col = 0; col < sizeX; ++col)
		{
			matchFound = CheckForMatch(row, col);
			if (matchFound)
				break;
		}
		if (matchFound)
			break;
	}
}

int Grid::GetCurrentScore() const 
{ 
	return mScore->GetScore(); 
}

void Grid::Update(const SDLEngine::Engine& engine)
{
	//Remove any animations from the list that are marked for delete
	for (size_t i = 0; i < mAnimations.size(); ++i)
	{
		Animation* currAnimation = mAnimations[i];
		if (currAnimation && currAnimation->markedForDelete)
		{
			//swap the animation at the end of the queue to current animation spot, and pop from the end of the queue
			mAnimations[i] = mAnimations[mAnimations.size() - 1];
			mAnimations.pop_back();
			delete currAnimation;

			--i;
		}
	}

	//Update the remaining animations
	float deltaSeconds = engine.GetLastFrameSeconds();
	for (size_t i = 0; i < mAnimations.size(); ++i)
	{
		Animation* currAnimation = mAnimations[i];
		if (currAnimation)
			currAnimation->Update(deltaSeconds);
	}

	//If a swap animation is currently playing, don't accept inputs
	if (waitingForSwap)
		return;

	bool isMouseDown = engine.GetMouseButtonDown();

	//If mouse is not pressed and no selection exists, no point continuing forward
	if (!isMouseDown)
	{
		if (selectedX == index_none && selectedY == index_none)
		{
			mouseDownLastFrame = isMouseDown;
			return;
		}
	}

	//get the array index for the current element based on mouse position
	float mouseX = engine.GetMouseX();
	float mouseY = engine.GetMouseY();

	int col = static_cast<int>(floor((mouseX - xPos) / (averageTexWidth + gridSpacing)));
	int row = static_cast<int>(floor((mouseY - yPos) / (averageTexHeight + gridSpacing)));

	//invalid item. early out.
	if (row < 0 || col < 0 || row >= sizeY || col >= sizeX)
		return;

	//if mouse was clicked on an item this frame, make that the selected item
	if (isMouseDown && isMouseDown != mouseDownLastFrame)
	{
		if (selectedX == index_none && selectedY == index_none)
		{
			selectedX = col;
			selectedY = row;
		}
	}

	int xDifference = std::abs(selectedX - col);
	int yDifference = std::abs(selectedY - row);

	if ((isMouseDown != mouseDownLastFrame) //checking current mouse state against previous mouse state allows us to handle both dragging and clicking scenarios
		&& (selectedX != index_none && selectedY != index_none) //ensure that we have a valid item selected
		&& (xDifference != 0 || yDifference != 0)) //ensure that the item we try to swap with is not the same as our current selection
	{
		//check to see if the swap position is adjacent to our selected block, but not diagonally so
		if ((xDifference <= 1 && yDifference <= 1 && xDifference != yDifference))
		{
			Swap(row, col);
		}
		//swap selection is not adjacent, invalidate the selection
		else
		{
			selectedX = index_none;
			selectedY = index_none;
		}
	}
	
	mouseDownLastFrame = isMouseDown;
}

void Grid::Render(const SDLEngine::Engine& engine) const
{
	if (!mGridElements)
		return;

	mScore->Render(engine);

	for (int row = 0; row < sizeY; ++row)
	{
		for (int col = 0; col < sizeX; ++col)
		{
			Sprite* CurrSprite = mGridElements[row][col];
			if(CurrSprite)
				CurrSprite->Render(engine);
		}
	}
}