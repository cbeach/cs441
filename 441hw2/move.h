
class move
{

	public:
	move();
	~move();


	int xMove();	//maximizing
	int oMove();	//minimizing
	
	int evalBoard(int player, int* board);
	int legalMove(int column);
	
	move copy();
	
	int height[3];
	int currentPlayer;
	int lastMove[2];
	int board[12];


};
