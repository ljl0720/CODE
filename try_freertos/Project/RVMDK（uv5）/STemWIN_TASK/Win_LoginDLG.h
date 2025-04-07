#ifndef CONNECT_WIN
#define CONNECT_WIN
#include "DIALOG.h"
#include "GUI.h"

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
	
};


struct Keyboard_Value{
	 
	 WM_HWIN Keyboard;
	 
 };

struct Menu_Value{
	
	WM_HWIN Menu;
	
};


#endif