#include "DIALOG.h"
#include "bsp_frame.h"
#include "bsp_frame_turn.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/* �ؼ�ID */
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0   (GUI_ID_USER + 0x01)

/* ��Դ�� */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button0", ID_BUTTON_0, 30, 25, 240, 50, 0, 0x64, 0 },
};

/**
  * @brief �Ի���ص�����
  * @note ��
  * @param pMsg����Ϣָ��
  * @retval ��
  */
static void _cbDialog(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:
			
		
      /* ��ʼ����ܴ��ڿؼ� */
      hItem = pMsg->hWin;
//      FRAMEWIN_SetTitleHeight(hItem, 32);
//      FRAMEWIN_SetFont(hItem, GUI_FONT_32_1);
//      FRAMEWIN_SetText(hItem, "STemWIN@EmbedFire STM32F429");
      /* ��ʼ��Button0 */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		  BUTTON_SetText(hItem, "123");
      BUTTON_SetFont(hItem, GUI_FONT_24B_ASCII);
      break;
    case WM_NOTIFY_PARENT:
      /* ��ȡ�ؼ�ID */
      Id = WM_GetId(pMsg->hWinSrc);
      /* ��ȡ��Ϣ���� */
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
  * @brief �ԶԻ���ʽ��Ӵ����ؼ�
  * @note ��
  * @param ��
  * @retval hWin����Դ���е�һ���ؼ��ľ��
  */
WM_HWIN CreateFramewin(void)
{
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	/* �����Ի���󣬽����������Ƴ��� */
   GUI_Exec();
  return hWin;
}

/**
  * @brief GUI������
  * @note ��
  * @param ��
  * @retval ��
  */
void MainTask(void)
{ 
	while (1)
	{
		GUI_Delay(200);
	}
}

/*************************** End of file ****************************/
