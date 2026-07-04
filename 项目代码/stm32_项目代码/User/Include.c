#include "Include.h"
#include "stm32f10x_usart.h"

int Level = 0;
int Mode = 0;
int Refresh = 0;

u8 adjust;                                                      // 定义电机校准标志
u8 buffer[3];                                                   // 定义串口缓冲接收数组
u8 rec_flag = 0;                                                // 定义串口接收标志
u8 Angle[8] = {0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c}; // 定义舵机角度数组
int Cruising_Flag = 0x00;
int Pre_Cruising_Flag = 0x00;
int overcount = 0; // 用于检测是否定时器已经溢出
float result = 0;
u16 speed_left = 500;  // 左侧速度初值
u16 speed_right = 500; // 右侧速度初值

//**************以下代码为重定义printf函数*****************
//#ifdef __GNUC__
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */

//PUTCHAR_PROTOTYPE
//{
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//  {
//  }
//  USART_SendData(USART1, (u16)ch);
//  return ch;
//}
//**************以上代码为重定义printf函数*****************

