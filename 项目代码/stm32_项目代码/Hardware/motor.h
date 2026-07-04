#ifndef __MOTOR_H_
#define __MOTOR_H_

#define MOTOR_GO_FORWARD   MOTOR_A_CON1_HIGH;MOTOR_A_CON2_LOW;MOTOR_B_CON1_HIGH;MOTOR_B_CON2_LOW; //电机前进	                            
#define MOTOR_GO_BACK	   MOTOR_A_CON1_LOW;MOTOR_A_CON2_HIGH;MOTOR_B_CON1_LOW;MOTOR_B_CON2_HIGH; //电机后退
#define MOTOR_GO_RIGHT	   MOTOR_A_CON1_LOW;MOTOR_A_CON2_HIGH;MOTOR_B_CON1_HIGH;MOTOR_B_CON2_LOW; //电机左转
#define MOTOR_GO_LEFT	   MOTOR_A_CON1_HIGH;MOTOR_A_CON2_LOW;MOTOR_B_CON1_LOW;MOTOR_B_CON2_HIGH; //电机右转
#define MOTOR_GO_STOP	   MOTOR_A_CON1_LOW;MOTOR_A_CON2_LOW;MOTOR_B_CON1_LOW;MOTOR_B_CON2_LOW;   //电机停止


void MOTOR_GPIO_Config(void);
void TIM5_Mode_Config(void);
void TIM5_GPIO_Config(void);
void Motor_Init(void);
void Set_Left_Speed(u16 Speed_Left);
void Set_Right_Speed(u16 Speed_Right);
#endif
