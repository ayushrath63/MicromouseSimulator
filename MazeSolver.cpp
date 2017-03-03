#include "MazeSolver.h"
//////DEBUG
#include <cstdio>

MazeSolver::MazeSolver(Mouse* mouse) : mMouse(mouse)
{
	mDir = N;

	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		setWallSouth(i, 0, true);
		setWallNorth(i, MAZE_HEIGHT - 1, true);
	}
	for (int i = 0; i < MAZE_HEIGHT; i++)
	{
		setWallWest(0, i, true);
		setWallEast(MAZE_WIDTH - 1, i, true);
	}
	floodFill();
};

void MazeSolver::nextMovement()
{
	std::stack<Cell> cellStack;
	cellStack.push(Cell(mPos.X, mPos.Y));

	//update walls
	if (mMouse->wallFront())
	{
		switch (mDir)
		{
		case N:
			setWallNorth(mPos.X, mPos.Y, true);
			break;
		case S:
			setWallSouth(mPos.X, mPos.Y, true);
			break;
		case E:
			setWallEast(mPos.X, mPos.Y, true);
			break;
		case W:
			setWallWest(mPos.X, mPos.Y, true);
			break;
		}
	}
	if (mMouse->wallLeft())
	{
		switch (mDir)
		{
		case N:
			setWallWest(mPos.X, mPos.Y, true);
			break;
		case S:
			setWallEast(mPos.X, mPos.Y, true);
			break;
		case E:
			setWallNorth(mPos.X, mPos.Y, true);
			break;
		case W:
			setWallSouth(mPos.X, mPos.Y, true);
			break;
		}
	}
	if (mMouse->wallRight())
	{
		switch (mDir)
		{
		case N:
			setWallEast(mPos.X, mPos.Y, true);
			break;
		case S:
			setWallWest(mPos.X, mPos.Y, true);
			break;
		case E:
			setWallSouth(mPos.X, mPos.Y, true);
			break;
		case W:
			setWallNorth(mPos.X, mPos.Y, true);
			break;
		}
	}

	unsigned char minDist = 255;
	unsigned char currDist = getMDist(mPos.X, mPos.Y);

	while (!cellStack.empty() && !mSolved)
	{
		Cell curr = cellStack.top();
		cellStack.pop();
		if ((getMDist(curr.X, curr.Y) == 0))
		{
			mSolved = true;
			return;
		}
		//for each neighboring cell w/o wall check distances
		{
			if (!wallNorth(curr.X, curr.Y) && getMDist(curr.X, curr.Y + 1) < minDist) minDist = getMDist(curr.X, curr.Y + 1);
			if (!wallSouth(curr.X, curr.Y) && getMDist(curr.X, curr.Y - 1) < minDist) minDist = getMDist(curr.X, curr.Y - 1);
			if (!wallEast(curr.X, curr.Y) && getMDist(curr.X + 1, curr.Y) < minDist) minDist = getMDist(curr.X + 1, curr.Y);
			if (!wallWest(curr.X, curr.Y) && getMDist(curr.X - 1, curr.Y) < minDist) minDist = getMDist(curr.X - 1, curr.Y);
		}
		if (currDist > minDist)
		{
			if (!wallNorth(mPos.X, mPos.Y) && getMDist(mPos.X, mPos.Y + 1) == minDist)
			{
				Dir lastDir = mDir;
				mDir = N;
				switch (lastDir)
				{
				case N:
					mPos.Y++;
					mMouse->move(MoveForward);
					return;
				case S:
					mMouse->move(TurnAround);
					return;
				case E:
					mMouse->move(TurnCounterClockwise);
					return;
				case W:
					mMouse->move(TurnClockwise);
					return;
				}
			}
			if (!wallSouth(mPos.X, mPos.Y) && getMDist(mPos.X, mPos.Y - 1) == minDist)
			{
				Dir lastDir = mDir;
				mDir = S;
				switch (lastDir)
				{
				case N:
					mMouse->move(TurnAround);
					return;
				case S:
					mPos.Y--;
					mMouse->move(MoveForward);
					return;
				case E:
					mMouse->move(TurnClockwise);
					return;
				case W:
					mMouse->move(TurnCounterClockwise);
					return;
				}
			}
			if (!wallEast(mPos.X, mPos.Y) && getMDist(mPos.X + 1, mPos.Y) == minDist)
			{
				Dir lastDir = mDir;
				mDir = E;
				switch (lastDir)
				{
				case N:
					mMouse->move(TurnClockwise);
					return;
				case S:
					mMouse->move(TurnCounterClockwise);
					return;
				case E:
					mPos.X++;
					mMouse->move(MoveForward);
					return;
				case W:
					mMouse->move(TurnAround);
					return;
				}
			}
			if (!wallWest(mPos.X, mPos.Y) && getMDist(mPos.X - 1, mPos.Y) == minDist)
			{
				Dir lastDir = mDir;
				mDir = W;
				switch (lastDir)
				{
				case N:
					mMouse->move(TurnCounterClockwise);
					return;
				case S:
					mMouse->move(TurnClockwise);
					return;
				case E:
					mMouse->move(TurnAround);
					return;
				case W:
					mPos.X--;
					mMouse->move(MoveForward);
					return;
				}
			}
		}
		else if (currDist <= minDist)
		{
			floodFill();
		}
	}
}

bool MazeSolver::wallNorth(unsigned char x, unsigned char y)
{
	return cellInfo[x][y] & 0b1000;
}

