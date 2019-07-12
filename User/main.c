#include <stdio.h>
#include "stm32f10x.h"
#include "sys_config.h"
#include "nrf24l01.h"
#include "button.h"
#include "oled.h"
#include "display.h"

#include "12864.h"
#include "delay.h"

#define KEY_NUM                0
#define KEY_FUN                1

void Delay_Ms(unsigned int delay);
nrf24l01_t nrf24l01;
static int fun_flag = KEY_FUN;//按键复用标志
unsigned char addr[5];

//有限状态机 FSM
typedef enum FSM_status{
	LINK_START = 0,
	LINK_CONNECTED,
	LINK_DISCONNECTED,
}FSM_status_t;

typedef struct FSM{
	FSM_status_t status;
	int key;
	void (*action)(struct FSM *); 
}FSM_t;

void FSM_link_connected(struct FSM *me);
void key_trans(u16 key, u8 *data);

void FSM_link_start(struct FSM *me){
	int key;
	unsigned char buf[32] = {0};
	
	oled_show_line1(INPUT_TYPE, "N");
	oled_show_line1_bmp(BMP_NO_SIGNAL);
	oled_clear_line(LINE2);
	oled_clear_line(LINE3);

	if(read_addr(addr) != -1){
		sprintf(buf, "%d:%d:%d:%d:%d\0", addr[0], addr[1], addr[2], addr[3], addr[4]);
		OLED_ShowString(0, 2, buf);
		OLED_ShowCHinese(0,LINE4,22);
		OLED_ShowCHinese(7*16,LINE4,23);
		while(1){
			key = get_key();
			if(BUTTON_FUN == key){
				break;
			}else if(BUTTON_COLON == key){
				oled_clear_line(LINE2);
				oled_clear_line(LINE3);
				oled_clear_line(LINE4);
				oled_show_line2(INPUT_ADDR, 0);
				while(get_addr(addr)!= 0);
				break;
			}
		}
	}else{
		oled_clear_line(LINE2);
		oled_clear_line(LINE3);
		oled_clear_line(LINE4);
		oled_show_line2(INPUT_ADDR, 0);
		while(get_addr(addr)!= 0);
	}

	NRF24L01_TX_Mode(addr);

	if(NRF24L01_Send(buf)){
		//提示连接不成功
		oled_show_line3(CONNECT_STATUS_FAIL, 0);
		oled_show_line1(INPUT_TYPE, "N");
		oled_show_line1_bmp(BMP_NO_SIGNAL);
		oled_clear_line(LINE2);
		
		me->status = LINK_START;
		me->action = FSM_link_start;
		Delay_Ms(2000);
	}else{
		//提示连接成功
		oled_show_line3(CONNECT_STATUS_OK, 0);
		oled_show_line1(INPUT_TYPE, "F");
		oled_show_line1_bmp(BMP_SIGNAL);
		oled_clear_line(LINE2);
		
		me->status = LINK_CONNECTED;
		me->action = FSM_link_connected;
		
		save_addr(addr);
	}
	
	return;
}

void FSM_link_connected(struct FSM *me){

	unsigned char buf[32] = {0};
	
	if(me->key!=-1){
		oled_show_line3(SENDING, 0);
		Delay_Ms(150);
		key_trans(me->key, buf);
		disable_key();
		if(NRF24L01_Send(buf)==0){
			//提示发送成功
			oled_show_line3(SEND_OK, 0);
		}else{
			//提示发送不成功
			oled_show_line3(SEND_FAIL, 0);
			NRF24L01_TX_Mode(addr);
		}
		enable_key();
	}
	return;
}

void FSM_link_disconnected(struct FSM *me){

	//TODO
	return;
}

static u8 fun_xor(char *p, int len){
	u8 ret = 0;
	
	while(len --){
		ret = ret ^ *p;
		p += 1;
	}
	
	return ret;
}

