//Copy Right 2011 Casey Beach
//beachc at (NoSpam) gmail.com

#ifndef HALFPLY_H
#define HALFPLY_H


class halfPly
{

	public:
	halfPly();
	halfPly(int);
	halfPly(halfPly &);
	~halfPly();

	enum{PlayerO = -1, Empty = 0, PlayerX = 1};

	int xMove();	//maximizing
	int oMove();	//minimizing
	
	int evalBoard();		//see if a board is a win
	
	//check all the possible "strips"  only checks around the last move
	bool hStrip(int x, int y);	//horizontal
	bool vStrip(int x, int y);	//verticle
	bool dDStrip(int x, int y);	//diagonal down from left to right
	bool dUStrip(int x, int y);	//diagonal up from left to right

	bool legalMove(int column);	//is a move legal
		
	void move(int column, int player);	//put a peice in the board and fiddle with the required variables


	int best;		//best board so far
	int boardHeight;	
	int columnHeight[3];
	int lastPlayer;
	int lastMove[2];	//(column, row)
	int **board;		//data for the board

	halfPly *parrent;	
	halfPly **children;	//board's children for the breadth first search.

};

#endif
