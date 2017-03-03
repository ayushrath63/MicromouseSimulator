#include "Window.h"
#include "Constants.h"
#include <sstream>

extern SDL_Renderer* gRenderer;

LWindow::LWindow()
{
	//Init non-existant window
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
}

bool LWindow::init()
{
	//Create window
	mWindow = SDL_CreateWindow("Micromouse Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN /*| SDL_WINDOW_RESIZABLE*/ );
	if (mWindow != NULL)
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;
		
		
		//Start in fullscreen
		//SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
		//mFullScreen = true;
		//mMinimized = false;
		
	}

	return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer()
{
	return SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void LWindow::handleEvent(SDL_Event& e)
{
	//Window Even Occurs
	if (e.type = SDL_WINDOWEVENT)
	{
		//caption update flag
		bool updateCaption = false;
		switch (e.window.event)
		{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SDL_RenderPresent(gRenderer);
			break;
			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(gRenderer);
			break;
			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			break;
			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			break;
			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			break;
			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			break;
			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;
			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;
			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
			break;
		}
		//Update window caption with new data
		if (updateCaption)
		{
			SDL_SetWindowTitle(mWindow, "Micromouse Simulator");
		}
	}

	/*

	Disabled due to bugginess - window border disappears when
	fullscreen switched off, KEYDOWN events not working either
	
	//Enter exit full screen on return key
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_RETURN]) {
		if (mFullScreen)
		{
			SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
			SDL_SetWindowPosition(mWindow, 100, 100);
			SDL_SetWindowBordered(mWindow, SDL_TRUE);
			mFullScreen = false;
			SDL_Delay(500);
		}
		else
		{
			SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
			mFullScreen = true;
			mMinimized = false;
			SDL_Delay(500);
		}
	}
	*/
}

void LWindow::free()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(mWindow);
	gRenderer = NULL;
	mWindow = NULL;
}

int LWindow::getWidth()
{
	return mWidth;
}

int LWindow::getHeight()
{
	return mHeight;
}

bool LWindow::hasMouseFocus()
{
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool LWindow::isMinimized()
{
	return mMinimized;
}