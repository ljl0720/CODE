#include "DIALOG.h"
#include "GUI.h"
#include "WM.h"
#include "EDIT.h"
#include "Win_LoginDLG.h"
#include "Win_DispositionDLG.h"
#include "bsp_keyboard.h"

#include "stdio.h"
#include "math.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define ID_FRAMEWIN_50      (GUI_ID_USER + 0x501)
#define ID_EDIT_50			(GUI_ID_USER + 0x502)
#define ID_BUTTON_50        (GUI_ID_USER + 0x503)
#define ID_BUTTON_51        (GUI_ID_USER + 0x504)
#define ID_BUTTON_52        (GUI_ID_USER + 0x505)
#define ID_BUTTON_53        (GUI_ID_USER + 0x506)
#define ID_BUTTON_54        (GUI_ID_USER + 0x507)
#define ID_BUTTON_55        (GUI_ID_USER + 0x508)
#define ID_BUTTON_56        (GUI_ID_USER + 0x509)
#define ID_BUTTON_57        (GUI_ID_USER + 0x50A)
#define ID_BUTTON_58        (GUI_ID_USER + 0x50B)
#define ID_BUTTON_59        (GUI_ID_USER + 0x50C)
#define ID_BUTTON_5A        (GUI_ID_USER + 0x50D)
#define ID_BUTTON_5B        (GUI_ID_USER + 0x50E)
#define ID_BUTTON_5C        (GUI_ID_USER + 0x50F)
#define Edit_SetText(Edit_buff)  EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_50), Edit_buff) ;			//���Edit�����ּ����ϵ�Edit�ؼ����ã��������ֵд������Edit�Ϳ�����

void Write_edit(char* edit_buff);//д�봥���ı༭��
extern WM_HWIN hItem_slave;//�ϸ����汻����༭��ľ������
extern WM_HWIN EDIT_bottle;//
extern WM_HWIN EDIT_capacity;//


extern char Edit_buff[10];//���ּ�����ʾ��ǰ�����ַ���
extern char Input_Num[10];//���ּ��������ַ�
extern char bottle_buff[10];//���������ַ���
extern char capacity_buff[10];//���������ַ���
extern char User_num[10];

extern struct Login_Value LV;

extern int num_sel ;//�жϰ����������
extern int flag ;//�жϰ����������ַ��Ƿ������һ����ѯ
extern WM_HWIN Show_prev,Show_next,login,number;

extern EventGroupHandle_t EventGroup_next_Handle;
extern EventGroupHandle_t EventGroup_prev_Handle;
extern TaskHandle_t BSP_LCD_DEL_Handle;

extern struct Keyboard_Value 			KV;
extern struct Disposition_Value   DISPV;
extern struct Connect_Value				CONTV;
extern struct Canlender_Value     CANLV;
extern struct Log_Value     			LOGV;
extern struct Calibrate_Value     CALIV;

extern int flag_login,flag_keyboard;

