#ifndef _SYSTEM_CLOCK_H_
#define _SYSTEM_CLOCK_H_

void RCC_Configuration(void);
void NVIC_Configuration(void);


void ad_init(void);
uint16_t get_ad_val(void);
void save_addr(u8 *addr);
int read_addr(u8 *addr);
#endif
