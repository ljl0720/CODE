#ifndef CONNECT_WIN
#define CONNECT_WIN
#include "DIALOG.h"
#include "GUI.h"
#include "stm32f4xx.h"

WM_HWIN CreateLogin(void);
void Edit_P_Callback(void);
void Button_ok_Callback(void);
void Del_CallBack(void);
void Del_CallBack1(void);
	
struct Login_Value{
	char User_Text[10];
	char Pwd_Text[10];
	
	WM_HWIN User_Wm;
	WM_HWIN Pwd_Wm;
	
	WM_HWIN Login;
	WM_HWIN Logo;
	
};
/*Login������ʾ*/
extern GUI_CONST_STORAGE GUI_BITMAP bmcode;
extern GUI_CONST_STORAGE GUI_BITMAP bmDrcode;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum;
struct Keyboard_Value{
	 
	 WM_HWIN Keyboard;
	 
 };

struct Menu_Value{
	
	WM_HWIN Menu;
	WM_HWIN Menu_Child;
	
	char Exception_Num_Text[10];
	char Term_Show_Text[10];
	
//	GUI_BITMAP Logo;//�豸LOGO
//	GUI_BITMAP Exception;//����
//	GUI_BITMAP User;//�û�
//	GUI_BITMAP Term;//����ʱ��
//	GUI_BITMAP Quit;//�˳�
//	GUI_BITMAP Monitor;//���
//	GUI_BITMAP Disposition;//����
//	GUI_BITMAP Manage;//����
//	GUI_BITMAP Statisticsv;//ͳ��
//	GUI_BITMAP Analyse;//����
	
};
/*Menu������ʾ*/
extern GUI_CONST_STORAGE GUI_BITMAP bmAnalyse_Icon;//����
extern GUI_CONST_STORAGE GUI_BITMAP bmWarning_Icon;//����
extern GUI_CONST_STORAGE GUI_BITMAP bmUser_Icon;//�û�
extern GUI_CONST_STORAGE GUI_BITMAP bmDesprition_Icon;//����
extern GUI_CONST_STORAGE GUI_BITMAP bmMenu_Info_Icon;//����
extern GUI_CONST_STORAGE GUI_BITMAP bmQuit_Icon;//�˳�
extern GUI_CONST_STORAGE GUI_BITMAP bmStatistics_Icon;//ͳ��
extern GUI_CONST_STORAGE GUI_BITMAP bmMonitor_Icon;//���
extern GUI_CONST_STORAGE GUI_BITMAP bmSetting_Icon;//����
extern GUI_CONST_STORAGE GUI_BITMAP bmWorking_Time_Icon;//ʱ��
extern GUI_CONST_STORAGE GUI_BITMAP bmLogo_Icon;//Logo
extern GUI_CONST_STORAGE GUI_BITMAP bmLogo_mini_Icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmMenu_Info_Icon;//����
extern GUI_CONST_STORAGE GUI_BITMAP bmClose_Icon;//�ر�


struct Manage_Sel_Value{
	
	WM_HWIN Manage_Sel;
//	GUI_BITMAP Menu;//������
//	GUI_BITMAP User_Manage;//�û�����
//	GUI_BITMAP System_Manage;//ϵͳ����
//	GUI_BITMAP Log_Manage;//��־����
};

/*Manage_Selҳ��*/
extern GUI_CONST_STORAGE GUI_BITMAP bmUser_Setting_Icon;//�û�����
extern GUI_CONST_STORAGE GUI_BITMAP bmMenu_Icon;//������
extern GUI_CONST_STORAGE GUI_BITMAP bmSystem_Setting_Icon;//ϵͳ����
extern GUI_CONST_STORAGE GUI_BITMAP bmLog_Setting_Icon;//��־����


struct System_Set_Value{
	
	WM_HWIN System_Set;
//	GUI_BITMAP Menu;//������
//	GUI_BITMAP Return;//����
//	GUI_BITMAP Connect;//�ⲿ����
//	GUI_BITMAP Time;//�����޸�
//	GUI_BITMAP Calibrate;//ϵ��У׼
//	GUI_BITMAP Reposition;//ϵͳ��λ
};

/*System_Setҳ��*/
extern GUI_CONST_STORAGE GUI_BITMAP bmLink_Icon;//�ⲿ����
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn_Icon;//����
extern GUI_CONST_STORAGE GUI_BITMAP bmSystem_Calender_Icon;//�����޸�
extern GUI_CONST_STORAGE GUI_BITMAP bmSystem_Calibrate_Icon;//ϵ��У׼
extern GUI_CONST_STORAGE GUI_BITMAP bmSystem_Repositon_Icon;//ϵͳ��λ

/*Monitorҳ��*/
struct Monitor_Value{
	
	WM_HWIN Monitor;
	
