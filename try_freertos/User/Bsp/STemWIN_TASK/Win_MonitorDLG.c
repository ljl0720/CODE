/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 6.16                          *
*        Compiled Nov 13 2020, 12:50:00                              *
*        (c) 2019 Segger Microcontroller GmbH                        *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "bsp_keyboard.h"
#include "Win_MonitorDLG.h"
#include "Win_LoginDLG.h"
#include "Win_DispositionDLG.h"

/*BSP 板级配置头文件*/
#include "./Bsp/STemWIN_TOOL/bsp_tool.h"
#include <string.h>
#include <stddef.h>
#include "stdlib.h"
#include "stdio.h"

/*FREERTOS 头文件*/
#include "FreeRTOS.h"
#include "event_groups.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    					(GUI_ID_USER + 0x00)
#define ID_GRAPH_0   					  (GUI_ID_USER + 0x01)
#define ID_IMAGE_Logo   				(GUI_ID_USER + 0x02)
#define ID_ICONVIEW_Connect   	(GUI_ID_USER + 0x03)
#define ID_TEXT_Task    				(GUI_ID_USER + 0x04)
#define ID_TEXT_Time    				(GUI_ID_USER + 0x05)
#define ID_ICONVIEW_Menu    		(GUI_ID_USER + 0x06)
#define ID_TEXT_Nm_Value    		(GUI_ID_USER + 0x07)
#define ID_TEXT_Deg_Value    		(GUI_ID_USER + 0x08)
#define ID_TEXT_Num_Value    		(GUI_ID_USER + 0x09)
#define ID_TEXT_Nm_Title    		(GUI_ID_USER + 0x0A)
#define ID_TEXT_Deg_Title    		(GUI_ID_USER + 0x0B)
#define ID_TEXT_Num_Title    		(GUI_ID_USER + 0x0C)
#define ID_TEXT_Effect    			(GUI_ID_USER + 0x0D)

#define ID_TEXT_Nm_Min    		(GUI_ID_USER + 0x019)
#define ID_TEXT_Nm_Max    		(GUI_ID_USER + 0x01A)
#define ID_TEXT_Deg_Min    		(GUI_ID_USER + 0x01B)
#define ID_TEXT_Deg_Max    		(GUI_ID_USER + 0x01C)

#define ID_IMAGE_0_IMAGE_0  0x00

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _acImage_0, "BMP", ID_IMAGE_0_IMAGE_0
*/

// USER START (Optionally insert additional static data)
// USER END
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsong;
extern GUI_CONST_STORAGE GUI_FONT GUI_YaHei24B_cn;
extern EventGroupHandle_t EventGroup_next_Handle;
extern EventGroupHandle_t EventGroup_prev_Handle;

extern struct Monitor_Value MONV;
extern DataPoint *head;

 char Nm_Title[]="\xe6\x89\xad\xe7\x9f\xa9";
 char Deg_Title[]="\xe8\xa7\x92\xe5\xba\xa6";
 char Num_Tiltle[]="\xe6\x95\xb0\xe9\x87\x8f";
 char Effect[]="\xe5\x90\x88\xe6\xa0\xbc";//合格
 char Run[]="\xe6\x89\xa7\xe8\xa1\x8c";//执行
 char Task[]="\xe4\xbb\xbb\xe5\x8a\xa1";//任务
 char show[20];
 
 DataPoint points[] = {
    {200,0.2}, {800,1.2}, {900,1.4}, {1600, 1.6}, {2000,2.4}, {2000, 2.4}
};
 
