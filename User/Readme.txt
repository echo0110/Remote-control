/****************************************
** 红牛103开发板
** 版本：V1.0
** 论坛：bbs.openmcu.com
** 旺宝：www.openmcu.com   
** 邮箱：support@openmcu.com

** 实验名：【A】LED跑马灯实验
** keil MDK-ARM Standard Version: V4.1.0A 
** 软件库(keil) Version: V2.1.0
** 固件库(官方) Version：V3.5

****************************************/

/***************************************
** 实验说明：
   1、STM32系统初始化，包括RCC--复位时钟控制，NVIC--嵌套中断，GPIO--端口配置(模式，速率等)
      这是所有程序必须有的，你可以参考现有的模版，或ST发布的固件库等

   2、通过调用库函数GPIO_Setbits()、GPIO_Resetbits()来实现端口的赋值
   3、这是官方的GPIO模版，流水灯的速度可以修改Delay()的参数

** 实验现象：
   1、通过JLINK下载后，按下RESET键，程序开始运行
   2、D1~D4交替闪烁，D5慢闪烁(闪烁的效果，你可以通过端口组合实现)
***************************************/