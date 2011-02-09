#ifndef HALFPLY_H
#define HALFPLY_H


class halfPly
{

	public:
	halfPly();
	halfPly(int);
	~halfPly();

	enum{PlayerO = -1, Empty = 0, PlayerX = 1};

	int xMove();	//maximizing
	int oMove();	//minimizing
	
	int evalBoard();
	bool hStrip(int x, int y);
	bool vStrip(int x, int y);
	bool dDStrip(int x, int y);
	bool dUStrip(int x, int y);

	bool legalMove(int column);
	
	void move(int column);

	halfPly* copy();

	int boardHeight;
	int columnHeight[3];
	int lastPlayer;
	int lastMove[2];	//(column, row)
	int **board;
	
	halfPly *parrent;
	halfPly **children;

};

#endif