int num_x=0;
	  float num_y=0;
 
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Monitor_Win", ID_WINDOW_0, 0, 0, 320, 240, 0, 0x0, 0 },
//  { GRAPH_CreateIndirect, "Nm_Deg_Graph", ID_GRAPH_0, 5, 60, 310, 180, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Device_Logo", ID_IMAGE_Logo, 5, 10, 37, 33, 0, 0x00140014, 0 },
  { ICONVIEW_CreateIndirect, "Connect_Show", ID_ICONVIEW_Connect, 45, 10, 41, 32, 0, 0x00140020, 0 },
  { TEXT_CreateIndirect, "Task_Show_Text", ID_TEXT_Task, 90, 10, 120, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "ID_TEXT_Nm_Min", ID_TEXT_Nm_Min, 185, 65, 50, 10, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "ID_TEXT_Nm_Max", ID_TEXT_Nm_Max, 185, 75, 50, 10, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "ID_TEXT_Deg_Min", ID_TEXT_Deg_Min, 185, 87, 50, 10, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "ID_TEXT_Deg_Max", ID_TEXT_Deg_Max, 185, 97, 50, 10, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Time_Show", ID_TEXT_Time, 215, 10, 55, 30, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Menu_Icon", ID_ICONVIEW_Menu, 280, 10, 35, 30, 0, 0x00140014, 0 },
  { EDIT_CreateIndirect, "Nm_Value_Text", ID_TEXT_Nm_Value, 75, 65, 110, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Deg_Value_Text", ID_TEXT_Deg_Value, 75, 87, 110, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Num_Value_Text", ID_TEXT_Num_Value, 75, 110, 110, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "N.m_Title_Text", ID_TEXT_Nm_Title, 37, 65, 30, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Deg_Title_Text", ID_TEXT_Deg_Title, 37, 87, 30, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Num_Title_Text", ID_TEXT_Num_Title, 37, 112, 30, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Effect_Show", ID_TEXT_Effect, 250, 160, 40, 40, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetImageById
*/
// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  const void * pData;
  WM_HWIN      hItem;
  U32          FileSize;
  int          NCode;
  int          Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Nm_Deg_Graph'
    //
	
    //
    // Initialization of 'Device_Logo'
    //
	  //设备Logo
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_Logo);
	  ICONVIEW_AddBitmapItem(hItem,&bmLogo_mini_Icon,"");//bmMenu_Info_Icon 帮助图标 bmLogo_mini_Icon 设备图标
	  //链接标识
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Connect);
	  ICONVIEW_AddBitmapItem(hItem,&bmConnect_Icon,"");//
	  //菜单
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Menu);
	  ICONVIEW_AddBitmapItem(hItem,&bmMenu_Icon,"");//
    //
    // Initialization of 'Task_Show_Text'
    //
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Nm_Min);
		TEXT_SetFont(hItem,&GUI_Font10_1);
		TEXT_SetText(hItem,"+3.256");
		TEXT_SetTextColor(hItem,GUI_MAKE_COLOR(0x00FF0080));
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Nm_Max);
		TEXT_SetFont(hItem,&GUI_Font10_1);
		TEXT_SetText(hItem,"-1.256");	
		TEXT_SetTextColor(hItem,GUI_MAKE_COLOR(0x00FF0080));
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Deg_Min);
		TEXT_SetFont(hItem,&GUI_Font10_1);
		TEXT_SetText(hItem,"+1200.89");
		TEXT_SetTextColor(hItem,0x00FF0000);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Deg_Max);
		TEXT_SetFont(hItem,&GUI_Font10_1);
		TEXT_SetText(hItem,"-50.63");
		TEXT_SetTextColor(hItem,0x00FF0000);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
	
//	  strcpy(MONV.Task_Num,"001");
		snprintf(show, sizeof(show), "%s%s%s!", Run, "001",Task);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Task);
		TEXT_SetFont(hItem,&GUI_Fontsong);
		TEXT_SetText(hItem,show);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Time_Show'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Time);
		TEXT_SetFont(hItem,&GUI_Font13B_1);
		TEXT_SetText(hItem,"  17:00 \n 2025-3-6");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Nm_Value_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Nm_Value);
		EDIT_SetFont(hItem,&GUI_Font16B_1);
		EDIT_SetText(hItem, "2.395/2.567");
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem,1, GUI_MAKE_COLOR(0x00FF0080));
    //
    // Initialization of 'Deg_Value_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Deg_Value);
		EDIT_SetFont(hItem,&GUI_Font16B_1);
    EDIT_SetText(hItem, "800.61/1000.12");
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem,1, 0x00FF0000);
    //
    // Initialization of 'Num_Value_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Num_Value);
		EDIT_SetFont(hItem,&GUI_Font16B_1);
    EDIT_SetText(hItem, "777/1000");
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem,1, GUI_MAKE_COLOR(0x0040FF00));
    //
    // Initialization of 'N.m_Title_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Nm_Title);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetText(hItem, Nm_Title);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF0080));
    //
    // Initialization of 'Deg_Title_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Deg_Title);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetText(hItem, Deg_Title);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, 0x00FF0000);
    //
    // Initialization of 'Num_Title_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Num_Title);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetText(hItem, Num_Tiltle);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0040FF00));//GUI_MAKE_COLOR(0x0040FF00)
    //
    // Initialization of 'Effect_Show'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Effect);
		EDIT_SetTextColor(hItem,1, 0x00008000);//0x00008000  0x0000FF00
		EDIT_SetFont(hItem,&GUI_Fontsong);
    EDIT_SetText(hItem, Effect);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	
//	  TEXT_Point();
 
//WM_CreateTimer(WM_GetClientWindow(pMsg->hWin),0,100,0);
//				TEXT_Point(num_x,num_y);
	
