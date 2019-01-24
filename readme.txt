Developed using Visual Studio 2017 and uses the SDL library.

Assets downloaded from - https://devilsworkshop.itch.io/match-3-free-2d-sprites-game-art-and-ui

Game Flow-

MatchThreeGame is the main game class, that inherits from SDLWrapper::Updater, and is created in Main.

It holds the current game state.

The game is divided into 3 game states, each with their own update loop and a transition logic that takes it to the next state.

The game launches by default in the main menu game state (GameStateMainMenu).

When the player clicks anywhere on the screen, this takes the player to the next game state, which has the main game. (GameStateInGame)

Once the player enters this game state, it starts a game timer, that runs for 60 seconds, before moving the player to the final game state (GameStatePostGame). In this game state, we display the final score of the player. Clicking anywhere on this screen will take the player back to the GameStateInGame and restart the game.


Drawable-

Drawable is the base class for any objects that can be drawn on screen. 

It is an abstract class with two functions
	Drawable::Update() 
	Drawable::Render()

The game classes Sprite, Grid, Score and Timer are derived from it, and in turn perform their functionalities.

CallbackInterface-

The callback interface is a template class that can be inherited by any of the classes, to allow us to conveniently set up callback functions through the use of function pointers.

It is used by the Timer and Animation classes to raise callbacks for when the timer runs out and when the animation is finished respectively.

Animation -

The Animation class holds a Drawable that it can move to a position over a given time. The movement happens using standard lerp, and a callback is raised once the target reaches the final position using the CallbackInterface.


Grid -

The Grid class is where most of the major gameplay logic is located.

It populates the grid on spawn with a 2D array of Sprites. It ensures that when the grid is initialized, there is no match present by default. It then animates all the elements into view, once ready.

In the Update function, it first destroys any animations on the grid that are pending kill, updating the rest. 
It then checks to see if the player selects grid elements to swap, and then calls the swap logic on it.

Swap function checks to see if there are any matches, and if none are found, swaps the elements back to their original positions.

If a match is found, any elements that need to be destroyed are stored in a temporary array (to be re-used when we need to reset the grid with new elements). All elements in the rows above the destroyed element are moved below with an animation. Once the elements have reached their target spots, we use the ones stored in our temporary array, and re-assign it a new index, position and texture, so that it is now on top of the grid.

A sweep is then done on the grid to ensure that no new matches have been formed due to objects shifting down. If so, repeat the above logic.

The score is then updated.