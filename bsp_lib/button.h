#ifndef __BUTTON_H__
#define __BUTTON_H__

#define ADDR_TYPE_ERROR        -2

#define BUTTON_OK              10
#define BUTTON_DEL             11
#define BUTTON_FUN             12
#define BUTTON_COLON           13 //":"

void button_init(void);
int get_key(void);
int get_addr(unsigned char *addr);
void beep(u32 cnt);
void disable_key(void);
void enable_key(void);
#endif //
