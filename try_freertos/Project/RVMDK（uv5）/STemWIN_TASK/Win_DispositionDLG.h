#ifndef __DISPOSITION_H
#define __DISPOSITION_H	
#include "GUI.h"
#include "DIALOG.h"

WM_HWIN CreateWin_Disposition(void);

#define KEY1_EVENT  (0x01 << 0)//�����¼������λ0
#define KEY2_EVENT  (0x01 << 1)//�����¼������λ1
#define KEY3_EVENT  (0x01 << 2)//�����¼������λ2
#define KEY4_EVENT  (0x01 << 4)//�����¼������λ3
#endif