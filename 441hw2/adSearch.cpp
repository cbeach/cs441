//Copy Right 2011 Casey Beachi
//beachc at (NoSpam) gmail.com

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "halfPly.h"

using namespace std;



int main(int argc, char** argv)
{
	halfPly *game;
	int winner = 0;
	int boardHeight = 4;

	game = new halfPly(boardHeight);	
	winner = game->xMove();

	cout << "a game of connect 3 is being played on a " << boardHeight  << "X3 board." 
	<< endl << 
	"The winner is... ";
	switch(winner)
	{
		case -1:
			cout << "Black!" << endl;
			break;
		case 0:
			cout << "No one, it's a draw." << endl;
			break;
		case 1:
			cout << "Red!" << endl;
			break;

	}
	
	return 0;
}
