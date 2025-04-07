#ifndef __DISPOSITION_H
#define __DISPOSITION_H	
#include "GUI.h"
#include "DIALOG.h"

WM_HWIN CreateWin_Disposition(void);
void Move_All(void);
void Move_Al(void);
void Update_All(void);

#define DATETIME_EVENT  (0x01 << 0)//�����¼������λ0
#define KEY2_EVENT  (0x01 << 1)//�����¼������λ1
#define KEY3_EVENT  (0x01 << 2)//�����¼������λ2
#define KEY4_EVENT  (0x01 << 4)//�����¼������λ3

#define LOGIN_EVENT 																(0x01 << 0)//��¼�����¼�����
#define KEYBOARD_EVENT 															(0x01 << 1)//���̽����¼�����
#define MENU_EVENT 																	(0x01 << 2)//���˵������¼�����
#define MANAGE_SELECT_EVENT 												(0x01 << 3)//����ѡ������¼�����
#define SYSTEM_SET_EVENT 														(0x01 << 4)//ϵͳ��������¼�����
#define MENU_CHILD_EVENT 														(0x01 << 5)//���˵������֧�Ӵ��ڽ����¼�����
#define MONITOR_EVENT 			  											(0x01 << 6)//����������¼�����
#define DISPOSITION_EVENT     											(0x01 << 7)//�����������¼�����
#define CONNECT_EVENT																(0x01 << 8)//�ⲿ���ӽ����¼�����

#define CANLENDER_EVENT															(0x01 << 9)//У׼�����¼�����

#define LOG_EVENT																		(0x01 << 10)//��־�����¼�����
#define LOG_CHILD_EVENT															(0x01 << 11)//��־�ӽ����¼�����
#define CALIBRATE_EVENT															(0x01 << 12)//У׼�����¼�����

#define DISPOSITION_CHILD_EVENT											(0x01 << 13)//�����ӽ����¼�����


//#define LOGIN_EVENT (0x01 << 0)//��¼�����¼�����
//#define LOGIN_EVENT (0x01 << 0)//��¼�����¼�����
//#define LOGIN_EVENT (0x01 << 0)//��¼�����¼�����

#endif