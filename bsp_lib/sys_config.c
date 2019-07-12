/******************************************************************
**	  红龙开发板（V1.0）
**	  系统时钟配置文件
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.0
**	  作    者：FXL
**	  完成日期:	2012.7.20
********************************************************************/
#include "stm32f10x.h"
#include <stdio.h>
#include "sys_config.h"	 
/********************************************************************************************
*函数名称：void RCC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：系统时钟初始化配置
*	       RCC_HSICmd(ENABLE);//使能内部高速晶振 ;
* 	       RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);//选择内部高速时钟作为系统时钟SYSCLOCK=8MHZ	
*	       RCC_HCLKConfig(RCC_SYSCLK_Div1);       //选择HCLK时钟源为系统时钟SYYSCLOCK
*  	       RCC_PCLK1Config(RCC_HCLK_Div4);        //APB1时钟为2M 
*  	       RCC_PCLK2Config(RCC_HCLK_Div4);        //APB2时钟为2M
*  	       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//使能APB2外设GPIOB时钟
*******************************************************************************************/
void RCC_Configuration(void) 
{
	/* RCC system reset(for debug purpose) */
  	RCC_DeInit();

  	/* Enable HSE */
  	RCC_HSEConfig(RCC_HSE_ON);

  	/* Wait till HSE is ready */
  	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);
		
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
												 RCC_APB2Periph_SPI1  | RCC_APB2Periph_AFIO, ENABLE);

	SysTick_Config(SystemCoreClock / 1000);//1ms滴答
	return;
}

void NVIC_Configuration(void)
{	
	#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	return;
}


void save_addr(u8 *addr){
	u32 count=0;
	
	RCC_HSICmd(ENABLE);	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);	
	FLASH_ErasePage(0x8008000);
	while(count < 5){
		FLASH_ProgramHalfWord((0x8008000 +count*2),addr[count]);  //flash  为一个字节存储，16位数据必须地址加2
		count++;
	}
	FLASH_Lock();

	return;
}

int read_addr(u8 *addr){
	u32 count=0;
	u16 tmp;
	
	while(count < 5){
		tmp = *(u16 *)(0x8008000 + count*2);
		if(tmp > 255){
			return -1;
		}
		addr[count] = tmp&0xFF;
		count++;
	}
	
	return 0;
}

#define ADC1_DR_ADDRESS    ((u32)0x4001244C) 
volatile uint16_t ADCConvertedValue[16]; 

void ad_init(void){
	ADC_InitTypeDef ADC_InitStructure;  
	DMA_InitTypeDef DMA_InitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	//DMA1通道1配置  
	DMA_DeInit(DMA1_Channel1);  
	//外设地址  
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;  
	//内存地址  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;  
	//dma传输方向单向  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
	//设置DMA在传输时缓冲区的长度  
	DMA_InitStructure.DMA_BufferSize = 16;  
	//设置DMA的外设递增模式，一个外设  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	//设置DMA的内存递增模式  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	//外设数据字长  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  
	//内存数据字长  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  
	//设置DMA的传输模式：连续不断的循环模式  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  
	//设置DMA的优先级别  
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
	//设置DMA的2个memory中的变量互相访问  
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 	
	//使能通道1  
	DMA_Cmd(DMA1_Channel1, ENABLE);	

	//ADC1配置  
	//独立工作模式  
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  
	//扫描方式  
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //单通道
	//连续转换  
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  
	//外部触发禁止  
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  
	//数据右对齐  
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  
	//用于转换的通道数  
	ADC_InitStructure.ADC_NbrOfChannel = 1;  
	ADC_Init(ADC1, &ADC_InitStructure);  
      
	//规则模式通道配置  
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8 , 1, ADC_SampleTime_239Cycles5);        
	//使能ADC1的DMA  
	ADC_DMACmd(ADC1, ENABLE);        
	//使能ADC1  
	ADC_Cmd(ADC1, ENABLE);  
      
	//使能ADC1复位校准寄存器    
	ADC_ResetCalibration(ADC1);  
	//检查校准寄存器是否复位完毕  
	while(ADC_GetResetCalibrationStatus(ADC1));  
      
	//开始校准  
	ADC_StartCalibration(ADC1);  
	//检测是否校准完毕  
	while(ADC_GetCalibrationStatus(ADC1));  
       
	//开启ADC1的软件转换  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 

	return;
}

uint16_t get_ad_val(void){
	int i;
	uint32_t sum = 0;
	for(i=0;i<16;i++){
		sum += ADCConvertedValue[i];
	}
	return sum>>4;
}
