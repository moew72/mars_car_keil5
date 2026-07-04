#include "stm32f10x.h" // Device header
#include <stdio.h>
#include <stdlib.h>
#include "include.h"
#include <string.h>
#include <Timer.h>

#include "Key.h"

#include "UART1.h"
#include "UART3.h"
#include "wit_c_sdk.h"
#include "WT901C_Sensor.h"

#include "motor.h"
#include "ULTRASONIC.h"
#include "SENSOR.h"
#include "dht11.h"

#include "menu_application.h"
#include "motor_function.h"

/*oled屏幕创建菜单，小车上没有oled，自备。

void MENU_RunToolsMenu(void)
{
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},

                                                   {"Serial", NULL},          // 串口
                                                   {"SensorIn3", Sensor_App}, // 传感器
                                                   {"Dht11", DHT11_App},      // DHT11
                                                   {"PWM Input", NULL},       // PWM 输入
                                                   {"ADC Input", NULL},       // ADC 输入
                                                   {".."}};

    static MENU_HandleTypeDef MENU = {.OptionList = MENU_OptionList};

    MENU_RunMenu(&MENU);
}
static MENU_OptionTypeDef MENU_OptionList1[] = {
    {"<<<", NULL},              // 固定格式, 用于退出
    {"Welcome Line", NULL},     // 工具
    {"Settings", NULL},         // 游戏
    {"APP", MENU_RunToolsMenu}, // 游戏
    {"..", NULL},               // 固定格式, 用于计算选项列表长度和退出
};

static MENU_HandleTypeDef MENU_APP = {.OptionList = MENU_OptionList1};
*/

uint8_t RxData; // 定义用于接收串口数据的变量

int main(void)
{
    /* 初始化 */

    Timer_Init();		
    SystemInit();		//主频72mhz
    OLED_Init();		//oled屏幕
    Key_Init();			//按键
    WT901C_Init();		//三合一姿态传感器
    DHT11_Init();		//温湿度传感器
    Motor_Init();		//小车
    Ultra_Init();
    Sensor_Init();
    Config_Init();
  

    Serial_Init();
    Usart3Init(9600);
    // Uart3Send(p, 3);
    Set_Left_Speed(120);
    Set_Right_Speed(120);





        // 主循环
        while (1)
        {
			TrackLine();
			Delayms(10);
		}

		/*
		  while (1)
        {
			oled菜单显示
			MENU_RunMenu(&MENU_APP);
		}
		
		*/
}


/**********************定时中断服务函数************************************/

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 判断是否是TIM2的更新事件触发的中断
    {
        Tick++;                                     // Num变量自增，用于测试定时中断
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除TIM2更新事件的中断标志位
                                                    // 中断标志位必须清除
                                                    // 否则中断将连续不断地触发，导致主程序卡死
    }
}
//滴答定时器
static u16 pwmcount = 0;
void TIM3_IRQHandler(void) // TIM2中断
{

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // 检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除TIMx的中断待处理位:TIM 中断源
        pwmcount++;
        if (pwmcount == 3600) // 20ms周期
        {
            pwmcount = 0;
        }
        if (pwmcount == 500)
        {
            Servo1_High;
            Servo2_High;
        }
        if (pwmcount == (2 * Angle[0] + 590))
        {
            Servo1_Low;
        }
        if (pwmcount == (2 * Angle[1] + 590))
        {
            Servo2_Low;
        }
    }
}
void TIM4_IRQHandler(void) // TIM4中断
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) // 检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // 清除TIMx的中断待处理位:TIM 中断源
        overcount++;                                // 中断溢出计数
    }
}