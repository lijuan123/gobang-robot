#include "imageprocess.h"
#include "gobang.h"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

static IplImage* gOriginImg;
static IplImage* gBinImg;
static CvCapture* cam;
static CvPoint rectCorner[4];
static CvPoint gridCorners[13][13];
static float boardGridWidth = 0; 

/** 函数声明 */
static void GetBinaryImage(void);
static void GetGridCorner(void);
static char GetChessKind(CvPoint anchor);
static int GetPointVal(IplImage* pImg, CvPoint point);
static void GetBoardCorner(int event,int x,int y,int flags,void * param);
static void GetFrameFromCam(void);

/**
 * 从摄像头获取一帧图像
 * 图像存储在全局变量IplImage* pOriginImg所指向的内存中
 */
void GetFrameFromCam(void)
{

	IplImage* pFrame;
	//while (1)
	//{
	for (int i = 0; i < 10; i++)
	{
		pFrame = cvQueryFrame(cam);

		if (pFrame == NULL)
		{
			perror("Fail to query frame\r\n");
			exit(-1);
		}
	}
	//cvShowImage("Camera", pFrame);

	//	char c = cvWaitKey(33);
	//	if (c == 27)
	//		break;
	//}
	
	cvCopy(pFrame, gOriginImg, NULL);
//	cvReleaseCapture(&cam);

}

/**
 * 将三通道的gOriginImg转化为单通道的gBinImg，dst必须先创建
 */
static void GetBinaryImage(void)
{
	IplImage* pGrayImage = cvCreateImage(cvGetSize(gOriginImg), IPL_DEPTH_8U, 1);
	cvCvtColor(gOriginImg, pGrayImage, CV_BGR2GRAY);//灰度化


	IplImage* pBinImg = cvCreateImage(cvGetSize(gOriginImg), IPL_DEPTH_8U, 1);
	cvAdaptiveThreshold(pGrayImage, pBinImg, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 71, 3);//自适应二值化

	IplImage* pTemp = cvCreateImage(cvGetSize(gOriginImg), IPL_DEPTH_8U, 1);
	IplConvKernel* pKernel = cvCreateStructuringElementEx(9, 9, 1, 1, CV_SHAPE_RECT, NULL);

	cvMorphologyEx(pBinImg, gBinImg, pTemp, pKernel, CV_MOP_CLOSE, 1);
	cvMorphologyEx(gBinImg, gBinImg, pTemp, pKernel, CV_MOP_ERODE, 1);

	cvReleaseImage(&pGrayImage);
	cvReleaseImage(&pBinImg);
	cvReleaseImage(&pTemp);
	cvReleaseStructuringElement(&pKernel);

}

//获取指定坐标的棋子是白子还是黑子
static char GetChessKind(CvPoint anchor)
{
	long cnt = 0;
	for (int i = -boardGridWidth*0.5; i < boardGridWidth*0.5; i++)
	{
		for (int j = -boardGridWidth *0.5; j < boardGridWidth *0.5; j++)
		{

			//cvCircle(gOriginImg, cvPoint(anchor.x + i, anchor.y + j), 1, CV_RGB(255, 255, 0), 1, 8, 0);
			int val = GetPointVal(gBinImg, cvPoint(anchor.x + i, anchor.y + j));
			if (val == 0)//如果是黑子
			{
				//cvCircle(gOriginImg, cvPoint(anchor.x + i, anchor.y + j), 1, CV_RGB(0, 255, 255), 1, 8, 0);
				cnt++;
			}


		}
		//printf("\n");
	}
	
	float blackPercent = (float)cnt / (boardGridWidth*boardGridWidth);
	if (blackPercent > BLACK_CHESS_THRESH)
	{
		//printf("Black chess：%g\n", blackPercent);
		return BLACK_FLAG;
	}
	else
		return NO_CHESS_FLAG;
	
}

//获取图像上指定点的像素值，pImg必须指向二值图像（当然，指向灰度图像，甚至是三通道图像也是可以的，
//但是这个函数的目的是判断棋子类型，所以必须是二值图像）
static int GetPointVal(IplImage* pImg, CvPoint point)
{
	uchar val = *((uchar*)(pImg->imageData + point.y*pImg->widthStep) + point.x);
	return (int)val;
}



