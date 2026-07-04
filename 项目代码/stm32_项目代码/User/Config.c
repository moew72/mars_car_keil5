#include "stm32f10x.h"                  // Device header
#include "config.h"

void Config_Init(){
	TIM3_PWM_Init();
	TIM3_Servo_GPIO_Config();
	Infrared_GPIO_Config();	
}

/*
 * 函数名：TIM3_PWM_Init
 * 描述  ：配置TIM2输出PWM信号（舵机PWM）
 * 输入  ：无
 * 输出  ：无
 * 调用  ：初始化调用
 */
void TIM3_PWM_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_DeInit(TIM3);
  TIM_TimeBaseStructure.TIM_Period = 399;                     // 自动重装载寄存器的值
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                    // 时钟预分频数
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 采样分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数方式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update); // 清除溢出中断标志
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, ENABLE); // 开启时钟

  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           // 通道TIM2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        // 副优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：TIM2_Servo_GPIO_Config
 * 描述  ：定时器2(舵机引脚初始化)
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void TIM3_Servo_GPIO_Config(void) //定时器2(舵机引脚初始化)
{
      GPIO_InitTypeDef GPIO_InitStructure;
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_Init(GPIOB,&GPIO_InitStructure);
 
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_Init(GPIOA,&GPIO_InitStructure);

}

void Infrared_GPIO_Config(void)
{
      GPIO_InitTypeDef GPIO_InitStructure;
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 	
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);	
}
