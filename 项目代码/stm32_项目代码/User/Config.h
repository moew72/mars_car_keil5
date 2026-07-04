#ifndef __CONFIG_H
#define __CONFIG_H

#define MOTOR_A_EN_High GPIO_SetBits(GPIOA, GPIO_Pin_0);  // 电机使能端A设置为高 PA0
#define MOTOR_A_EN_Low GPIO_ResetBits(GPIOA, GPIO_Pin_0); // 电机使能端A设置为低 PA0
#define MOTOR_B_EN_High GPIO_SetBits(GPIOA, GPIO_Pin_1);  // 电机使能端B设置为高 PA1
#define MOTOR_B_EN_Low GPIO_ResetBits(GPIOA, GPIO_Pin_1); // 电机使能端B设置为低 PA1

#define MOTOR_A_CON1_HIGH GPIO_SetBits(GPIOC, GPIO_Pin_6);  // 电机接口1-A设置为高电平 PC6
#define MOTOR_A_CON1_LOW GPIO_ResetBits(GPIOC, GPIO_Pin_6); // 电机接口1-A设置为低电平 PC6
#define MOTOR_A_CON2_HIGH GPIO_SetBits(GPIOC, GPIO_Pin_7);  // 电机接口2-A设置为高电平 PC7
#define MOTOR_A_CON2_LOW GPIO_ResetBits(GPIOC, GPIO_Pin_7); // 电机接口2-A设置为低电平 PC7
#define MOTOR_B_CON1_HIGH GPIO_SetBits(GPIOC, GPIO_Pin_8);  // 电机接口1-B设置为高电平 PC8
#define MOTOR_B_CON1_LOW GPIO_ResetBits(GPIOC, GPIO_Pin_8); // 电机接口1-B设置为低电平 PC8
#define MOTOR_B_CON2_HIGH GPIO_SetBits(GPIOC, GPIO_Pin_9);  // 电机接口2-B设置为高电平 PC9
#define MOTOR_B_CON2_LOW GPIO_ResetBits(GPIOC, GPIO_Pin_9); // 电机接口2-B设置为低电平 PC9

#define Servo1_High GPIO_SetBits(GPIOA, GPIO_Pin_6); // 舵机1引脚设置为高 PA6
#define Servo2_High GPIO_SetBits(GPIOB, GPIO_Pin_1); // 舵机2引脚设置为高 PB1
#define Servo3_High GPIO_SetBits(GPIOB, GPIO_Pin_0); // 舵机3引脚设置为高 PB0
#define Servo4_High GPIO_SetBits(GPIOA, GPIO_Pin_7); // 舵机4引脚设置为高 PA7
#define Servo5_High GPIO_SetBits(GPIOA, GPIO_Pin_4); // 舵机5引脚设置为高 PA4
#define Servo6_High GPIO_SetBits(GPIOA, GPIO_Pin_5); // 舵机6引脚设置为高 PA5
#define Servo7_High GPIO_SetBits(GPIOA, GPIO_Pin_2); // 舵机7引脚设置为高 PA2
#define Servo8_High GPIO_SetBits(GPIOA, GPIO_Pin_3); // 舵机8引脚设置为高 PA3

#define Servo1_Low GPIO_ResetBits(GPIOA, GPIO_Pin_6); // 舵机1引脚设置为低 PA6
#define Servo2_Low GPIO_ResetBits(GPIOB, GPIO_Pin_1); // 舵机2引脚设置为低 PB1
#define Servo3_Low GPIO_ResetBits(GPIOB, GPIO_Pin_0); // 舵机3引脚设置为低 PB0
#define Servo4_Low GPIO_ResetBits(GPIOA, GPIO_Pin_7); // 舵机4引脚设置为低 PA7
#define Servo5_Low GPIO_ResetBits(GPIOA, GPIO_Pin_4); // 舵机5引脚设置为低 PA4
#define Servo6_Low GPIO_ResetBits(GPIOA, GPIO_Pin_5); // 舵机6引脚设置为低 PA5
#define Servo7_Low GPIO_ResetBits(GPIOA, GPIO_Pin_2); // 舵机7引脚设置为低 PA2
#define Servo8_Low GPIO_ResetBits(GPIOA, GPIO_Pin_3); // 舵机8引脚设置为高 PA3

#define Open_Light GPIO_SetBits(GPIOC, GPIO_Pin_0);    // 车灯打开 PC0
#define Closs_Light GPIO_ResetBits(GPIOC, GPIO_Pin_0); // 车灯关闭 PC0

#define LED1_High GPIO_SetBits(GPIOB, GPIO_Pin_5);  // LED1亮
#define LED1_Low GPIO_ResetBits(GPIOB, GPIO_Pin_5); // LED1灭
#define LED2_High GPIO_SetBits(GPIOD, GPIO_Pin_2);  // LED2亮
#define LED2_Low GPIO_ResetBits(GPIOD, GPIO_Pin_2); // LED2灭

#define IR GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)    // 中间红外
#define TR_L GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) // 循迹红外左
#define TR_R GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) // 循迹红外右
#define FR_L GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)  // 跟随红外左
#define FR_R GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)  // 跟随红外右

void Config_Init(void);
static void TIM3_PWM_Init(void);
static void TIM3_Servo_GPIO_Config(void);
static void Infrared_GPIO_Config(void);

#endif
