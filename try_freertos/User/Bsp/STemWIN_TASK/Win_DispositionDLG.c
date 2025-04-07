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


/*EMWIN 头文件*/
#include "DIALOG.h"
#include "Win_DispositionDLG.h"
#include "bsp_keyboard.h"
#include "Win_LoginDLG.h"
/*BSP 板级配置头文件*/
#include "bsp_tool.h"
#include <string.h>
#include <stddef.h>

/*FREERTOS 头文件*/
#include "FreeRTOS.h"
#include "event_groups.h"



WM_HWIN hItem_slave;//上个界面被点击编辑框的句柄保存
//           
WM_HWIN draw,QR;
//WM_HWIN hItem_bottle;//总瓶数Text控件句柄
//WM_HWIN hItem_capacity;//灌装量Text控件句柄

/*软键盘数据变量*/
char Edit_buff[10];//数字键盘显示当前输入字符串
char bottle_buff[10];//备份输入字符串
char capacity_buff[10];//备份输入字符串
char Input_Num[10];//数字键盘输入字符
char User_num[10];
/*数据对象句柄*/
GRAPH_DATA_Handle GraphdataV,GraphdataH;


/*图片对象句柄*/
//extern GUI_CONST_STORAGE GUI_BITMAP bmAnalyse_Icon;

/*emwin显示封装函数共用参数*/
extern WM_HMEM hMem;
extern GUI_MEMDEV_Handle hBMP;
extern GUI_BITMAP ac;
extern char *_acbuffer;

/* 事件句柄 */
extern EventGroupHandle_t EventGroup_next_Handle;
extern EventGroupHandle_t EventGroup_prev_Handle;

