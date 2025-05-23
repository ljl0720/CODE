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
#include "Win_DispositionDLG.h"
#include "Win_LoginDLG.h"
#include "Calibrate_WindowDLG.h"
/*FREERTOS 头文件*/
#include "FreeRTOS.h"
#include "event_groups.h"  

#include <string.h>
#include <stddef.h>
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    				 (GUI_ID_USER + 0x00)
#define ID_ICONVIEW_Menu     	 (GUI_ID_USER + 0x01)
#define ID_TEXT_Calibrate      (GUI_ID_USER + 0x02)
#define ID_ICONVIEW_Save       (GUI_ID_USER + 0x03)
#define ID_ICONVIEW_Return     (GUI_ID_USER + 0x04)
#define ID_EDIT_FloatHigh      (GUI_ID_USER + 0x05)
#define ID_EDIT_SlipperyTeeth  (GUI_ID_USER + 0x06)
#define ID_EDIT_Locking     	 (GUI_ID_USER + 0x07)
#define ID_EDIT_Exemption      (GUI_ID_USER + 0x08)
#define ID_EDIT_Speed     		 (GUI_ID_USER + 0x09)
#define ID_EDIT_Reset     		 (GUI_ID_USER + 0x0A)
#define ID_TEXT_FloatHigh      (GUI_ID_USER + 0x0B)
#define ID_TEXT_SlipertTeeth   (GUI_ID_USER + 0x0C)
#define ID_TEXT_Locking     	 (GUI_ID_USER + 0x0D)
#define ID_TEXT_Exemption      (GUI_ID_USER + 0x0E)
#define ID_TEXT_Speed     		 (GUI_ID_USER + 0x0F)
#define ID_ICONVIEW_Reset      (GUI_ID_USER + 0x10)


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
  { WINDOW_CreateIndirect, "Calibrate_Window", ID_WINDOW_0, 0, 0, 320, 240, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Menu_Icon", ID_ICONVIEW_Menu, 5, 8, 33, 33, 0, 0x00140014, 0 },
  { TEXT_CreateIndirect, "Calibrate_Text", ID_TEXT_Calibrate, 87, 10, 90, 32, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Save_Icon", ID_ICONVIEW_Save, 213, 8, 33, 33, 0, 0x00140014, 0 },
  { ICONVIEW_CreateIndirect, "Return_Icon", ID_ICONVIEW_Return, 272, 8, 33, 33, 0, 0x00140014, 0 },
  { EDIT_CreateIndirect, "FloatHigh_Edit", ID_EDIT_FloatHigh, 90, 70, 55, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "SlipperyTeeth_Edit", ID_EDIT_SlipperyTeeth, 90, 120, 55, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Locking_Edit", ID_EDIT_Locking, 90, 170, 55, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Exemption_Edit", ID_EDIT_Exemption, 255, 70, 55, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Speed_Edit", ID_EDIT_Speed, 255, 120, 55, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Reset_Edit", ID_EDIT_Reset, 245, 160, 76, 40, 0, 0, 0 },
  { TEXT_CreateIndirect, "FloatHigh_Text", ID_TEXT_FloatHigh, 10, 60, 76, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "SlipertTeeth_Text", ID_TEXT_SlipertTeeth, 10, 110, 76, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Locking_Text", ID_TEXT_Locking, 10, 160, 76, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Exemption_Text", ID_TEXT_Exemption, 175, 60, 76, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Speed_Text", ID_TEXT_Speed, 175, 110, 76, 40, 0, 0x0, 0 },
  { ICONVIEW_CreateIndirect, "Reset_Icon", ID_ICONVIEW_Reset, 189, 158, 40, 40, 0, 0x00200020, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/


extern EventGroupHandle_t EventGroup_next_Handle;
extern EventGroupHandle_t EventGroup_prev_Handle;
extern WM_HWIN hItem_slave;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsong;
extern struct Calibrate_Value  CALIV;

char FloatHigh[]="\xe6\xb5\xae\xe9\xab\x98\xe5\x9c\x88\xe6\x95\xb0";//浮高圈数
char Exemption[]="\xe5\x85\x8d\xe6\xa3\x80\xe5\x9c\x88\xe6\x95\xb0";//免检圈数
char SlipperyTeeth[]="\xe6\xbb\x91\xe7\x89\x99\xe5\x9c\x88\xe6\x95\xb0";//滑牙圈数
char Locking[]="\xe9\x94\x81\xe9\x99\x84\xe5\x9c\x88\xe6\x95\xb0";//锁附圈数
char Speed[]="\xe9\x94\x81\xe9\x99\x84\xe9\x80\x9f\xe5\xba\xa6";//锁附速度
char Calibrate[]="\xe6\xa0\xa1\xe5\x87\x86\xe7\xb3\xbb\xe6\x95\xb0";//系数校准
char Reset[]="\xe5\xa4\x8d\xe4\xbd\x8d";//复位
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

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Calibrate_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Calibrate);
		TEXT_SetText(hItem,Calibrate);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'FloatHigh_Edit'
    //
		if(!CALIV.FloatHigh[0])
		{
			strcpy(CALIV.FloatHigh,"5r");
		}
//	  snprintf(CALIV.FloatHigh,sizeof(CALIV.FloatHigh),"%s  r",CALIV.FloatHigh);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_FloatHigh);
	  CALIV.FloatHigh_H=hItem;
    EDIT_SetText(hItem, CALIV.FloatHigh);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'SlipperyTeeth_Edit'
    //
	  if(!CALIV.SlipperyTeeth[0])
		{
			strcpy(CALIV.SlipperyTeeth,"2r");
		}
//		snprintf(CALIV.SlipperyTeeth,sizeof(CALIV.SlipperyTeeth),"%s r",CALIV.SlipperyTeeth);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SlipperyTeeth);
	  CALIV.SlipperyTeeth_H=hItem;
    EDIT_SetText(hItem, CALIV.SlipperyTeeth);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Locking_Edit'
    //
		if(!CALIV.Locking[0])
		{
			strcpy(CALIV.Locking,"20r");
		}
//		snprintf(CALIV.Locking,sizeof(CALIV.Locking),"%s r",CALIV.Locking);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Locking);
		CALIV.Locking_H=hItem;
    EDIT_SetText(hItem, CALIV.Locking);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Exemption_Edit'
    //
		if(!CALIV.Exemption[0])
		{
			strcpy(CALIV.Exemption,"0r");
		}
//		snprintf(CALIV.Exemption,sizeof(CALIV.Exemption),"%s r",CALIV.Exemption);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Exemption);
		CALIV.Exemption_H=hItem;
    EDIT_SetText(hItem, CALIV.Exemption);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Speed_Edit'
    //
			if(!CALIV.Speed[0])
		{
			strcpy(CALIV.Speed,"5000rpm");
		}
//		snprintf(CALIV.Speed,sizeof(CALIV.Speed),"%s rpm",CALIV.Speed);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Speed);
		CALIV.Speed_H=hItem;
    EDIT_SetText(hItem, CALIV.Speed);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Reset_Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Reset);
		TEXT_SetText(hItem,Reset);
		TEXT_SetTextColor(hItem,0x0000FF00);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'FloatHigh_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FloatHigh);
		TEXT_SetText(hItem,FloatHigh);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'SlipertTeeth_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SlipertTeeth);
		TEXT_SetText(hItem,SlipperyTeeth);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Locking_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Locking);
		TEXT_SetText(hItem,Locking);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Exemption_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Exemption);
		TEXT_SetText(hItem,Exemption);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Speed_Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Speed);
		TEXT_SetText(hItem,Speed);
		TEXT_SetFont(hItem,&GUI_Fontsong);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Menu);
		ICONVIEW_AddBitmapItem(hItem,&bmMenu_Icon ,"");
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Save);
		ICONVIEW_AddBitmapItem(hItem,&bmSave_Icon ,"");
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Return);
		ICONVIEW_AddBitmapItem(hItem,&bmReturn_Icon ,"");
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_Reset);
		ICONVIEW_AddBitmapItem(hItem,&bmSystem_Repositon_Icon ,"");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_ICONVIEW_Menu: // Notifications sent by 'Menu_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
				xEventGroupSetBits(EventGroup_prev_Handle,MENU_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Save: // Notifications sent by 'Save_Icon'
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
    case ID_ICONVIEW_Return: // Notifications sent by 'Return_Icon'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				xEventGroupSetBits(EventGroup_prev_Handle,SYSTEM_SET_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_EDIT_FloatHigh: // Notifications sent by 'FloatHigh_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=CALIV.FloatHigh_H;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_EDIT_SlipperyTeeth: // Notifications sent by 'SlipperyTeeth_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=CALIV.SlipperyTeeth_H;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_EDIT_Locking: // Notifications sent by 'Locking_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=CALIV.Locking_H;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_EDIT_Exemption: // Notifications sent by 'Exemption_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=CALIV.Exemption_H;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_EDIT_Speed: // Notifications sent by 'Speed_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
				hItem_slave=CALIV.Speed_H;
				xEventGroupSetBits(EventGroup_next_Handle,KEYBOARD_EVENT);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_EDIT_Reset: // Notifications sent by 'Reset_Edit'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_ICONVIEW_Reset: // Notifications sent by 'Reset_Icon'
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
*       CreateCalibrate_Window
*/
WM_HWIN CreateCalibrate_Window(void);
WM_HWIN CreateCalibrate_Window(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
