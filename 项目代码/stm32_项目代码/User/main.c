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

uint8_t RxData; // 定义用于接收串口数据的变量

int main(void)
{
    /* 初始化 */

    Timer_Init();
    SystemInit();
    OLED_Init();
    Key_Init();
    WT901C_Init();
    DHT11_Init();
    Motor_Init();
    Ultra_Init();
    Sensor_Init();
    Config_Init();
    // 定义数组用于存储从传感器模块获取的数据

    Serial_Init();
    Usart3Init(9600);
    // Uart3Send(p, 3);
    Set_Left_Speed(120);
    Set_Right_Speed(120);
    OLED_ShowString(50, 50, "Tmp:", OLED_6X8);
    OLED_Update();

    Angle[0] = 90;
    Angle[1] = 130;

    // 显示模式选择提示
    OLED_Clear();
    OLED_ShowString(0, 0, "Mode Selection:", OLED_6X8);
    OLED_ShowString(0, 10, "KEY1: Serial", OLED_6X8);
    OLED_ShowString(0, 20, "KEY2: Menu", OLED_6X8);
    OLED_Update();

    // 等待用户按键选择模式，只执行一次
    uint8_t key = 0;
    while (key == 0)
    {
        // 直接读取按键状态而不是依赖Key_Scan函数
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)
        {
            Delay_ms(20); // 消抖
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)
            {
                key = 1;
                // 等待按键释放
                while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)
                    ;
                Delay_ms(20);
            }
        }
        else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
        {
            Delay_ms(20); // 消抖
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
            {
                key = 2;
                // 等待按键释放
                while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
                    ;
                Delay_ms(20);
            }
        }
        Delay_ms(10);
    }

    if (key == 1)
    {
        // KEY1 - 串口模式
        OLED_Clear();
        OLED_ShowString(0, 0, "Serial Mode", OLED_6X8);
        OLED_ShowString(0, 10, "Waiting cmd...", OLED_6X8);
        OLED_ShowString(0, 20, "Commands: DHT11,", OLED_6X8);
        OLED_ShowString(0, 30, "DHT11C, STOP,", OLED_6X8);
        OLED_ShowString(0, 40, "SENSOR, SERVO", OLED_6X8);
        OLED_Update();

        uint8_t dht11_continuous_mode = 0;
        uint8_t sensor_continuous_mode = 0;
        uint8_t trackline_mode = 0;

        // 串口模式主循环
        while (1)
        {
            // 检查串口数据
            if (Serial_RxFlag == 1)
            {
                Serial_RxFlag = 0;

                if (strcmp(Serial_RxPacket, "DHT11") == 0)
                {
                    // 单次读取温湿度
                    uint8_t temp, humi;
                    if (DHT11_Read_Data(&temp, &humi) == 0)
                    {
                        printf("Temperature: %d C, Humidity: %d%%\r\n", temp, humi);

                        OLED_Clear();
                        OLED_ShowString(0, 0, "Serial Mode", OLED_6X8);
                        OLED_ShowString(0, 10, "Temp:", OLED_6X8);
                        OLED_ShowNum(40, 10, temp, 2, OLED_6X8);
                        OLED_ShowString(52, 10, "C", OLED_6X8);
                        OLED_ShowString(0, 20, "Humi:", OLED_6X8);
                        OLED_ShowNum(40, 20, humi, 2, OLED_6X8);
                        OLED_ShowString(52, 20, "%", OLED_6X8);
                        OLED_Update();
                    }
                    else
                    {
                        printf("Failed to read DHT11 sensor!\r\n");
                    }
                }
                else if (strcmp(Serial_RxPacket, "DHT11C") == 0)
                {
                    // 连续读取模式
                    dht11_continuous_mode = 1;
                    sensor_continuous_mode = 0;
                    trackline_mode = 0;
                    printf("Entering continuous DHT11 mode...\r\n");
                }
                else if (strcmp(Serial_RxPacket, "SENSOR") == 0)
                {
                    // 单次读取角度传感器数据
                    float angles[3];
                    GetSensorAngle(angles);
                    SensorProcess();
                    printf("Sensor X: %.1f deg, Y: %.1f deg, Z: %.1f deg\r\n", angles[0], angles[1], angles[2]);

                    OLED_Clear();
                    OLED_ShowString(0, 0, "Serial Mode", OLED_6X8);
                    OLED_ShowString(0, 10, "X:", OLED_6X8);
                    OLED_ShowFloatNum(18, 10, angles[0], 3, 1, OLED_6X8);
                    OLED_ShowString(48, 10, "deg", OLED_6X8);
                    OLED_ShowString(0, 20, "Y:", OLED_6X8);
                    OLED_ShowFloatNum(18, 20, angles[1], 3, 1, OLED_6X8);
                    OLED_ShowString(48, 20, "deg", OLED_6X8);
                    OLED_ShowString(0, 30, "Z:", OLED_6X8);
                    OLED_ShowFloatNum(18, 30, angles[2], 3, 1, OLED_6X8);
                    OLED_ShowString(48, 30, "deg", OLED_6X8);
                    OLED_Update();
                }
                else if (strcmp(Serial_RxPacket, "SENSORC") == 0)
                {
                    // 连续读取角度传感器数据
                    dht11_continuous_mode = 0;
                    sensor_continuous_mode = 1;
                    trackline_mode = 0;
                    printf("Entering continuous SENSOR mode...\r\n");
                }
                else if (strcmp(Serial_RxPacket, "TRACKLINE") == 0)
                {
                    // 启动循迹模式
                    dht11_continuous_mode = 0;
                    sensor_continuous_mode = 0;
                    trackline_mode = 1;
                    printf("Entering TRACKLINE mode...\r\n");
                    printf("Send STOP to exit TRACKLINE mode\r\n");
                }
                // 解析舵机控制命令，格式为 SERVO,<servo_number>,<angle>
                // 例如: SERVO,1,90 表示将1号舵机设置为90度
                else if (strncmp(Serial_RxPacket, "SERVO,", 6) == 0)
                {
                    uint8_t servo_num = 0;
                    uint8_t angle = 0;
                    uint8_t result = sscanf(Serial_RxPacket, "SERVO,%hhu,%hhu", &servo_num, &angle);

                    if (result == 2 && servo_num >= 1 && servo_num <= 8 && angle >= 0 && angle <= 180)
                    {
                        Angle[servo_num - 1] = angle;
                        printf("Set servo %d to %d degrees\r\n", servo_num, angle);

                        OLED_Clear();
                        OLED_ShowString(0, 0, "Serial Mode", OLED_6X8);
                        OLED_ShowString(0, 10, "Servo Control", OLED_6X8);
                        OLED_ShowString(0, 20, "Servo:", OLED_6X8);
                        OLED_ShowNum(40, 20, servo_num, 1, OLED_6X8);
                        OLED_ShowString(0, 30, "Angle:", OLED_6X8);
                        OLED_ShowNum(40, 30, angle, 3, OLED_6X8);
                        OLED_Update();
                    }
                    else
                    {
                        printf("Invalid SERVO command. Format: SERVO,<servo>,<angle>\r\n");
                        printf("servo: 1-8, angle: 0-180\r\n");
                    }
                }
                else if (strcmp(Serial_RxPacket, "STOP") == 0)
                {
                    // 停止所有连续读取模式
                    dht11_continuous_mode = 0;
                    sensor_continuous_mode = 0;
                    trackline_mode = 0;
                    MOTOR_GO_STOP;
                    printf("Stopped all continuous modes.\r\n");

                    OLED_Clear();
                    OLED_ShowString(0, 0, "Serial Mode", OLED_6X8);
                    OLED_ShowString(0, 10, "Waiting cmd...", OLED_6X8);
                    OLED_ShowString(0, 20, "Commands: DHT11,", OLED_6X8);
                    OLED_ShowString(0, 30, "DHT11C, STOP,", OLED_6X8);
                    OLED_ShowString(0, 40, "SENSOR, SERVO", OLED_6X8);
                    OLED_Update();
                }
            }

            // 连续读取温湿度模式处理
            if (dht11_continuous_mode)
            {
                uint8_t temp, humi;
                if (DHT11_Read_Data(&temp, &humi) == 0)
                {
                    printf("Temperature: %d C, Humidity: %d%%\r\n", temp, humi);

                    OLED_Clear();
                    OLED_ShowString(0, 0, "Serial Mode - Cont", OLED_6X8);
                    OLED_ShowString(0, 10, "Temp:", OLED_6X8);
                    OLED_ShowNum(40, 10, temp, 2, OLED_6X8);
                    OLED_ShowString(52, 10, "C", OLED_6X8);
                    OLED_ShowString(0, 20, "Humi:", OLED_6X8);
                    OLED_ShowNum(40, 20, humi, 2, OLED_6X8);
                    OLED_ShowString(52, 20, "%", OLED_6X8);
                    OLED_Update();
                }
                Delay_ms(2000);
            }

            // 连续读取角度传感器模式处理
            if (sensor_continuous_mode)
            {
                float angles[3];
                GetSensorAngle(angles);
                SensorProcess();
                printf("Sensor X: %.1f deg, Y: %.1f deg, Z: %.1f deg\r\n", angles[0], angles[1], angles[2]);

                OLED_Clear();
                OLED_ShowString(0, 0, "Serial Mode-S Cont", OLED_6X8);
                OLED_ShowString(0, 10, "X:", OLED_6X8);
                OLED_ShowFloatNum(18, 10, angles[0], 3, 1, OLED_6X8);
                OLED_ShowString(48, 10, "deg", OLED_6X8);
                OLED_ShowString(0, 20, "Y:", OLED_6X8);
                OLED_ShowFloatNum(18, 20, angles[1], 3, 1, OLED_6X8);
                OLED_ShowString(48, 20, "deg", OLED_6X8);
                OLED_ShowString(0, 30, "Z:", OLED_6X8);
                OLED_ShowFloatNum(18, 30, angles[2], 3, 1, OLED_6X8);
                OLED_ShowString(48, 30, "deg", OLED_6X8);
                OLED_Update();

                Delay_ms(500);
            }

            // 循迹模式处理
            if (trackline_mode)
            {
                TrackLine();
                Delay_ms(10);
            }

            Delay_ms(50);
        }
    }
    else if (key == 2)
    {
        // KEY2 - 菜单模式
        while (1)
        {
            MENU_RunMenu(&MENU_APP);
        }
    }
}

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
/**********************定时2中断服务函数************************************/
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