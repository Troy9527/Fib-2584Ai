#include "Fib2584Ai.h"
using namespace std;

const int outf[4][4][2]={{{0,0},{0,1},{0,2},{0,3}},
			{{3,0},{3,1},{3,2},{3,3}},
			{{0,0},{1,0},{2,0},{3,0}},
			{{0,3},{1,3},{2,3},{3,3}}
			};

const int inf[4][4][2]={{{0,1},{1,1},{2,1},{3,1}},
			{{0,2},{1,2},{2,2},{3,2}},
			{{1,0},{1,1},{1,2},{1,3}},
			{{2,0},{2,1},{2,2},{2,3}}
			};

Fib2584Ai::Fib2584Ai()
{
	Training=false;
	count=0;
	for(int i=0;i<2;i++){
		WeightTable[i]=new double[WEIGHT_SIZE];
	}
	
	FILE *weight_file=fopen(WEIGHT_TABLE_FILE,"r");
	if(weight_file==NULL){
		for(int i=0;i<WEIGHT_SIZE;i++){
			WeightTable[OUTER][i]=0;
			WeightTable[INNER][i]=0;
		}
	}
	else{
		int index;
		double outw,inw;
		while(fscanf(weight_file,"%d %lf %lf\n",&index,&outw,&inw) != EOF){
			WeightTable[OUTER][index]=outw;
			WeightTable[INNER][index]=inw;
		}
		cout<<"complete loading file...."<<endl;
		fclose(weight_file);
	}
}

void Fib2584Ai::initialize(int argc, char* argv[])
{
	srand(time(NULL));
	double rate;
	if( argc >= 3 && sscanf(argv[2],"%lf",&rate) == 1){
		cout<<"Start Training"<<endl;
		Training=true;
		cout<<"Learning Rate = "<<rate<<endl;
		Learning_rate = rate;
	}
	return;
}


MoveDirection Fib2584Ai::generateMove(int board[4][4])
{
	/*for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			cout<<board[i][j]<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;*/
	int index;
	const MoveDirection dir[] = {MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT};
	MoveDirection Move = chooseDirection(board);
	
	for(int i=0;i<4;i++){
		if(dir[i]==Move){
			index=i;
		}
	}

	int a[4][4];
	int score=next_board_score(board,index,a);
	
	if(Training){
		Feature f(a,score);
		total_feature.push(f);
	}
	
	return Move;
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
	/*for(int i=0;i<WEIGHT_SIZE;i++){
		cout<<i<<"  "<<WeightTable[OUTER][i]<<"  "<<WeightTable[INNER][i]<<endl;
	}*/
	if(Training){
		Feature f(board,0);
		total_feature.push(f);
		trainTable();
		cout<<"count = "<<++count<<endl;
	}
	return;
}



int Fib2584Ai::getFibIndex(int num){
	for(int i=0;i<32;i++){
		if(num==GameBoard::fibonacci_[i]){
			return i;
		}
	}
	
	cout<<"No such Fibonacci number!";
}

int Fib2584Ai::tuple_to_index(int tuple[4]){
	int num=tuple[0];
	for(int i=0;i<3;i++){
		num <<= 5;
		num |= tuple[i+1];
	}
	return num;
}

void Fib2584Ai::index_to_tuple(int tuple[4], int index){
	tuple[3]=((index)&(0b00000000000000011111));
	tuple[2]=((index>>5)&(0b000000000011111));
	tuple[1]=((index>>10)&(0b0000011111));
	tuple[0]=(index>>15);
	
	/*cout<<"1: "<<(index>>15)<<endl;
	cout<<"2: "<<(index>>10)<<endl;
	cout<<"3: "<<(index>>5)<<endl;
	cout<<"4: "<<index<<endl;*/
}

bool Fib2584Ai::Mergeable(int num1, int num2){
	if(num1==0 || num2==0){
		return false;
	}

	int sum=num1+num2;
	for(int i=0;i<32;i++){
		if(sum==GameBoard::fibonacci_[i]){
			return true;
		}
	}
	
	return false;
}

void Fib2584Ai::copyBoard(int board1[4][4], const int board2[4][4]){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			board1[i][j]=board2[i][j];
		}
	}
}

//adjust the zeros
void Fib2584Ai::Adjust_Board(int current[4][4], int dir){
	if(dir==0){  //move up
		for(int k=0;k<3;k++){
			for(int i=0;i<4;i++){
				for(int j=0;j<3;j++){
					if(current[j][i]==0){
						current[j][i]=current[j+1][i];
						current[j+1][i]=0;
					}
				}
			}
		}
	}
	else if(dir==1){ //move down
		for(int k=0;k<3;k++){
			for(int i=0;i<4;i++){
				for(int j=3;j>=1;j--){
					if(current[j][i]==0){
						current[j][i]=current[j-1][i];
						current[j-1][i]=0;
					}
				}
			}
		}
	}
	else if(dir==2){ //move left
		for(int k=0;k<3;k++){
			for(int i=0;i<4;i++){
				for(int j=0;j<3;j++){
					if(current[i][j]==0){
						current[i][j]=current[i][j+1];
						current[i][j+1]=0;
					}
				}
			}
		}
	}
	else{		 //move right
		for(int k=0;k<3;k++){
			for(int i=0;i<4;i++){
				for(int j=3;j>=1;j--){
					if(current[i][j]==0){
						current[i][j]=current[i][j-1];
						current[i][j-1]=0;
					}
				}
			}
		}
	}
}

