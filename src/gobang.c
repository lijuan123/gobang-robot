#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gobang.h"
#include "hardware.h"
/***************************************************************/

static int heng(int hang, int lie, char whoFlag);
static int shu(int hang, int lie, char whoFlag);
static int shu(int hang, int lie, char whoFlag);
static int zuoxie(int hang, int lie, char whoFlag);
static int youxie(int hang, int lie, char whoFlag);
static void FigureoutScore(void);
static int FigureoutWeight(int left, int right, int count, int k, char num);

/***************************************************************/

static char map[BOARD_HEIGHT][BOARD_WIDTH]={NO_CHESS_FLAG};//用于存储双方棋子
int PCScore[BOARD_HEIGHT][BOARD_WIDTH]={0};//用于存储PC的棋子
int PlayerScore[BOARD_HEIGHT][BOARD_WIDTH]={0};//用于存储玩家的棋子
ChessPosition_t lastPosition={7,7};
/***************************************************************/

//玩家落子
// static void PlayerLoad(void)
// {
// 	map[lastPosition.x-1][lastPosition.y-1]=WHITE_FLAG;
// }



// +-+-+-+-+
// +-@-@-@-+
// +-+-+-+-+
static int heng(int hang, int lie, char whoFlag)
{
    int spaceNum=0;//空白数
    int count =1;//几连，包含当前要下的子
    int leftHad=0;//左边是否有同子
    int x=hang;
    int y=lie;
    int liveLeft = 0;
    int liveRight=0;

    if( map[hang][lie] != NO_CHESS_FLAG )
    {
        return 0;
    }
    while( y>0 && ( map[x][y-1]==NO_CHESS_FLAG || map[x][y-1]==whoFlag ) )
    {
        if( map[x][y-1]==NO_CHESS_FLAG && spaceNum<1 )
        {//第一个空白
            if( map[x][y-2] != whoFlag )
            {
                liveLeft = 1;
                break;
            }
            spaceNum++;
            y--;
        }else if( map[x][y-1] == whoFlag ){
            leftHad=1;
            y--;
            count++;
        }else{//第2个空白
            liveLeft=1;
            break;
        }
    }

    //如果左边没有同色子，设置空白数为0
    if( !leftHad ){
        spaceNum=0;
    }

    y=lie;
    while( y<14 && ( map[x][y+1]==NO_CHESS_FLAG || map[x][y+1]==whoFlag ) ){
        if( map[x][y+1]==NO_CHESS_FLAG && spaceNum<1){//第一个空白
            if( map[x][y+2] != whoFlag ){
                liveRight = 1;
                break;
            }
            spaceNum++;
            y++;
        }else if( map[x][y+1]==NO_CHESS_FLAG && spaceNum>0 ){//第2个空白
            liveRight = 1;
            break;
        }else{
            y++;
            count++;
        }

    }
    return FigureoutWeight( liveLeft,liveRight, count,spaceNum, whoFlag );
}

// +-+-@-+-+
// +-+-@-+-+
// +-+-@-+-+
static int shu(int hang, int lie, char whoFlag)
{
    int spaceNum=0;//空白数
    int count =1;//几连，包含当前要下的子
    int topHad=0;//上边是否有同子
    int x=hang;
    int y=lie;
    int liveLeft = 0;
    int liveRight=0;

    if( map[hang][lie] != NO_CHESS_FLAG ){
        return 0;
    }
    while( x>0 && ( map[x-1][y]==NO_CHESS_FLAG || map[x-1][y]==whoFlag ) ){
        if( map[x-1][y]==NO_CHESS_FLAG && spaceNum<1 ){//第一个空白
            if( map[x-2][y] != whoFlag ){
                liveLeft = 1;
                break;
            }
            spaceNum++;
            x--;
        }else if( map[x-1][y] == whoFlag ){
            topHad=1;
            x--;
            count++;
        }else{//第2个空白
            liveLeft=1;
            break;
        }
    }

    //如果左边没有同色子，设置空白数为0
    if( !topHad ){
        spaceNum=0;
    }
    x=hang;
    while( x<14 && ( map[x+1][y]==NO_CHESS_FLAG || map[x+1][y]==whoFlag ) ){
        if( map[x+1][y]==NO_CHESS_FLAG && spaceNum<1){//第一个空白
            if( map[x+2][y] != whoFlag ){
                liveRight = 1;
                break;
            }
            spaceNum++;
            x++;
        }else if( map[x+1][y]==NO_CHESS_FLAG && spaceNum>0 ){//第2个空白
            liveRight = 1;
            break;
        }else{
            x++;
            count++;
        }

    }
    return FigureoutWeight( liveLeft,liveRight, count,spaceNum, whoFlag );
}


