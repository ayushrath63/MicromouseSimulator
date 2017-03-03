#include "Particle.h"

extern LTexture gRedTexture;
extern LTexture gGreenTexture;
extern LTexture gBlueTexture;
extern LTexture gShimmerTexture;

Particle::Particle(int x, int y)
{
	//randomize position
	mPosX = x - 5 + (rand() % 25);
	mPosY = y - 5 + (rand() % 25);

	//init animation
	mFrame = rand() % 5;

	//set particle type
	switch (rand() % 3)
	{
	case 0:
		mTexture = &gRedTexture;
		break;
	case 1:
		mTexture = &gGreenTexture;
		break;
	case 2:
		mTexture = &gBlueTexture;
		break;
	}
}

void Particle::render()
{
	mTexture->render(mPosX, mPosY);

	//Show Shimmer
	if(mFrame % 2 == 0)
	{
		gShimmerTexture.render(mPosX, mPosY);
	}

	//animate 
	mFrame++;
}

bool Particle::isDead()
{
	return mFrame > 10;
}