int num_sel = 0;
int flag = 0;
int Wm_prv, Wm_now;//�жϰ����Ƿ�Ϊͬһ��
extern int FLAG_Window;

 static const GUI_WIDGET_CREATE_INFO _aDialogCreateKeyBoard[] = {
  { WINDOW_CreateIndirect, "KeyBoard", ID_FRAMEWIN_50, 0, 0	, 320, 240, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "Edit", ID_EDIT_50, 15, 12, 180, 30, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, "0", ID_BUTTON_50, 120, 188, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "1 abc", ID_BUTTON_51, 15, 53, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2 def", ID_BUTTON_52, 120, 53, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3 ghi", ID_BUTTON_53, 225, 53, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4 jkl", ID_BUTTON_54, 15, 98, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5 mno", ID_BUTTON_55, 120, 98, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6 pqrs", ID_BUTTON_56, 225, 98, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "7 tuv", ID_BUTTON_57, 15, 143, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "8 wxyz", ID_BUTTON_58, 120, 143, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "9 #", ID_BUTTON_59, 225, 143, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Esc", ID_BUTTON_5A, 15, 188, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Del", ID_BUTTON_5B, 225, 188, 75, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_5C, 225, 8, 75, 40, 0, 0x0, 0 },
};
 
/*
{ TEXT_CreateIndirect, "0", ID_BUTTON_50, 120, 188, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "1 abc", ID_BUTTON_51, 15, 53, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "2 def", ID_BUTTON_52, 115, 53, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "3 ghi", ID_BUTTON_53, 220, 53, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "4 jkl", ID_BUTTON_54, 10, 98, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "5 mno", ID_BUTTON_55, 120, 98, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "6 pqrs", ID_BUTTON_56, 225, 98, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "7 tuv", ID_BUTTON_57, 15, 143, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "8 wxyz", ID_BUTTON_58, 120, 143, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "9 #", ID_BUTTON_59, 215, 143, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Esc", ID_BUTTON_5A, 15, 188, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Del", ID_BUTTON_5B, 225, 188, 75, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "OK", ID_BUTTON_5C, 210, 18, 100, 30, 0, 0x0, 0 },
*/

 static void _cbDialog5(WM_MESSAGE* pMsg) {
	WM_HWIN hItem;
	WM_HWIN Numbpad;//���ּ��̵ľ��
	int     NCode;
	int     Id;
	int     i;
	unsigned short int buff_length = 0;
	
	static U8 back = 0;
	static U16 setting_ml = 300;//Ĭ�Ϲ�װ��������λml
//	Show_next=pMsg->hWin;
	 
	memset(Input_Num, '\0', sizeof(Input_Num));//������һ���ڴ�ռ�ȫ������Ϊĳ���ַ���һ�����ڶԶ�����ַ������г�ʼ��Ϊ�� ����\0��

	Numbpad = pMsg->hWin;
	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		for (i = 0x01; i <= 0x0F; i++) {
			hItem = WM_GetDialogItem(Numbpad, GUI_ID_USER + i);
			BUTTON_SetFocussable(hItem, 0);/* �������а�ť���Խ� */
		  }WM_GetDialogItem(Numbpad, GUI_ID_USER + 0XEB);

		//
		// Initialization of 'number'
		//
//			Set_batton_skin();
//		hItem = pMsg->hWin;
//		FRAMEWIN_SetText(hItem, "Name");
//		FRAMEWIN_SetFont(hItem, GUI_FONT_24B_1);
//		FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of 'Edit'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
		EDIT_SetText(hItem, "");
		EDIT_SetFont(hItem, GUI_FONT_D24X32);
		EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
		// Initialization of '0'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_50);
		BUTTON_SetText(hItem, "0");
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of '1'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_51);
		BUTTON_SetText(hItem, "1  abc");
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of '2'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_52);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "2  def");
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of '3'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_53);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "3  ghi");
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of '4'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_54);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "4  jkl");
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of '5'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_55);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "5  mno");
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of '6'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_56);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "6 pqrs");
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of '7'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_57);
		BUTTON_SetText(hItem, "7  tuv");
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of '8'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_58);
		BUTTON_SetText(hItem, "8 wxyz");
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of '9'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_59);
		BUTTON_SetText(hItem, "9 . #");
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of 'ESC'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5A);
		BUTTON_SetText(hItem, "ESC");
		BUTTON_SetFont(hItem, GUI_FONT_24B_1);
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of 'Delete'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5B);
		BUTTON_SetText(hItem, "Del");
		BUTTON_SetFont(hItem, GUI_FONT_24B_1);
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		//
		// Initialization of 'OK'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5C);
		BUTTON_SetFont(hItem, GUI_FONT_32B_1);
		BUTTON_SetText(hItem, "OK");
		BUTTON_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
			long ssss=GUI_ALLOC_GetNumUsedBytes ();
			  printf("keyboard %ld\n",ssss);
		// USER START (Optionally insert additional code for further widget initialization)
		// USER END
		break;
	case WM_NOTIFY_PARENT:
		
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {
		case ID_EDIT_50: // Notifications sent by 'Edit'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:


				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_50: // Notifications sent by '0'
			if (Wm_prv != ID_BUTTON_50) { num_sel = 0; }
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED://�ؼ������

				
				break;
			case WM_NOTIFICATION_RELEASED://�ؼ����ͷ�
				
//				if (strlen(Edit_buff) < 4)//�������ֻ������4��0
//					strcat(Edit_buff, "0");//����0��ʱ��ѡ�0������ַ����ŵ�Edit_buff�ַ�����������
//				EDIT_SetFont(hItem, &GUI_Font24B_1);
//				Edit_SetText(Edit_buff);//ͬʱ���¼�����Edit��ֵ
			
			switch (num_sel) {
				    case 0:
					strcpy(Input_Num, "0");
					if (flag == 1) {
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					strcat(Edit_buff, Input_Num);
					break; 
					case 1:
						strcpy(Input_Num, "0");
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
						strcat(Edit_buff, Input_Num);
						
						break; 
					case 2:
						strcpy(Input_Num, "0");
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
						strcat(Edit_buff, Input_Num);

						break;
					case 3:
						strcpy(Input_Num, "0");
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
						strcat(Edit_buff, Input_Num);

						break;
					default:

						num_sel = 0;
				}
				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				
				Edit_SetText(Edit_buff);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				flag = 0;
				if ((num_sel) == 4) {
					flag = 1;
					num_sel = 0;
				}
				// USER END
				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END 
			}
			break;
		case ID_BUTTON_51: // Notifications sent by '1'
			if (Wm_prv != ID_BUTTON_51) { num_sel = 0; }
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				
				switch (num_sel) {
				    case 0:
					strcpy(Input_Num, "1");
					if (flag == 1) {
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					strcat(Edit_buff, Input_Num);
					break; 
					case 1:
						strcpy(Input_Num, "a");
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
						strcat(Edit_buff, Input_Num);
						
						break; 
					case 2:
						strcpy(Input_Num, "b");
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
						strcat(Edit_buff, Input_Num);

						break;
					case 3:
						strcpy(Input_Num, "c");
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
						strcat(Edit_buff, Input_Num);

						break;
					default:

						num_sel = 0;
				}
				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				
				Edit_SetText(Edit_buff);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				flag = 0;
				if ((num_sel) == 4) {
					flag = 1;
					num_sel = 0;
				}
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				
				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_52: // Notifications sent by '2'
			if (Wm_prv != ID_BUTTON_52) { num_sel = 0; }
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				
				switch (num_sel) {
				case 0:
					strcpy(Input_Num, "2");
					if (flag == 1) {
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, "d");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "e");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 3:
					strcpy(Input_Num, "f");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				default:

					num_sel = 0;
				}

				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				Edit_SetText(Edit_buff);
				flag = 0;
				if ((num_sel) == 4) {
					flag = 1;
					num_sel = 0;
				}
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_53: // Notifications sent by '3'
			if (Wm_prv != ID_BUTTON_53 ) { num_sel = 0; }
			
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				switch (num_sel) {
				case 0:
					strcpy(Input_Num, "3");
					
					if(flag==1){
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, "g");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "h");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 3:
					strcpy(Input_Num, "i");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				default:

					num_sel = 0;
				}
				
				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				Edit_SetText(Edit_buff);
				flag = 0;
				if ((num_sel)==4) {  flag = 1;num_sel = 0;}//�жϵ�������Ƿ�ﵽ�����������������ܱ�������ַ������������¼���
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_54: // Notifications sent by '4'
			if (Wm_prv != ID_BUTTON_54 ) { num_sel = 0; }
			
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				switch (num_sel) {
				case 0:
					strcpy(Input_Num, "4");
					
					if(flag==1){
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, "j");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "k");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 3:
					strcpy(Input_Num, "l");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				default:

					num_sel = 0;
				}
				
				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				Edit_SetText(Edit_buff);
				flag = 0;
				if ((num_sel)==4) {  flag = 1;num_sel = 0;}//�жϵ�������Ƿ�ﵽ�����������������ܱ�������ַ������������¼���

				//WM_HideWindow(pMsg->hWin);