// +-+-+-@-+
// +-+-@-+-+
// +-@-+-+-+
static int zuoxie(int hang, int lie, char whoFlag)
{
    int spaceNum=0;//空白数
    int count =1;//几连，包含当前要下的子
    int topHad=0;//上边是否有同子
    int x=hang;
    int y=lie;
    int liveLeft = 0;
    int liveRight=0;

    if( map[hang][lie] != NO_CHESS_FLAG )
    {
        return 0;
    }
    //向下
    while( x<14 && y>0 && ( map[x+1][y-1]==NO_CHESS_FLAG || map[x+1][y-1]==whoFlag ) )
    {
        if( map[x+1][y-1]==NO_CHESS_FLAG && spaceNum<1 )
        {//第一个空白
            if( map[x+2][y-2] != whoFlag )
            {
                liveLeft = 1;
                break;
            }
            spaceNum++;
            x++;
            y--;
        }
        else if( map[x+1][y-1] == whoFlag )
        {
            topHad=1;
            x++;
            y--;
            count++;
        }
        else
        {//第2个空白
            liveLeft=1;
            break;
        }
    }

    //如果上边没有同色子，设置空白数为0
    if( !topHad )
    {
        spaceNum=0;
    }

    x=hang;
    y=lie;

    //向上
    while( x>0 && y<14 && ( map[x-1][y+1]==NO_CHESS_FLAG || map[x-1][y+1]==whoFlag ) )
    {
        if( map[x-1][y+1]==NO_CHESS_FLAG && spaceNum<1)
        {//第一个空白
            if( map[x-2][y+2] != whoFlag )
            {
                liveRight = 1;
                break;
            }
            spaceNum++;
            x--;
            y++;
        }
        else if( map[x-1][y+1]==NO_CHESS_FLAG && spaceNum>0 )
        {//第2个空白
            liveRight = 1;
            break;
        }
        else
        {
            x--;
            y++;
            count++;
        }

    }
    return FigureoutWeight(liveLeft,liveRight, count,spaceNum, whoFlag);
}

// +-@-+-+-+
// +-+-@-+-+
// +-+-+-@-+
static int youxie(int hang, int lie, char whoFlag)
{
    int spaceNum=0;//空白数
    int count =1;//几连，包含当前要下的子
    int topHad=0;//上边是否有同子
    int x=hang;
    int y=lie;
    int liveLeft = 0;
    int liveRight=0;

    if( map[hang][lie] != NO_CHESS_FLAG ){
        return 0;
    }
    //向上
    while( x>0 && y>0 && ( map[x-1][y-1]==NO_CHESS_FLAG || map[x-1][y-1]==whoFlag ) ){
        if( map[x-1][y-1]==NO_CHESS_FLAG && spaceNum<1 ){//第一个空白
            if( map[x-2][y-2] != whoFlag ){
                liveLeft = 1;
                break;
            }
            spaceNum++;
            x--;
            y--;
        }else if( map[x-1][y-1] == whoFlag ){
            topHad=1;
            x--;
            y--;
            count++;
        }else{//第2个空白
            liveLeft=1;
            break;
        }
    }

    //如果上边没有同色子，设置空白数为0
    if( !topHad ){
        spaceNum=0;
    }

    x=hang;
    y=lie;

    //向下
    while( x<14 && y<14 && ( map[x+1][y+1]==NO_CHESS_FLAG || map[x+1][y+1]==whoFlag ) ){
        if( map[x+1][y+1]==NO_CHESS_FLAG && spaceNum<1){//第一个空白
            if( map[x+2][y+2] != whoFlag ){
                liveRight = 1;
                break;
            }
            spaceNum++;
            x++;
            y++;
        }else if( map[x+1][y+1]==NO_CHESS_FLAG && spaceNum>0 ){//第2个空白
            liveRight = 1;
            break;
        }else{
            x++;
            y++;
            count++;
        }

    }
    return FigureoutWeight( liveLeft,liveRight, count,spaceNum, whoFlag );
}

