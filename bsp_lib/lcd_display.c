/*************************************************************************************
*说明：oled的分辨率：64X128 汉字：16X16 其他字符：8X16，一行可以显示16个字符，8个汉字
*
*
*
*
**************************************************************************************/
#include "stm32f10x.h"
#include "oled.h"
#include "lcd_display.h"
#include "button.h"
#include "display.h"
#include "st7565.h"
#include "12864.h"

extern const unsigned char F8X16[];

extern const unsigned char F6x8[][6];

/********************************************************************************
*line 1, 显示内容--连接状态、输入方式、电量、
*
*连接状态：bmp
*输入方式：数字键-“N” 功能键-“F”  说明：未连接状态时显示N，连接成功后为F，只有在连接成功后可以设置
*电量：
********************************************************************************/
void lcd_show_line1(u8 x, u8 *chr){

	//OLED_ShowString(x, 0, chr);
	LCD_P8x16Str(5,x,chr);
	return;
}


/********************************************************************************
*line 2, 显示内容--
*
*
*
*
********************************************************************************/
void lcd_show_line2(u8 index, char chr){
	static u8 x_pos = 0;

	if(index != INPUT_NUM){
		//oled_clear_line(LINE2);
		lcd_clear_line(LINE2);
	}
	switch(index){
		case INPUT_ADDR:
//			OLED_ShowCHinese(0,LINE2,6);
//			OLED_ShowCHinese(16,LINE2,7);
//			OLED_ShowCHinese(32,LINE2,8);
//			OLED_ShowCHinese(48,LINE2,9);	
//			OLED_ShowCHinese(64,LINE2,10);
//			OLED_ShowCHinese(80,LINE2,11);
//			OLED_ShowCHinese(96,LINE2,12);
//			OLED_ShowCHinese(112,LINE2,13);	
        LCD_ShowCHinese((u8*)"输入信号机地址",0,2);				
			break;
		
		case FUN_LIGHT_OFF:
//			OLED_ShowCHinese(0,LINE2,28);
//			OLED_ShowCHinese(16,LINE2,29);		
        LCD_ShowCHinese((u8*)"关灯",0,2);			
			break;
		case FUN_LIGHT_RED:
//			OLED_ShowCHinese(0,LINE2,24);
//			OLED_ShowCHinese(16,LINE2,25);
        LCD_ShowCHinese((u8*)"全红",0,2);		
			break;
		case FUN_YELLOW_FLASH:
//			OLED_ShowCHinese(0,LINE2,26);
//			OLED_ShowCHinese(16,LINE2,27);
        LCD_ShowCHinese((u8*)"黄闪",0,2);			
			break;
		case FUN_EAST_WEST_STRAIGHT:
//			OLED_ShowCHinese(0,LINE2,30);
//			OLED_ShowCHinese(16,LINE2,31);	
//			OLED_ShowCHinese(32,LINE2,34);
//			OLED_ShowCHinese(48,LINE2,35);
        LCD_ShowCHinese((u8*)"东西直行",0,2);			
			break;	
		case FUN_SOUTH_NORTH_STRAIGHT:
//			OLED_ShowCHinese(0,LINE2,32);
//			OLED_ShowCHinese(16,LINE2,33);	
//			OLED_ShowCHinese(32,LINE2,34);
//			OLED_ShowCHinese(48,LINE2,35);		
        LCD_ShowCHinese((u8*)"南北直行",0,2);			
			break;
		case FUN_EAST_WEST_TURN_LEFT:
//			OLED_ShowCHinese(0,LINE2,30);
//			OLED_ShowCHinese(16,LINE2,31);	
//			OLED_ShowCHinese(32,LINE2,36);
//			OLED_ShowCHinese(48,LINE2,37);	
        LCD_ShowCHinese((u8*)"东西左转",0,2);			
			break;	
		case FUN_SOUTH_NORTH_TURN_LEFT:
//			OLED_ShowCHinese(0,LINE2,32);
//			OLED_ShowCHinese(16,LINE2,33);	
//			OLED_ShowCHinese(32,LINE2,36);
//			OLED_ShowCHinese(48,LINE2,37);
        LCD_ShowCHinese((u8*)"南北左转",0,2);				
			break;
		case FUN_HANDLE_CONTROL_CANCEL:
//			OLED_ShowCHinese(0,LINE2,40);
//			OLED_ShowCHinese(16,LINE2,41);	
//			OLED_ShowCHinese(32,LINE2,42);
//			OLED_ShowCHinese(48,LINE2,43);
//			OLED_ShowCHinese(64,LINE2,44);
//			OLED_ShowCHinese(80,LINE2,45);	
      LCD_ShowCHinese((u8*)"手动控制取消",0,2);			
			break;
		case INPUT_NUM:
			if(BUTTON_OK == chr){
				x_pos = 0;
				disp_ch('_',x_pos*8,2);
			}else if(BUTTON_DEL == chr){
				//OLED_ShowChar(x_pos*8, LINE2, ' ');
				disp_ch(' ',x_pos*8,2);
				x_pos = x_pos>0 ? x_pos-1 : 0;
				disp_ch('_',x_pos*8,2);
			}else{
				if(BUTTON_FUN == chr || BUTTON_COLON == chr){
					return;
				}
				chr += 0x30;
				disp_ch(chr,x_pos*8,2);
				x_pos ++;
				disp_ch('_',x_pos*8,2);
			}
  			break;
		default:
			break;  
	}
	return;
}


