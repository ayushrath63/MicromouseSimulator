#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <cmath>

//The window renderer
extern SDL_Renderer* gRenderer;

#ifdef _SDL_TTF_H
//Global Font
extern TTF_Font *gFont;
#endif


//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Color bgColor);
#endif

	//Deallocates texture
	void free();

	//set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set Blending
	void setBlendMode(SDL_BlendMode blending);

	//set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};


#endif