bool Fib2584Ai::compare(const int a[4][4], const int b[4][4]){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(a[i][j]!=b[i][j]){
				return false;
			}
		}
	}
	return true;
}

MoveDirection Fib2584Ai::chooseDirection(const int board[4][4]){
	const MoveDirection dir[] = {MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT};
	int maxs=-2147483648,maxi=0;
	
	for(int i=0;i<4;i++){
		int next[4][4];
		int nextscore=next_board_score(board,i,next);
		if(compare(next,board)){
			continue;
		}

		Feature next_f=Feature(next,nextscore);
		nextscore+=next_f.getScore(WeightTable);
		if(nextscore>maxs){
			maxs=nextscore;
			maxi=i;
		}
		
	}
	
	return dir[maxi];
}


int Fib2584Ai::next_board_score(const int board[4][4], int dir, int next[4][4]){
	int current[4][4];
	int score=0;
	copyBoard(current,board);
	
	if(dir==0){ //move up
		Adjust_Board(current,0);
		for(int k=0;k<3;k++){
			for(int i=0;i<4;i++){
				for(int j=0;j<3;j++){
					if(Mergeable(current[j][i],current[j+1][i])){
						current[j][i]=current[j][i]+current[j+1][i];
						score+=current[j][i];
						current[j+1][i]=0;
						Adjust_Board(current,0);
					}
				}
			}
		}
	}
	else if(dir==1){ //move down
		Adjust_Board(current,1);
		for(int k=0;k<3;k++){
			for(int i=0;i<4;i++){
				for(int j=3;j>=1;j--){
					if(Mergeable(current[j][i],current[j-1][i])){
						current[j][i]=current[j][i]+current[j-1][i];
						score+=current[j][i];
						current[j-1][i]=0;
						Adjust_Board(current,1);
					}
				}
			}
		}
	}
	else if(dir==2){ //move left
		Adjust_Board(current,2);
		for(int k=0;k<3;k++){
			for(int i=0;i<4;i++){
				for(int j=0;j<3;j++){
					if(Mergeable(current[i][j],current[i][j+1])){
						current[i][j]=current[i][j]+current[i][j+1];
						score+=current[i][j];
						current[i][j+1]=0;
						Adjust_Board(current,2);
					}
				}
			}
		}
	}
	else{
		Adjust_Board(current,3);
		for(int k=0;k<3;k++){
			for(int i=0;i<4;i++){
				for(int j=3;j>=1;j--){
					if(Mergeable(current[i][j],current[i][j-1])){
						current[i][j]=current[i][j]+current[i][j-1];
						score+=current[i][j];
						current[i][j-1]=0;
						Adjust_Board(current,3);
					}
				}
			}
		}

	}
	copyBoard(next,current);
	return score;
}

void Fib2584Ai::trainTable(){
	bool last=true;
	double preScore=0;
	while(total_feature.size()>0){
		Feature f=total_feature.top();
		total_feature.pop();
		if(last){
			last=false;
			preScore=0;
		}
		else{
			double delta = (preScore - f.getScore(WeightTable));
			delta *= Learning_rate;
			for(int i=0;i<4;i++){
				WeightTable[OUTER][f.outerFeature[i]]+=delta;
				WeightTable[INNER][f.innerFeature[i]]+=delta;
			}
			preScore = f.BoardScore + f.getScore(WeightTable);
		}
	}
}

void Fib2584Ai::Save(){
	FILE *file = fopen(WEIGHT_TABLE_FILE,"w");
	for(int i=0;i<WEIGHT_SIZE;i++){
		fprintf(file,"%d %lf %lf\n",i,WeightTable[OUTER][i],WeightTable[INNER][i]);
	}
	fclose(file);
}

Fib2584Ai::~Fib2584Ai(){
	/*for(int i=0;i<WEIGHT_SIZE;i++){
		cout<<i<<"  "<<WeightTable[OUTER][i]<<"  "<<WeightTable[INNER][i]<<endl;
	}*/

	
	if(Training){
		Save();
	}
}

int Fib2584Ai::Feature::to_index(int tuple[4]){
	int num=tuple[0];
	for(int i=0;i<3;i++){
		num <<= 5;
		num |= tuple[i+1];
	}
	return num;
}

int Fib2584Ai::Feature::getFib(int num){
	for(int i=0;i<32;i++){
		if(num==GameBoard::fibonacci_[i]){
			return i;
		}
	}
	
	cout<<"No such Fibonacci number!";

}

Fib2584Ai::Feature::Feature(const int board[4][4], int score){
	for(int i=0;i<4;i++){
		int temp_out[4],temp_in[4];
		for(int j=0;j<4;j++){
			temp_out[j]=getFib(board[outf[i][j][0]][outf[i][j][1]]);
			temp_in[j]=getFib(board[inf[i][j][0]][inf[i][j][1]]);
		}
		outerFeature[i]=to_index(temp_out);
		innerFeature[i]=to_index(temp_in);
	}
	
	BoardScore=score;
}

int Fib2584Ai::Feature::getScore(const double *const* weighttable){
	int score=0;
	for(int i=0;i<4;i++){
		score += weighttable[OUTER][outerFeature[i]] + weighttable[INNER][innerFeature[i]];
	}

	return score;
}

/**********************************
You can implement any additional functions
you may need.
**********************************/
