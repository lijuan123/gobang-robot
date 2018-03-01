#pragma once

#include "gobang.h"

#define CAM_HEIGHT 480
#define CAM_WIDTH  640
#define MAIN_WINDOW "Main window"
#define CAM_ID 0

#define BLACK_CHESS_THRESH 0.5

#define DEBUG_CAM 1

void CamInit(void);
void GetNewMap(void);
