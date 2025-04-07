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
#include "bsp_tool.h"
#include "Win_MenuDLG.h"
#include "Win_Manage_SelectDLG.h"
#include "Win_System_SetDLG.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_System_Set     (GUI_ID_USER + 0x00)
#define ID_IMAGE_Menu     (GUI_ID_USER + 0x01)
#define ID_TEXT_System_Set     (GUI_ID_USER + 0x02)
#define ID_IMAGE_Return     (GUI_ID_USER + 0x03)
#define ID_IMAGE_Connect     (GUI_ID_USER + 0x04)
#define ID_IMAGE_Time     (GUI_ID_USER + 0x05)
#define ID_IMAGE_Calibrate     (GUI_ID_USER + 0x06)
#define ID_IMAGE_Reposition     (GUI_ID_USER + 0x07)
#define ID_TEXT_Connect     (GUI_ID_USER + 0x08)
#define ID_TEXT_Time     (GUI_ID_USER + 0x09)
#define ID_TEXT_Calibrate     (GUI_ID_USER + 0x0A)
#define ID_TEXT_Reposition     (GUI_ID_USER + 0x0B)

// USER START (Optionally insert additional defines)
// USER END


// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "System_Set_Win", ID_WINDOW_System_Set, 0, 0, 320, 240, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Menu_Image", ID_IMAGE_Menu, 12, 10, 24, 24, 0, 0x00140014, 0 },
  { TEXT_CreateIndirect, "System_Set_Text", ID_TEXT_System_Set, 128, 12, 60, 20, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Return_Image", ID_IMAGE_Return, 283, 9, 24, 24, 0, 0x00140014, 0 },
  { ICONVIEW_CreateIndirect, "Connect_Image", ID_IMAGE_Connect, 57, 44, 50, 50, 0, 0x00320032, 0 },
  { ICONVIEW_CreateIndirect, "Time_Image", ID_IMAGE_Time, 205, 45, 50, 50, 0, 0x00140014, 0 },
  { ICONVIEW_CreateIndirect, "Calibrate_Image", ID_IMAGE_Calibrate, 57, 150, 50, 50, 0, 0x00140014, 0 },
  { ICONVIEW_CreateIndirect, "Reposition_Image", ID_IMAGE_Reposition, 205, 150, 50, 50, 0, 0x00140014, 0 },
  { TEXT_CreateIndirect, "Connect_Text", ID_TEXT_Connect, 57, 113, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Time_Text", ID_TEXT_Time, 205, 109, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Calibrate_Text", ID_TEXT_Calibrate, 57, 210, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Reposition_Text", ID_TEXT_Reposition, 205, 210, 50, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};


extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsong;
extern WM_HWIN Show_prev,Show_next;

char System_Set_Title[]="\xe7\xb3\xbb\xe7\xbb\x9f\xe8\xae\xbe\xe7\xbd\xae";//系统设置
char System_Set_Connect[]="\xe7\xbd\x91\xe7\xbb\x9c\xe8\xae\xbe\xe7\xbd\xae";//网络设置
char System_Set_Time[]="\xe6\x97\xa5\xe6\x9c\x9f\xe4\xbf\xae\xe6\x94\xb9";//日期修改
char System_Set_Calibrate[]="\xe7\xb3\xbb\xe6\x95\xb0\xe6\xa0\xa1\xe5\x87\x86";//系数校准
char System_Set_Reposition[]="\xe7\xb3\xbb\xe7\xbb\x9f\xe5\xa4\x8d\xe4\xbd\x8d";//系统复位
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
  const void * pData;
  WM_HWIN      hItem;
  U32          FileSize;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END
  Show_next=pMsg->hWin;
	
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Menu_Image'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_Menu);
	  LoadBMP_UsingMEMDEV("0:/icon/Win_System_SetDLG/Menu_Icon.dta",hItem);
    //
    // Initialization of 'System_Set_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_System_Set);
	  TEXT_SetFont(hItem,&GUI_Fontsong);
	  TEXT_SetText(hItem,System_Set_Title);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Return_Image'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_Return);
		LoadBMP_UsingMEMDEV("0:/icon/Win_System_SetDLG/Return_Icon.dta",hItem);
    //
    // Initialization of 'Connect_Image'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_Connect);
		LoadBMP_UsingMEMDEV("0:/icon/Win_System_SetDLG/Link_Icon.dta",hItem);
    //
    // Initialization of 'Time_Image'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_Time);
   LoadBMP_UsingMEMDEV("0:/icon/Win_System_SetDLG/System_Calender_Icon.dta",hItem);

    //
    // Initialization of 'Calibrate_Image'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_Calibrate);
    LoadBMP_UsingMEMDEV("0:/icon/Win_System_SetDLG/System_Calibrate_Icon.dta",hItem);
    //
    // Initialization of 'Reposition_Image'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_Reposition);
    LoadBMP_UsingMEMDEV("0:/icon/Win_System_SetDLG/System_Repositon_Icon.dta",hItem);
    // Initialization of 'Connect_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Connect);
		TEXT_SetFont(hItem,&GUI_Fontsong);
	  TEXT_SetText(hItem,System_Set_Connect);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Time_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Time);
		TEXT_SetFont(hItem,&GUI_Fontsong);
	  TEXT_SetText(hItem,System_Set_Time);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Calibrate_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Calibrate);
		TEXT_SetFont(hItem,&GUI_Fontsong);
	  TEXT_SetText(hItem,System_Set_Calibrate);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Reposition_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Reposition);
		TEXT_SetFont(hItem,&GUI_Fontsong);
	  TEXT_SetText(hItem,System_Set_Reposition);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
		long ssss=GUI_ALLOC_GetNumFreeBytes ();
			  printf("%ld\n",ssss);
		
		
	 case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
			
		case ID_IMAGE_Menu: // Notifications sent by 'ID_IMAGE_Menu'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				Show_prev=pMsg->hWin;
//			  GUI_EndDialog(pMsg->hWin, 0);//结束登录页面
			  CreateMenu_Win();
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
			
		case ID_IMAGE_Return: // Notifications sent by 'ID_IMAGE_Return'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Show_prev=pMsg->hWin;
//			  GUI_EndDialog(pMsg->hWin, 0);//结束登录页面
			  CreateManage_Select_Win();
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
			
    case ID_IMAGE_Connect: // Notifications sent by 'ID_IMAGE_Connect'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
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
    case ID_IMAGE_Time: // Notifications sent by 'ID_IMAGE_Time'
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
    case ID_IMAGE_Calibrate: // Notifications sent by 'ID_IMAGE_Calibrate'
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
		case ID_IMAGE_Reposition: // Notifications sent by 'ID_IMAGE_Reposition'
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
			}
    break;
			
		case WM_PAINT:
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
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
*       CreateSystem_Set_Win
*/
WM_HWIN CreateSystem_Set_Win(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