/********************************************************************************
*line 3, 显示内容--
*
*
*
*
********************************************************************************/
void lcd_show_line3(u8 index,  char chr){
	
	static u8 x_pos = 0;//[0-31]
	if(index != INPUT_NUM){
//		oled_clear_line(LINE3);  
//		oled_clear_line(LINE4);
//		lcd_clear_line(LINE3);  
//		lcd_clear_line(LCD_LINE4);
		//lcd_clear_line(LINE3);
		//LCD_ShowCHinese((unsigned char*)" ",6*16,5);
		//lcd_clear_line(1);
		lcd_clear_line(2);
		lcd_clear_line(5);
		//ClearDisplay();
	}
	
	switch(index){
		case CONNECT_STATUS_FAIL:
//			OLED_ShowCHinese(0,LINE3,0);
//			OLED_ShowCHinese(16,LINE3,1);
//			OLED_ShowCHinese(32,LINE3,16);
//			OLED_ShowCHinese(48,LINE3,2);
//			OLED_ShowCHinese(64,LINE3,3);
		    LCD_ShowCHinese((u8*)"连接不成功",0,LINE3);
			break;
		case CONNECT_STATUS_OK:
//			OLED_ShowCHinese(0,LINE3,0);
//			OLED_ShowCHinese(16,LINE3,1);
//			OLED_ShowCHinese(32,LINE3,2);
//			OLED_ShowCHinese(48,LINE3,3);
		    LCD_ShowCHinese((u8*)"连接成功",0,LINE3);
			break;		
		case SEND_FAIL:
//			OLED_ShowCHinese(0,LINE3,14);
//			OLED_ShowCHinese(16,LINE3,15);
//			OLED_ShowCHinese(32,LINE3,4);
//			OLED_ShowCHinese(48,LINE3,5);
		  LCD_ShowCHinese((u8*)"发送失败",0,LINE3);
			break;
		case SEND_OK:
//			OLED_ShowCHinese(0,LINE3,14);
//			OLED_ShowCHinese(16,LINE3,15);
//			OLED_ShowCHinese(32,LINE3,2);
//			OLED_ShowCHinese(48,LINE3,3);
		  LCD_ShowCHinese((u8*)"发送成功",0,LINE3);
			break;
		case ADDR_ERROR:
//			OLED_ShowCHinese(0,LINE3,11);
//			OLED_ShowCHinese(16,LINE3,12);
//			OLED_ShowCHinese(32,LINE3,18);
//			OLED_ShowCHinese(48,LINE3,19);
//			OLED_ShowCHinese(64,LINE3,20);
//			OLED_ShowCHinese(80,LINE3,21);	
      LCD_ShowCHinese((u8*)"地址格式错误",0,LINE3);		
			break;
		case SENDING:
//			OLED_ShowCHinese(0,LINE3,38);
//			OLED_ShowCHinese(16,LINE3,39);
//			OLED_ShowCHinese(32,LINE3,14);
//			OLED_ShowCHinese(48,LINE3,15);
				LCD_ShowCHinese((u8*)"正在发送",0,LINE3);		
			break;
		case INPUT_NUM:
			if(BUTTON_OK == chr){
				x_pos = 0;
			}else if(BUTTON_DEL == chr){
				//OLED_ShowChar(x_pos*8, LINE3, ' ');
				//lcd_ShowChar(x_pos*8, LINE3, ' ');
				disp_ch(' ',x_pos*8,4);
				x_pos = x_pos>0 ? x_pos-1 : 0;
				//OLED_ShowChar(x_pos*8, LINE3, '_');
				//lcd_ShowChar(x_pos*8, LINE3, ' ');
				disp_ch(' ',x_pos*8,4);
			}else{
				chr = BUTTON_COLON == chr ? ':' : chr+0x30;//转成字符
				if(x_pos<19){
					//OLED_ShowChar(x_pos*8, LINE3, chr);
					//lcd_ShowChar(x_pos*8, LINE3, ' ');
					disp_ch(chr,x_pos*8,4);
					x_pos ++;
					//OLED_ShowChar(x_pos*8, LINE3, '_');
					//lcd_ShowChar(x_pos*8, LINE3, ' ');
					disp_ch('_',x_pos*8,4);
				}
			}
			break;
		default:			
			break;
	}
	return;
}


void lcd_clear_line(int x){
	int i=16;
	
	if(x<0 || x>6){
		return;
	}
	
	while(i--){
		//OLED_ShowString(8*i, x, " ");
		//LCD_P8x16Str(x,8*i,(unsigned char*)" ");
		//LCD_ShowCHinese((unsigned char*)" ",8*i,x);
		LCD_P8x16Str(8*i,x,(unsigned char*)" ");
	}
	
	return;
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void lcd_ShowChar(u8 x,u8 y,u8 chr)
{      	
	  unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x-=Max_Column;y=y+2;}
		if(SIZE ==16)
			{
			//OLED_Set_Pos(x,y);
      Setadd12864(x,y+1);				
			for(i=0;i<8;i++)
			//OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		  St7565_WriteDat(F8X16[c*16+i]);
			//OLED_Set_Pos(x,y+1);
		  Setadd12864(x,y+1);
			for(i=0;i<8;i++)
			//OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			St7565_WriteDat(F8X16[c*16+i]);
			}
			else {	
				//OLED_Set_Pos(x,y+1);
				Setadd12864(x,y+1);
				for(i=0;i<6;i++)
				//OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				St7565_WriteDat(F6x8[c][i]);				
			}
}



