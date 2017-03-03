#ifndef _CONSTANTS_H
#define _CONSTANTS_H

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 1280;

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TILESET_WIDTH = 480;
const int TILESET_HEIGHT = 80;
const int TOTAL_TILES = 256;
const int TOTAL_TILE_SPRITES = 16;
///Tile Types
const int NO_WALLS = 0;
const int N_WALL = 1;
const int NE_WALL = 2;
const int EW_WALL = 3;
const int SEW_WALL = 4;
const int BOX = 5;

//Framerate constants
const int SCREEN_FPS = 20;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

#endif // !_CONSTANTS_H
