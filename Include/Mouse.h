#ifndef _MOUSE_H
#define _MOUSE_H

#include <SDL.h>
#include "Tile.h"

enum MouseMovement
{
	MoveForward,            // Move in the direction mouse is facing
	MoveBackward,           // Move opposite of the direction mouse is facing
	TurnClockwise,          // Self explanatory
	TurnCounterClockwise,   // Self explanatory
	TurnAround,             // Face the opposite direction currently facing
	Wait,                   // Do nothing this time, do some computation, then try again later
	Finish                  // Mouse has achieved goals and is ending the simulation
};

class Mouse
{
public:
	//The dimensions of the mouse
	static const int MOUSE_WIDTH = 60;
	static const int MOUSE_HEIGHT = 60;

	//Maximum axis velocity of the mouse
	static const int MOUSE_VEL = 80;

	//Initializes the variables
	Mouse(Tile** tileSet);
	~Mouse();

	//Moves the mouse
	void move(MouseMovement doThis);

	//locate nearby walls;
	bool wallFront();
	bool wallLeft();
	bool wallRight();

	//check if at center/crashed
	bool atCenter();
	bool hasCrashed();

	bool isMoving();
	void setMoving(bool moving);

	void setCamera(SDL_Rect & camera);

	//Shows the mouse on the screen relative to the camera
	void render(SDL_Rect& camera);

private:
	int getPosX();
	int getPosY();
	void fourLSBCircShiftL(unsigned char &vec, int nTimes);
	unsigned char getTileWallsNESW();
	Tile** mTiles;

	bool mCrashed;
	bool mMoving;

	//collision box
	SDL_Rect mBox;

	//The velocity of the mouse
	int mVelX, mVelY;

	//Angle of mouse
	int mRot;
};
#endif // !_MOUSE_H