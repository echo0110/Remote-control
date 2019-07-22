/*************************************************************************************
*说明：oled的分辨率：64X128 汉字：16X16 其他字符：8X16，一行可以显示16个字符，8个汉字
*
*
*
*
**************************************************************************************/
#include "stm32f10x.h"
#include "oled.h"
#include "display.h"
#include "button.h"

extern u8 BmpSignal[];
extern u8 BmpNoSignal[];
void oled_clear_all(void){
	
	OLED_Clear();
	return;
}

//
void oled_clear_line(int x){
	int i=16;
	
	if(x<0 || x>6){
		return;
	}
	
	while(i--){
		OLED_ShowString(8*i, x, " ");
	}
	
	return;
}

void oled_show_string(u8 x,u8 y,u8 *chr){

	OLED_ShowString(x, y, chr);
	
	return;
}


/********************************************************************************
*line 1, 显示内容--连接状态、输入方式、电量、
*
*连接状态：bmp
*输入方式：数字键-“N” 功能键-“F”  说明：未连接状态时显示N，连接成功后为F，只有在连接成功后可以设置
*电量：
********************************************************************************/
void oled_show_line1(u8 x, u8 *chr){

	OLED_ShowString(x, 0, chr);
	return;
}

void oled_show_line1_bmp(u8 index){

	switch(index){
		case BMP_SIGNAL:
			OLED_DrawBMP(CONNECT_STATUS, 0, CONNECT_STATUS+16, 2, BmpSignal);
			break;
		case BMP_NO_SIGNAL:
			OLED_DrawBMP(CONNECT_STATUS, 0, CONNECT_STATUS+16, 2, BmpNoSignal);
			break;		
		default:
			break;
	}
	return;
}

