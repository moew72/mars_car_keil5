#include "Include.h"

void TIM4_Init(void);
void  Wave_GPIO_Config(void);


void Ultra_Init(){
	TIM4_Init();
	Wave_GPIO_Config();
	
}

void TIM4_Init(void) // 定时器4初始化
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  TIM_DeInit(TIM4);
  TIM_TimeBaseStructure.TIM_Period = 5000;                     // 设置自动重装载寄存器，当计数值达到这个寄存器锁存数值时，溢出产生事件
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                     // 分频系数0,不分频
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      // 设置时间分割值
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              // 初始化定时器4
  TIM_PrescalerConfig(TIM4, 719, TIM_PSCReloadMode_Immediate); // 时钟频率为72MHz/(719+1)=100000
  TIM_ARRPreloadConfig(TIM4, DISABLE);                         // 关闭自动重装初值
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);                   // 打开定时器中断
  TIM_Cmd(TIM4, DISABLE);                                      // 打开定时器

  /*定时器4中断优先级设置*/
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           // 中断组1
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           // 定时器4中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        // 副优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 中断使能打开
  NVIC_Init(&NVIC_InitStructure);
}

void  Wave_GPIO_Config(void)
{
      GPIO_InitTypeDef GPIO_InitStructure;  
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
      
      /*Echo pin define*/
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//悬浮模式
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      
      /*Trigle pin define*/
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//输出模式
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      GPIO_ResetBits(GPIOC,GPIO_Pin_4);
      GPIO_ResetBits(GPIOC,GPIO_Pin_5);
}

float Get_distance(void)
{
  float distance=0,sum_distance=0;
  u16 TIM=0;
  u8 i=0;
  
  while(i!=5)
  {
      GPIO_SetBits(GPIOC,GPIO_Pin_4);
      Delay_us(12);
      GPIO_ResetBits(GPIOC,GPIO_Pin_4);
      
      TIM_Cmd(TIM4, ENABLE);
      while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) && overcount==0);
      TIM4->CNT=0;
      while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) && overcount==0);
      TIM_Cmd(TIM4, DISABLE);
      i++;
      if(overcount!=0)
      {
            overcount=0;
            return 0.0;
      }
      TIM=TIM_GetCounter(TIM4);
      distance=(float)sqrt((TIM/10.0*17)*(TIM/10.0*17)-module_interval*module_interval/4.0)+12.0; 
      sum_distance=sum_distance+distance;
  }
  distance=sum_distance/5;
  return distance;
}


void Send_Distance(void)
{
    USART_SendData(USART1, 0xFF);
    Delay_ms(5);
    USART_SendData(USART1, 0x03);
    Delay_ms(5);
    USART_SendData(USART1, 0x00);
    Delay_ms(5);
    USART_SendData(USART1, (u16)Get_distance()/10);
    Delay_ms(5);
    USART_SendData(USART1, 0xFF);
    Delay_ms(1000);   
}

