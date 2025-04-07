#ifndef __BSP_CODE_H
#define	__BSP_CODE_H


#include "GUI.h"
#include "DIALOG.h"
#include "stm32f4xx.h"
void DemoBarcode(const char *str);
void DrawCode128(int x0, int y0, int moduleWidth, int height, const char* text) ;

typedef enum {
    CODE128_A,
    CODE128_B,
    CODE128_C
} CODE128_Type;

char* CODE128_Encode(const char* input, CODE128_Type type);
void DrawBarcode(const char* encoded, int x, int y, int moduleWidth, int height) ;

#endif