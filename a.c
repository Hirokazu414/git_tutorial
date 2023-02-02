#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> //どの関数のために入れたのか
#include <sys/time.h> 

#define dPlayerNum          (0)
#define dEnemyNum           (1)
#define dMatchPlayers       (2) // 2人
#define dNumberSize         (3) // 3桁数字 
#define dGuessMaxNum        (10) // 予測回数
#define dSuccess            (0)     // 正常終了
#define dOptionError        (-1)    // エラー終了(コマンドラインの読み込み失敗)
#define dSame               (0)
#define dDiff               (1)
#define dTrue               (!0)
#define dFalse              (0)
#define dBufSize				(16)

// マクロ
#define GetOneDigit()       (rand() % 10)

typedef struct _HISTORY { // 予測値とhitとblowを管理する構造体
    char guess[dNumberSize + 1];
    char hit;
    char blow;
} HISTORY;

int GenerateRandomNumber(char *answer);
int GenerateNumber (char *answer);
int  InputNumber(char *guess);
int JudgeNumber(HISTORY *history, char *answer);
void Drawing(HISTORY *player, HISTORY *enemy, int ct);
void InitDrawing(char *answer);
int Judgement(int *Judge);
double MyClock(void);

int main(void){
    char answer[(dNumberSize + 1) * 2]; 
    int ct;
    HISTORY playerHist[dGuessMaxNum * 2];
    int judge[dMatchPlayers] = {dFalse, dFalse}; // 自分と敵のフラグ
	 
	InitDrawing(answer);
    for(ct = 0; ct < dGuessMaxNum; ct++){ // max回数まで繰り返す
        printf("3桁で各桁がユニークな数値を入力してください \n > ");
        InputNumber(playerHist[ct].guess);
        GenerateRandomNumber(playerHist[dGuessMaxNum+ct].guess);

        judge[dPlayerNum] = JudgeNumber(playerHist + ct, answer + dNumberSize + 1);
        judge[dEnemyNum] = JudgeNumber(playerHist + dGuessMaxNum + ct, answer);
        Drawing(playerHist, playerHist+dGuessMaxNum, ct);

        if(Judgement(judge) == dTrue){
            return dSuccess;
        }
    }
    if(judge[dPlayerNum] == dFalse){
        printf("あなたの負けです．\n");
    }

    return dSuccess;
}

void InitDrawing(char *answer){
	GenerateNumber(answer); // 自分の番号を決める
   GenerateRandomNumber(answer+dNumberSize + 1); // 敵の番号を決める
}

int GenerateNumber (char *answer){
	system("clear");
	printf("自分の答えを3桁で各桁がユニークな数値を入力してください \n > ");
	while(InputNumber(answer)==dTrue){
		printf("失敗：3桁で各桁がユニークな数値を入力してください\n >");
	}
	printf("あなたの数字は%sです\n",answer);
	sleep(1.5);
	return dTrue;
}


int GenerateRandomNumber(char *answer){
    int i, ct = 0;
    srand(MyClock() * 1e+4);

    while(ct < dNumberSize){
        int num = GetOneDigit() + '0';
        for(i = 0; num != answer[i] && i < ct; i++);
        if(num != answer[i] && ct == i)
            answer[ct++] = num;
    }
    answer[ct] = '\0';

    return dTrue;
}



int InputNumber (char *guess){
	int i;
	char buf[dBufSize];
	scanf("%16s", buf);
	for(i = 0; i < dNumberSize && '0' <= buf[i] && buf[i] <= '9' && strchr(buf,buf[i]) == buf+i; i++){
	}
   if(i == dNumberSize){
    	buf[dNumberSize]='\0';
    	strcpy(guess, buf);
   		return dTrue;
   	}
   return dFales;
}

int JudgeNumber (HISTORY *history, char *answer){
    int i, j;

    history->hit = 0;
    history->blow = 0;

    for(i = 0; i < dNumberSize; i++){
        for(j = 0; j < dNumberSize; j++){
            if(history->guess[i] == answer[j]){
                if(i == j)
                    history->hit++;
                else
                    history->blow++;
            }
        }
    }
    return history->hit != 3 ? dFalse : dTrue;
}

void Drawing (HISTORY *player, HISTORY *enemy, int ct){
    int i;
    system("clear");
    printf("Player\t\t\t\t\tEnemy\n");
    for(i = 0; i <= ct-1; i++){
        printf("%s: hit %d, blow %d\t\t\t%s: hit %d, blow %d\n", player[i].guess, player[i].hit, player[i].blow, enemy[i].guess, enemy[i].hit, enemy[i].blow);
    }
   printf("%s: hit %d, blow %d\n", player[i].guess, player[i].hit, player[i].blow);
   sleep(1);
    
    system("clear");
    printf("Player\t\t\t\t\tEnemy\n");
    for(i = 0; i <= ct; i++){
        printf("%s: hit %d, blow %d\t\t\t%s: hit %d, blow %d\n", player[i].guess, player[i].hit, player[i].blow, enemy[i].guess, enemy[i].hit, enemy[i].blow);
    }
    printf("\n");
}

int Judgement(int *judge){
    if(judge[dPlayerNum] == dTrue && judge[dEnemyNum] == dFalse){
            printf("あなたの勝ちです．\n");
            return dTrue;
        } 
        else if(judge[dPlayerNum] == dFalse && judge[dEnemyNum] == dTrue){
            printf("あなたの負けです．\n");
            return dTrue;
        }
        else if(judge[dPlayerNum] == dTrue && judge[dEnemyNum] == dTrue){
            printf("引き分けです．\n");
            return dTrue;
        }
    return dFalse;
}

double MyClock (void){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    return tv.tv_sec + (double)tv.tv_usec * 1e-6;
}
