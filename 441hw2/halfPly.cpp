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

	lastPlayer = PlayerX;

	lastMove[0] = -1;
	lastMove[1] = -1;
	
	//call to first play
}

halfPly::~halfPly(){}

int halfPly::xMove()
{
	children = new halfPly*[3];

	if(evalBoard() != 0)
		return evalBoard();

	for(int i = 0; i < 3; i++)
	{
		children[i] = NULL;
		if(legalMove(i))
		{
			children[i] = copy();
			children[i]->move(i);
			children[i]->lastPlayer = PlayerX;
			children[i]->parrent = this;
		}
	}
	return 0;
}

int halfPly::oMove()
{
	return 0;
}

//returns 1 on X win
//returns 0 on draw
//returns -1 on Y win
int halfPly::evalBoard()
{
	bool win = false;
	
	int x = lastMove[0];
	int y = lastMove[1];

	win = hStrip(1, y);
	win = vStrip(x, y);
	win = dDStrip(1, y);
	win = dUStrip(1, y);


/*
	switch(lastMove[0])
	{
		case 0:	//case for the left most column
			assert(lastPlayer == board[x][y]);
			//check for a winning column
			if(x >= 2)		//x has to be higher then 2 for there to be a 
			{			//winning column or downward diagonal
				if(board[x][y-1] == lastPlayer && board[x][y-2] == lastPlayer)
					return lastPlayer;		
				if(board[x-1][y-1] == lastPlayer && board[x-2][y-2] == lastPlayer)
					return lastPlayer;		
				
			}
				
			//check for a winning row
			if(board[x+1][y] == lastPlayer && board[x+2][y] == lastPlayer)
				return lastPlayer;
			//check for an up/left diagonal
			if(board[x+1][y+1] == lastPlayer && board[x+2][y+2] == lastPlayer)
				return lastPlayer;

			break;
		
		case 1:	//case for the middle column
			assert(lastPlayer == board[x][y]);
			//check for a winning column
			if(x >= 2)		//x has to be higher then 2 for there to be a 
			{			//winning column or downward diagonal
				if(board[x][y-1] == lastPlayer && board[x][y-2] == lastPlayer)
					return lastPlayer;		
				if(board[x+1][y+1] == lastPlayer && board[x-1][y-1] == lastPlayer)
					return lastPlayer;		
				
			}
				
			//check for a winning row
			if(board[x-1][y] == lastPlayer && board[x+1][y] == lastPlayer)
				return lastPlayer;
			//check for an up/left diagonal
			if(board[x-1][y-1] == lastPlayer && board[x+1][y+1] == lastPlayer)
				return lastPlayer;
			break;

		case 2:	//case for the right most column
			assert(lastPlayer == board[x][y]);
			//check for a winning column
			if(x >= 2)		//x has to be higher then 2 for there to be a 
			{			//winning column or downward diagonal
				if(board[x][y-1] == lastPlayer && board[x][y-2] == lastPlayer)
					return lastPlayer;		
				if(board[x-1][y-1] == lastPlayer && board[x-2][y-2] == lastPlayer)
					return lastPlayer;		
				
			}
				
			//check for a winning row
			if(board[x+1][y] == lastPlayer && board[x+2][y] == lastPlayer)
				return lastPlayer;
			//check for an up/left diagonal
			if(board[x+1][y+1] == lastPlayer && board[x+2][y+2] == lastPlayer)
				return lastPlayer;

			break;

		

		default:	
			break;
	
	}
*/	
	return 0;
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
	y--;

	if(y == 0)
		return false;

	if(board[x][y-1] == lastPlayer &&
	   board[x][y] == lastPlayer &&
	   board[x][y+1] == lastPlayer)
		return true;
	else
		return false;


}
bool halfPly::dDStrip(int x, int y)
{
	if(x == 0)
	{
		x++;
		y--;
	}
	else if(x == 2)
	{
		x--;
		y++;
	}

	if(board[x+1][y+1] == lastPlayer &&
	   board[x][y] == lastPlayer &&
	   board[x-1][y-1] == lastPlayer)
		return true;
	else
		return false;

}
bool halfPly::dUStrip(int x, int y)
{
	if(x == 0)
	{
		x--;
		y++;
	}
	else if(x == 2)
	{
		x++;
		y--;
	}if(board[x-1][y-1] == lastPlayer &&
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

void halfPly::move(int column) 
{
	if(legalMove(column))
	{
		lastMove[0] = columnHeight[column];
		lastMove[1] = column;
		columnHeight[column]++;	
		board[column][columnHeight[column]] = lastPlayer;
		
	}
}

halfPly* halfPly::copy() 
{
	halfPly* newHalfPly = new halfPly;
	
	for(int i = 0; i < 3; i++)
	{
		newHalfPly->columnHeight[i] = columnHeight[i];
	}

	newHalfPly->lastPlayer = lastPlayer;

	newHalfPly->lastMove[0] = lastMove[0];
	newHalfPly->lastMove[1] = lastMove[1];
	
	for(int i = 0; i < 12; i++)
	{
		newHalfPly->board[i] = board[i];
	
	}

	return newHalfPly;
}




