/*界面结构体*/
extern struct Disposition_Value DISPV;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsong;
/*字符串*/
char Signal_Step[]="\xe5\x8d\x95\xe6\xad\xa5";//单步
char Muti_Step[]="\xe5\xa4\x9a\xe6\xad\xa5";//多步
char Task_Select[20];//任务？？？单/多步
extern char Task[];//任务
extern WM_HWIN hItem_slave;//上个界面被点击编辑框的句柄保存
struct Disposition_Value DISP_SHOW;
struct Disposition_Value DISP_SHOW = {
    .Nm_Max = "3.256",
    .Nm_Min = "1.256",
    .Nm_Target = "2.467",
    .Deg_Max = "1200.89",
    .Deg_Min = "500.63",
    .Deg_Target = "900.99",
    .Task_Sel = 1
};
/*********************************************************************
*
*       Defines
*
**********************************************************************

*/
#define ID_WINDOW_0     					(GUI_ID_USER + 0x09)
#define ID_EDIT_Deg_Target     		(GUI_ID_USER + 0x0B)
#define ID_EDIT_Nm_Max     				(GUI_ID_USER + 0x0C)
#define ID_EDIT_Nm_Target    			(GUI_ID_USER + 0x0D)
#define ID_EDIT_Nm_Min     				(GUI_ID_USER + 0x0E)
#define ID_ICONVIEW_Nm_Max     		(GUI_ID_USER + 0x0F)
#define ID_ICONVIEW_Nm_Target     (GUI_ID_USER + 0x10)
#define ID_ICONVIEW_Deg_Target    (GUI_ID_USER + 0x11)
#define ID_ICONVIEW_Nm_Min    		(GUI_ID_USER + 0x12)
#define ID_ICONVIEW_Menu    			(GUI_ID_USER + 0x14)
#define ID_ICONVIEW_Task_Left     (GUI_ID_USER + 0x15)
#define ID_TEXT_Task_Select     	(GUI_ID_USER + 0x16)
#define ID_ICONVIEW_Task_Right    (GUI_ID_USER + 0x17)
#define ID_ICONVIEW_Task_Add      (GUI_ID_USER + 0x18)
#define ID_ICONVIEW_Task_Delete   (GUI_ID_USER + 0x19)
#define ID_EDIT_Deg_Max     			(GUI_ID_USER + 0x1A)
#define ID_ICONVIEW_Deg_Max     	(GUI_ID_USER + 0x1B)
#define ID_EDIT_Deg_Min     			(GUI_ID_USER + 0x1C)
#define ID_ICONVIEW_Deg_Min     	(GUI_ID_USER + 0x1D)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Win_Disposition", ID_WINDOW_0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x0, 0 },
  { TEXT_CreateIndirect, "Deg_Target_Edit", ID_EDIT_Deg_Target, 130, 225, 75, 18, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Nm_Max_Edit", ID_EDIT_Nm_Max, 255, 60, 55, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Nm_Target_Edit", ID_EDIT_Nm_Target, 255, 120, 55, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Nm_Min_Edit", ID_EDIT_Nm_Min, 260, 170, 55, 20, 0, 0x64, 0 },
  { ICONVIEW_CreateIndirect, "Nm_Max_Icon", ID_ICONVIEW_Nm_Max, 245, 60, 14, 16, 0, 0x000A000B, 0 },
  { ICONVIEW_CreateIndirect, "Nm_Target_Icon", ID_ICONVIEW_Nm_Target, 240, 115, 18, 20, 0, 0x00100010, 0 },
  { ICONVIEW_CreateIndirect, "Deg_Target_Icon", ID_ICONVIEW_Deg_Target, 105, 222, 15, 20, 0, 0x000D0005, 0 },
  { ICONVIEW_CreateIndirect, "Nm_Min_Icon", ID_ICONVIEW_Nm_Min, 240, 170, 18, 18, 0, 0x0010000D, 0 },
  { ICONVIEW_CreateIndirect, "Menu_Icon", ID_ICONVIEW_Menu, 5, 8, 33, 25, 0, 0x00140014, 0 },
  { ICONVIEW_CreateIndirect, "Task_Left_Icon", ID_ICONVIEW_Task_Left, 45, 8, 25, 25, 0, 0x00140014, 0 },
  { TEXT_CreateIndirect, "Task_Select_Text", ID_TEXT_Task_Select, 92, 12, 110, 22, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Task_Right_Icon", ID_ICONVIEW_Task_Right, 215, 8, 20, 25, 0, 0x00100010, 0 },
  { ICONVIEW_CreateIndirect, "Task_Add_Icon", ID_ICONVIEW_Task_Add, 245, 8, 33, 27, 0, 0x0010001C, 0 },
  { ICONVIEW_CreateIndirect, "Task_Delete_Icon", ID_ICONVIEW_Task_Delete, 285, 6, 40, 35, 0, 0x00140014, 0 },
  { TEXT_CreateIndirect, "Deg_Max_Edit", ID_EDIT_Deg_Max, 165, 65, 55, 20, 20, 0x00140014, 0 },
  { ICONVIEW_CreateIndirect, "Deg_Max_Icon", ID_ICONVIEW_Deg_Max, 150, 65, 12, 15, 0, 0x000A000A, 0 },
  { TEXT_CreateIndirect, "Deg_Min_Edit", ID_EDIT_Deg_Min, 63, 65, 55, 20, 0, 0x64, 0 },
  { ICONVIEW_CreateIndirect, "Deg_Min_Icon", ID_ICONVIEW_Deg_Min, 50, 65, 15, 15, 0, 0x000A000D, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	
	DISPV.flag_Deg=0;
	DISPV.flag_Nm=1;
  // USER START (Optionally insert additional variables)
  // USER END
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Deg_Target_Edit'
    //
	  if(!atof(DISPV.Deg_Target))
		{
	  snprintf(DISPV.Deg_Target, sizeof(DISPV.Deg_Target), "%s", "900.99");
		}
		else
		{
			snprintf(DISPV.Deg_Target, sizeof(DISPV.Deg_Target), "%s", DISPV.Deg_Target);
		}
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Target);
	  DISPV.DegTarget=hItem;
	  Move_Figure(hItem,WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Target),DISPV.flag_Deg,atof(DISPV.Deg_Target));
	  TEXT_SetText(hItem, DISPV.Deg_Target);
	  TEXT_SetBkColor(hItem,0x00FFFFFF);
	  TEXT_SetFont(hItem,&GUI_Font13B_1);
	  TEXT_SetTextColor(hItem, 0x00FF0000);
	  WIDGET_SetEffect(hItem,&WIDGET_Effect_None);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Nm_Max_Edit'
    //
		if(!atof(DISPV.Nm_Max))
		{
	  snprintf(DISPV.Nm_Max, sizeof(DISPV.Nm_Max), "%s", "3.256");
		}
		else
		{
		snprintf(DISPV.Nm_Max, sizeof(DISPV.Nm_Max), "%s",DISPV.Nm_Max);
		}
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Nm_Max);
    DISPV.NmMax=hItem;
		Move_Figure(hItem,WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Max),DISPV.flag_Nm,atof(DISPV.Nm_Max));
    TEXT_SetText(hItem,DISPV.Nm_Max);
	  TEXT_SetFont(hItem,&GUI_Font13B_1);
	  TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF0080));
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Nm_Target_Edit'
    //
		if(!atof(DISPV.Nm_Target))
		{
		snprintf(DISPV.Nm_Target, sizeof(DISPV.Nm_Target), "%s", "2.467");
		}
		else
		{
			snprintf(DISPV.Nm_Target, sizeof(DISPV.Nm_Target), "%s", DISPV.Nm_Target);
		}
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Nm_Target);
		DISPV.NmTarget=hItem;
		Move_Figure(hItem,WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Target),DISPV.flag_Nm,atof(DISPV.Nm_Target));
    TEXT_SetText(hItem, DISPV.Nm_Target);
   	TEXT_SetFont(hItem,&GUI_Font13B_1);
	  TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF0080));
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Nm_Min_Edit'
    //
		if(!atof(DISPV.Nm_Min))
		{
		snprintf(DISPV.Nm_Min, sizeof(DISPV.Nm_Min), "%s", "1.256");
		}
		else
		{
		snprintf(DISPV.Nm_Min, sizeof(DISPV.Nm_Min), "%s", DISPV.Nm_Min);
		}
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Nm_Min);
		DISPV.NmMin=hItem;
		Move_Figure(hItem,WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Min),DISPV.flag_Nm,atof(DISPV.Nm_Min));
    TEXT_SetText(hItem, DISPV.Nm_Min);
		TEXT_SetFont(hItem,&GUI_Font13B_1);
		TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF0080));
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Task_Select_Text'
    //
		snprintf(Task_Select, sizeof(Task_Select), "%s%d%s", Task, DISPV.Task_Sel,Muti_Step);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Task_Select);
		TEXT_SetText(hItem, Task_Select);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Deg_Max_Edit'
    //
		if(!atof(DISPV.Deg_Max))
		{
		snprintf(DISPV.Deg_Max, sizeof(DISPV.Deg_Max), "%s", "1200.89");
		}
		else
		{
		snprintf(DISPV.Deg_Max, sizeof(DISPV.Deg_Max), "%s", DISPV.Deg_Max);
		}
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Max);
		DISPV.DegMax=hItem;
		Move_Figure(hItem,WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Max),DISPV.flag_Deg,atof(DISPV.Deg_Max));
    TEXT_SetText(hItem, DISPV.Deg_Max);
		TEXT_SetFont(hItem,&GUI_Font13B_1);
		TEXT_SetTextColor(hItem, 0x00FF0000);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Deg_Min_Edit'
    //
		if(!atof(DISPV.Deg_Min))
		{
		snprintf(DISPV.Deg_Min, sizeof(DISPV.Deg_Min), "%s", "500.63");
		}
		else
		{
		snprintf(DISPV.Deg_Min, sizeof(DISPV.Deg_Min), "%s", DISPV.Deg_Min);
		}
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Min);
		DISPV.DegMin=hItem;
		Move_Figure(hItem,WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Min),DISPV.flag_Deg,atof(DISPV.Deg_Min));
    TEXT_SetText(hItem, DISPV.Deg_Min);
		TEXT_SetFont(hItem,&GUI_Font13B_1);
    TEXT_SetTextColor(hItem, 0x00FF0000);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
		//扭矩MAX
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Max);
	  ICONVIEW_AddBitmapItem(hItem,&bmNm_Max_Icon,"");//
		//扭矩MIN
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Min);
	  ICONVIEW_AddBitmapItem(hItem,&bmNm_Min_Icon,"");//
		//扭矩目标
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Target);
	  ICONVIEW_AddBitmapItem(hItem,&bmNm_Target_Icon,"");//
		//菜单
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Menu);
	  ICONVIEW_AddBitmapItem(hItem,&bmMenu_Icon,"");//
		//向前切换
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Task_Left);
	  ICONVIEW_AddBitmapItem(hItem,&bmTask_Prev_Icon,"");//
    //向后切换
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Task_Right);
	  ICONVIEW_AddBitmapItem(hItem,&bmTask_Next_Icon,"");//
		//添加任务
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Task_Add);
	  ICONVIEW_AddBitmapItem(hItem,&bmAdd_Icon,"");//
		//删除任务
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Task_Delete);
	  ICONVIEW_AddBitmapItem(hItem,&bmDel_Icon,"");//
		//角度MAX
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Max);
	  ICONVIEW_AddBitmapItem(hItem,&bmDeg_Max_Icon,"");//
		//角度MIN
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Min);
	  ICONVIEW_AddBitmapItem(hItem,&bmDeg_Min_Icon,"");//
		//角度目标
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Target);
	  ICONVIEW_AddBitmapItem(hItem,&bmDeg_Target_Icon,"");//
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Target),WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Target),DISPV.flag_Deg,atof(DISPV.Deg_Target));
//		Move_All();
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Target),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Target),DISPV.flag_Deg,atof(DISPV.Deg_Target));
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Max),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Max),DISPV.flag_Deg,atof(DISPV.Deg_Max));
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Min),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Min),DISPV.flag_Deg,atof(DISPV.Deg_Min));
//		
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Nm_Max),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Max),DISPV.flag_Nm,atof(DISPV.Nm_Max));
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Nm_Target),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Target),DISPV.flag_Nm,atof(DISPV.Nm_Target));
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Nm_Min),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Min),DISPV.flag_Nm,atof(DISPV.Nm_Min));
		
		
//		GRAPH_SCALE_SetNumDecs(hItem,1);//设置刻度条小数点位数
//		GRAPH_AttachScale(hScaleV_Monitor_H,hItem);
//		GRAPH_SetAutoScrollbar(hItem,GUI_COORD_X,1);//打开水平滚动条

		
		/* 加载BMP图片数据到内存设备 */