static void FigureoutScore(void)
{
    for(int n=0;n<BOARD_HEIGHT;n++)
    {
       for(int m=0;m<BOARD_WIDTH; m++ )
       {
            PCScore[n][m] = heng(n,m,BLACK_FLAG)+shu(n,m,BLACK_FLAG)+zuoxie(n,m,BLACK_FLAG)+youxie(n,m,BLACK_FLAG);
            PlayerScore[n][m] = heng(n,m,WHITE_FLAG)+shu(n,m,WHITE_FLAG)+zuoxie(n,m,WHITE_FLAG)+youxie(n,m,WHITE_FLAG);
       }
    }
}




static int FigureoutWeight(int left, int right, int count, int k, char num)
{
	if( count ==1 )
	{
        return 1;
    }
    else if( count == 2 )
    {
        if( left && right )
        {//左右两边都是空的
            if( k==0 )
            {
                //电脑60
                return num == BLACK_FLAG ? 60 : 50;
            }
            else
            {
                return num == BLACK_FLAG ? 40 : 35;
            }
        }
        else if( !left && !right )
        {
            return 1;
        }
        else
        {
            return 10;
        }
    }
    else if( count == 3 )
    {

        if( left && right )
        {//左右两边都是空的
            if( k==0 )
            {
                //电脑950
                return num == BLACK_FLAG ? 950 : 700;
            }
            else
            {
                return num == BLACK_FLAG ? 900 : 650;
            }
        }
        else if( !left && !right )
        {
            return 1;
        }
        else
        {
            return 100;
        }
    }
    else if( count == 4 )
    {
        if( left && right )
        {//左右两边都是空的
            if( k==0 )
            {
                return num == BLACK_FLAG ? 6000 : 3500;
            }
            else
            {
                return num == BLACK_FLAG ? 5000 : 3000;
            }
        }
        else if( !left && !right )
        {
            return 1;
        }
        else
        {
            if( k==0 )
            {
                return num == BLACK_FLAG ? 4000 : 800;
            }
            else
            {
                return num == BLACK_FLAG ? 3600 : 750;
            }
        }
    }
    else
    {
        if( k==0 )
        {
            return num == BLACK_FLAG ? 20000 : 15000;
        }
        else
        {
            return num == BLACK_FLAG ? 10000 : 3300;
        }
    }
}

