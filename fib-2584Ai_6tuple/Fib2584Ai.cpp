#include "Fib2584Ai.h"
using namespace std;

const int sequence[4] = {1,1,1,3};

const int rectangle_l[8][6][2]={{{0,0},{0,1},{1,1},{2,1},{2,0},{1,0}},
				{{0,0},{0,1},{0,2},{1,2},{1,1},{1,0}},
				{{1,0},{1,1},{2,1},{3,1},{3,0},{2,0}},
				{{2,0},{2,1},{2,2},{3,2},{3,1},{3,0}},
				{{0,1},{0,2},{0,3},{1,3},{1,2},{1,1}},
				{{0,2},{0,3},{1,3},{2,3},{2,2},{1,2}},
				{{1,2},{1,3},{2,3},{3,3},{3,2},{2,2}},
				{{2,1},{2,2},{2,3},{3,3},{3,2},{3,1}}
				};

const int rectangle_m[4][6][2]={{{0,1},{0,2},{1,2},{2,2},{2,1},{1,1}},
				{{1,1},{1,2},{1,3},{2,3},{2,2},{2,1}},
				{{1,1},{1,2},{2,2},{3,2},{3,1},{2,1}},
				{{1,0},{1,1},{1,2},{2,2},{2,1},{2,0}}
				};

const int ax_l[8][6][2]={{{0,0},{1,0},{2,0},{3,0},{3,1},{2,1}},
			{{3,0},{2,0},{1,0},{0,0},{0,1},{1,1}},
			{{3,0},{3,1},{3,2},{3,3},{2,3},{2,2}},
			{{3,3},{3,2},{3,1},{3,0},{2,0},{2,1}},
			{{3,3},{2,3},{1,3},{0,3},{0,2},{1,2}},
			{{0,3},{1,3},{2,3},{3,3},{3,2},{2,2}},
			{{0,0},{0,1},{0,2},{0,3},{1,3},{1,2}},
			{{0,3},{0,2},{0,1},{0,0},{1,0},{1,1}}
			};
			
const int ax_m[8][6][2]={{{0,1},{1,1},{2,1},{3,1},{3,2},{2,2}},
			{{3,1},{2,1},{1,1},{0,1},{0,2},{1,2}},
			{{0,2},{1,2},{2,2},{3,2},{3,1},{2,1}},
			{{3,2},{2,2},{1,2},{0,2},{0,1},{1,1}},
			{{1,0},{1,1},{1,2},{1,3},{2,3},{2,2}},
			{{1,3},{1,2},{1,1},{1,0},{2,0},{2,1}},
			{{2,0},{2,1},{2,2},{2,3},{1,3},{1,2}},
			{{2,3},{2,2},{2,1},{2,0},{1,0},{1,1}}
			};
			
const int ax_r[8][6][2]={{{0,1},{1,1},{2,1},{3,1},{3,0},{2,0}},
			{{3,1},{2,1},{1,1},{0,1},{0,0},{1,0}},
			{{3,2},{2,2},{1,2},{0,2},{0,3},{1,3}},
			{{0,2},{1,2},{2,2},{3,2},{3,3},{2,3}},
			{{1,0},{1,1},{1,2},{1,3},{0,3},{0,2}},
			{{1,3},{1,2},{1,1},{1,0},{0,0},{0,1}},
			{{2,0},{2,1},{2,2},{2,3},{3,3},{3,2}},
			{{2,3},{2,2},{2,1},{2,0},{3,0},{3,1}}
			};
			