//	  LoadBMP_UsingMEMDEV("0:/icon/Analyse_Icon.bmp");
//      ShowPNG("0:/icon/Analyse_Icon.c",240,170);

//		hItem=WM_GetDialogItem(pMsg->hWin,ID_ICONVIEW_2);
//	  LoadBMP_UsingMEMDEV("0:/icon/Add_Icon.dta",hItem);//Analyse_Icon.dta
////		ICONVIEW_AddBitmapItem(hItem,&ac,"");
////      ShowPNGEx("0:/icon/Add_Icon.bmp",hItem);
//		
//		hItem=WM_GetDialogItem(pMsg->hWin,ID_ICONVIEW_3);
//		LoadBMP_UsingMEMDEV("0:/icon/Analyse_Icon.dta",hItem);//Add_Icon.dta
////		ICONVIEW_AddBitmapItem(hItem,&ac,"");
//		 ShowPNGEx("0:/icon/Analyse_Icon.bmp",hItem);

//		ICONVIEW_AddBitmapItem(hItem,&bmAnalyse_Icon,"");
//		ICONVIEW_AddStreamedBitmapItem(hItem,(const void *)hBMP,"");
//		ICONVIEW_SetTextColor(hItem,ICONVIEW_CI_SEL, GUI_WHITE);
		
		
//		/* 设置初始选择的图标，-1表示不选中任何图标 */
//	ICONVIEW_SetSel(hItem, 0);
//	/* 设置图标标题的字体 */
//	ICONVIEW_SetFont(hItem, GUI_FONT_16B_1);
//	/* 设置标题的对齐方式 */
//	ICONVIEW_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_BOTTOM);
//	/* 设置图标被选中时的背景色 */
//	ICONVIEW_SetBkColor(hItem, ICONVIEW_CI_SEL, GUI_MAKE_COLOR(0x40ul << 24 | 0xFFFFFF));
//	/* 设置图标的对齐方式 */
//	ICONVIEW_SetIconAlign(hItem, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
//	/* 设置图标在X和Y方向上的间距 */
//	ICONVIEW_SetSpace(hItem, GUI_COORD_X, 70);
//	ICONVIEW_SetSpace(hItem, GUI_COORD_Y, 50);
	
	/* 将背景图片从内存设备写入LCD */
