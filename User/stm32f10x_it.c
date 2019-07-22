/*******************************************************************************
** 文件名: 		stm32f10x_it.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 4.20
** 作者: 		吴国炎
** 生成日期: 	2011-04-10
** 功能:		中断源文件
** 相关文件:	无
** 修改日志：	2011-04-10   创建文档
*******************************************************************************/
#include "stm32f10x_it.h"

typedef struct key{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	unsigned int delay;    //
	unsigned int touch_flag;//按下
	unsigned int untouch_flag;//松开 防止连续触发
	const char key_value;
	//char key_value;
	//char busy;
}key_t;

u8 key6_flag=0;
static unsigned int cnt = 0;
static unsigned int delay_cnt = 0;
											 //                   delay  touch  untouch  key_value
//static key_t key[14] = {{GPIOC, GPIO_Pin_6,   0,     0,      1,       2},
//												{GPIOC, GPIO_Pin_7,   0,     0,      1,       5},
//												{GPIOC, GPIO_Pin_8,   0,     0,      1,       8},
//                        {GPIOC, GPIO_Pin_9,   0,     0,      1,       0},
//                        {GPIOA, GPIO_Pin_8,   0,     0,      1,       11},
//                        {GPIOA, GPIO_Pin_9,   0,     0,      1,       9},
//                        {GPIOA, GPIO_Pin_10,  0,     0,      1,       6},
//                        {GPIOA, GPIO_Pin_11,  0,     0,      1,       3},
//                        {GPIOA, GPIO_Pin_12,  0,     0,      1,       13},
//                        {GPIOB, GPIO_Pin_11,  0,     0,      1,       12},
//												{GPIOB, GPIO_Pin_12,  0,     0,      1,       1},
//												{GPIOB, GPIO_Pin_13,  0,     0,      1,       4},
//												{GPIOB, GPIO_Pin_14,  0,     0,      1,       7},
//												{GPIOB, GPIO_Pin_15,  0,     0,      1,       10},
//												 };

static key_t key[14] = {{GPIOC, GPIO_Pin_6,   0,     0,      1,       2},
												{GPIOC, GPIO_Pin_7,   0,     0,      1,       5},
												{GPIOC, GPIO_Pin_8,   0,     0,      1,       8},
                        {GPIOC, GPIO_Pin_9,   0,     0,      1,       0},
                        {GPIOA, GPIO_Pin_11,   0,     0,      1,       11},
                        {GPIOA, GPIO_Pin_10,   0,     0,      1,       9},
                        {GPIOA, GPIO_Pin_9,  0,     0,      1,       6},
                        {GPIOA, GPIO_Pin_8,  0,     0,      1,       3},//key
                        {GPIOA, GPIO_Pin_12,  0,     0,      1,       13},
                        {GPIOB, GPIO_Pin_11,  0,     0,      1,       12},
												{GPIOB, GPIO_Pin_12,  0,     0,      1,       1},
												{GPIOB, GPIO_Pin_13,  0,     0,      1,       4},
												{GPIOB, GPIO_Pin_14,  0,     0,      1,       7},
												{GPIOB, GPIO_Pin_15,  0,     0,      1,       10},
												 };


void put_key(char button);

unsigned int Get_SysTick(void){
	return cnt;
}

void Delay_Ms(unsigned int delay){

	delay_cnt = 0;
	while(delay>delay_cnt);
	return;
}

//防抖处理
void key_prograss(void){
	int i;

#if 0	
	if(key.busy){
		if(!GPIO_ReadInputDataBit(key.GPIOx, key.GPIO_Pin)){
				key.delay++;
		}else{
				key.busy = 0;
		}
		if(key.delay > 50){
			put_key(key.key_value);
			key.busy = 0;
		}
	}
#endif
	for(i=0; i<14; i++){
  		if(!GPIO_ReadInputDataBit(key[i].GPIOx, key[i].GPIO_Pin)){
			if(!GPIO_ReadInputDataBit(key[i].GPIOx, key[i].GPIO_Pin))
			{
			
			
			}
			key[i].touch_flag = 1;
			key[i].delay++;
			if((key[i].delay > 50) && key[i].untouch_flag){
				put_key(key[i].key_value);
				key[i].untouch_flag = 0;
			}
		}else{
			key[i].delay = 0;
			key[i].untouch_flag = 1;
			key[i].touch_flag = 0;
		}
	}
	return;
}