Fib2584Ai::Fib2584Ai()
{
	Training=false;
	count=0;
	for(int i=0;i<5;i++){
		WeightTable[i]=new float[WEIGHT_SIZE];
	}
	
	FILE *weight_file=fopen(WEIGHT_TABLE_FILE,"r");
	if(weight_file==NULL){
	//cout<<"zzzzzzz"<<endl;
		for(long long int i=0;i<WEIGHT_SIZE;i++){
			WeightTable[REC_LEFT][i]=0;
			WeightTable[REC_MID][i]=0;
			WeightTable[AX_LEFT][i]=0;
			WeightTable[AX_MID][i]=0;
			WeightTable[AX_RIGHT][i]=0;
			//cout<<"i = "<<i<<endl;
		}

	//cout<<"test~~~~~~~~~~"<<endl;
	}
	else{
		long long int index;
		float rec_lw,rec_mw,ax_lw,ax_mw,ax_rw;
		while(fscanf(weight_file,"%lld %f %f %f %f %f\n", &index, &rec_lw, &rec_mw, &ax_lw, &ax_mw, &ax_rw) != EOF){
			WeightTable[REC_LEFT][index]=rec_lw;
			WeightTable[REC_MID][index]=rec_mw;
			WeightTable[AX_LEFT][index]=ax_lw;
			WeightTable[AX_MID][index]=ax_mw;
			WeightTable[AX_RIGHT][index]=ax_rw;
		}
		fclose(weight_file);
	}
	
	cout<<"Start loading extra table..."<<endl;
	ExtraTable = new float[EXTRA_SIZE];
	FILE *extra_file=fopen(EXTRA_TABLE_FILE,"r");
	if(extra_file==NULL){
		for(int i=0;i<EXTRA_SIZE;i++){
			ExtraTable[i]=0;
		}
	}
	else{
		int ind;
		float weight;
		while(fscanf(extra_file,"%d %f\n", &ind, &weight) != EOF){
			ExtraTable[ind]=weight;
		}
		fclose(extra_file);
	}
	
		cout<<"complete loading file...."<<endl;
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

	//return dir[0];
}

int Fib2584Ai::generateEvilMove(int board[4][4]){
	alternate = 0;
	
	int *evil;
	evil = (int *)malloc(sizeof(int));

	maxTT = (int *)malloc(sizeof(int)*TT_SIZE);	
	minTT = (int *)malloc(sizeof(int)*TT_SIZE);	

	memset(maxTT,-1,TT_SIZE);
	memset(minTT,-1,TT_SIZE);

	// iterative deeping
	for(int i=0;i<5;i++){
		MIN(board, INT_MIN, INT_MAX,i,0,evil);
	}

	free(maxTT);
	free(minTT);

	return *evil;
}

int Fib2584Ai::MAX(const int board[4][4], int alpha, int beta, int depth, int reward, int *dir){

	int hashvalue = hashboard(board);		
	
	if(depth == 0){  // reach leaf
		int m = alpha;
		bool move = false;
		
		for(int i=0;i<4;i++){
			int next[4][4];
			int r=next_board_score(board,i,next);
			if(compare(board,next)) continue;

			move = true;

			Feature f(next,0);		
	
			int score = reward + f.getScore(WeightTable, ExtraTable) +r;
			
			if(score>m){
				*dir = i;
				m = score;
			}
			
			if(m >= beta){
				return m;	
			}
		}
		
		// all invalid move
		
		maxTT[hashvalue] = *dir;
		return move ? m : INT_MIN + reward;
	}
	else{
		int m = alpha;
		bool move = false;

		int olddir = maxTT[hashvalue];
		if(olddir != -1 ){
			int n[4][4];
			int r = next_board_score(board,olddir,n);
			if(!compare(n,board)){
				move = true;
				int *evil;
				evil = (int *)malloc(sizeof(int));
				int score = MIN(n,m,beta,reward+r,depth-1,evil);

				if(score > m){
					*dir = olddir;
					m = score;
				}
				
				if(m >= beta){
					return m;
				}
			}
		}

		for(int i=0;i<4;i++){
			int next[4][4];
			int r=next_board_score(board,i,next);
			if(compare(board,next)) continue;

			move = true;
			int *evil;
			evil = (int *)malloc(sizeof(int));
			int score = MIN(next,m,beta,reward+r,depth-1,evil);

			if(score > m){
				*dir = i;
				m = score;
			}
			
			if(m >= beta){
				maxTT[hashvalue] = i;
				return m;
			}
		}
		
		maxTT[hashvalue] = *dir;
		return move ? m : INT_MIN + reward;
	}
}

