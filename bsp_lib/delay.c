/*-------------------------------------------------------------------------------
文件名称：SysTick.c
文件描述：SysTick 系统滴答时钟1us中断函数库,中断时间可自由配置，
          常用的有 1us 10us 1ms 中断。        
备    注：程序默认使用72M时钟，无分频 
---------------------------------------------------------------------------------*/
#include "delay.h"

static __IO u32 TimingDelay;


//#include "SysTick.h"

static u8  fac_us=0;							//us?????			   
static u16 fac_ms=0;							//ms?????

//void SysTick_Init(u8 SYSCLK)


/*-------------------------------------------------------------------------------
程序名称：SysTick_Init
程序描述：启动系统滴答定时器 SysTick
输入参数：无
返回参数：无
备    注：使用此功能之前需要调用此函数，初始化寄存器等参数
---------------------------------------------------------------------------------*/
//void SysTick_Init(void)
//{
//	 // SystemFrequency / 1000     1ms中断一次
//	 // SystemFrequency / 100000	 10us中断一次
//	 // SystemFrequency / 1000000  1us中断一次
//	 
//	if (SysTick_Config(SystemCoreClock / 1000000))	// 1ms中断一次
//	{ 
//		/* Capture error */ 
//		while (1);
//	}
//}


/*-------------------------------------------------------------------------------
程序名称：Delay_us
程序描述：延时nTime us  
输入参数：nTime
返回参数：无
备    注：无
---------------------------------------------------------------------------------*/
void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}

/*-------------------------------------------------------------------------------
程序名称：Delay_ms
程序描述：延时nTime us  
输入参数：nTime
返回参数：无
备    注：无
---------------------------------------------------------------------------------*/
//void delay_ms(__IO u32 nTime)
//{ 
//	TimingDelay = nTime;	

//	while(TimingDelay != 0);
//}

/*-------------------------------------------------------------------------------
程序名称：TimingDelay_Decrement
程序描述：数字递减直到0  
输入参数：无
返回参数：无
备    注：需要在中断函数SysTick_Handler()中调用
---------------------------------------------------------------------------------*/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
	TimingDelay--;
	}
}
/*-------------------------------------------------------------------------------
程序名称：delay
程序描述：计数延时  
输入参数：无
返回参数：无
备    注：LORA中断读寄存器值
---------------------------------------------------------------------------------*/
#if 0
void delay(int time)
{
 int i;
 while(time--)
 {
  i=500;
  while(i--);
 }
}
#endif


//??:???????misc.c??????
//???????
//SYSTICK??????AHB???1/8
//SYSCLK:??????
void delay_Init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;					
	fac_ms=(u16)fac_us*1000;				   
}								    


/*******************************************************************************
* ? ? ?: delay_us
* ????	: us??,
* ?    ?: nus:????us?
??:nus??,????798915us(????2^24/fac_us@fac_us=21)
* ?    ?: ?
*******************************************************************************/		    								   
void delay_uss(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//????	  		 
	SysTick->VAL=0x00;        					//?????
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//????	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//??????   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//?????
	SysTick->VAL =0X00;      					 //?????	 
}


/*******************************************************************************
* ? ? ?         : delay_ms
* ????         : ms??,
* ?    ?         : nms:????ms?
                    ??:nms??,SysTick->LOAD?24????,
                    ????0xffffff*8*1000/SYSCLK
                    ?72M???,nms<=1864ms 
* ?    ?         : ?
*******************************************************************************/
void delay_mss(u16 nms)
{                 
    u32 temp;          
    SysTick->LOAD=(u32)nms*fac_ms;              //????(SysTick->LOAD?24bit)
    SysTick->VAL =0x00;                         //?????
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;    //????  
    do
    {
        temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));       //??????   
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;    //?????
    SysTick->VAL =0X00;                         //?????         
} 



/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/