void DrawMap(void)
{

    //system("clear");
    
    printf("    X--------------------------->\n");
    printf("    ");
    for (int i = 0; i < BOARD_HEIGHT; i++) 
    {
        if( i>=0 && i<=8)
        {
            printf("%d ",i+1);
        }
        else
        {
            printf("%d",i+1);
        }
    }
    putchar('\n');
    for (int i = 0; i < BOARD_WIDTH; i++) 
    {
        printf("Y");
        if( i>=0 && i<=8)
        {
            printf("0%d ",i+1);
        }
        else
        {
            printf("%d ",i+1);
        }
        for (int j = 0; j < BOARD_WIDTH; j++) 
        {
            putchar(map[i][j]);
            if(j<(BOARD_WIDTH-1)) 
            	putchar('-');
        }
        putchar('\n');
    }
}
void GobangInit(void)
{
	for (int i = 0; i < BOARD_HEIGHT; i++) 
	{
    	for (int j = 0; j < BOARD_WIDTH; j++) 
    	{
            map[i][j] = NO_CHESS_FLAG;
        }
    }
    DrawMap();

}
WhoWin IsGameOver(void)
{

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++) 
        {
            if (map[i][j] == WHITE_FLAG) 
            {
                if ((i + 4) < BOARD_HEIGHT) 
                {
                    if (map[i + 1][j] == WHITE_FLAG && map[i + 2][j] == WHITE_FLAG && map[i + 3][j] == WHITE_FLAG && map[i + 4][j] == WHITE_FLAG)
      					return WHITE_WIN;
                }
                if ((j + 4) < BOARD_WIDTH) 
                {
                    if (map[i][j + 1] == WHITE_FLAG && map[i][j + 2] == WHITE_FLAG && map[i][j + 3] == WHITE_FLAG && map[i][j + 4] == WHITE_FLAG)
      					return WHITE_WIN;
                }
                if ((i + 4) < BOARD_HEIGHT && (j + 4) < BOARD_WIDTH) 
                {
                    if (map[i + 1][j + 1] == WHITE_FLAG && map[i + 2][j + 2] == WHITE_FLAG && map[i + 3][j + 3] == WHITE_FLAG && map[i + 4][j + 4] == WHITE_FLAG)
      					return WHITE_WIN;

                }
                if ((i + 4) < BOARD_HEIGHT && (j - 4) >= 0) 
                {
                    if (map[i + 1][j - 1] == WHITE_FLAG && map[i + 2][j - 2] == WHITE_FLAG && map[i + 3][j - 3] == WHITE_FLAG && map[i + 4][j - 4] == WHITE_FLAG)
      					return WHITE_WIN;                    	
                }
            }
            else if(map[i][j] == BLACK_FLAG)
            {
            	if ((i + 4) < BOARD_HEIGHT) 
                {
                    if (map[i + 1][j] == BLACK_FLAG && map[i + 2][j] == BLACK_FLAG && map[i + 3][j] == BLACK_FLAG && map[i + 4][j] == BLACK_FLAG)
      					return BLACK_WIN;
                }
                if ((j + 4) < BOARD_WIDTH) 
                {
                    if (map[i][j + 1] == BLACK_FLAG && map[i][j + 2] == BLACK_FLAG && map[i][j + 3] == BLACK_FLAG && map[i][j + 4] == BLACK_FLAG)
      					return BLACK_WIN;
                }
                if ((i + 4) < BOARD_HEIGHT && (j + 4) < BOARD_WIDTH) 
                {
                    if (map[i + 1][j + 1] == BLACK_FLAG && map[i + 2][j + 2] == BLACK_FLAG && map[i + 3][j + 3] == BLACK_FLAG && map[i + 4][j + 4] == BLACK_FLAG)
      					return BLACK_WIN;
                }
                if ((i + 4) < BOARD_HEIGHT && (j - 4) >= 0) 
                {
                    if (map[i + 1][j - 1] == BLACK_FLAG && map[i + 2][j - 2] == BLACK_FLAG && map[i + 3][j - 3] == BLACK_FLAG && map[i + 4][j - 4] == BLACK_FLAG)
      					return BLACK_WIN;
                }
            }
        }
    }
    
    return NO_WINNER;
}


//电脑落子
void PCLoad(void)
{
	FigureoutScore();
	int score=0;//用于存放最高分
	int hang=0,lie=0;
	for(int i=0;i<BOARD_HEIGHT;i++)
	for(int j=0;j<BOARD_WIDTH;j++)
	{
		if(PCScore[i][j]>score)
		{
			score=PCScore[i][j];
			hang=i;
			lie=j;
		}
		if (PlayerScore[i][j]>score)
		{
			score=PlayerScore[i][j];
			hang=i;
			lie=j;

		}
	}

    if( map[hang][lie] == NO_CHESS_FLAG )
    {  
        map[hang][lie] = WHITE_FLAG;
		printf("PC Load:(%d %d)\n", hang + 1, lie + 1);
		MoveTo(hang, lie);
    }
	//DrawMap();

}

void SetMap(char who,int x,int y)
{
    assert(who==BLACK_FLAG||who==WHITE_FLAG);
    assert(x>=0&&x<BOARD_WIDTH);
    assert(y>=0&&y<BOARD_HEIGHT);
    map[x][y]=who;
}

char GetMap(int x, int y)
{
	assert(x >= 0 && x<BOARD_WIDTH);
	assert(y >= 0 && y<BOARD_HEIGHT);
	return map[x][y];
}