int Fib2584Ai::MIN(const int board[4][4], int alpha, int beta, int depth, int reward, int *evil){
	int m = beta;
	int hashvalue = hashboard(board);

	int oldevil = minTT[hashvalue];
	if(oldevil!=-1 && board[oldevil/4][oldevil%4] == 0){
		int n[4][4];
		copyBoard(n,board);
		n[oldevil/4][oldevil%4] = sequence[alternate];
		alternate = (alternate + 1) % 4;

		int *dir;
		dir = (int *)malloc(sizeof(int));
		int score = MAX(n,alpha,m,depth,reward,dir);
		
		if(score < m){
			*evil = oldevil;
			m = score;
		} 
		
		if(score <= alpha){
			return m;
		}
	}

	for(int i=0;i<16;i++){
		int next[4][4];
		int *dir;
		copyBoard(next,board);
		
		if(next[i/4][i%4] != 0) continue;
		
		next[i/4][i%4] = sequence[alternate];
		alternate = (alternate + 1) % 4;

		dir = (int *)malloc(sizeof(int));
		int score = MAX(next,alpha,m,depth,reward,dir);
		
		if(score < m){
			*evil = i;
			m = score;
		} 
		
		if(score <= alpha){
			minTT[hashvalue] = i;
			return m;
		}
	}
	
	minTT[hashvalue] = oldevil;
	return m;
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
	/*for(int i=0;i<5;i++){
		free(WeightTable[i]);
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

/*void Fib2584Ai::index_to_tuple(int tuple[4], int index){
	tuple[3]=((index)&(0b00000000000000011111));
	tuple[2]=((index>>5)&(0b000000000011111));
	tuple[1]=((index>>10)&(0b0000011111));
	tuple[0]=(index>>15);
	
}*/

int Fib2584Ai::hashboard(const int board[4][4]){
	size_t result;
	std::hash<int> hash_int;	

	for(int i=0;i<4;i++){
		size_t row;
		for(int j=0;j<4;j++){
			row |=board[i][j];
			if(j!=3) row<<5;
		}
		
		row = hash_int((int)row<<i);
		result ^=row;
		
	}

	return result%TT_SIZE;
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
	double maxs=-10000000000;
	int maxi=0;
	
	for(int i=0;i<4;i++){
		int next[4][4];
		int nextscore=next_board_score(board,i,next);
		if(compare(next,board)){
			continue;
		}

		Feature next_f=Feature(next,nextscore);
		nextscore+=next_f.getScore(WeightTable,ExtraTable);
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
		for(int i=0;i<4;i++){
			for(int j=3;j>=1;j--){
				if(Mergeable(current[j][i],current[j-1][i])){
					current[j][i]=current[j][i]+current[j-1][i];
					score+=current[j][i];
					current[j-1][i]=0;
					//Adjust_Board(current,0);
				}
			}
		}
		Adjust_Board(current,0);
		Adjust_Board(current,0);
		Adjust_Board(current,0);
	}
	else if(dir==1){ //move down
		Adjust_Board(current,1);
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++){
				if(Mergeable(current[j][i],current[j+1][i])){
					current[j][i]=current[j][i]+current[j+1][i];
					score+=current[j][i];
					current[j+1][i]=0;
					//Adjust_Board(current,1);
				}
			}
		}
		Adjust_Board(current,1);
		Adjust_Board(current,1);
		Adjust_Board(current,1);
	}
	else if(dir==2){ //move left
		Adjust_Board(current,2);
		for(int i=0;i<4;i++){
			for(int j=3;j>=1;j--){
				if(Mergeable(current[i][j],current[i][j-1])){
					current[i][j]=current[i][j]+current[i][j-1];
					score+=current[i][j];
					current[i][j-1]=0;
					//Adjust_Board(current,2);
				}
			}
		}
		Adjust_Board(current,2);
		Adjust_Board(current,2);
		Adjust_Board(current,2);
	}
	else{
		Adjust_Board(current,3);
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++){
				if(Mergeable(current[i][j],current[i][j+1])){
					current[i][j]=current[i][j]+current[i][j+1];
					score+=current[i][j];
					current[i][j+1]=0;
					//Adjust_Board(current,3);
				}
			}
		}
		Adjust_Board(current,3);
		Adjust_Board(current,3);
		Adjust_Board(current,3);
	}
	copyBoard(next,current);
	return score;
}

void Fib2584Ai::trainTable(){
	bool last=true;
	float preScore=0;
	while(total_feature.size()>0){
		Feature f=total_feature.top();
		total_feature.pop();
		if(last){
			last=false;
			preScore=0;
		}
		else{
			float delta = (preScore - f.getScore(WeightTable,ExtraTable));
			delta *= Learning_rate;
			for(int i=0;i<8;i++){
				if(i<4){
					WeightTable[REC_LEFT][f.rectangle_l_Feature[i]]+=delta;
					WeightTable[REC_MID][f.rectangle_m_Feature[i]]+=delta;
					WeightTable[AX_LEFT][f.ax_l_Feature[i]]+=delta;
					WeightTable[AX_MID][f.ax_m_Feature[i]]+=delta;
					WeightTable[AX_RIGHT][f.ax_r_Feature[i]]+=delta;

				}
				else{
					WeightTable[REC_LEFT][f.rectangle_l_Feature[i]]+=delta;
					WeightTable[AX_LEFT][f.ax_l_Feature[i]]+=delta;
					WeightTable[AX_MID][f.ax_m_Feature[i]]+=delta;
					WeightTable[AX_RIGHT][f.ax_r_Feature[i]]+=delta;
				}
			}
			ExtraTable[f.extraFeature]+=delta;
			preScore = f.BoardScore + f.getScore(WeightTable,ExtraTable);
		}
	}
}

