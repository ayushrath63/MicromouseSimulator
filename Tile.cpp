#include "Include/Tile.h"
#include "Include/Texture.h"
#include "Include/Constants.h"

extern LTexture gTileTexture;
extern SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

Tile::Tile(int x, int y, int tileType, int rotation)
{
	//Get the offsets
	mBox.x = x;
	mBox.y = y;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;

	mRot = rotation;
}

void Tile::render(SDL_Rect& camera)
{
	//Show the tile
	//Switch statment fixes off by one errors with tile rotation
	switch (mRot)
	{
	default:
		gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType], mRot);
		break;
	case 90:
		gTileTexture.render(mBox.x - camera.x - 1, mBox.y - camera.y, &gTileClips[mType], mRot);
		break;
	case 180:
		gTileTexture.render(mBox.x - camera.x - 1, mBox.y - camera.y - 1, &gTileClips[mType], mRot);
		break;
	case 270:
		gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y - 1, &gTileClips[mType], mRot);
	}
}

int Tile::getType()
{
	return mType;
}

int Tile::getRot()
{
	return mRot;
}

SDL_Rect Tile::getBox()
{
	return mBox;
}