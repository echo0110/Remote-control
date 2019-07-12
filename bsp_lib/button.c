#include "stm32f10x.h"
#include "button.h"
#include "display.h"

void Delay_Ms(unsigned int delay);

static int key_enable = 1;
static int valid = 0; //按键有效
static int key  = -1;

void delay(u32 delay){
	while(delay--);
	return;
}

void button_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	//pin配置
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	//Beep
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	return;
}


#if 0
static void exit_switch(int sw){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = sw ? ENABLE : DISABLE;
	NVIC_Init(&NVIC_InitStructure);		
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = sw ? ENABLE : DISABLE;
	NVIC_Init(&NVIC_InitStructure);		
		
	return;
}
#endif

void disable_key(void){
	key_enable = 0;
	return;
}

void enable_key(void){
	key_enable = 1;
	return;
}

void beep(u32 cnt){
	
	while(cnt --){
		GPIO_SetBits(GPIOC,GPIO_Pin_10);
		delay(0x8FF);
		GPIO_ResetBits(GPIOC,GPIO_Pin_10);
		delay(0x8FF);
	}
	return;
}

void put_key(char button){

	if(key_enable){
		key = button;
		valid = 1;
		beep(100);
	}
	return;
}

//没有按键返回-1
int get_key(void){
	int button = -1;

	if(valid){
		button = key;
		valid = 0;
	}

	return button;
}

int buf2addr(char *buf, int len, u8 *addr){
	int tmp;
	int addr_pos = 0;
	int index = 0;
	int i = 0;
	
	do{
		if(buf[index] != BUTTON_COLON){
			i++;
		}else{
			switch(i){
				case 0:
					tmp = 0;
					break;
				case 1:
					tmp = buf[index-1];
					break;
				case 2:
					tmp = buf[index-2]*10 + buf[index-1];
					break;
				case 3:
					tmp = buf[index-3]*100 + buf[index-2]*10 + buf[index-1];
					break;
				default:
					break;
			}
			if(tmp<256){
				addr[addr_pos] = tmp;
			}
			addr_pos ++;
			i = 0;
		}
		
		if(i>3 || addr_pos>5 || tmp>255){
			oled_show_line3(ADDR_ERROR, 0);
			Delay_Ms(2000);
			oled_clear_line(LINE3);
			return ADDR_TYPE_ERROR;
			break;
		}
		
		index ++;
	}while(len--);
	
	
	
	return 0;
}

int get_addr(u8 *addr){
	int ret = -1;
	int c;
	static char buf[20] = {0};
	static int i = 0;
	
	c = get_key();
	if(c != -1){
		if(BUTTON_FUN == c){
			return ret;
		}else if(BUTTON_OK == c){
			if(i>0){
				buf[i] = BUTTON_COLON;
				i++;
				ret = buf2addr(buf, i, addr);
			}
			for(i=0; i<20; i++){
				buf[i] = 0;
			}
			i = 0;
		}else if(BUTTON_DEL == c){
			i = i>0 ? i-1 : 0;
		}else{
			if(i<19){
				buf[i] = c;
				i++;
			}
		}
		oled_show_line3(INPUT_NUM, c);
	}
	
	return ret;
}

