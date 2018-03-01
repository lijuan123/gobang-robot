#pragma once
#include <stdbool.h>

/**玩家执白子，电脑执黑子
*/
#define BOARD_WIDTH 13
#define BOARD_HEIGHT 13
#define WHITE_FLAG 'O'
#define BLACK_FLAG '@'
#define NO_CHESS_FLAG '+'

typedef struct 
{
	int x;
	int y;
}ChessPosition_t;

typedef enum {BLACK_WIN,WHITE_WIN,NO_WINNER} WhoWin;

extern void GobangInit(void);
extern WhoWin IsGameOver(void);
extern void SetMap(char who,int x,int y);
extern void DrawMap(void);
extern void SetMap(char who,int x,int y);
extern void PCLoad(void);
extern char GetMap(int x, int y);
extern ChessPosition_t lastPosition;