//				GUI_EndDialog(pMsg->hWin,0);
//				WM_DeleteWindow(pMsg ->hWin);
//				CreateWin_Monitor();
//				GUI_Exec();
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_55: // Notifications sent by '5'
			if (Wm_prv != ID_BUTTON_55 ) { num_sel = 0; }
			
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				switch (num_sel) {
				case 0:
					strcpy(Input_Num, "5");
					
					if(flag==1){
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, "m");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "n");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 3:
					strcpy(Input_Num, "o");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				default:

					num_sel = 0;
				}
				
				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				Edit_SetText(Edit_buff);
				flag = 0;
				if ((num_sel)==4) {  flag = 1;num_sel = 0;}//�жϵ�������Ƿ�ﵽ�����������������ܱ�������ַ������������¼���
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_56: // Notifications sent by '6'
			if (Wm_prv != ID_BUTTON_56 ) { num_sel = 0; }
			
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				switch (num_sel) {
				case 0:
					strcpy(Input_Num, "6");
					
					if(flag==1){
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, "p");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "q");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 3:
					strcpy(Input_Num, "r");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 4:
					strcpy(Input_Num, "s");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				default:

					num_sel = 0;
				}
				
				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				Edit_SetText(Edit_buff);
				flag = 0;
				if ((num_sel)==5) {  flag = 1;num_sel = 0;}//�жϵ�������Ƿ�ﵽ�����������������ܱ�������ַ������������¼���
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_57: // Notifications sent by '7'
			if (Wm_prv != ID_BUTTON_57 ) { num_sel = 0; }
			
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				switch (num_sel) {
				case 0:
					strcpy(Input_Num, "7");
					
					if(flag==1){
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, "t");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "u");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 3:
					strcpy(Input_Num, "z");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				default:

					num_sel = 0;
				}
				
				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				Edit_SetText(Edit_buff);
				flag = 0;
				if ((num_sel)==4) {  flag = 1;num_sel = 0;}//�жϵ�������Ƿ�ﵽ�����������������ܱ�������ַ������������¼���
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_58: // Notifications sent by '8'
			if (Wm_prv != ID_BUTTON_58 ) { num_sel = 0; }
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				switch (num_sel) {
				case 0:
					strcpy(Input_Num, "8");
					
					if(flag==1){
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, "w");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "x");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 3:
					strcpy(Input_Num, "y");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 4:
					strcpy(Input_Num, "z");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				default:

					num_sel = 0;
				}
				
				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				Edit_SetText(Edit_buff);
				flag = 0;
				if ((num_sel)==5) {  flag = 1;num_sel = 0;}//�жϵ�������Ƿ�ﵽ�����������������ܱ�������ַ������������¼���
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_59: // Notifications sent by '9'
			if (Wm_prv != ID_BUTTON_59 ) { num_sel = 0; }
			
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				switch (num_sel) {
				case 0:
					strcpy(Input_Num, "9");
					
					if(flag==1){
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					}
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, ".");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "#");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					strcat(Edit_buff, Input_Num);

					break;
				default:

					num_sel = 0;
				}
				
				num_sel++;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_50);
				EDIT_SetFont(hItem, &GUI_Font24B_1);
				//strcat(Edit_buff, Input_Num);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				Edit_SetText(Edit_buff);
				flag = 0;
				if ((num_sel)==3) {  flag = 1;num_sel = 0;}//�жϵ�������Ƿ�ﵽ�����������������ܱ�������ַ������������¼���
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_5A: // Notifications sent by 'ESC'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				num_sel=0;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5A);
				//strcat(Edit_buff, Input_Num);
				Wm_prv = WM_GetId(pMsg->hWinSrc);
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_5B: // Notifications sent by 'Delete'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				if (strlen(Edit_buff) > 0)//�ж��Ƿ������������ַ��������˲��ܼ�ȥ��������ܻ�����ж�
				{
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//��������ּ����ϴ����ַ����ģ��൱�ڼ�ȥһ���ַ�
					Edit_SetText(Edit_buff);//Ȼ����¼����ַ����Edit
				}
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END();
			}
			break;
		case ID_BUTTON_5C: // Notifications sent by 'OK'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				
			