//oled dispaly
void display_by_key(int key, struct FSM *me){
	static u32 tmp = 0;
	
	if(key < 0){
		return;
	}
	
	if((LINK_START != me->status) && (key == BUTTON_FUN)){
		oled_clear_line(LINE2);
		oled_clear_line(LINE3);
		if(fun_flag == KEY_FUN){
			oled_show_line1(INPUT_TYPE, "N");
			fun_flag = KEY_NUM;
			tmp = 0;
			oled_show_line2(INPUT_NUM, BUTTON_OK);
		}else if(fun_flag == KEY_NUM){
			oled_show_line1(INPUT_TYPE, "F");
			fun_flag = KEY_FUN;
		}
		me->key = -1;
		return;
	}
	
	if(LINK_CONNECTED == me->status){
		if(KEY_FUN == fun_flag){
			if(key>9){
				me->key = -1;
				return;
			}
			switch(key){
				case 0:
					oled_show_line2(FUN_LIGHT_OFF, key);
					break;	
				case 1:
					oled_show_line2(FUN_LIGHT_RED, key);
					break;				
				case 2:
					oled_show_line2(FUN_YELLOW_FLASH, key);
					break;				
				case 3:
					oled_show_line2(FUN_EAST_WEST_STRAIGHT, key);
					break;				
				case 4:
					oled_show_line2(FUN_SOUTH_NORTH_STRAIGHT, key);
					break;				
				case 5:
					oled_show_line2(FUN_EAST_WEST_TURN_LEFT, key);
					break;				
				case 6:
					oled_show_line2(FUN_SOUTH_NORTH_TURN_LEFT, key);
					break;
				
				case 7:
					oled_show_line2(FUN_HANDLE_CONTROL_CANCEL, key);
					break;
				
				case 8:			
				case 9:
				case 10:
				case 11:
				case 12:		
				case 13:
					oled_clear_line(LINE2);
					break;					
				default:
					break;
			}
		}else if(KEY_NUM == fun_flag){//用户自定义方案号
			oled_clear_line(LINE2);
			oled_clear_line(LINE3);
			tmp = 0;
			oled_show_line2(INPUT_NUM, BUTTON_OK);
			while(key != BUTTON_OK){				
				if(BUTTON_FUN == key){//切换功能
					oled_clear_line(LINE2);
					oled_show_line1(INPUT_TYPE, "F");
					fun_flag = KEY_FUN;
					me->key = -1;
					tmp = 0;
					return;
				}
				if(key != -1){
					if(key < 10){
						tmp = tmp*10 + key;
					}else if(BUTTON_DEL == key){
						tmp /= 10;
					}
					oled_show_line2(INPUT_NUM, key);
				}
				key = get_key();
			}
			me->key = tmp;
		}
	}
	
	return;
}

void key_trans(u16 key, u8 *data){
	int i;
	
	if(fun_flag == KEY_FUN){
		data[0] = 2;
	}else{
		data[0] = 1;
	}
	data[1] = 2;//数据长度
	data[2] = (key>>8)&0xFF;
	data[3] = key&0xFF;
	data[4] = fun_xor(data, 4);
	for(i=5;i<32;i++){
		data[i] = 0;
	}
	
	return;
}

FSM_t me = {LINK_START, -1, FSM_link_start};
#define FSM_dispatch(me) (me.action(&me)) 

int main(void)
{
	int key;
	RCC_Configuration();   //初始化系统
	NVIC_Configuration();
	delay_init();	    //延时函数初始化	  
	spi_init();
	button_init();
	OLED_Init();
	LCD_Init();
	OLED_Clear();
	ad_init();
	
	Init_display();//init  12864 LCD

  while (1){
		key = get_key();
		me.key = key;
		display_by_key(key, &me);
		FSM_dispatch(me);
		//OLED_ShowString(8*16, 0, "T");
		//OLED_ShowString(32, 0, "T");
		//OLED_ShowString(0, 4, "T");
		//OLED_ShowString(0, 6, "T");
  }
}

