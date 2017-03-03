#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "Texture.h"

const int TOTAL_PARTICLES = 20;

class Particle
{
public:
	Particle(int x, int y);

	//show particle
	void render();

	//check if particle is dead
	bool isDead();

private:
	//position
	int mPosX, mPosY;

	//animation frame
	int mFrame;

	//Particle texture
	LTexture* mTexture;
};

#endif // !_PARTICLE_H