//						  LV.Login=CreateLogin();
//			 Show_prev=KV.Keyboard;
//			  Show_next=LV.Login;
//			WM_DeleteWindow(KV.Keyboard);
	
				
				
				//GUI_EndDialog(pMsg->hWin, 0);//��������С����
			  
			 
			 //Show_next = Show_prev;
			
			  
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				
			Write_edit(Edit_buff);				//�����ּ����ϵ���ֵд�뵽Edit��	sa
			memset(User_num, Edit_buff, sizeof Edit_buff);
			memset(Edit_buff, '\0', sizeof Edit_buff);//������һ���ڴ�ռ�ȫ������Ϊĳ���ַ���һ�����ڶԶ�����ַ������г�ʼ��Ϊ�� ����\0��
			flag_login=1;			
//			Del_CallBack1();
			
		
//			WM_SetCallback(ID_BUTTON_5C,Button_ok_Callback);
			switch(FLAG_Window)
			{
				case 0:
					xEventGroupSetBits(EventGroup_next_Handle,LOGIN_EVENT);
				break;
				case 7:
					xEventGroupSetBits(EventGroup_prev_Handle,DISPOSITION_EVENT);
				break;
				case 8:
					xEventGroupSetBits(EventGroup_prev_Handle,CONNECT_EVENT);
				break;
				case 9:
					xEventGroupSetBits(EventGroup_prev_Handle,CANLENDER_EVENT);
				break;
				case 10:
					xEventGroupSetBits(EventGroup_prev_Handle,LOG_EVENT);
				break;
				case 11:
					xEventGroupSetBits(EventGroup_prev_Handle,LOG_CHILD_EVENT);
				break;
				case 12:
					xEventGroupSetBits(EventGroup_prev_Handle,CALIBRATE_EVENT);
				break;
				case 13:
					xEventGroupSetBits(EventGroup_prev_Handle,DISPOSITION_CHILD_EVENT);
				break;
			}
			FLAG_Window=1001;
