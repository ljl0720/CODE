#ifndef __DISPOSITION_H
#define __DISPOSITION_H	
#include "GUI.h"
#include "DIALOG.h"

WM_HWIN CreateWin_Disposition(void);

#define KEY1_EVENT  (0x01 << 0)//设置事件掩码的位0
#define KEY2_EVENT  (0x01 << 1)//设置事件掩码的位1
#define KEY3_EVENT  (0x01 << 2)//设置事件掩码的位2
#define KEY4_EVENT  (0x01 << 4)//设置事件掩码的位3
#endif