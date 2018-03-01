#include "hardware.h"
#include "gobang.h"
#include <errno.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <string.h>
#include <stdlib.h>

static int fd = -1;

static void SerialInit(void);
static void KeyInit(void);

static void SerialInit(void)
{
    if ((fd = serialOpen("/dev/ttyUSB0", 115200))<0)
    {
		fprintf(stderr, "Unable to open serial devive:%s\n", strerror(errno));
		exit(-1);
    }
	//printf("fd=%d\n", fd);
	serialPuts(fd, "G92X0Y0Z1\n");
	//serialPuts(fd, "G00X0Y0Z1F100\n");
    serialPuts(fd, "G00X-30F100\n");
}
static void KeyInit(void)
{
	pinMode(KEY0,INPUT);
}
void WaitForKeyPress(void)
{
	while(1)
	{
		if(digitalRead(KEY0) == LOW)
		{
			//TODO:delay()函数的单位
			delay(20);
			if(digitalRead(KEY0) == LOW)
			{
				break;
			}
		}
	}
}

void SetOrigin(void)
{
    serialPuts(fd, "G92X0Y0Z0\n");
}

void MoveTo(float x, float y)
{
    char xBuf[5] = {0}, yBuf[5] = {0}; //用于存放x，y的坐标

    //将浮点数转化为字符串
	sprintf(xBuf, "%.1f", ((BOARD_WIDTH - x - 1)*GRID_LEN));
    sprintf(yBuf, "%.1f", y*GRID_LEN);

    char sendBuf[100] = {0}; //用于存放G代码指令
	
	//吸取棋子
	serialPuts(fd, "G00Z0F100\n");
	printf("%s", "G00Z0F100\n");
	serialPuts(fd, "G00Z1F100\n");
	printf("%s", "G00Z1F100\n");
	
	//x轴方向移动
    strcpy(sendBuf, "G00X");
    strcat(sendBuf, xBuf);
	strcat(sendBuf, "F120\n");
	serialPuts(fd, sendBuf);
	printf("%s", sendBuf);
	
	//y轴方向移动
	strcpy(sendBuf, "G00Y");
	strcat(sendBuf, yBuf);
	strcat(sendBuf, "F120\n");
	serialPuts(fd, sendBuf);
	printf("%s", sendBuf);
	
	//放下棋子
	serialPuts(fd, "G00Z0F100\n");
	printf("%s", "G00Z0F100\n");
	serialPuts(fd, "G00Z1F100\n");
	printf("%s", "G00Z1F100\n");
	
	//y轴方向移动
	strcpy(sendBuf, "G00Y");
	strcat(sendBuf, "0");
	strcat(sendBuf, "F120\n");
	serialPuts(fd, sendBuf);
	printf("%s", sendBuf);
	
	//x轴方向移动
	strcpy(sendBuf, "G00X");
	strcat(sendBuf, "-30");
	strcat(sendBuf, "F120\n");
	serialPuts(fd, sendBuf);	
	printf("%s", sendBuf);


}

void HardwareInit(void)
{
    //TODO:由于wiringPi需要root权限，注释掉这句就可以暂时以普通用户启动
  //  if (wiringPiSetup() == -1)
  //  {
		//fprintf(stderr, "Unable to start wiringPi:%s\n", strerror(errno));
		//exit(-1);
  //  }

	SerialInit();
   // KeyInit();
}
