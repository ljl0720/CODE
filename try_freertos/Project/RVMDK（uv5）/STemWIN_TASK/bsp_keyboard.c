#include "DIALOG.h"
#include "GUI.h"
#include "WM.h"
#include "EDIT.h"
#include "Win_LoginDLG.h"
#include "Win_DispositionDLG.h"
/* FreeRTOS头文件 */
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
#define Edit_SetText(Edit_buff)  EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_50), Edit_buff) ;			//这个Edit是数字键盘上的Edit关键作用，后面把数值写入两个Edit就靠它了

void Write_edit(char* edit_buff);//写入触发的编辑框
extern WM_HWIN hItem_slave;//上个界面被点击编辑框的句柄保存
extern WM_HWIN EDIT_bottle;//
extern WM_HWIN EDIT_capacity;//


extern char Edit_buff[10];//数字键盘显示当前输入字符串
extern char Input_Num[10];//数字键盘输入字符
extern char bottle_buff[10];//备份输入字符串
extern char capacity_buff[10];//备份输入字符串
extern char User_num[10];

extern struct Login_Value LV;

extern int num_sel ;//判断按键点击次数
extern int flag ;//判断按键的所有字符是否已完成一次轮询
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
	WM_HWIN Numbpad;//数字键盘的句柄
	int     NCode;
	int     Id;
	int     i;
	unsigned short int buff_length = 0;
	
	static U8 back = 0;
	static U16 setting_ml = 300;//默认灌装容量，单位ml
//	Show_next=pMsg->hWin;
	 
	memset(Input_Num, '\0', sizeof(Input_Num));//用来对一段内存空间全部设置为某个字符，一般用在对定义的字符串进行初始化为‘ ’或‘\0’

	Numbpad = pMsg->hWin;
	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		for (i = 0x01; i <= 0x0F; i++) {
			hItem = WM_GetDialogItem(Numbpad, GUI_ID_USER + i);
			BUTTON_SetFocussable(hItem, 0);/* 设置所有按钮不对焦 */
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
			case WM_NOTIFICATION_CLICKED://控件被点击

				if (strlen(Edit_buff) < 4)//就是最多只能输入4个0
					strcat(Edit_buff, "0");//按下0的时候把“0”这个字符串放到Edit_buff字符串数组里面
				Edit_SetText(Edit_buff);//同时更新键盘上Edit的值
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED://控件被释放
				
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
						Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
					}
					strcat(Edit_buff, Input_Num);
					break; 
					case 1:
						strcpy(Input_Num, "a");
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
						strcat(Edit_buff, Input_Num);
						
						break; 
					case 2:
						strcpy(Input_Num, "b");
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
						strcat(Edit_buff, Input_Num);

						break;
					case 3:
						strcpy(Input_Num, "c");
						buff_length = strlen(Edit_buff);
						Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
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
						Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
					}
					
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, "d");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "e");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
					strcat(Edit_buff, Input_Num);

					break;
				case 3:
					strcpy(Input_Num, "f");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
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
						Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
					}
					strcat(Edit_buff, Input_Num);
					break;
				case 1:
					strcpy(Input_Num, "g");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
					strcat(Edit_buff, Input_Num);

					break;
				case 2:
					strcpy(Input_Num, "h");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
					strcat(Edit_buff, Input_Num);

					break;
				case 3:
					strcpy(Input_Num, "i");
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
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
				if ((num_sel)==4) {  flag = 1;num_sel = 0;}//判断点击次数是否达到按键的最大次数，即能表达的最多字符，若是则重新计数
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

				GUI_EndDialog(pMsg->hWin, 0);//结束当前窗口
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

				if (strlen(Edit_buff) > 0)//判断是否输入了其他字符，输入了才能减去，否则可能会产生中断
				{
					buff_length = strlen(Edit_buff);
					Edit_buff[buff_length - 1] = '\0';//这个是数字键盘上储存字符串的，相当于减去一个字符
					Edit_SetText(Edit_buff);//然后更新减掉字符后的Edit
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
	
				
				
				//GUI_EndDialog(pMsg->hWin, 0);//结束数字小键盘
			  
			 
			 //Show_next = Show_prev;
			
			  
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				
			//Write_edit(Edit_buff);				//把数字键盘上的数值写入到Edit上	
			memset(User_num, Edit_buff, sizeof Edit_buff);
			memset(Edit_buff, '\0', sizeof Edit_buff);//用来对一段内存空间全部设置为某个字符，一般用在对定义的字符串进行初始化为‘ ’或‘\0’
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
			//EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_50), "");//把数字键盘的Edit设为空白
			
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
		strcpy(bottle_buff, Edit_buff);//如果点击了用户名输入框，则把Eidt的值赋值给bottle_buff数组
	}
	else if (EDIT_capacity == hItem_slave)
	{
		strcpy(capacity_buff, Edit_buff);////如果点击了密码输入框，则把Eidt的值赋值给capacity_buff数组
	}
	if(hItem_slave==LV.User_Wm)
	{
		strcpy(LV.User_Text, Edit_buff);//如果点击了用户名输入框，则把Eidt的值赋值给bottle_buff数组
	}
	else if(hItem_slave==LV.Pwd_Wm)
	{
		strcpy(LV.Pwd_Text, Edit_buff);////如果点击了密码输入框，则把Eidt的值赋值给capacity_buff数组
	}
//	EDIT_SetText(hItem_slave, edit_buff);//把数字键盘的值更新到当前点击的Edit框
}

WM_HWIN Createnumber(void) {
	WM_HWIN Hw_Win_Num;
	Hw_Win_Num = GUI_CreateDialogBox(_aDialogCreateKeyBoard, GUI_COUNTOF(_aDialogCreateKeyBoard), _cbDialog5, WM_HBKWIN, 0, 0);
//	Del_CallBack();
	printf("[DEBUG] 键盘窗口句柄: %p\n", Hw_Win_Num);
	return Hw_Win_Num;
}