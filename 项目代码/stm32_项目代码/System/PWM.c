#include "stm32f10x.h" // Device header

/**
 * 函    数：定时中断初始化
 * 参    数：无
 * 返 回 值：无
 */
void PWM_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启TIM2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*配置时钟源*/
	TIM_InternalClockConfig(TIM2); // 选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟

	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				// 定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV2;		// 时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;					// 计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;				// 预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);				// 将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = 10;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = ENABLE;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	/*TIM使能*/
	TIM_Cmd(TIM2, ENABLE); // 使能TIM2，定时器开始运行
}

void PWM_SetCompare1(uint16_t Compare)
{

	TIM_SetCompare1(TIM2, Compare);
}
