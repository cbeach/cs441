#include <iostream>
#include <assert.h>
#include "halfPly.h"

using namespace std;

halfPly::halfPly(){}

//constructor for an n height board
halfPly::halfPly(int n)
{

	boardHeight = n;
	board = new int*[3];
	
	for(int i = 0; i < 3; i++)
	{
		board[i] = new int[boardHeight];
		for(int j = 0; j < boardHeight; j++)
			board[i][j] = 0;
	}
	for(int i = 0; i < 3; i++)
	{
		columnHeight[i] = 0;
	}	
	lastMove[0] = -1;
	lastMove[1] = -1;
	

}

halfPly::halfPly(halfPly &old) 
{
	boardHeight = old.boardHeight;

	board = new int*[3];
	
	for(int i = 0; i < 3; i++)
	{
		board[i] = new int[boardHeight];
		for(int j = 0; j < boardHeight; j++)
			board[i][j] = 0;
	}

	
	for(int i = 0; i < 3; i++)
	{
		columnHeight[i] = old.columnHeight[i];
		
	}

	lastPlayer = old.lastPlayer;

	lastMove[0] = old.lastMove[0];
	lastMove[1] = old.lastMove[1];
	
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < boardHeight; j++)
		{
			board[i][j] = old.board[i][j];
		}
	
	}
}

halfPly::~halfPly()
{
	for(int i = 0; i < 3; i++)
	{	
		if(board[i])
			delete[] board[i];
	}
	if(board)
		delete[] board ;
	
}

int halfPly::xMove()
{
	children = new halfPly*[3];
	assert(children);	
	int value = 0;

	if(evalBoard() != 0)
		return lastPlayer;

	for(int i = 0; i < 3; i++)
	{
		children[i] = NULL;
		if(legalMove(i))
		{
			children[i] = new halfPly(*this);
			children[i]->move(i, PlayerX);
			children[i]->lastPlayer = PlayerX;
			children[i]->parrent = this;
			children[i]->best = 0;
			value = children[i]->oMove();	
			if(children[i])
				delete children[i];
			if(value > best)
				best = value;
		}
	}
	return best;
}

int halfPly::oMove()
{
	children = new halfPly*[3];
	
	int value = 0;

	if(evalBoard() != 0)
		return lastPlayer;

	for(int i = 0; i < 3; i++)
	{
		children[i] = NULL;
		if(legalMove(i))
		{
			children[i] = new halfPly(*this);
			children[i]->move(i, PlayerO);
			children[i]->lastPlayer = PlayerO;
			children[i]->parrent = this;
			children[i]->best = 0;
			value = children[i]->xMove();	
			if(value < best)
				best = value;
		}
	}

	return best;
}

//returns 1 on X win
//returns 0 on draw
//returns -1 on Y win
int halfPly::evalBoard()
{
	//variables to make things look prettier
	if(lastMove[0] == -1)
		return false;
	int x = lastMove[0];
	int y = lastMove[1];

	if(hStrip(1, y) 
	| vStrip(x, y)
	| dDStrip(x, y) 
	| dUStrip(x, y))
		return true;

	return false;
}

bool halfPly::hStrip(int x, int y)
{
	if(x == 0)
		x++;
	else if(x == 2)
		x--;

	if(board[x-1][y] == lastPlayer &&
	   board[x][y] == lastPlayer &&
	   board[x+1][y] == lastPlayer)
		return true;
	else
		return false;
}
bool halfPly::vStrip(int x, int y)
{
	bool win = false;
	
	if(y < 2)
		return false;
	
	if(board[x][y] == lastPlayer &&
	   board[x][y-1] == lastPlayer &&
	   board[x][y-2] == lastPlayer)
	{
		win = true;
	}
	
	return win;


}
bool halfPly::dDStrip(int x, int y)
{
	if(y == 0 && x == 0)
		return false;
	if(x == 0)
	{
		x = 1;
		y--;
	}
	else if(x == 2)
	{
		x = 1;
		y++;
	}

	if(board[x-1][y+1] == lastPlayer &&
	   board[x][y] == lastPlayer &&
	   board[x+1][y-1] == lastPlayer)
		return true;
	else
		return false;

}
bool halfPly::dUStrip(int x, int y)
{
	if(x == 0)
	{
		x++;
		y++;
	}
	else if(x == 2)
	{
		x--;
		y--;
	}
	if(board[x-1][y-1] == lastPlayer &&
	   board[x][y] == lastPlayer &&
	   board[x+1][y+1] == lastPlayer)
		return true;
	else
		return false;
}



bool halfPly::legalMove(int column)
{
	if(columnHeight[column] < boardHeight)
		return true;
	else
		return false;
}

void halfPly::move(int column, int player) 
{
	if(legalMove(column))
	{
		lastMove[0] = column;
		lastMove[1] = columnHeight[column];
		columnHeight[column]++;	
		board[column][columnHeight[column]] = player;
		
	}
}






























