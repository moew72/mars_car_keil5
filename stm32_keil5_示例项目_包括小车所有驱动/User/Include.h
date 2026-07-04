#ifndef __INCLUDE_H
#define __INCLUDE_H

#include "stm32f10x.h"

#include <stdarg.h>
#include <math.h>
#include <stdio.h>

#include "Timer.h"
#include "Delay.h"
#include <menu.h>
#include <OLED.h>
#include <OLED_Data.h>

#include "Config.h"

#include "uart1.h"
#include "uart3.h"

// #include "i2c_eeprom.h"
// #include "Timer.h"
// #include "function.h"
// #include "Motor.h"
// #include "Ultrasonic.h"
// #include "Interrupt.h"

#define INIT 0      // 初始化
#define NORMAL 0    // 正常模式
#define FOLLOW 1    // 跟随模式
#define TRACK 2     // 循迹模式
#define AVOID 3     // 红外避障模式
#define WAVEAVOID 4 // 超声波避障模式
#define RANG 5      // 超声波测距模式
#define DHT11 6     // 温湿度显示

#define module_interval 25    // 定义超声波模块接收发射端间距(单位：mm)
#define object_distance 100.0 // 检测物体的距离

extern u8 adjust;
extern u8 buffer[3];
extern u8 rec_flag;
extern u8 Angle[8];
extern int Level;
extern int Mode;
extern int Refresh;
extern int Cruising_Flag;
extern int Pre_Cruising_Flag;
extern int overcount; // 用于检测是否定时器已经溢出
extern float result;
extern u16 speed_left;  // 左侧速度初值
extern u16 speed_right; // 右侧速度初值

#endif /* __INCLUDE_H */
