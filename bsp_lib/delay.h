#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);	  //单位10us
//void delay_ms(__IO u32 nTime); 
extern void delay(int time);


void delay_Init(u8 SYSCLK);

void delay_uss(u32 nus);
void delay_mss(u16 nms);


#define Delay_ms(x) Delay_us(1000*x)//单位ms

#endif 
/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/
 