static void FreshMap(void)
{
	for (size_t i = 0; i < BOARD_HEIGHT; i++)
	{
		for (size_t j = 0; j < BOARD_WIDTH; j++)
		{
			char c = GetMap(i, j);
			if (c != BLACK_FLAG)
			{
				if ((GetChessKind(gridCorners[i][j]) == BLACK_FLAG)&&
					(GetPointVal(gBinImg, gridCorners[i][j])) == 0)
				{
					SetMap(BLACK_FLAG, i, j);
					printf("New:（%d,%d）=>%d\n", i+1, j+1,GetPointVal(gBinImg,gridCorners[i][j]));

				}

			}


		}
	}	
}

//获取棋盘的四个角的坐标
static void GetBoardCorner(int event,int x,int y,int flags,void * param)
{
	IplImage* img = (IplImage*)param;
	static int clkCnt = 0;
	switch (event)
	{
	case CV_EVENT_RBUTTONDBLCLK:
		cvCircle(param, cvPoint(x, y ), 3, CV_RGB(255, 0, 0),1,8,0);
		rectCorner[clkCnt++] = cvPoint(x, y);
		cvShowImage("Please set up corners", img);
		printf("(%4d,%4d)\n", x, y);
		break;
	case CV_EVENT_LBUTTONDBLCLK:
		printf("Value:%d\n", GetPointVal(gBinImg, cvPoint(x, y)));
		break;
	default:
		break;
	}
	if (clkCnt == 4)
	{
		clkCnt = 0;
		cvShowImage("Please set up corners", img);
	}	
}

/**
 * 获取棋盘上每一个格点的坐标
 */
static void GetGridCorner(void)
{

	for (size_t i = 0; i < BOARD_HEIGHT; i++)
	{
		gridCorners[i][0].x = (rectCorner[1].x - rectCorner[0].x) / 12.0*i + rectCorner[0].x;
		gridCorners[i][0].y = (rectCorner[1].y - rectCorner[0].y) / 12.0*i + rectCorner[0].y;

		gridCorners[i][12].x = (rectCorner[2].x - rectCorner[3].x) / 12.0*i + rectCorner[3].x;
		gridCorners[i][12].y = (rectCorner[2].y - rectCorner[3].y) / 12.0*i + rectCorner[3].y;
	}

	for (size_t i = 0; i < BOARD_HEIGHT; i++)
	{
		for (size_t j = 0; j < BOARD_WIDTH; j++)
		{
			gridCorners[i][j].x = (gridCorners[i][12].x - gridCorners[i][0].x) / 12.0*j + gridCorners[i][0].x;
			gridCorners[i][j].y = (gridCorners[i][12].y - gridCorners[i][0].y) / 12.0*j + gridCorners[i][0].y;

			cvCircle(gOriginImg, gridCorners[i][j], 3, CV_RGB(255, 0, 0), 1, 8, 0);
			printf("(%4d,%4d)", gridCorners[i][j].x, gridCorners[i][j].y);

		}

		printf("\n");
	}
	boardGridWidth = sqrt((gridCorners[0][0].x - gridCorners[0][1].x)*(gridCorners[0][0].x - gridCorners[0][1].x) +
		(gridCorners[0][0].y - gridCorners[0][1].y)*(gridCorners[0][0].y - gridCorners[0][1].y));
	cvShowImage(MAIN_WINDOW, gOriginImg);
	cvWaitKey(0);
	cvDestroyWindow(MAIN_WINDOW);
}


void CamInit(void)
{

	gOriginImg = cvCreateImage(cvSize(CAM_WIDTH,CAM_HEIGHT),IPL_DEPTH_8U,3);
	gBinImg = cvCreateImage(cvSize(CAM_WIDTH,CAM_HEIGHT),IPL_DEPTH_8U,1);
	cam = cvCreateCameraCapture(CAM_ID);
	if (cam == NULL)
	{
		perror("Fail to open camera\r\n");
		exit(-1);
	}
	//cvNamedWindow("Chess board image",0);
	//调试的时候打开摄像头显示视频，通过鼠标点击获得棋盘的四个角的坐标
	GetFrameFromCam();
	cvShowImage("Please set up corners", gOriginImg);
	cvSetMouseCallback("Please set up corners", GetBoardCorner, gOriginImg);
	cvSetMouseCallback("Chess board image", GetBoardCorner, gOriginImg);

	//printf("Please set up corners");
	
	while (1)
	{
		char c = cvWaitKey(33);
		if (c == 27)
			break;
	}
	cvDestroyWindow("Please set up corners");

	GetGridCorner();

}
void GetNewMap(void)
{
	GetFrameFromCam();
	GetBinaryImage();
	//cvShowImage("Chess board image", gBinImg);
//
	//while (1)
	//{
		//char c = cvWaitKey(33);
		//if (c == 27)
			//break;
	//}
	FreshMap();
}

