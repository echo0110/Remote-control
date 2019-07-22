/*******************************************************************************
** �ļ���: 		stm32f10x_it.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.20
** ����: 		�����
** ��������: 	2011-04-10
** ����:		�ж�Դ�ļ�
** ����ļ�:	��
** �޸���־��	2011-04-10   �����ĵ�
*******************************************************************************/
#include "stm32f10x_it.h"

typedef struct key{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	unsigned int delay;    //
	unsigned int touch_flag;//����
	unsigned int untouch_flag;//�ɿ� ��ֹ��������
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

//��������
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
/*            Cortex-M3 ����������ط�����                                  */
/******************************************************************************/

/*******************************************************************************
  * @��������	NMI_Handler
  * @����˵��   ���������жϷ������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
  * @��������	HardFault_Handler
  * @����˵��   Ӳ�����������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void HardFault_Handler(void)
{
    //��Ӳ��������ʱ������ѭ�� 
    while (1)
    {
    }
}

/*******************************************************************************
  * @��������	MemManage_Handler
  * @����˵��   �ڴ�����жϷ������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void MemManage_Handler(void)
{
    //���ڴ�����жϷ���ʱ������ѭ�� 
    while (1)
    {
    }
}

/*******************************************************************************
  * @��������	BusFault_Handler
  * @����˵��   ���ߴ��������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void BusFault_Handler(void)
{
    //�����ߴ�����ʱ������ѭ�� 
    while (1)
    {
    }
}

/*******************************************************************************
  * @��������	UsageFault_Handler
  * @����˵��   Usage ������������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void UsageFault_Handler(void)
{
    //��Usage ������ʱ������ѭ�� 
    while (1)
    {
    }
}

/*******************************************************************************
  * @��������	SVC_Handler
  * @����˵��   SVC�����жϷ������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
  * @��������	DebugMon_Handler
  * @����˵��   ���Լ���жϷ������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
  * @��������	PendSV_Handler
  * @����˵��   Pend SV�жϷ������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
  * @��������	SysTick_Handler
  * @����˵��   SysTick�жϷ������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
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
/*                 STM32F10x ���� �ж� ������                               */
/*  ������Ҫ�������жϺ��������档�жϵĺ������ֶ��Ѿ���startup_stm32f10x_xx.s*/
/*  ���ļ��ж�����ˣ����������д��                                          */
/******************************************************************************/


//�û������Լ����жϷ�����������档

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

/***********************************�ļ�����***********************************/
