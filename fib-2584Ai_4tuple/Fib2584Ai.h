#ifndef __FIB2584AI_H__
#define __FIB2584AI_H__

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <stack>
#include "Fib2584/MoveDirection.h"
#include "Fib2584/GameBoard.h"


#define	WEIGHT_SIZE 1048576
#define OUTER 0
#define INNER 1
#define WEIGHT_TABLE_FILE "WeightTable.txt"

class Fib2584Ai
{
public:
	Fib2584Ai();
	// initialize ai
	void initialize(int argc, char* argv[]);
	// generate one move
	MoveDirection generateMove(int board[4][4]);
	// do some action when game over
	void gameOver(int board[4][4], int iScore);

	/**********************************
	You can implement any additional functions
	or define any variables you may need.
	**********************************/
	MoveDirection chooseDirection(const int board[4][4]);
	int next_board_score(const int board[4][4], int dir, int next[4][4]);
	int getFibIndex(int);
	int tuple_to_index(int tuple[4]);
	void index_to_tuple(int tuple[4], int index);
	bool Mergeable(int, int);
	void copyBoard(int board1[4][4], const int board2[4][4]);
	void Adjust_Board(int board[4][4],int dir);
	bool compare(const int a[4][4], const int b[4][4]);
	void trainTable();
	void Save();
	~Fib2584Ai();
private:
	double *WeightTable[2];
	class Feature{
		public:
			Feature(const int board[4][4],int score);
			int getFib(int);
			int getScore(const double *const* weighttable);
			int to_index(int tuple[4]);
	
			int innerFeature[4],outerFeature[4];
			int BoardScore;
	};
	bool Training;
	double Learning_rate;
	int count;
	std::stack<Fib2584Ai::Feature> total_feature;
};

#endif