//	GUI_MEMDEV_WriteOpaqueAt(hBMP, 0, 0);
	
		
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END                                                             
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_EDIT_Deg_Target: // Notifications sent by 'Deg_Target_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.DegTarget;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_EDIT_Nm_Max: // Notifications sent by 'Nm_Max_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.NmMax;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_Nm_Target: // Notifications sent by 'Nm_Target_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
			  
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
			hItem_slave =	DISPV.NmTarget;
			xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_Nm_Min: // Notifications sent by 'Nm_Min_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
			  
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.NmMin;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
//      case WM_NOTIFICATION_VALUE_CHANGED:
//			 EDIT_SetText(hItem,Edit_buff);
//			 EDIT_SetFont(hItem,&GUI_Font13B_ASCII);
//			 EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x00000000));
//       EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        // USER START (Optionally insert code for reacting on notification message)
      }
      break;
    case ID_ICONVIEW_Nm_Max: // Notifications sent by 'Nm_Max_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.NmMax;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Nm_Target: // Notifications sent by 'Nm_Target_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.NmTarget;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Deg_Target: // Notifications sent by 'Deg_Target_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.DegTarget;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Nm_Min: // Notifications sent by 'Nm_Min_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.NmMin;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_ICONVIEW_Menu: // Notifications sent by 'Menu_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
