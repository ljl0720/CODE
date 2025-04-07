#include "DIALOG.h"
#include "GUI.h"
#include "WM.h"
#include "EDIT.h"
#include "Win_LoginDLG.h"
#include "Win_DispositionDLG.h"
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
extern int Wm_prv, Wm_now;
extern WM_HWIN Show_prev,Show_next,login,number;
extern EventGroupHandle_t EventGroup1_Handle;
extern TaskHandle_t BSP_LCD_DEL_Handle;

extern struct Keyboard_Value KV;
extern int flag_login,flag_keyboard;;

int num_sel = 0;
int flag = 0;
int Wm_prv, Wm_now;

 static const GUI_WIDGET_CREATE_INFO _aDialogCreateKeyBoard[] = {
  { WINDOW_CreateIndirect, "KeyBoard", ID_FRAMEWIN_50, 0, 0	, 320, 240, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "Edit", ID_EDIT_50, 30, 2, 100, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "0", ID_BUTTON_50, 130, 173, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "1 abc", ID_BUTTON_51, 30, 38, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2 def", ID_BUTTON_52, 130, 38, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3 ghi", ID_BUTTON_53, 225, 38, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4 jkl", ID_BUTTON_54, 30, 83, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5 mno", ID_BUTTON_55, 130, 83, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6 pqrs", ID_BUTTON_56, 225, 83, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "7 tuv", ID_BUTTON_57, 30, 128, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "8 wxyz", ID_BUTTON_58, 130, 128, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "9 #", ID_BUTTON_59, 225, 128, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Esc", ID_BUTTON_5A, 30, 173, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Del", ID_BUTTON_5B, 225, 173, 50, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_5C, 175, 3, 100, 30, 0, 0x0, 0 },
};
 

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
		TEXT_SetFont(hItem, &GUI_Font24B_1);
		TEXT_SetText(hItem, "0");
		//
		// Initialization of '1'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_51);
		BUTTON_SetText(hItem, "1 abc");
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		//
		// Initialization of '2'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_52);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "2 def");
		//
		// Initialization of '3'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_53);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "3 ghi");
		//
		// Initialization of '4'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_54);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "4 jkl");
		//
		// Initialization of '5'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_55);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "5 mno");
		//
		// Initialization of '6'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_56);
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		BUTTON_SetText(hItem, "6 pqrs");
		//
		// Initialization of '7'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_57);
		BUTTON_SetText(hItem, "7 tuv");
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		//
		// Initialization of '8'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_58);
		BUTTON_SetText(hItem, "8 wxyz");
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		//
		// Initialization of '9'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_59);
		BUTTON_SetText(hItem, "9 #");
		BUTTON_SetFont(hItem, &GUI_Font24B_1);
		//
		// Initialization of 'ESC'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5A);
		BUTTON_SetText(hItem, "ESC");
		BUTTON_SetFont(hItem, GUI_FONT_24B_1);
		//
		// Initialization of 'Delete'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5B);
		BUTTON_SetText(hItem, "Del");
		BUTTON_SetFont(hItem, GUI_FONT_24B_1);
		//
		// Initialization of 'OK'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5C);
		BUTTON_SetFont(hItem, GUI_FONT_32B_1);
		BUTTON_SetText(hItem, "OK");
		
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
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED://�ؼ������

				if (strlen(Edit_buff) < 4)//�������ֻ������4��0
					strcat(Edit_buff, "0");//����0��ʱ��ѡ�0������ַ����ŵ�Edit_buff�ַ�����������
				Edit_SetText(Edit_buff);//ͬʱ���¼�����Edit��ֵ
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED://�ؼ����ͷ�
				
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
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				if (strlen(Edit_buff) < 10)
					strcat(Edit_buff, "4");
				Edit_SetText(Edit_buff);

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
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				if (strlen(Edit_buff) < 10)
					strcat(Edit_buff, "5");
				Edit_SetText(Edit_buff);
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
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				if (strlen(Edit_buff) < 10)
					strcat(Edit_buff, "6");
				Edit_SetText(Edit_buff);
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
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				if (strlen(Edit_buff) < 10)
					strcat(Edit_buff, "7");
				Edit_SetText(Edit_buff);
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
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				if (strlen(Edit_buff) < 10)
					strcat(Edit_buff, "8");
				Edit_SetText(Edit_buff);
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
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

				if (strlen(Edit_buff) < 10)
					strcat(Edit_buff, "9");
				Edit_SetText(Edit_buff);
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

				GUI_EndDialog(pMsg->hWin, 0);//������ǰ����
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
				
			//Write_edit(Edit_buff);				//�����ּ����ϵ���ֵд�뵽Edit��	
			memset(User_num, Edit_buff, sizeof Edit_buff);
			memset(Edit_buff, '\0', sizeof Edit_buff);//������һ���ڴ�ռ�ȫ������Ϊĳ���ַ���һ�����ڶԶ�����ַ������г�ʼ��Ϊ�� ����\0��
			flag_keyboard=2;
			flag_login=1;			
//			Del_CallBack1();
			
		
//			WM_SetCallback(ID_BUTTON_5C,Button_ok_Callback);
			
			xEventGroupSetBits(EventGroup1_Handle,KEY4_EVENT);
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
 

void Write_edit(char* edit_buff)
{
	if (Edit_buff[0] != '\0')
		edit_buff = Edit_buff;
	if (EDIT_bottle== hItem_slave )
	{
		strcpy(bottle_buff, Edit_buff);//���������û�����������Eidt��ֵ��ֵ��bottle_buff����
	}
	else if (EDIT_capacity == hItem_slave)
	{
		strcpy(capacity_buff, Edit_buff);////��������������������Eidt��ֵ��ֵ��capacity_buff����
	}
	if(hItem_slave==LV.User_Wm)
	{
		strcpy(LV.User_Text, Edit_buff);//���������û�����������Eidt��ֵ��ֵ��bottle_buff����
	}
	else if(hItem_slave==LV.Pwd_Wm)
	{
		strcpy(LV.Pwd_Text, Edit_buff);////��������������������Eidt��ֵ��ֵ��capacity_buff����
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