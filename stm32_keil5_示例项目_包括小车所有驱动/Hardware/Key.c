#include "stm32f10x.h" // Device header
#include "Delay.h"

// 增加消抖时间从5ms到20ms，提高稳定性
#define KEY_DEBOUNCE_TIME_MS 20

/**
 * 函    数：按键初始化
 * 参    数：无
 * 返 回 值：无
 */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB的时钟

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 将PB8, PB9和PB0引脚初始化为上拉输入
}

/**
 * 函    数：按键获取键码
 * 参    数：无
 * 返 回 值：按下按键的键码值，范围：0~3，返回0代表没有按键按下
 * 注意事项：此函数是非阻塞式操作，不会等待按键释放
 */

uint8_t Key_Scan(void)
{
	uint8_t KeyNum = 0;

	// 检查各个按键状态，避免使用else if结构
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)
	{
		Delay_ms(KEY_DEBOUNCE_TIME_MS);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)
		{
			KeyNum = 1;
			Delay_ms(KEY_DEBOUNCE_TIME_MS);
		}
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
	{
		Delay_ms(KEY_DEBOUNCE_TIME_MS);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
		{
			KeyNum = 2;
			Delay_ms(KEY_DEBOUNCE_TIME_MS);
		}
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
	{
		Delay_ms(KEY_DEBOUNCE_TIME_MS);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			KeyNum = 3;
		}
	}

	return KeyNum;
}
