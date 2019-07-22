#ifndef __lcd_DISPLAY_H__
#define __lcd_DISPLAY_H__
#include "stm32f10x.h"
void lcd_clear_line(int x);
void lcd_show_line3(u8 index,  char chr);
//void lcd_show_line1(unsigned char x,unsigned char y,unsigned char ch[]);
void lcd_show_line2(u8 index, char chr);
void lcd_show_line1(u8 x, u8 *chr);

void lcd_ShowChar(u8 x,u8 y,u8 chr);



#endif
