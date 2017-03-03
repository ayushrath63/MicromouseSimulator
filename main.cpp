#include "Window.h"
#include "Constants.h"
#include "Texture.h"
#include "Timer.h"
#include "Mouse.h"
#include "MazeSolver.h"

#include <sstream>
#include <fstream>
#include <algorithm>



/*
GLOBALS
*/

//The window renderer
SDL_Renderer* gRenderer = NULL;

#ifdef _SDL_TTF_H
//Global Font
TTF_Font *gFont = NULL;
#endif

//Window Object
LWindow gWindow;

//Starts up SDL and creates window
bool init();

//Scene textures
LTexture gMouseTexture;
LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

//Path to maze file
std::string maze;

/**/

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		if (!gWindow.init())
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = gWindow.createRenderer();
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
#ifdef _SDL_TTF_H
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
#endif
#ifdef _AUDIO_HANDLER_H
				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
#endif
			}
		}
	}

	return success;
}

int randInt(int min, int max);

bool setTiles(Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::string mazeFileList[10] =
	{
		"Resources/MAZE_CAMM_2012.map",
		"Resources/MAZE_CAMM_2011.map",
		"Resources/MAZE_APEC_2013.map",
		"Resources/MAZE_APEC_2012.map",
		"Resources/MAZE_ALL_JAPAN_2013.map",
		"Resources/MAZE_ALL_JAPAN_2012.map",
		"Resources/MAZE_ALL_JAPAN_2011.map",
		"Resources/MAZE_ALL_JAPAN_2010.map",
		"Resources/MAZE_ALL_JAPAN_2009.map",
		"Resources/MAZE_ALL_JAPAN_2008.map",
	};

	maze = mazeFileList[randInt(0,9)];
	std::ifstream map(maze);
	//remove file name artifacts from string 
	std::replace(maze.begin(), maze.end(), '_', ' ');
	std::replace(maze.begin(), maze.end(), '.', '\0');
	maze = maze.substr(15);

	//If the map couldn't be loaded
	if (!map.is_open())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			if ((tileType >= 0) && (tileType < (TOTAL_TILE_SPRITES << 3) + 8))
			{	
				int tileNum = tileType >> 3;
				int rotation;
				switch (tileType & 0b111)
				{
				case 0b001:
					rotation = 90;
					break;
				case 0b010:
					rotation = 180;
					break;
				case 0b100:
					rotation = 270;
					break;
				default:
					rotation = 0;
				}
				tiles[i] = new Tile(x, y, tileNum, rotation);
			}
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}
			
			x += TILE_WIDTH;
			if (x >= LEVEL_WIDTH)
			{
				x = 0;
				y += TILE_HEIGHT;
			}
		}
		//Clip the sprite sheet
		if (tilesLoaded)
		{
			for (int i = 0; i < TILESET_WIDTH / TILE_WIDTH; i++)
			{
				for (int j = 0; j < TILESET_HEIGHT / TILE_HEIGHT; j++)
				{
					gTileClips[i + j*(TILESET_WIDTH / TILE_WIDTH)].x = i*TILE_WIDTH;
					gTileClips[i + j*(TILESET_WIDTH / TILE_WIDTH)].y = j*TILE_WIDTH;
					gTileClips[i + j*(TILESET_WIDTH / TILE_WIDTH)].w = TILE_WIDTH;
					gTileClips[i + j*(TILESET_WIDTH / TILE_WIDTH)].h = TILE_HEIGHT;
				}
			}
		}
	}

	map.close();

	return tilesLoaded;
}

bool loadMedia(Tile* tiles[])
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("Resources/Monda-Regular.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Set text color as black
		SDL_Color textColor = { 0, 0, 0, 255 };
	}

	//Load Mouse texture
	if (!gMouseTexture.loadFromFile("Resources/mouse.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}
	gMouseTexture.setColor(44, 255, 255);

	//Load tile texture
	if (!gTileTexture.loadFromFile("Resources/mazeTiles.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	return success;
}

void close(Tile* tiles[])
{

	//Free loaded images
	//Deallocate tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tiles[i] == NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}

	//Free loaded images
	gMouseTexture.free();
	gTileTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	gWindow.free();

	//Quit SDL subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//The level tiles
		Tile* tileSet[TOTAL_TILES];

		//Load media
		if (!loadMedia(tileSet))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Text color
			SDL_Color textColor = { 0x15, 0x65,0xC0, 0xFF };
			
			//framerate stuff
			LTimer fpsTimer;
			LTimer capTimer;
			fpsTimer.start();

			//Maze name texture
			LTexture mazeName;
			mazeName.loadFromRenderedText(maze, textColor);

			//Goal/Fail Texture
			LTexture failMessage;
			LTexture goalMessage;
			failMessage.loadFromRenderedText("FAILURE", textColor);
			goalMessage.loadFromRenderedText("GOAL", textColor);

			//Mouse object
			Mouse mouse(tileSet);

			//Maze Solver Object
			MazeSolver wallFollower(&mouse);

			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			int countedFrames = 0;

			//While application is running
			while (!quit)
			{
				//start framerate cap timer
				capTimer.start();

				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Keyboard input
					const Uint8 *keyBoardState = SDL_GetKeyboardState(NULL);
					//quit on ctrl + q
					if ((keyBoardState[SDL_SCANCODE_RCTRL] || keyBoardState[SDL_SCANCODE_LCTRL]) && keyBoardState[SDL_SCANCODE_Q])
					{
						quit = true;
					}
					//* Debugging commands
					if (keyBoardState[SDL_SCANCODE_UP])
					{
						mouse.move(MoveForward);
					}
					if (keyBoardState[SDL_SCANCODE_RIGHT])
					{
						mouse.move(TurnClockwise);
					}
					if (keyBoardState[SDL_SCANCODE_LEFT])
					{
						mouse.move(TurnCounterClockwise);
					}
					if (mouse.atCenter())
					//*/

					//window update
					gWindow.handleEvent(e);

				}

				if (!gWindow.isMinimized())
				{
					wallFollower.nextMovement();
					mouse.setCamera(camera);

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					//Render level
					for (int i = 0; i < TOTAL_TILES; ++i)
					{
						tileSet[i]->render(camera);
					}

					mouse.render(camera);

					if (mouse.atCenter())
					{
						goalMessage.render((SCREEN_WIDTH - goalMessage.getWidth()) / 2, (SCREEN_HEIGHT - goalMessage.getHeight()) / 2);
					}
					if (mouse.hasCrashed())
					{
						failMessage.render((SCREEN_WIDTH - failMessage.getWidth()) / 2, (SCREEN_HEIGHT - failMessage.getHeight()) / 2);
					}

					mazeName.render(20, 20);

					//Update screen
					SDL_RenderPresent(gRenderer);
				}

				//if frame finished early
				int frameTicks = capTimer.getTicks();
				if (frameTicks < SCREEN_TICKS_PER_FRAME && !gWindow.isMinimized())
				{
					//wait remaining time
					SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
				}
				countedFrames++;
			}
		}

		//Free resources and close SDL
		close(tileSet);
	}


	return 0;
}