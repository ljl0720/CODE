#include "DIALOG.h"
#include "bsp_frame_turn.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/* �ؼ�ID */
#define ID_FRAMEWIN_1 (GUI_ID_USER + 0x04)
#define ID_BUTTON_1   (GUI_ID_USER + 0x05)

/* ��Դ�� */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] = {
  { WINDOW_CreateIndirect, "Framewin", ID_FRAMEWIN_1, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button0", ID_BUTTON_1, 30, 25, 240, 50, 0, 0x64, 0 },
};

/**
 . * @brief �Ի���ص�����
  * @note ��
  * @param pMsg����Ϣָ��
  * @retval ��
  */
static void _cbDialog2(WM_MESSAGE * pMsg)
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
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		  BUTTON_SetText(hItem, "456");
      BUTTON_SetFont(hItem, GUI_FONT_24B_ASCII);
      break;
    case WM_NOTIFY_PARENT:
      /* ��ȡ�ؼ�ID */
      Id = WM_GetId(pMsg->hWinSrc);
      /* ��ȡ��Ϣ���� */
      NCode = pMsg->Data.v;
      switch(Id)
      {
        case ID_BUTTON_1: // Notifications sent by 'Button'
        switch(NCode)
        {
          case WM_NOTIFICATION_CLICKED:
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
WM_HWIN CreateFrameTurnwin(void)
{
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate2, GUI_COUNTOF(_aDialogCreate2), _cbDialog2, WM_HBKWIN, 0, 50);
	/* �����Ի���󣬽����������Ƴ��� */
   GUI_Exec();
  return hWin;
}

/*************************** End of file ****************************/