	char Task_Num[10];
	char Nm[10];
	char Deg[10];
	char Num_Mon[10];
	
	long hTimer;
	
	int 		X_Block;
	float   Y_Block;
};

extern GUI_CONST_STORAGE GUI_BITMAP bmConnect_Icon;//���ӱ�ʶ

/*Disposition����*/
struct Disposition_Value{
	
	WM_HWIN Disposition;
	WM_HWIN Disposition_Child;
	int Task_Sel;
	uint16_t Task_Sort;
	
	char Nm_Min[10];
	char Nm_Max[10];
	char Nm_Target[10];
	char Deg_Min[10];
	char Deg_Max[10];
	char Deg_Target[10];
	
	WM_HWIN NmMin;
	WM_HWIN NmMax;
	WM_HWIN NmTarget;
	WM_HWIN DegMin;
	WM_HWIN DegMax;
	WM_HWIN DegTarget;
	
	int flag_Nm;
	int flag_Deg;
	
	int 		X_Block;
	float   Y_Block;
	
	WM_HWIN Step_H;
	WM_HWIN Speed_H;
	WM_HWIN Num_H;
	
	char Step[5];
	char Speed[10];
	char Num[5];
	
	long hTimer;
};
extern GUI_CONST_STORAGE GUI_BITMAP bmDeg_Max_Icon;//�Ƕ�MAX
extern GUI_CONST_STORAGE GUI_BITMAP bmDeg_Min_Icon;//�Ƕ�MIN
extern GUI_CONST_STORAGE GUI_BITMAP bmDeg_Target_Icon;//�Ƕ�Ŀ��
extern GUI_CONST_STORAGE GUI_BITMAP bmDel_Icon;//ɾ������
extern GUI_CONST_STORAGE GUI_BITMAP bmAdd_Icon;//�������
extern GUI_CONST_STORAGE GUI_BITMAP bmNm_Max_Icon;//Ť��MAX
extern GUI_CONST_STORAGE GUI_BITMAP bmNm_Min_Icon;//Ť��MIN
extern GUI_CONST_STORAGE GUI_BITMAP bmNm_Target_Icon;//Ť��Ŀ��
extern GUI_CONST_STORAGE GUI_BITMAP bmTask_Next_Icon;//����л�
extern GUI_CONST_STORAGE GUI_BITMAP bmTask_Prev_Icon;//��ǰ�л�

/*�ⲿ��������*/
struct Connect_Value{
	
	WM_HWIN Connect;	
	WM_HWIN IP_H;
	WM_HWIN DCIM_H;
	WM_HWIN IP_Target_H;
	WM_HWIN Port_H;	
	
	char IP[20];
	char DCIM[20];
	char IP_Target[20];
	char Port[5];	
}; 


extern GUI_CONST_STORAGE GUI_BITMAP bmSave_Icon;//����

/*�����޸�*/
struct Canlender_Value{
	
	WM_HWIN Canlender;
	
	WM_HWIN Year_H;
	WM_HWIN Month_H;
	WM_HWIN Day_H;
	WM_HWIN Hour_H;
	WM_HWIN Minute_H;
	WM_HWIN Second_H;
	
	char Year[10];
	char Month[10];
	char Day[10];
	char Hour[10];
	char Minute[10];
	char Second[10];
};

/*ϵ��У׼*/
struct Calibrate_Value{
	
	WM_HWIN Calibrate;
	
	WM_HWIN FloatHigh_H;
	WM_HWIN SlipperyTeeth_H;
	WM_HWIN Exemption_H;
	WM_HWIN Locking_H;
	WM_HWIN Speed_H;
	
	char FloatHigh[10];
	char SlipperyTeeth[10];
	char Exemption[10];
	char Locking[10];
	char Speed[10];
	
};

/*��־�������*/
struct Log_Value{
	
	WM_HWIN Log;
	WM_HWIN Log_Child;
	
	WM_HWIN Log_DateTime;
	WM_HWIN Key;
	
	WM_HWIN Year_H;
	WM_HWIN Month_H;
	WM_HWIN Day_H;
	WM_HWIN Save_OK;
	
	char Log_TimeSel[20];
	char Key_Search[10];
	
	char Year[10];
	char Month[10];
	char Day[10];
};

extern GUI_CONST_STORAGE GUI_BITMAP bmSearch_Icon;//����
extern GUI_CONST_STORAGE GUI_BITMAP bmCalender_Icon;//����ѡ��

/*��������*/
typedef struct Task{
	int Task_Num;
	uint32_t Task_Sort;
	
	char Nm_Min[10];
	char Nm_Max[10];
	char Nm_Target[10];
	char Deg_Min[10];
	char Deg_Max[10];
	char Deg_Target[10];
	
	struct Task* next;
	struct Task* prev;
}_List;

#endif