//			GUI_EndDialog(KV.Keyboard,0);	
//			WM_DeleteWindow(KV.Keyboard);
//			Del_CallBack1();
//			LV.Login=CreateLogin();
//				WM_DeleteWindow(KV.Keyboard);	
		
//			 LV.Login=0;
//			GUI_EndDialog(pMsg->hWin,0);
			//EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_50), "");//�����ּ��̵�Edit��Ϊ�հ�
			
				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
			// USER START (Optionally insert additional code for fu-------rther Ids)
			// USER END
		}
		break;
		
//   case WM_DELETE:
//		 
////	  LV.Login=CreateLogin();printf("CreateLogin\n");
////	 
////      if(KV.Keyboard)
////			 {
////				 WM_DeleteWindow(KV.Keyboard);
////				 KV.Keyboard=0;
////				 printf("Deletenumber\n");
////			 }
////			 
////			  
////			 

////     long used_mem = GUI_ALLOC_GetNumUsedBytes();
////            printf("After keyboard destroy: %ld bytes\n", used_mem);
//      break;
		// USER END
//		case WM_DELETE:
//			
//		
//			long mm=GUI_ALLOC_GetNumFreeBytes ();
//		  printf("Delete Login After FreeBytes %ld\n",mm);	
//    break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}
 


void Set_batton_skin(void)
{
	BUTTON_SKINFLEX_PROPS Props;
	BUTTON_GetSkinFlexProps(&Props, BUTTON_SKINFLEX_PI_ENABLED);
	Props.aColorUpper[0] = GUI_TRANSPARENT;        //��������Ķ�����ɫ��
	Props.aColorLower[0] = GUI_TRANSPARENT;        //��������ĵײ���ɫ��
	Props.aColorUpper[1] = GUI_TRANSPARENT;        //�ײ�����Ķ�����ɫ��
	Props.aColorLower[1] = GUI_TRANSPARENT;        //�ײ�����ĵײ���ɫ��
	Props.aColorFrame[0] = GUI_TRANSPARENT;                   //��Χ��ܵ��ⲿ��ɫ��
	Props.aColorFrame[1] = GUI_TRANSPARENT;  //��Χ��ܵ��ڲ���ɫ��
	Props.aColorFrame[2] = GUI_TRANSPARENT;  //��Χ������ڲ���������֮���������ɫ��
	Props.Radius = 5;
	BUTTON_SetSkinFlexProps(&Props, BUTTON_SKINFLEX_PI_ENABLED); //��������״̬Ƥ��
	
	BUTTON_GetSkinFlexProps(&Props, BUTTON_SKINFLEX_PI_PRESSED);
	Props.aColorUpper[0] = 0x301E90FF;        //��������Ķ�����ɫ��
	Props.aColorLower[0] = 0x301E90FF;        //��������ĵײ���ɫ��
	Props.aColorUpper[1] = 0x301E90FF;        //�ײ�����Ķ�����ɫ��
	Props.aColorLower[1] = 0x301E90FF;        //�ײ�����ĵײ���ɫ��
	Props.aColorFrame[0] = 0x301E90FF;                   //��Χ��ܵ��ⲿ��ɫ��
	Props.aColorFrame[1] = 0x301E90FF;  //��Χ��ܵ��ڲ���ɫ��
	Props.aColorFrame[2] = 0x301E90FF;  //��Χ������ڲ���������֮���������ɫ��
	Props.Radius = 5;
	BUTTON_SetSkinFlexProps(&Props, BUTTON_SKINFLEX_PI_PRESSED); //��������״̬Ƥ��
}