bool MazeSolver::wallEast(unsigned char x, unsigned char y)
{
	return cellInfo[x][y] &  0b0100;
}

bool MazeSolver::wallSouth(unsigned char x, unsigned char y)
{
	return cellInfo[x][y] & 0b0010;
}

bool MazeSolver::wallWest(unsigned char x, unsigned char y)
{
	return cellInfo[x][y] & 0b0001;
}

bool MazeSolver::hasVisited(unsigned char x, unsigned char y)
{
	return cellInfo[x][y] & 0b10000;
}

bool MazeSolver::isDead(unsigned char x, unsigned char y)
{
	return cellInfo[x][y] & 0b100000;
}

unsigned char MazeSolver::getMDist(unsigned char x, unsigned char y)
{
	if((x >=0 && x < MAZE_WIDTH) && (y>=0 && y < MAZE_HEIGHT))
		return (cellInfo[x][y] >> 8) & 0b11111111;
	else return 255;
}

void MazeSolver::setWallNorth(unsigned char x, unsigned char y, bool setVal)
{
	if (setVal) cellInfo[x][y] |= 0b1000;
	else cellInfo[x][y] &= 0b1111111111110111;
	if (y < MAZE_HEIGHT - 1)
	{
		if (setVal) cellInfo[x][y+1] |= 0b10;
		else cellInfo[x][y+1] &= 0b1111111111111101;
	}
}

void MazeSolver::setWallEast(unsigned char x, unsigned char y, bool setVal)
{
	if (setVal) cellInfo[x][y] |= 0b100;
	else cellInfo[x][y] &= 0b1111111111111011;
	if (x < MAZE_WIDTH - 1)
	{
		if (setVal) cellInfo[x + 1][y] |= 0b1;
		else cellInfo[x + 1][y] &= 0b1111111111111110;
	}
}

void MazeSolver::setWallSouth(unsigned char x, unsigned char y, bool setVal)
{
	if (setVal) cellInfo[x][y] |= 0b10;
	else cellInfo[x][y] &= 0b1111111111111101;
	if (x > 0)
	{
		if (setVal) cellInfo[x][y - 1] |= 0b1000;
		else cellInfo[x][y - 1] &= 0b1111111111110111;
	}
}

void MazeSolver::setWallWest(unsigned char x, unsigned char y, bool setVal)
{
	if (setVal) cellInfo[x][y] |= 0b1;
	else cellInfo[x][y] &= 0b1111111111111110;
	if (x > 0)
	{
		if (setVal) cellInfo[x - 1][y] |= 0b100;
		else cellInfo[x - 1][y] &= 0b1111111111111011;
	}
}

void MazeSolver::setVisited(unsigned char x, unsigned char y, bool setVal)
{
	if (setVal) cellInfo[x][y] |= 0b10000;
	else cellInfo[x][y] &= 0b1111111111101111;
}

void MazeSolver::setDead(unsigned char x, unsigned char y, bool setVal)
{
	if(setVal) cellInfo[x][y] |= 0b100000;
	else cellInfo[x][y] &= 0b1111111111011111;
}

void MazeSolver::setMDist(unsigned char x, unsigned char y, unsigned short setVal)
{
	//clear 8 MSB and set new dist
	cellInfo[x][y] = (cellInfo[x][y] & 0b11111111) | (setVal << 8);
}

void MazeSolver::floodFill()
{
	for (unsigned char i = 0; i < MAZE_HEIGHT; i++)
	{
		for (unsigned char j = 0; j < MAZE_HEIGHT; j++)
		{
			setMDist(i,j,255);
		}
	}

	//set 8,8 to 0 Distance
	setMDist(MAZE_WIDTH / 2,MAZE_HEIGHT / 2,0);

	std::queue<Cell> cellQ;
	cellQ.push(Cell(MAZE_WIDTH / 2, MAZE_HEIGHT / 2));
	while (cellQ.size() != 0)
	{
		Cell curr = cellQ.front();
		cellQ.pop();
		if ((!wallNorth(curr.X, curr.Y)) && getMDist(curr.X, curr.Y + 1) == 255)
		{
			cellQ.push(Cell(curr.X, curr.Y + 1));
			setMDist(curr.X, curr.Y + 1, getMDist(curr.X, curr.Y) + 1);
		}
		if ((!wallSouth(curr.X, curr.Y)) && getMDist(curr.X, curr.Y - 1) == 255)
		{
			cellQ.push(Cell(curr.X, curr.Y - 1));
			setMDist(curr.X, curr.Y - 1, getMDist(curr.X, curr.Y) + 1);
		}
		if ((!wallEast(curr.X, curr.Y)) && getMDist(curr.X + 1, curr.Y) == 255)
		{
			cellQ.push(Cell(curr.X + 1, curr.Y));
			setMDist(curr.X + 1, curr.Y, getMDist(curr.X, curr.Y) + 1);
		}
		if ((!wallWest(curr.X, curr.Y)) && getMDist(curr.X - 1, curr.Y) == 255)
		{
			cellQ.push(Cell(curr.X - 1, curr.Y));
			setMDist(curr.X-1, curr.Y, getMDist(curr.X, curr.Y) + 1);
		}
	}
	/*
	for (int i = MAZE_HEIGHT - 1; i >= 0; i--)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			if (getMDist(j, i) < 10)
				printf("00%d ", getMDist(j, i));
			else if (getMDist(j, i) < 100)
				printf("0%d ", getMDist(j, i));
			else
				printf("%d ", getMDist(j, i));
		}
		printf("\n");
	}
	*/
}
