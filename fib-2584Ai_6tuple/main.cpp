#include <iostream>
#include <cstdlib>
#include "Fib2584/GameBoard.h"
#include "Fib2584/MoveDirection.h"
#include "Fib2584/Statistic.h"
#include "Fib2584Ai.h"

using namespace std;

void sh(int b[4][4]){
	cout<<endl<<"hello"<<endl;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			cout<<b[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"~~~~~~~~~~~~~~~"<<endl;
}

int main(int argc, char* argv[])
{
	if(argc == 1) {
		cerr << "usage: play_game rounds [other arguments which your AI needs]" << endl;
		return 1;
	}
	int iPlayRounds = atoi(argv[1]);
	// create and initialize AI
	
	cout<<"hi!!!!~~~~~~~"<<endl;
	
	Fib2584Ai ai;
	ai.initialize(argc, argv);
	
	cout<<"hello~~~~~~~"<<endl;

	// initialize statistic data
	Statistic statistic;
	statistic.setStartTime();
	// play each round
	for(int i = 0;i < iPlayRounds;i++) {
		GameBoard gameBoard;
		gameBoard.initialize();
		int iScore = 0;
		int arrayBoard[4][4];
		while(!gameBoard.terminated()) {
			gameBoard.getArrayBoard(arrayBoard);
			MoveDirection moveDirection = ai.generateMove(arrayBoard);
			
			
			GameBoard originalBoard = gameBoard;
			iScore += gameBoard.move(moveDirection);
			if(originalBoard == gameBoard)
				continue;
			statistic.increaseOneMove();
			
			gameBoard.addRandomTile();
		}
		gameBoard.getArrayBoard(arrayBoard);
		ai.gameOver(arrayBoard, iScore);
		statistic.increaseOneGame();

		// update statistic data
		statistic.updateScore(iScore);
		statistic.updateMaxTile(gameBoard.getMaxTile());
	}
	statistic.setFinishTime();
	
	/*int a[4],b=8287;
	b=ai.tuple_to_index(a);
	ai.index_to_tuple(a,b);
	
	for(int i=0;i<4;i++){
		cout<<i<<" "<<a[i]<<endl;
	}*/

	/*int b[4][4]={1,2,3,2,0,3,0,8,1,0,5,0,0,8,0,1};
	int a[4][4];
	sh(b);
	ai.next_board_score(b,3,a);
	sh(a);*/
	
	// output statistic data
	statistic.show();

	return 0;
}