//				Task_Free();
			
				xEventGroupSetBits(EventGroup_prev_Handle,MENU_EVENT);
			
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Task_Left: // Notifications sent by 'Task_Left_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				if(DISP_SHOW.Task_Sel-1>0){
				DISP_SHOW.Task_Sel--;
				Task_Update(&DISP_SHOW,DISP_SHOW.Task_Sel,1);//DISPV.Task_Sel-1
		  	DISPV.hTimer=WM_CreateTimer(DISPV.Disposition,0,100,0);
		   	}
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Task_Right: // Notifications sent by 'Task_Right_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				if(DISP_SHOW.Task_Sel+1<=DISPV.Task_Sel){
				DISP_SHOW.Task_Sel++;
				Task_Update(&DISP_SHOW,DISP_SHOW.Task_Sel,0);//DISPV.Task_Sel
//					Update_All();
			  DISPV.hTimer=WM_CreateTimer(DISPV.Disposition,0,100,0);
				}
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Task_Add: // Notifications sent by 'Task_Add_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				DISPV.Task_Sel++;
			  DISP_SHOW.Task_Sel=DISPV.Task_Sel;
				Add_Task(DISPV);
			long sssss=xPortGetFreeHeapSize ();
			printf("Init Connect Task FreeBytes %ld\n",sssss);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_ICONVIEW_Task_Delete: // Notifications sent by 'Task_Delete_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				Test_del(DISPV.Task_Sel);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_Deg_Max: // Notifications sent by 'Deg_Max_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.DegMax;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Deg_Max: // Notifications sent by 'Deg_Max_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_EDIT_Deg_Min: // Notifications sent by 'Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.DegMin;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Deg_Min: // Notifications sent by 'Deg_Min_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=DISPV.DegMin;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
			
		case ID_TEXT_Task_Select: // Notifications sent by 'Deg_Max_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
				xEventGroupSetBits(EventGroup_next_Handle,DISPOSITION_CHILD_EVENT);
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
				GUI_DrawLine(200,0,200,45);
				
		
		    GUI_SetColor(GUI_BLACK);
				GUI_SetLineStyle(GUI_LS_SOLID);
				GUI_DrawLine(89,0,89,45);
		GUI_SetPenSize(5);
		
		DrawAxes();
			DrawTicks(DISPV.X_Block,DISPV.Y_Block);
		Move_All();