void Fib2584Ai::Save(){
	FILE *file = fopen(WEIGHT_TABLE_FILE,"w");
	for(long long int i=0;i<WEIGHT_SIZE;i++){
		fprintf(file,"%lld %f %f %f %f %f\n",i,WeightTable[REC_LEFT][i],WeightTable[REC_MID][i],WeightTable[AX_LEFT][i],WeightTable[AX_MID][i],WeightTable[AX_RIGHT][i]);
	}
	fclose(file);

	FILE *efile = fopen(EXTRA_TABLE_FILE,"w");
	for(int i=0; i<EXTRA_SIZE;i++){
		fprintf(efile,"%d %f\n",i,ExtraTable[i]);
	}
	fclose(efile);
	
	cout<<"Finish writing file......"<<endl;
}

Fib2584Ai::~Fib2584Ai(){
	/*for(int i=0;i<WEIGHT_SIZE;i++){
		cout<<i<<"  "<<WeightTable[OUTER][i]<<"  "<<WeightTable[INNER][i]<<endl;
	}*/

	
	if(Training){
		Save();
	}
}

int Fib2584Ai::Feature::to_index(int tuple[6]){
	/*int num=tuple[0];
	for(int i=0;i<5;i++){
		num <<= 5;
		num |= tuple[i+1];
	}
	return num;*/

	int sum=0;
	for(int i=0;i<6;i++){
		sum+=tuple[i]*pow(22,i);
	}	
	
	return sum;
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
	for(int i=0;i<8;i++){
		int temp_recl[6],temp_recm[6],temp_axl[6],temp_axm[6],temp_axr[6];
		for(int j=0;j<6;j++){
			if(i<4){
				temp_recm[j]=getFib(board[rectangle_m[i][j][0]][rectangle_m[i][j][1]]);
			}
			temp_recl[j]=getFib(board[rectangle_l[i][j][0]][rectangle_l[i][j][1]]);
			temp_axl[j]=getFib(board[ax_l[i][j][0]][ax_l[i][j][1]]);
			temp_axm[j]=getFib(board[ax_m[i][j][0]][ax_m[i][j][1]]);
			temp_axr[j]=getFib(board[ax_r[i][j][0]][ax_r[i][j][1]]);
		}
		
		if(i<4){
			rectangle_m_Feature[i]=to_index(temp_recm);
		}
			
		rectangle_l_Feature[i]=to_index(temp_recl);
		ax_l_Feature[i]=to_index(temp_axl);
		ax_m_Feature[i]=to_index(temp_axm);
		ax_r_Feature[i]=to_index(temp_axr);
		
		
	}
	
	extraFeature=getExtraFeature(board);	
	BoardScore=score;
}

bool Fib2584Ai::Feature::canMerge(int a, int b){
	if(a == 0 || b == 0){
		return false;
	}
	int sum=a+b;
	for(int i=0;i<32;i++){
		if(sum == GameBoard::fibonacci_[i]){
			return true;
		}
	}
	
	return false;
}

int Fib2584Ai::Feature::getExtraFeature(const int board[4][4]){
	int emptycount=0, mergecount=0;
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(board[i][j]==0){
				emptycount+=1;
			}
				
			
			if(j+1<3 && canMerge(board[i][j], board[i][j+1])){
				mergecount+=1;
			}
			
			if(i+1<3 && canMerge(board[i][j], board[i+1][j])){
				mergecount+=1;
			}
			
		}
			
	}
	
	return mergecount<<4 | emptycount;
}

int Fib2584Ai::Feature::getScore(const float *const* weighttable, float const* extratable){
	double score=0;
	for(int i=0;i<8;i++){
		if(i<4){
			score += weighttable[REC_MID][rectangle_m_Feature[i]];
		}
		score+=weighttable[REC_LEFT][rectangle_l_Feature[i]] + weighttable[AX_LEFT][ax_l_Feature[i]] + weighttable[AX_MID][ax_m_Feature[i]] + weighttable[AX_RIGHT][ax_r_Feature[i]];
	}
	
	score += extratable[extraFeature];
	return score;
}

/**********************************
You can implement any additional functions
you may need.
**********************************/
