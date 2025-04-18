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
#include "Win_MenuDLG.h"
#include "Win_Manage_SelectDLG.h"
#include "Win_LoginDLG.h"
#include "Win_DispositionDLG.h"
/*BSP 板级配置头文件*/
#include "./Bsp/STemWIN_TOOL/bsp_tool.h"
#include <string.h>
#include <stddef.h>

/*FREERTOS 头文件*/
#include "FreeRTOS.h"
#include "event_groups.h"  
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_Menu    (GUI_ID_USER + 0x0E0)
#define ID_ICONVIEW_Logo    (GUI_ID_USER + 0x0E1)
#define ID_ICONVIEW_Exception    (GUI_ID_USER + 0x0E2)
#define ID_TEXT_Exception_Num    (GUI_ID_USER + 0x0E3)
#define ID_ICONVIEW_User    (GUI_ID_USER + 0x0E4)
#define ID_TEXT_User_Show    (GUI_ID_USER + 0x0E5)
#define ID_ICONVIEW_Term    (GUI_ID_USER + 0x0E6)
#define ID_TEXT_Term_Show    (GUI_ID_USER + 0x0E7)
#define ID_ICONVIEW_Quit    (GUI_ID_USER + 0x0E8)
#define ID_ICONVIEW_Monitor    (GUI_ID_USER + 0x0E9)
#define ID_ICONVIEW_Disposition    (GUI_ID_USER + 0x0EA)
#define ID_TEXT_Monitor    (GUI_ID_USER + 0x0EB)
#define ID_TEXT_Disposition    (GUI_ID_USER + 0x0EC)
#define ID_ICONVIEW_Manage    (GUI_ID_USER + 0x0ED)
#define ID_ICONVIEW_Statisticsv    (GUI_ID_USER + 0x0EE)
#define ID_ICONVIEW_Analyse    (GUI_ID_USER + 0x0EF)
#define ID_TEXT_Manage    (GUI_ID_USER + 0x1E0)
#define ID_TEXT_Statisticsv    (GUI_ID_USER + 0x1E1)
#define ID_TEXT_Analyse    (GUI_ID_USER + 0x1E2)

// USER START (Optionally insert additional static data)
// USER END

extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsong;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontci_cn;
extern WM_HWIN Show_prev,Show_next;

/*名称字符串*/
char Menu_Monitor[]="\xe7\x9b\x91\xe6\x8e\xa7";//监控
char Menu_Disposition[]="\xe9\x85\x8d\xe7\xbd\xae";//配置
char Menu_Manage []="\xe7\xae\xa1\xe7\x90\x86";//管理
char Menu_Statisticsv[]="\xe7\xbb\x9f\xe8\xae\xa1";//统计
char Menu_Analyse[]="\xe5\x88\x86\xe6\x9e\x90";//分析
char Menu_User[]="\xe7\x94\xa8\xe6\x88\xb7";//用户
char Menu_Exception_Num[]="\xe6\xac\xa1";//次