//		Update_All();
//		Move_Al();
			break;
		case WM_TIMER:
//	strcpy(DISPV.Nm_Max,DISP_SHOW.Nm_Max);
//	strcpy(DISPV.Nm_Min,DISP_SHOW.Nm_Min);
//	strcpy(DISPV.Nm_Target,DISP_SHOW.Nm_Target);
//	strcpy(DISPV.Deg_Max,DISP_SHOW.Deg_Max);
//	strcpy(DISPV.Deg_Min,DISP_SHOW.Deg_Min);
//	strcpy(DISPV.Deg_Target,DISP_SHOW.Deg_Target);
//		
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Target),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Target),DISPV.flag_Deg,atof(DISPV.Deg_Target));
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Max),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Max),DISPV.flag_Deg,atof(DISPV.Deg_Max));
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Deg_Min),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Deg_Min),DISPV.flag_Deg,atof(DISPV.Deg_Min));
//		
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Nm_Max),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Max),DISPV.flag_Nm,atof(DISPV.Nm_Max));
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Nm_Target),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Target),DISPV.flag_Nm,atof(DISPV.Nm_Target));
//		Move_Figure(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Nm_Min),WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Nm_Min),DISPV.flag_Nm,atof(DISPV.Nm_Min));
//		
//			WM_InvalidateWindow(DISPV.Disposition);
			Update_All();
		
////		Move_All();
////		WM_DeleteTimer(DISPV.Disposition);
//			WM_RestartTimer(DISPV.hTimer,100);
			break;
		case WM_DELETE:
			WM_DeleteTimer(DISPV.hTimer);
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
*       CreateWin_Disposition
*/

void Move_All(void)
{
			
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Target),DISPV.flag_Deg,atof(DISPV.Deg_Target));
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Max),DISPV.flag_Deg,atof(DISPV.Deg_Max));
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Min),DISPV.flag_Deg,atof(DISPV.Deg_Min));
		
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Nm_Max),DISPV.flag_Nm,atof(DISPV.Nm_Max));
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Nm_Target),DISPV.flag_Nm,atof(DISPV.Nm_Target));
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Nm_Min),DISPV.flag_Nm,atof(DISPV.Nm_Min));
	
//		WM_InvalidateWindow(DISPV.Disposition); // 触发重绘
}

void Move_Al(void)
{
			
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Target),DISPV.flag_Deg,atof(DISP_SHOW.Deg_Target));
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Max),DISPV.flag_Deg,atof(DISP_SHOW.Deg_Max));
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Min),DISPV.flag_Deg,atof(DISP_SHOW.Deg_Min));
		
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Nm_Max),DISPV.flag_Nm,atof(DISP_SHOW.Nm_Max));
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Nm_Target),DISPV.flag_Nm,atof(DISP_SHOW.Nm_Target));
		Move_Line(WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Nm_Min),DISPV.flag_Nm,atof(DISP_SHOW.Nm_Min));
		
}

