//********************************************************************************
#ifndef __12864_H
#define __12864_H			  	    	
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
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
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_6)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

//ʹ��4�ߴ��нӿ�ʱʹ�� 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)
#endif

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

extern void LCD_Init(void);
//OLED�����ú���

extern void Init_display(void);

#endif

