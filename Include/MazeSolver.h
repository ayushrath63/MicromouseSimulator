#ifndef _MAZE_SOLVER_H
#define _MAZE_SOLVER_H

#include "Mouse.h"
#include <stack>
#include <queue>
#define ABS(a) (a >> (sizeof(a)*) ^ a)

const unsigned char MAZE_WIDTH = 16;
const unsigned char MAZE_HEIGHT = 16;

enum Dir
{
	N,
	S,
	E,
	W
};

class MazeSolver
{
public:
	MazeSolver(Mouse* mouse);
	void nextMovement();
	
protected:
	//tile info accessors
	bool wallNorth(unsigned char x, unsigned char y);
	bool wallEast(unsigned char x, unsigned char y);
	bool wallSouth(unsigned char x, unsigned char y);
	bool wallWest(unsigned char x, unsigned char y);
	bool hasVisited(unsigned char x, unsigned char y);
	bool isDead(unsigned char x, unsigned char y);
	unsigned char getMDist(unsigned char x, unsigned char y);

	//cell info mutators
	void setWallNorth(unsigned char x, unsigned char y, bool setVal);
	void setWallEast(unsigned char x, unsigned char y, bool setVal);
	void setWallSouth(unsigned char x, unsigned char y, bool setVal);
	void setWallWest(unsigned char x, unsigned char y, bool setVal);
	void setVisited(unsigned char x, unsigned char y, bool setVal);
	void setDead(unsigned char x, unsigned char y, bool setVal);
	void setMDist(unsigned char x, unsigned char y, unsigned short setVal);

	void floodFill();
private:
	//8 MSB store manhattan distance, 8 LSB are flags
	//[0:3] -> [W,S,E,N]
	//[4] -> has visited
	//[5] -> is alive
	//[6:7] -> unused, possibly cost in future?
	//[8:15] -> Manhattan Distance
	unsigned short cellInfo[MAZE_WIDTH][MAZE_HEIGHT];

	struct Cell
	{
		Cell() : X(0), Y(0) {};
		Cell(unsigned char X, unsigned char Y) : X(X), Y(Y) {};
		unsigned char X, Y;
	};
	Dir mDir;
	Cell mPos;
	Mouse* mMouse;
	bool mSolved = false;
};

#endif