void Update_All()
{
	
	
	
	TEXT_SetFont(DISPV.DegMax,&GUI_Font13B_1);
	TEXT_SetText(DISPV.DegMax,DISP_SHOW.Deg_Max);
	TEXT_SetFont(DISPV.DegMin,&GUI_Font13B_1);
	TEXT_SetText(DISPV.DegMin,DISP_SHOW.Deg_Min);
	TEXT_SetFont(DISPV.DegTarget,&GUI_Font13B_1);
	TEXT_SetText(DISPV.DegTarget,DISP_SHOW.Deg_Target);
	
	TEXT_SetFont(DISPV.NmMax,&GUI_Font13B_1);
	TEXT_SetText(DISPV.NmMax,DISP_SHOW.Nm_Max);
	TEXT_SetFont(DISPV.NmMin,&GUI_Font13B_1);
	TEXT_SetText(DISPV.NmMin,DISP_SHOW.Nm_Min);
	
	TEXT_SetFont(DISPV.NmTarget,&GUI_Font13B_1);
	TEXT_SetText(DISPV.NmTarget,DISP_SHOW.Nm_Target);
	TEXT_SetTextColor(DISPV.NmTarget, GUI_MAKE_COLOR(0x00FF0080));
  TEXT_SetTextAlign(DISPV.NmTarget, GUI_TA_HCENTER | GUI_TA_VCENTER);
//	Move_Figure(DISPV.NmTarget,WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Target),DISPV.flag_Nm,atof(DISP_SHOW.Nm_Target));
	
	TEXT_SetFont(WM_GetDialogItem(DISPV.Disposition, ID_TEXT_Task_Select),&GUI_Fontsong);
	snprintf(Task_Select, sizeof(Task_Select), "%s%d%s", Task, DISP_SHOW.Task_Sel,Muti_Step);
	TEXT_SetText(WM_GetDialogItem(DISPV.Disposition, ID_TEXT_Task_Select),Task_Select);

strcpy(DISPV.Nm_Max,DISP_SHOW.Nm_Max);
	strcpy(DISPV.Nm_Min,DISP_SHOW.Nm_Min);
	strcpy(DISPV.Nm_Target,DISP_SHOW.Nm_Target);
	strcpy(DISPV.Deg_Max,DISP_SHOW.Deg_Max);
	strcpy(DISPV.Deg_Min,DISP_SHOW.Deg_Min);
	strcpy(DISPV.Deg_Target,DISP_SHOW.Deg_Target);
		
		Move_Figure(WM_GetDialogItem(DISPV.Disposition, ID_EDIT_Deg_Target),WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Target),DISPV.flag_Deg,atof(DISPV.Deg_Target));
		Move_Figure(WM_GetDialogItem(DISPV.Disposition, ID_EDIT_Deg_Max),WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Max),DISPV.flag_Deg,atof(DISPV.Deg_Max));
		Move_Figure(WM_GetDialogItem(DISPV.Disposition, ID_EDIT_Deg_Min),WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Deg_Min),DISPV.flag_Deg,atof(DISPV.Deg_Min));
		
		Move_Figure(WM_GetDialogItem(DISPV.Disposition, ID_EDIT_Nm_Max),WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Nm_Max),DISPV.flag_Nm,atof(DISPV.Nm_Max));
		Move_Figure(WM_GetDialogItem(DISPV.Disposition, ID_EDIT_Nm_Target),WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Nm_Target),DISPV.flag_Nm,atof(DISPV.Nm_Target));
		Move_Figure(WM_GetDialogItem(DISPV.Disposition, ID_EDIT_Nm_Min),WM_GetDialogItem(DISPV.Disposition, ID_ICONVIEW_Nm_Min),DISPV.flag_Nm,atof(DISPV.Nm_Min));
		
			WM_InvalidateWindow(DISPV.Disposition);
}


WM_HWIN CreateWin_Disposition(void);
WM_HWIN CreateWin_Disposition(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
//	WM_CreateTimer(DISPV.Disposition,0,100,0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/