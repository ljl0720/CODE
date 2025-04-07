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
/*Login界面显示*/
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
	
//	GUI_BITMAP Logo;//设备LOGO
//	GUI_BITMAP Exception;//警告
//	GUI_BITMAP User;//用户
//	GUI_BITMAP Term;//工作时间
//	GUI_BITMAP Quit;//退出
//	GUI_BITMAP Monitor;//监控
//	GUI_BITMAP Disposition;//配置
//	GUI_BITMAP Manage;//管理
//	GUI_BITMAP Statisticsv;//统计
//	GUI_BITMAP Analyse;//分析
	
};
/*Menu界面显示*/
extern GUI_CONST_STORAGE GUI_BITMAP bmAnalyse_Icon;//分析
extern GUI_CONST_STORAGE GUI_BITMAP bmWarning_Icon;//警告
extern GUI_CONST_STORAGE GUI_BITMAP bmUser_Icon;//用户
extern GUI_CONST_STORAGE GUI_BITMAP bmDesprition_Icon;//配置
extern GUI_CONST_STORAGE GUI_BITMAP bmMenu_Info_Icon;//帮助
extern GUI_CONST_STORAGE GUI_BITMAP bmQuit_Icon;//退出
extern GUI_CONST_STORAGE GUI_BITMAP bmStatistics_Icon;//统计
extern GUI_CONST_STORAGE GUI_BITMAP bmMonitor_Icon;//监控
extern GUI_CONST_STORAGE GUI_BITMAP bmSetting_Icon;//管理
extern GUI_CONST_STORAGE GUI_BITMAP bmWorking_Time_Icon;//时长
extern GUI_CONST_STORAGE GUI_BITMAP bmLogo_Icon;//Logo
extern GUI_CONST_STORAGE GUI_BITMAP bmLogo_mini_Icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmMenu_Info_Icon;//帮助
extern GUI_CONST_STORAGE GUI_BITMAP bmClose_Icon;//关闭


struct Manage_Sel_Value{
	
	WM_HWIN Manage_Sel;
//	GUI_BITMAP Menu;//主界面
//	GUI_BITMAP User_Manage;//用户管理
//	GUI_BITMAP System_Manage;//系统管理
//	GUI_BITMAP Log_Manage;//日志管理
};

/*Manage_Sel页面*/
extern GUI_CONST_STORAGE GUI_BITMAP bmUser_Setting_Icon;//用户管理
extern GUI_CONST_STORAGE GUI_BITMAP bmMenu_Icon;//主界面
extern GUI_CONST_STORAGE GUI_BITMAP bmSystem_Setting_Icon;//系统管理
extern GUI_CONST_STORAGE GUI_BITMAP bmLog_Setting_Icon;//日志管理


struct System_Set_Value{
	
	WM_HWIN System_Set;
//	GUI_BITMAP Menu;//主界面
//	GUI_BITMAP Return;//返回
//	GUI_BITMAP Connect;//外部链接
//	GUI_BITMAP Time;//日期修改
//	GUI_BITMAP Calibrate;//系数校准
//	GUI_BITMAP Reposition;//系统复位
};

/*System_Set页面*/
extern GUI_CONST_STORAGE GUI_BITMAP bmLink_Icon;//外部链接
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn_Icon;//返回
extern GUI_CONST_STORAGE GUI_BITMAP bmSystem_Calender_Icon;//日期修改
extern GUI_CONST_STORAGE GUI_BITMAP bmSystem_Calibrate_Icon;//系数校准
extern GUI_CONST_STORAGE GUI_BITMAP bmSystem_Repositon_Icon;//系统复位

/*Monitor页面*/
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

extern GUI_CONST_STORAGE GUI_BITMAP bmConnect_Icon;//链接标识

/*Disposition界面*/
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
extern GUI_CONST_STORAGE GUI_BITMAP bmDeg_Max_Icon;//角度MAX
extern GUI_CONST_STORAGE GUI_BITMAP bmDeg_Min_Icon;//角度MIN
extern GUI_CONST_STORAGE GUI_BITMAP bmDeg_Target_Icon;//角度目标
extern GUI_CONST_STORAGE GUI_BITMAP bmDel_Icon;//删除任务
extern GUI_CONST_STORAGE GUI_BITMAP bmAdd_Icon;//添加任务
extern GUI_CONST_STORAGE GUI_BITMAP bmNm_Max_Icon;//扭矩MAX
extern GUI_CONST_STORAGE GUI_BITMAP bmNm_Min_Icon;//扭矩MIN
extern GUI_CONST_STORAGE GUI_BITMAP bmNm_Target_Icon;//扭矩目标
extern GUI_CONST_STORAGE GUI_BITMAP bmTask_Next_Icon;//向后切换
extern GUI_CONST_STORAGE GUI_BITMAP bmTask_Prev_Icon;//向前切换

/*外部链接配置*/
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


extern GUI_CONST_STORAGE GUI_BITMAP bmSave_Icon;//保存

/*日期修改*/
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

/*系数校准*/
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

/*日志管理界面*/
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

extern GUI_CONST_STORAGE GUI_BITMAP bmSearch_Icon;//查找
extern GUI_CONST_STORAGE GUI_BITMAP bmCalender_Icon;//日期选择

/*任务链表*/
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