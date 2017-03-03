#ifndef _TILE_H
#define _TILE_H

#include <SDL.h>

class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType, int rotation = 0);

	//Shows the tile
	void render(SDL_Rect& camera);

	//Get the tile type
	int getType();

	//Tile rotation accessor
	int getRot();

	//Get the collision box
	SDL_Rect getBox();

private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;

	//Tile Rotation (Clockwise)
	int mRot;
};

#endif // !_TILE_H