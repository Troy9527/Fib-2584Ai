#ifndef __FIB2584AI_H__
#define __FIB2584AI_H__

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <stack>
#include <cmath>
#include <limits.h>
#include <functional>
#include "Fib2584/MoveDirection.h"
#include "Fib2584/GameBoard.h"


#define	WEIGHT_SIZE 113379904
#define EXTRA_SIZE 256
#define REC_LEFT 0
#define REC_MID 1
#define AX_LEFT 2
#define AX_MID 3
#define AX_RIGHT 4
#define WEIGHT_TABLE_FILE "WeightTable_6tuple.txt"
#define EXTRA_TABLE_FILE "ExtraTable.txt"
#define TT_SIZE 1048576

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

	int generateEvilMove(int board[4][4]);
	int MAX(const int board[4][4], int alpha, int beta, int depth, int reward, int *dir);
	int MIN(const int board[4][4], int alpha, int beta, int depth, int reward, int *evil);
	int hashboard(const int board[4][4]);
	
private:
	float *WeightTable[5];
	float *ExtraTable;
	int alternate;

	int *maxTT;
	int *minTT;	

	class Feature{
		public:
			Feature(const int board[4][4],int score);
			int getFib(int);
			int getScore(const float *const* weighttable, float const* extratable);
			int to_index(int tuple[6]);
			int getExtraFeature(const int board[4][4]);
			bool canMerge(int,int);
			
			int ax_l_Feature[8],ax_m_Feature[8],ax_r_Feature[8];
			int rectangle_l_Feature[8],rectangle_m_Feature[4];
			int extraFeature;
			int BoardScore;
	};
	bool Training;
	double Learning_rate;
	int count;
	std::stack<Fib2584Ai::Feature> total_feature;
};

#endif