/********************************************************************************
*line 2, 显示内容--
*
*
*
*
********************************************************************************/
void oled_show_line2(u8 index, char chr){
	static u8 x_pos = 0;

	if(index != INPUT_NUM){
		oled_clear_line(LINE2);
	}
	switch(index){
		case INPUT_ADDR:
			OLED_ShowCHinese(0,LINE2,6);
			OLED_ShowCHinese(16,LINE2,7);
			OLED_ShowCHinese(32,LINE2,8);
			OLED_ShowCHinese(48,LINE2,9);	
			OLED_ShowCHinese(64,LINE2,10);
			OLED_ShowCHinese(80,LINE2,11);
			OLED_ShowCHinese(96,LINE2,12);
			OLED_ShowCHinese(112,LINE2,13);		
			break;
		
		case FUN_LIGHT_OFF:
			OLED_ShowCHinese(0,LINE2,28);
			OLED_ShowCHinese(16,LINE2,29);			
			break;
		case FUN_LIGHT_RED:
			OLED_ShowCHinese(0,LINE2,24);
			OLED_ShowCHinese(16,LINE2,25);				
			break;
		case FUN_YELLOW_FLASH:
			OLED_ShowCHinese(0,LINE2,26);
			OLED_ShowCHinese(16,LINE2,27);				
			break;
		case FUN_EAST_WEST_STRAIGHT:
			OLED_ShowCHinese(0,LINE2,30);
			OLED_ShowCHinese(16,LINE2,31);	
			OLED_ShowCHinese(32,LINE2,34);
			OLED_ShowCHinese(48,LINE2,35);			
			break;	
		case FUN_SOUTH_NORTH_STRAIGHT:
			OLED_ShowCHinese(0,LINE2,32);
			OLED_ShowCHinese(16,LINE2,33);	
			OLED_ShowCHinese(32,LINE2,34);
			OLED_ShowCHinese(48,LINE2,35);			
			break;
		case FUN_EAST_WEST_TURN_LEFT:
			OLED_ShowCHinese(0,LINE2,30);
			OLED_ShowCHinese(16,LINE2,31);	
			OLED_ShowCHinese(32,LINE2,36);
			OLED_ShowCHinese(48,LINE2,37);			
			break;	
		case FUN_SOUTH_NORTH_TURN_LEFT:
			OLED_ShowCHinese(0,LINE2,32);
			OLED_ShowCHinese(16,LINE2,33);	
			OLED_ShowCHinese(32,LINE2,36);
			OLED_ShowCHinese(48,LINE2,37);			
			break;
		case FUN_HANDLE_CONTROL_CANCEL:
			OLED_ShowCHinese(0,LINE2,40);
			OLED_ShowCHinese(16,LINE2,41);	
			OLED_ShowCHinese(32,LINE2,42);
			OLED_ShowCHinese(48,LINE2,43);
			OLED_ShowCHinese(64,LINE2,44);
			OLED_ShowCHinese(80,LINE2,45);		
			break;
		case INPUT_NUM:
			if(BUTTON_OK == chr){
				x_pos = 0;
				//OLED_ShowChar(0, LINE2, '_');
			}else if(BUTTON_DEL == chr){
				//OLED_ShowChar(x_pos*8, LINE2, ' ');				
				lcd_ShowChar(x_pos*8, LINE2, ' ');
				x_pos = x_pos>0 ? x_pos-1 : 0;
				//OLED_ShowChar(x_pos*8, LINE2, '_');
				lcd_ShowChar(x_pos*8, LINE2, ' ');
			}else{
				if(BUTTON_FUN == chr || BUTTON_COLON == chr){
					return;
				}
				chr += 0x30;
				//OLED_ShowChar(x_pos*8, LINE2, chr);
				lcd_ShowChar(x_pos*8, LINE2, ' ');
				x_pos ++;
				//OLED_ShowChar(x_pos*8, LINE2, '_');
				lcd_ShowChar(x_pos*8, LINE2, ' ');
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
void oled_show_line3(u8 index,  char chr){
	
	static u8 x_pos = 0;//[0-31]
	if(index != INPUT_NUM){
		oled_clear_line(LINE3);
		oled_clear_line(LINE4);
	}
	
	switch(index){
		case CONNECT_STATUS_FAIL:
			OLED_ShowCHinese(0,LINE3,0);
			OLED_ShowCHinese(16,LINE3,1);
			OLED_ShowCHinese(32,LINE3,16);
			OLED_ShowCHinese(48,LINE3,2);
			OLED_ShowCHinese(64,LINE3,3);
			break;
		case CONNECT_STATUS_OK:
			OLED_ShowCHinese(0,LINE3,0);
			OLED_ShowCHinese(16,LINE3,1);
			OLED_ShowCHinese(32,LINE3,2);
			OLED_ShowCHinese(48,LINE3,3);
			break;		
		case SEND_FAIL:
			OLED_ShowCHinese(0,LINE3,14);
			OLED_ShowCHinese(16,LINE3,15);
			OLED_ShowCHinese(32,LINE3,4);
			OLED_ShowCHinese(48,LINE3,5);
			break;
		case SEND_OK:
			OLED_ShowCHinese(0,LINE3,14);
			OLED_ShowCHinese(16,LINE3,15);
			OLED_ShowCHinese(32,LINE3,2);
			OLED_ShowCHinese(48,LINE3,3);
			break;
		case ADDR_ERROR:
			OLED_ShowCHinese(0,LINE3,11);
			OLED_ShowCHinese(16,LINE3,12);
			OLED_ShowCHinese(32,LINE3,18);
			OLED_ShowCHinese(48,LINE3,19);
			OLED_ShowCHinese(64,LINE3,20);
			OLED_ShowCHinese(80,LINE3,21);		
			break;
		case SENDING:
			OLED_ShowCHinese(0,LINE3,38);
			OLED_ShowCHinese(16,LINE3,39);
			OLED_ShowCHinese(32,LINE3,14);
			OLED_ShowCHinese(48,LINE3,15);			
			break;
		case INPUT_NUM:
			if(BUTTON_OK == chr){
				x_pos = 0;
			}else if(BUTTON_DEL == chr){
				OLED_ShowChar(x_pos*8, LINE3, ' ');
				x_pos = x_pos>0 ? x_pos-1 : 0;
				OLED_ShowChar(x_pos*8, LINE3, '_');
			}else{
				chr = BUTTON_COLON == chr ? ':' : chr+0x30;//转成字符
				if(x_pos<19){
					OLED_ShowChar(x_pos*8, LINE3, chr);
					x_pos ++;
					OLED_ShowChar(x_pos*8, LINE3, '_');
				}
			}
			break;
		default:			
			break;
	}
	return;
}

