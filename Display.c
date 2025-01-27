#include "MKL05Z4.h"
#include "lcd1602.h"
#include <stdio.h>

void Display(uint8_t cursor_l, uint8_t cursor_r, uint8_t value, uint8_t width, char char_value[], char display[] ) {
	LCD1602_SetCursor(cursor_l,cursor_r);
	LCD1602_Print(char_value);
	sprintf(display,"%0*d",width,value);
	LCD1602_Print(display);
}