void Write_edit(char* edit_buff)
{
	if (Edit_buff[0] != '\0')
		edit_buff = Edit_buff;
//	if (EDIT_bottle== hItem_slave )
//	{
//		strcpy(bottle_buff, Edit_buff);//���������û�����������Eidt��ֵ��ֵ��bottle_buff����
//	}
//	else if (EDIT_capacity == hItem_slave)
//	{
//		strcpy(capacity_buff, Edit_buff);////��������������������Eidt��ֵ��ֵ��capacity_buff����
//	}
	
	if(hItem_slave==LV.User_Wm)
	{
		strcpy(LV.User_Text, Edit_buff);//���������û�����������Eidt��ֵ��ֵ��bottle_buff���� �û���
		FLAG_Window=0;
	}
	else if(hItem_slave==LV.Pwd_Wm)
	{
		strcpy(LV.Pwd_Text, Edit_buff);//��������������������Eidt��ֵ��ֵ��capacity_buff����  ����
		FLAG_Window=0;
	}
	else if(hItem_slave==DISPV.DegMax)
	{
		strcpy(DISPV.Deg_Max, Edit_buff);//�Ƕ�MAX
		FLAG_Window=7;
	}
	else if(hItem_slave==DISPV.DegMin)
	{
		strcpy(DISPV.Deg_Min, Edit_buff);//�Ƕ�MIN
		FLAG_Window=7;
	}
	else if(hItem_slave==DISPV.DegTarget)
	{
		strcpy(DISPV.Deg_Target, Edit_buff);//�Ƕ�Ŀ��
		FLAG_Window=7;
	}
	else if(hItem_slave==DISPV.NmMax)
	{
		strcpy(DISPV.Nm_Max, Edit_buff);//Ť��MAX
		FLAG_Window=7;
	}
	else if(hItem_slave==DISPV.NmMin)
	{
		strcpy(DISPV.Nm_Min, Edit_buff);//Ť��MIN
		FLAG_Window=7;
	}
	else if(hItem_slave==DISPV.NmTarget)
	{
		strcpy(DISPV.Nm_Target, Edit_buff);//Ť��Ŀ��
		FLAG_Window=7;
	}
	else if(hItem_slave==CONTV.IP_H)
	{
		strcpy(CONTV.IP, Edit_buff);//IP��ַ
		FLAG_Window=8;
	}
	else if(hItem_slave==CONTV.DCIM_H)
	{
		strcpy(CONTV.DCIM, Edit_buff);//����
		FLAG_Window=8;
	}
	else if(hItem_slave==CONTV.IP_Target_H)
	{
		strcpy(CONTV.IP_Target, Edit_buff);//Ŀ��IP��ַ
		FLAG_Window=8;
	}
	else if(hItem_slave==CONTV.Port_H)
	{
		strcpy(CONTV.Port, Edit_buff);//�˿ں�
		FLAG_Window=8;
	}
	else if(hItem_slave==CANLV.Year_H)
	{
		strcpy(CANLV.Year, Edit_buff);//��
		FLAG_Window=9;
	}
	else if(hItem_slave==CANLV.Month_H)
	{
		strcpy(CANLV.Month, Edit_buff);//��
		FLAG_Window=9;
	}
	else if(hItem_slave==CANLV.Day_H)
	{
		strcpy(CANLV.Day, Edit_buff);//��
		FLAG_Window=9;
	}
	else if(hItem_slave==CANLV.Hour_H)
	{
		strcpy(CANLV.Hour, Edit_buff);//ʱ
		FLAG_Window=9;
	}
	else if(hItem_slave==CANLV.Minute_H)
	{
		strcpy(CANLV.Minute, Edit_buff);//��
		FLAG_Window=9;
	}
	else if(hItem_slave==CANLV.Second_H)
	{
		strcpy(CANLV.Second, Edit_buff);//��
		FLAG_Window=9;
	}
	else if(hItem_slave==LOGV.Key)
	{
		strcpy(LOGV.Key_Search, Edit_buff);//����
		FLAG_Window=10;
	}
	else if(hItem_slave==LOGV.Log_DateTime)
	{
		strcpy(LOGV.Log_TimeSel, Edit_buff);//��־��ҳ�� ��
		FLAG_Window=10;
	}
	else if(hItem_slave==LOGV.Year_H)
	{
		strcpy(LOGV.Year, Edit_buff);//��־��ҳ�� ��
		FLAG_Window=11;
	}
	else if(hItem_slave==LOGV.Month_H)
	{
		strcpy(LOGV.Month, Edit_buff);//��־��ҳ�� ��
		FLAG_Window=11;
	}
	else if(hItem_slave==LOGV.Day_H)
	{
		strcpy(LOGV.Day, Edit_buff);//��־��ҳ�� ��
		FLAG_Window=11;
	}
	else if(hItem_slave==CALIV.FloatHigh_H)
	{
		strcpy(CALIV.FloatHigh, Edit_buff);//ϵ��У׼ҳ�� ����Ȧ��
		strcat(CALIV.FloatHigh,"r");
		FLAG_Window=12;
	}
	else if(hItem_slave==CALIV.Exemption_H)
	{
		strcpy(CALIV.Exemption, Edit_buff);//ϵ��У׼ҳ�� ���Ȧ��
		strcat(CALIV.Exemption,"r");
		FLAG_Window=12;
	}
	else if(hItem_slave==CALIV.SlipperyTeeth_H)
	{
		strcpy(CALIV.SlipperyTeeth, Edit_buff);//ϵ��У׼ҳ�� ����Ȧ��
		strcat(CALIV.SlipperyTeeth,"r");
		FLAG_Window=12;
	}
	else if(hItem_slave==CALIV.Speed_H)
	{
		strcpy(CALIV.Speed, Edit_buff);//ϵ��У׼ҳ�� �����ٶ�
		strcat(CALIV.Speed,"rpm");
		FLAG_Window=12;
	}
	else if(hItem_slave==CALIV.Locking_H)
	{
		strcpy(CALIV.Locking, Edit_buff);//ϵ��У׼ҳ�� ����Ȧ��
		strcat(CALIV.Locking,"r");
		FLAG_Window=12;
	}
	else if(hItem_slave==DISPV.Step_H)
	{
		strcpy(DISPV.Step, Edit_buff);//������ҳ�� ����
		FLAG_Window=13;
	}
	else if(hItem_slave==DISPV.Speed_H)
	{
		strcpy(DISPV.Speed, Edit_buff);//������ҳ�� ת��
		strcat(DISPV.Speed,"rpm");
		FLAG_Window=13;
	}
	else if(hItem_slave==DISPV.Num_H)
	{
		strcpy(DISPV.Num, Edit_buff);//������ҳ�� Ԥ��š������
		FLAG_Window=13;
	}
//	EDIT_SetText(hItem_slave, edit_buff);//�����ּ��̵�ֵ���µ���ǰ�����Edit��
}

WM_HWIN Createnumber(void) {
	WM_HWIN Hw_Win_Num;
	Hw_Win_Num = GUI_CreateDialogBox(_aDialogCreateKeyBoard, GUI_COUNTOF(_aDialogCreateKeyBoard), _cbDialog5, WM_HBKWIN, 0, 0);
//	Del_CallBack();
	printf("[DEBUG] ���̴��ھ��: %p\n", Hw_Win_Num);
	return Hw_Win_Num;
}