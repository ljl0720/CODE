#include "DIALOG.h"
#include "bsp_frame.h"
#include "bsp_frame_turn.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/* 控件ID */
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0   (GUI_ID_USER + 0x01)

/* 资源表 */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button0", ID_BUTTON_0, 30, 25, 240, 50, 0, 0x64, 0 },
};

/**
  * @brief 对话框回调函数
  * @note 无
  * @param pMsg：消息指针
  * @retval 无
  */
static void _cbDialog(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:
			
		
      /* 初始化框架窗口控件 */
      hItem = pMsg->hWin;
//      FRAMEWIN_SetTitleHeight(hItem, 32);
//      FRAMEWIN_SetFont(hItem, GUI_FONT_32_1);
//      FRAMEWIN_SetText(hItem, "STemWIN@EmbedFire STM32F429");
      /* 初始化Button0 */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		  BUTTON_SetText(hItem, "123");
      BUTTON_SetFont(hItem, GUI_FONT_24B_ASCII);
      break;
    case WM_NOTIFY_PARENT:
      /* 获取控件ID */
      Id = WM_GetId(pMsg->hWinSrc);
      /* 获取消息内容 */
      NCode = pMsg->Data.v;
      switch(Id)
      {
        case ID_BUTTON_0: // Notifications sent by 'Button'
        switch(NCode)
        {
          case WM_NOTIFICATION_CLICKED:
						
//						WM_HWIN h1=WM_GetDialogItem();
					WM_HideWindow(WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0));
					CreateFrameTurnwin();
            break;
          case WM_NOTIFICATION_RELEASED:
            break;
        }
        break;
      }
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief 以对话框方式间接创建控件
  * @note 无
  * @param 无
  * @retval hWin：资源表中第一个控件的句柄
  */
WM_HWIN CreateFramewin(void)
{
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	/* 创建对话框后，将其立即绘制出来 */
   GUI_Exec();
  return hWin;
}

/**
  * @brief GUI主任务
  * @note 无
  * @param 无
  * @retval 无
  */
void MainTask(void)
{ 
	while (1)
	{
		GUI_Delay(200);
	}
}

/*************************** End of file ****************************/