//extern EventGroupHandle_t EventGroup1_Handle;
extern struct Menu_Value MV;
extern struct Login_Value LV;
extern EventGroupHandle_t EventGroup_next_Handle;
extern EventGroupHandle_t EventGroup_prev_Handle;
extern EventGroupHandle_t EventGroup_branch_Handle;//分支子窗口切换
extern GUI_CONST_STORAGE GUI_BITMAP bmAnalyse_Icon;

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Menu_Win", ID_WINDOW_Menu, 0, 0, 320, 240, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Device_Logo_Icon", ID_ICONVIEW_Logo, 5, 10, 30, 35, 0, 0x00140014, 0 },
  { ICONVIEW_CreateIndirect, "Exception_Icon", ID_ICONVIEW_Exception, 42, 10, 38, 33, 0, 0x00140018, 0 },
  { TEXT_CreateIndirect, "Exception_Num_Text", ID_TEXT_Exception_Num, 75, 12, 45, 24, 0, 0x64, 0 },
  { ICONVIEW_CreateIndirect, "User_Icon", ID_ICONVIEW_User, 125, 10, 35, 33, 0, 0x00140014, 0 },
  { TEXT_CreateIndirect, "User_Show_Text", ID_TEXT_User_Show, 152, 12, 40, 24, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Term_ICon", ID_ICONVIEW_Term, 195, 10, 30, 30, 0, 0x00100010, 0 },
  { TEXT_CreateIndirect, "Term_Show_Text", ID_TEXT_Term_Show, 235, 12, 45, 24, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Quit_Icon", ID_ICONVIEW_Quit, 287, 10, 28, 30, 0, 0x00100010, 0 },
  { ICONVIEW_CreateIndirect, "Monitor_Icon", ID_ICONVIEW_Monitor, 65, 60, 62, 48, 0, 0x00200032, 0 },
  { ICONVIEW_CreateIndirect, "Disposition_Icon", ID_ICONVIEW_Disposition, 188, 60,50, 45, 0, 0x00200032, 0 },
  { TEXT_CreateIndirect, "Monitor_Text", ID_TEXT_Monitor, 72, 115, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Disposition_Text", ID_TEXT_Disposition, 195, 115, 50, 20, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Manage_Icon", ID_ICONVIEW_Manage, 21, 145, 50, 48, 0, 0x002D0032, 0 },
  { ICONVIEW_CreateIndirect, "Statisticsv_Icon", ID_ICONVIEW_Statisticsv, 130, 145, 50, 48, 0, 0x002D0032, 0 },
  { ICONVIEW_CreateIndirect, "Analyse_Icon", ID_ICONVIEW_Analyse, 240, 145, 50, 45, 0, 0x002A0032, 0 },
  { TEXT_CreateIndirect, "Manage_Text", ID_TEXT_Manage, 25, 205, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Statisticsv_Text", ID_TEXT_Statisticsv, 135, 205, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Analyse_Text", ID_TEXT_Analyse, 245, 205, 50, 20, 0, 0x0, 0 },
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
  // USER START (Optionally insert additional variables)
  // USER END
  Show_next=pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		
	 
    //
    // Initialization of 'Exception_Num_Text'
    //
	  strcpy(MV.Exception_Num_Text,"2000");
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Exception_Num);
    TEXT_SetText(hItem, Text_Edit_Set(MV.Exception_Num_Text,Menu_Exception_Num));
	  TEXT_SetFont(hItem,&GUI_Fontci_cn);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'User_Show_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_User_Show);
 	  if((LV.User_Text[0]))
	  {TEXT_SetText(hItem,LV.User_Text);}
		else
		{TEXT_SetText(hItem,"li");}
	  TEXT_SetFont(hItem,&GUI_Font13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Term_Show_Text'
    //
	  strcpy(MV.Term_Show_Text,"90000");
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Term_Show);
	  TEXT_SetText(hItem,Text_Edit_Set(MV.Term_Show_Text,"h"));
    TEXT_SetFont(hItem,&GUI_Font13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Monitor_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Monitor);
		TEXT_SetText(hItem,Menu_Monitor);
		TEXT_SetTextColor(hItem,0x00FFFF00);
	  TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Disposition_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Disposition);
		TEXT_SetTextColor(hItem,0xFF800080);
		TEXT_SetText(hItem,Menu_Disposition);
	  TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Manage_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Manage);
		TEXT_SetTextColor(hItem,0x0000A5FF);
		TEXT_SetText(hItem,Menu_Manage);
	  TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Statisticsv_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Statisticsv);
		TEXT_SetTextColor(hItem,0x0000FFFF);
		TEXT_SetText(hItem,Menu_Statisticsv);
	  TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Analyse_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Analyse);
		TEXT_SetTextColor(hItem,0x00808000);
		TEXT_SetText(hItem,Menu_Analyse);
	  TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
		/*图标绘制*/
		
		//设备LOGOy 或 帮助LOGOy
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Logo);
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/Menu_Info_Icon.dta",MV.Logo);
//		ICONVIEW_AddBitmapItem(hItem,&MV.Logo,"");
    ICONVIEW_AddBitmapItem(hItem,&bmLogo_mini_Icon,"");//bmMenu_Info_Icon 帮助图标 bmLogo_mini_Icon 设备图标
		//警告y
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Exception);
		ICONVIEW_AddBitmapItem(hItem,&bmWarning_Icon,"");
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/Exception_Icon.dta",MV.Exception);
//		ICONVIEW_AddBitmapItem(hItem,&MV.Exception,"");
		//用户y
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_User);
		ICONVIEW_AddBitmapItem(hItem,&bmUser_Icon,"");
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/User_Icon.dta",MV.User);
//		ICONVIEW_AddBitmapItem(hItem,&MV.User,"");
		//工作时间y
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Term);
		ICONVIEW_AddBitmapItem(hItem,&bmWorking_Time_Icon,"");
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/Term_Icon.dta",MV.Term);
//		ICONVIEW_AddBitmapItem(hItem,&MV.Term,"");
		//退出y
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Quit);
		ICONVIEW_AddBitmapItem(hItem,&bmQuit_Icon,"");
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/Quit_Icon.dta",MV.Quit);
//		ICONVIEW_AddBitmapItem(hItem,&MV.Quit,"");
		//监控y
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Monitor );
		ICONVIEW_AddBitmapItem(hItem,&bmMonitor_Icon,"");
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/Monitor_Icon.dta",MV.Monitor);
//		ICONVIEW_AddBitmapItem(hItem,&MV.Monitor,"");
		//配置y
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Disposition );
		ICONVIEW_AddBitmapItem(hItem,&bmDesprition_Icon,"");
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/Disposition_Icon.dta",MV.Disposition);
//		ICONVIEW_AddBitmapItem(hItem,&MV.Disposition,"");
		//管理y
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Manage );
		ICONVIEW_AddBitmapItem(hItem,&bmSetting_Icon,"");
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/Manage_Icon.dta",MV.Manage);
//		ICONVIEW_AddBitmapItem(hItem,&MV.Manage,"");
		//统计y
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Statisticsv );
		ICONVIEW_AddBitmapItem(hItem,&bmStatistics_Icon,"");
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/Statistics_Icon.dta",MV.Statisticsv);
//		ICONVIEW_AddBitmapItem(hItem,&MV.Statisticsv,"");
		//分析y
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Analyse  );
		ICONVIEW_AddBitmapItem(hItem,&bmAnalyse_Icon,"");
