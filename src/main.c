#include <stdio.h>
#include <stdlib.h>
#include "imageprocess.h"
#include "gobang.h"
#include "hardware.h"

int main(void)
{
	HardwareInit();
	CamInit();
	GobangInit();


	
	for(int i=0;i<BOARD_WIDTH*BOARD_HEIGHT;i++)
	{ 
		printf("请棋手落子\n");
		WaitForKeyPress();//等待用户落子并按下按键确认
		GetNewMap();
		PCLoad();
 		DrawMap();
		if(IsGameOver() == BLACK_WIN)
		{
			printf("Black(%c) is winner\n",BLACK_FLAG);
			exit(0);
		}
		else if (IsGameOver() == WHITE_WIN)
		{
			printf("White(%c) is winner\n",WHITE_FLAG);
			exit(0);
		}
		
	}

}


