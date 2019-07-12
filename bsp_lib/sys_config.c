/******************************************************************
**	  ���������壨V1.0��
**	  ϵͳʱ�������ļ�
**
**	  ��    ̳��bbs.openmcu.com
**	  ��    ����www.openmcu.com
**	  ��    �䣺support@openmcu.com
**
**    ��    ����V1.0
**	  ��    �ߣ�FXL
**	  �������:	2012.7.20
********************************************************************/
#include "stm32f10x.h"
#include <stdio.h>
#include "sys_config.h"	 
/********************************************************************************************
*�������ƣ�void RCC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����ϵͳʱ�ӳ�ʼ������
*	       RCC_HSICmd(ENABLE);//ʹ���ڲ����پ��� ;
* 	       RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);//ѡ���ڲ�����ʱ����Ϊϵͳʱ��SYSCLOCK=8MHZ	
*	       RCC_HCLKConfig(RCC_SYSCLK_Div1);       //ѡ��HCLKʱ��ԴΪϵͳʱ��SYYSCLOCK
*  	       RCC_PCLK1Config(RCC_HCLK_Div4);        //APB1ʱ��Ϊ2M 
*  	       RCC_PCLK2Config(RCC_HCLK_Div4);        //APB2ʱ��Ϊ2M
*  	       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//ʹ��APB2����GPIOBʱ��
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

	SysTick_Config(SystemCoreClock / 1000);//1ms�δ�
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
		FLASH_ProgramHalfWord((0x8008000 +count*2),addr[count]);  //flash  Ϊһ���ֽڴ洢��16λ���ݱ����ַ��2
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
	
	//DMA1ͨ��1����  
	DMA_DeInit(DMA1_Channel1);  
	//�����ַ  
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;  
	//�ڴ��ַ  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;  
	//dma���䷽����  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
	//����DMA�ڴ���ʱ�������ĳ���  
	DMA_InitStructure.DMA_BufferSize = 16;  
	//����DMA���������ģʽ��һ������  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	//����DMA���ڴ����ģʽ  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	//���������ֳ�  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  
	//�ڴ������ֳ�  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  
	//����DMA�Ĵ���ģʽ���������ϵ�ѭ��ģʽ  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  
	//����DMA�����ȼ���  
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
	//����DMA��2��memory�еı����������  
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 	
	//ʹ��ͨ��1  
	DMA_Cmd(DMA1_Channel1, ENABLE);	

	//ADC1����  
	//��������ģʽ  
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  
	//ɨ�跽ʽ  
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //��ͨ��
	//����ת��  
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  
	//�ⲿ������ֹ  
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  
	//�����Ҷ���  
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  
	//����ת����ͨ����  
	ADC_InitStructure.ADC_NbrOfChannel = 1;  
	ADC_Init(ADC1, &ADC_InitStructure);  
      
	//����ģʽͨ������  
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8 , 1, ADC_SampleTime_239Cycles5);        
	//ʹ��ADC1��DMA  
	ADC_DMACmd(ADC1, ENABLE);        
	//ʹ��ADC1  
	ADC_Cmd(ADC1, ENABLE);  
      
	//ʹ��ADC1��λУ׼�Ĵ���    
	ADC_ResetCalibration(ADC1);  
	//���У׼�Ĵ����Ƿ�λ���  
	while(ADC_GetResetCalibrationStatus(ADC1));  
      
	//��ʼУ׼  
	ADC_StartCalibration(ADC1);  
	//����Ƿ�У׼���  
	while(ADC_GetCalibrationStatus(ADC1));  
       
	//����ADC1�����ת��  
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
