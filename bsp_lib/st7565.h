#ifndef __ST7565_H
#define __ST7565_H

#include "stm32f10x.h"

#define lcd_bk_high() GPIO_SetBits(GPIOB, GPIO_Pin_10);     
#define lcd_bk_low()  GPIO_ResetBits(GPIOB, GPIO_Pin_10); 

#define lcd_cs_high() GPIO_SetBits(GPIOA, GPIO_Pin_0);     
#define lcd_cs_low()  GPIO_ResetBits(GPIOA, GPIO_Pin_0); 

#define lcd_rst_high() GPIO_SetBits(GPIOA, GPIO_Pin_1); //rst     
#define lcd_rst_low()  GPIO_ResetBits(GPIOA, GPIO_Pin_1); 

#define lcd_sck_high() GPIO_SetBits(GPIOB, GPIO_Pin_6);     
#define lcd_sck_low()  GPIO_ResetBits(GPIOB, GPIO_Pin_6); 

#define lcd_sda_high() GPIO_SetBits(GPIOB, GPIO_Pin_7);     
#define lcd_sda_low()  GPIO_ResetBits(GPIOB, GPIO_Pin_7);

#define lcd_a0_high() GPIO_SetBits(GPIOA, GPIO_Pin_2);     
#define lcd_a0_low()  GPIO_ResetBits(GPIOA, GPIO_Pin_2); 

extern void Init_St7565(void);

void delayms(uint16_t ms);
void Init_St7565(void);

void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);

void St7565_WriteCmd(uint8_t com);

void St7565_WriteDat(uint8_t dat);

void LCD_show_line1_bmp(u8 index);
void LCD_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

#endif
