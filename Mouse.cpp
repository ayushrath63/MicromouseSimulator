
#include "Mouse.h"
#include "Constants.h"
#include "Texture.h"

extern LTexture gMouseTexture;

Mouse::Mouse(Tile** tileSet)
{
	mTiles = tileSet;
	//Collision box
	mBox.x = 0 + (TILE_WIDTH - Mouse::MOUSE_WIDTH) / 2;
	mBox.y = LEVEL_HEIGHT - (Mouse::MOUSE_HEIGHT + TILE_HEIGHT)/2;
	mBox.h = Mouse::MOUSE_HEIGHT;
	mBox.w = Mouse::MOUSE_WIDTH;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	//Initiallize angle
	mRot = 0;

	mMoving = false;
}

Mouse::~Mouse(){}

void Mouse::move(MouseMovement doThis)
{

	switch (doThis)
	{
	case MoveForward:
		if (wallFront())
		{
			mCrashed = true;
			return;
		}
		switch (mRot)
		{
		case 0:
			mVelY -= MOUSE_VEL;
			break;
		case 90:
			mVelX += MOUSE_VEL;
			break;
		case 180:
			mVelY += MOUSE_VEL;
			break;
		case 270:
			mVelX -= MOUSE_VEL;
			break;
		}
		break;
	case MoveBackward:
		mRot = (mRot + 180) % 360;
		switch (mRot)
		{
		case 0:
			mVelY -= MOUSE_VEL;
			break;
		case 90:
			mVelX += MOUSE_VEL;
			break;
		case 180:
			mVelY += MOUSE_VEL;
			break;
		case 270:
			mVelX -= MOUSE_VEL;
			break;
		}
		break;
	case TurnClockwise:
		mRot = (mRot + 90) % 360;
		break;
	case TurnCounterClockwise:
		mRot = (mRot - 90);
		if(mRot < 0) mRot += 360;
		mRot %= 360;
		break;
	case TurnAround:
		mRot = (mRot + 180) % 360;
		break;
	case Finish:
	case Wait:
	default:
		mVelX = 0;
		mVelY = 0;
	}

	//Move the mouse left or right
	mBox.x += mVelX;

	//If the mouse went too far to the left or right or touched a wall
	if ((mBox.x < 0) || (mBox.x + MOUSE_WIDTH > LEVEL_WIDTH))
	{
		//move back
		mBox.x -= mVelX;
		mCrashed = true;
	}

	//Move the mouse up or down
	mBox.y += mVelY;

	//If the mouse went too far up or down or touched a wall
	if ((mBox.y < 0) || (mBox.y + MOUSE_HEIGHT > LEVEL_HEIGHT))
	{
		//move back
		mBox.y -= mVelY;
		mCrashed = true;
	}

	//reset velocities
	mVelX = 0;
	mVelY = 0;
}

bool Mouse::wallFront()
{
	unsigned char walls = getTileWallsNESW();
	switch (mRot)
	{
	case 0:
		return walls & 0b1000; //N
	case 90:
		return walls & 0b0100; //E
	case 180:
		return walls & 0b0010; //S
	case 270:
		return walls & 0b0001; //W
	}
	
	return false;
}

bool Mouse::wallLeft()
{
	unsigned char walls = getTileWallsNESW();
	switch (mRot)
	{
	case 0:
		return walls & 0b0001; //W
	case 90:				   //
		return walls & 0b1000; //N
	case 180:				   //
		return walls & 0b0100; //E
	case 270:				   //
		return walls & 0b0010; //S
	}

	return false;
}

bool Mouse::wallRight()
{
	unsigned char walls = getTileWallsNESW();
	switch (mRot)
	{
	case 0:
		return walls & 0b0100; //E
	case 90:				   //
		return walls & 0b0010; //S
	case 180:				   //
		return walls & 0b0001; //W
	case 270:				   //
		return walls & 0b1000; //N
	}

	return false;
}

bool Mouse::atCenter()
{
	//if (x,y) == (7,7), (7,8), (8,7) or (8,8)
	if ((getPosX() == 7 || getPosX() == 8) && (getPosY() == 7 || getPosY() == 8))
		return true;
	else return false;
}

bool Mouse::hasCrashed()
{
	return mCrashed;
}

bool Mouse::isMoving()
{
	return mMoving;
}

void Mouse::setMoving(bool moving)
{
	mMoving = moving;
}

void Mouse::setCamera(SDL_Rect& camera)
{
	//Center the camera over the mouse
	camera.x = (mBox.x + MOUSE_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (mBox.y + MOUSE_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}


void Mouse::render(SDL_Rect& camera)
{
	//Show the mouse
	//Switch statment fixes off by one errors with tile rotation
	switch (mRot)
	{
	case 0:
		gMouseTexture.render(mBox.x - camera.x, mBox.y - camera.y, NULL, mRot);
		break;
	case 90:
		gMouseTexture.render(mBox.x - camera.x - 1, mBox.y - camera.y, NULL, mRot);
		break;
	case 180:
		gMouseTexture.render(mBox.x - camera.x - 1, mBox.y - camera.y - 1, NULL, mRot);
		break;
	case 270:
		gMouseTexture.render(mBox.x - camera.x, mBox.y - camera.y - 1, NULL, mRot);
		break;
	default:
		gMouseTexture.render(mBox.x - camera.x, mBox.y - camera.y, NULL, mRot);
		break;
	}
}

int Mouse::getPosX()
{
	return mBox.x / 80;
}

int Mouse::getPosY()
{
	return (mBox.y) / 80;
}

void Mouse::fourLSBCircShiftL(unsigned char &vec, int nTimes)
{
	//Circular shift on 4 LSB
	for(int i = 0; i < nTimes; i++)
		vec = (vec >> 1 | vec << 3);
	//mask 4 MSB
	vec &= 0b00001111;
}

unsigned char Mouse::getTileWallsNESW()
{
	Tile *tmp = mTiles[getPosX() + getPosY() * (LEVEL_HEIGHT / TILE_HEIGHT)];
	//walls stored as 4 LSB in NESW order
	unsigned char walls;
	switch (tmp->getType())
	{
	case NO_WALLS:
		walls = 0;
		break;
	case N_WALL:
		walls = 0b1000;
		break;
	case NE_WALL:
		walls = 0b1100;
		break;
	case EW_WALL:
		walls = 0b0101;
		break;
	case SEW_WALL:
		walls = 0b0111;
		break;
	case BOX:
		walls = 0b1111;
		break;
	}
	int tileAngle = tmp->getRot();
	//clockwise rotation by 90 -> circular shift right on 4 LSB
	fourLSBCircShiftL(walls, tileAngle / 90);
	return walls;
}
