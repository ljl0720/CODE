#ifndef __DISPOSITION_H
#define __DISPOSITION_H	
#include "GUI.h"
#include "DIALOG.h"

WM_HWIN CreateWin_Disposition(void);
void Move_All(void);
void Move_Al(void);
void Update_All(void);

#define DATETIME_EVENT  (0x01 << 0)//设置事件掩码的位0
#define KEY2_EVENT  (0x01 << 1)//设置事件掩码的位1
#define KEY3_EVENT  (0x01 << 2)//设置事件掩码的位2
#define KEY4_EVENT  (0x01 << 4)//设置事件掩码的位3

#define LOGIN_EVENT 																(0x01 << 0)//登录界面事件掩码
#define KEYBOARD_EVENT 															(0x01 << 1)//键盘界面事件掩码
#define MENU_EVENT 																	(0x01 << 2)//主菜单界面事件掩码
#define MANAGE_SELECT_EVENT 												(0x01 << 3)//管理选择界面事件掩码
#define SYSTEM_SET_EVENT 														(0x01 << 4)//系统管理界面事件掩码
#define MENU_CHILD_EVENT 														(0x01 << 5)//主菜单界面分支子窗口界面事件掩码
#define MONITOR_EVENT 			  											(0x01 << 6)//监控主界面事件掩码
#define DISPOSITION_EVENT     											(0x01 << 7)//配置主界面事件掩码
#define CONNECT_EVENT																(0x01 << 8)//外部链接界面事件掩码

#define CANLENDER_EVENT															(0x01 << 9)//校准界面事件掩码

#define LOG_EVENT																		(0x01 << 10)//日志界面事件掩码
#define LOG_CHILD_EVENT															(0x01 << 11)//日志子界面事件掩码
#define CALIBRATE_EVENT															(0x01 << 12)//校准界面事件掩码

#define DISPOSITION_CHILD_EVENT											(0x01 << 13)//配置子界面事件掩码


//#define LOGIN_EVENT (0x01 << 0)//登录界面事件掩码
//#define LOGIN_EVENT (0x01 << 0)//登录界面事件掩码
//#define LOGIN_EVENT (0x01 << 0)//登录界面事件掩码

#endif