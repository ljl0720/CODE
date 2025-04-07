#ifndef __BSP_TOOL_H
#define __BSP_TOOL_H

#include "GUI.h"
#include "DIALOG.h"

#include "stm32f4xx.h"
#include "GUI_Type.h"
#include <math.h>
#include <stdint.h>
#include "string.h"

#define BITMAP_NUM   10

// 箭头参数结构体
typedef struct {
    int   length;  // 箭头长度（像素）
    float angle;   // 箭头角度（弧度）
    int   fill;    // 是否填充
} ArrowConfig;


// 数据点结构
typedef struct DataPoint{
    float x;
    float y;
	  struct DataPoint *next;
//	  struct DataPoint *prev;
} DataPoint;



void LoadBMP_UsingMEMDEV(char *sFilename, GUI_BITMAP* ic);
void ShowPNGEx(const char *sFilename, WM_HWIN hItem);
void ShowPNG(const char *sFilename, int x0, int y0);
int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off);
char* Text_Edit_Set(char* str1,char* str2);
void GUI_DrawArrow(int x0, int y0, int x1, int y1, ArrowConfig cfg);

void DrawCurve(DataPoint points[],int num_points);

void TEXT_Point(int X,float Y);
void AddPoint(DataPoint **DP,float x,float y);
DataPoint *DP_create(float x,float y);
void DrawPoints(DataPoint *value );
void Destory_Point(DataPoint **DP);
// 图表尺寸和边距
#define CHART_WIDTH  320.0
#define CHART_HEIGHT 240.0
#define MARGIN_LEFT   30.0
#define MARGIN_RIGHT  20.0
#define MARGIN_TOP    20.0
#define MARGIN_BOTTOM 20.0

// 坐标轴范围
#define X_MIN 0.0
#define X_MAX 3600.0
#define Y_MIN 0.0
#define Y_MAX 4.0



void DataToScreen(int x, int y, int* sx, int* sy);
void DrawAxes(void) ;
void DrawTicks(int XBlock,int YBlock);
void XYBlock_Init(void);


void Move_Figure(WM_HWIN hItem,WM_HWIN Icon,int flag,float value);
void Move_Line(WM_HWIN hItem,int flag,float value);

typedef struct Task _List;
typedef _List* Task_List;
struct Disposition_Value;

_List* List_Create(struct Disposition_Value val);
void Insert_Task(_List** TL,struct Disposition_Value val);
void ShowList(_List **TL);
void Add_Task(struct Disposition_Value val);
void Destory_Task(_List **TL,int pos);
_List* Find_Task(_List* TL,int pos);
void Test_del(int pos);

void Task_Update(struct Disposition_Value *val,int pos,int flag);
void Task_Free(void);
#endif