/******************************************************************************/
/*            Cortex-M3 处理器的相关服务函数                                  */
/******************************************************************************/

/*******************************************************************************
  * @函数名称	NMI_Handler
  * @函数说明   不可屏蔽中断发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
  * @函数名称	HardFault_Handler
  * @函数说明   硬件错误发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void HardFault_Handler(void)
{
    //当硬件错误发生时进入死循环 
    while (1)
    {
    }
}

/*******************************************************************************
  * @函数名称	MemManage_Handler
  * @函数说明   内存管理中断发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void MemManage_Handler(void)
{
    //当内存管理中断发生时进入死循环 
    while (1)
    {
    }
}

/*******************************************************************************
  * @函数名称	BusFault_Handler
  * @函数说明   总线错误发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void BusFault_Handler(void)
{
    //当总线错误发生时进入死循环 
    while (1)
    {
    }
}

/*******************************************************************************
  * @函数名称	UsageFault_Handler
  * @函数说明   Usage 错误错误发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void UsageFault_Handler(void)
{
    //当Usage 错误发生时进入死循环 
    while (1)
    {
    }
}

/*******************************************************************************
  * @函数名称	SVC_Handler
  * @函数说明   SVC调用中断发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
  * @函数名称	DebugMon_Handler
  * @函数说明   调试监控中断发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
  * @函数名称	PendSV_Handler
  * @函数说明   Pend SV中断发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
  * @函数名称	SysTick_Handler
  * @函数说明   SysTick中断发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void SysTick_Handler(void)
{
	cnt++;
	delay_cnt++;
	//TimingDelay_Decrement();
	key_prograss();
	return;
}

/******************************************************************************/
/*                 STM32F10x 外设 中断 服务函数                               */
/*  增加需要的外设中断函数在下面。中断的函数名字都已经在startup_stm32f10x_xx.s*/
/*  的文件中定义好了，请参照它来写。                                          */
/******************************************************************************/


//用户增加自己的中断服务程序这下面。

void EXTI0_IRQHandler(void)
{
	#if 0
	if(!key.busy){
		key.GPIOx = GPIOA;
		key.GPIO_Pin = GPIO_Pin_0;
		key.key_value = 0;
		key.delay = 0;
		key.busy = 1;
	}	
	#endif
	EXTI_ClearITPendingBit(EXTI_Line0);
	return;
}

void EXTI9_5_IRQHandler(void)
{

	EXTI_ClearITPendingBit(EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9 | EXTI_Line10);
	return;
}

void EXTI15_10_IRQHandler(void)
{
	#if 0
	if(!key.busy){
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)){
			key.GPIOx = GPIOA;
			key.GPIO_Pin = GPIO_Pin_10;
			key.key_value = 6;
		}else if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)){
			key.GPIOx = GPIOA;
			key.GPIO_Pin = GPIO_Pin_11;
			key.key_value = 3;
		}else if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)){
			key.GPIOx = GPIOA;
			key.GPIO_Pin = GPIO_Pin_12;
			key.key_value = 13;
		}else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)){
			key.GPIOx = GPIOB;
			key.GPIO_Pin = GPIO_Pin_11;
			key.key_value = 12;
		}else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
			key.GPIOx = GPIOB;
			key.GPIO_Pin = GPIO_Pin_12;
			key.key_value = 1;
		}else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)){
			key.GPIOx = GPIOB;
			key.GPIO_Pin = GPIO_Pin_13;
			key.key_value = 4;
		}else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)){
			key.GPIOx = GPIOB;
			key.GPIO_Pin = GPIO_Pin_14;
			key.key_value = 7;
		}else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)){
			key.GPIOx = GPIOB;
			key.GPIO_Pin = GPIO_Pin_15;
			key.key_value = 10;
		}
		
		
		key.delay = 0;
		key.busy = 1;
	}
	#endif
	EXTI_ClearITPendingBit(EXTI_Line10 | EXTI_Line11 | EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15);
	return;
}

/***********************************文件结束***********************************/