//		GRAPH_SCALE_Handle hScaleV_Monitor_V,hScaleV_Monitor_H;
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
//		
//    GRAPH_SetColor(hItem, GUI_WHITE, GRAPH_CI_BK);
//    GRAPH_SetColor(hItem, GUI_BLACK, GRAPH_CI_GRID);
//    GRAPH_SetBorder(hItem, 30, 10, 10, 10);//设置Graph图形左、上、右和下边框
//    GRAPH_SetGridDistX(hItem, 25);
//    GRAPH_SetGridDistY(hItem, 10);
//    GRAPH_SetLineStyleH(hItem, GUI_LS_DOT);
//    GRAPH_SetLineStyleV(hItem, GUI_LS_DOT);
////    GRAPH_SetGridVis(hItem, 1);//设置网格线的可见性
//        /* 创建垂直刻度对象 */
//    hScaleV_Monitor_V = GRAPH_SCALE_Create(20, GUI_TA_HCENTER | GUI_TA_LEFT,
//            GRAPH_SCALE_CF_VERTICAL, 25);
//		GRAPH_SCALE_SetNumDecs(hScaleV_Monitor_V,3);//设置刻度条小数点位数

//    GRAPH_SCALE_SetFactor(hScaleV_Monitor_V, 0.04);
//        /* 创建水平刻度对象 */
//    hScaleV_Monitor_H = GRAPH_SCALE_Create(170, GUI_TA_HCENTER | GUI_TA_LEFT,
//            GRAPH_SCALE_CF_HORIZONTAL, 25);
//		GRAPH_SCALE_SetNumDecs(hScaleV_Monitor_H,1);//设置刻度条小数点位数
//		GRAPH_SCALE_SetFactor(hScaleV_Monitor_H, 0.04);
//		
//    GRAPH_AttachScale(hItem, hScaleV_Monitor_H);
//    GRAPH_AttachScale(hItem, hScaleV_Monitor_V);
//    
//    GRAPH_SCALE_SetTextColor(hScaleV_Monitor_H, 0x00000000);
//    GRAPH_SCALE_SetTextColor(hScaleV_Monitor_V, GUI_MAKE_COLOR(0x00000000));
//		
//		GRAPH_SetVSizeX(hItem,300);
//		GRAPH_SetVSizeY(hItem,300);


			
		
//		GraphdataV=GRAPH_DATA_XY_Create(GUI_RED, 600, 0, 0);
//		GRAPH_AttachData(hItem, GraphdataV);
//		GraphdataH=GRAPH_DATA_XY_Create(GUI_BLUE, 600, 0, 0);
//	  GRAPH_AttachData(hItem, GraphdataH);   
		
		
//		GRAPH_SCALE_SetNumDecs(hItem,1);//设置刻度条小数点位数
//		GRAPH_AttachScale(hScaleV_Monitor_H,hItem);
//		GRAPH_SetAutoScrollbar(hItem,GUI_COORD_X,1);//打开水平滚动条
    
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_ICONVIEW_Connect: // Notifications sent by 'Connect_Show'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				int cnt=0;
				while(cnt<1000000)
				{
				num_x+=1;
			num_y+=0.01;
		 TEXT_Point(num_x,num_y);
					WM_InvalidateWindow(MONV.Monitor);
					cnt++;
				}
		printf("Point Num:%d",cnt);
//		WM_InvalidateWindow(MONV.Monitor);
			long sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Connect Task FreeBytes %ld\n",sssss);
//				WM_RestartTimer(pMsg->Data.v,100);
			
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Menu: // Notifications sent by 'Menu_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				xEventGroupSetBits(EventGroup_prev_Handle,MENU_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
		
		case WM_PAINT:
				
				GUI_SetColor(GUI_BLACK);
				GUI_SetLineStyle(GUI_LS_SOLID);
				GUI_DrawLine(0,45,320,45);
				GUI_SetPenSize(5);
		
        GUI_SetPenSize(1);
		    GUI_SetColor(GUI_BLACK);
				GUI_SetLineStyle(GUI_LS_SOLID);
				GUI_DrawLine(211,0,211,45);
				
		
		    GUI_SetColor(GUI_BLACK);
				GUI_SetLineStyle(GUI_LS_SOLID);
				GUI_DrawLine(89,0,89,45);
		GUI_SetPenSize(5);
		
		DrawAxes();
			DrawTicks(MONV.X_Block,MONV.Y_Block);
//		GUI_MULTIBUF_Begin();
		
//		int num_points = sizeof(points) / sizeof(points[0]);
//		DrawCurve(points,num_points);

			DrawPoints(head);	
			
			
//			GUI_MULTIBUF_End();
				break;
		case WM_TIMER:
			num_x+=100;
			num_y+=0.3;
		 TEXT_Point(num_x,num_y);
		
		
		WM_InvalidateWindow(MONV.Monitor);
		
//		WM_RestartTimer(pMsg->Data.v,100);
		
			break;
	
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateMonitor_Win
*/
WM_HWIN CreateMonitor_Win(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
//	WM_CreateTimer(MONV.Monitor,0,100,0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
