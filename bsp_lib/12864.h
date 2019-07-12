//********************************************************************************
#ifndef __12864_H
#define __12864_H			  	    	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	








#if 1
//-----------------OLED端口定义----------------  					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_6)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

//使用4线串行接口时使用 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)
#endif

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

extern void LCD_Init(void);
//OLED控制用函数

extern void Init_display(void);

#endif

