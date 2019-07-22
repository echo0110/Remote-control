#ifndef __DISPLAY_H__
#define __DISPLAY_H__

////显示行
//#define LINE0      0    //F和N
//#define LINE1      1//2 //nrf2401  address
//#define LINE5      5//4 //是和否
//#define LINE4      6

//显示行
#define LINE1      0    //F和N
#define LINE2      1//2 //nrf2401  address
#define LINE3      5//4 //是和否
#define LINE4      6


//LCD   显示行
#define LCD_LINE4      5

//line1 内容显示位置
#define INPUT_TYPE                     0
#define CONNECT_STATUS                 (7*8)
#define POEWR                          (12*8)

#define BMP_SIGNAL                     0
#define BMP_NO_SIGNAL                  1

//line2 显示内容
#define INPUT_ADDR                          1
#define FUN_LIGHT_OFF                       2
#define FUN_LIGHT_RED                       3
#define FUN_YELLOW_FLASH                    4
#define FUN_EAST_WEST_STRAIGHT              5
#define FUN_SOUTH_NORTH_STRAIGHT            7
#define FUN_EAST_WEST_TURN_LEFT             8
#define FUN_SOUTH_NORTH_TURN_LEFT           9
#define FUN_HANDLE_CONTROL_CANCEL           10

//line3-4 显示内容
#define CONNECT_STATUS_FAIL                    1
#define CONNECT_STATUS_OK                      2
#define SEND_FAIL                              3
#define SEND_OK                                4
#define ADDR_ERROR                             5
#define INPUT_NUM                              6
#define SENDING                                7

void oled_clear_line(int x);
void oled_show_line1(u8 x, u8 *chr);
void oled_show_line1_bmp(u8 index);
void oled_show_line2(u8 index, char chr);
void oled_show_line3(u8 index,  char chr);
#endif