//		LoadBMP_UsingMEMDEV("0:/icon/Win_MenuDLG/Analyse_Icon.dta",MV.Analyse);
//		ICONVIEW_AddBitmapItem(hItem,&MV.Analyse,"");

			long sss=GUI_ALLOC_GetNumUsedBytes ();
			  printf("Menu %ld\n",sss);
		
		 long ssss=GUI_ALLOC_GetNumFreeBytes ();
			  printf("Init 2 Task FreeBytes %ld\n",ssss);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_ICONVIEW_Logo: // Notifications sent by 'Device_Logo_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				xEventGroupSetBits(EventGroup_next_Handle,MENU_CHILD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Exception: // Notifications sent by 'Exception_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_User: // Notifications sent by 'User_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Term: // Notifications sent by 'Term_ICon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Quit : // Notifications sent by 'Quit_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				xEventGroupSetBits(EventGroup_prev_Handle,LOGIN_EVENT);

//				GUI_EndDialog(pMsg->hWin, 0);//结束数字小键盘
//			WM_DeleteWindow(pMsg->hWin);
//			CreateLogin();
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Monitor: // Notifications sent by 'Monitor_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					xEventGroupSetBits(EventGroup_next_Handle,MONITOR_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_ICONVIEW_Disposition: // Notifications sent by 'Disposition_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				xEventGroupSetBits(EventGroup_next_Handle,DISPOSITION_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_ICONVIEW_Manage: // Notifications sent by 'Manage_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
//		 
			xEventGroupSetBits(EventGroup_next_Handle,MANAGE_SELECT_EVENT);
//			 GUI_EndDialog(pMsg->hWin, 0);//结束主页面
////			WM_DeleteWindow(WM_GetClientWindow(pMsg->hWin));
//			Show_prev=pMsg->hWin;
//			Show_next=CreateManage_Select_Win();
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Statisticsv: // Notifications sent by 'Statisticsv_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SCROLL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_ICONVIEW_Analyse: // Notifications sent by 'Analyse_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SCROLL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
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
*       CreateMenu_Win
*/
WM_HWIN CreateMenu_Win(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
