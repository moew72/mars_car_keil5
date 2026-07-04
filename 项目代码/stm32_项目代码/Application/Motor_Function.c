#include "stm32f10x.h" // Device header
#include "MOTOR.h"
#include "include.h"
#include "Ultrasonic.h"
void TrackLine(void)
{
  if ((TR_L == 0) && (TR_R == 1)) // 右侧传感器检测到黑线（即线路右拐）
  {
    Set_Left_Speed(300);
    Set_Right_Speed(120);
    // 开始右转一段时间，脱离黑线
    MOTOR_GO_FORWARD; // 调用右转函数

    Delay_ms(200);
    MOTOR_GO_STOP;
    Delay_ms(50);
    return;
  }
  else if ((TR_L == 1) && (TR_R == 0)) // 线路左拐
  {
    Set_Left_Speed(120);
    Set_Right_Speed(300);
    MOTOR_GO_FORWARD;
    Delay_ms(200);
    MOTOR_GO_STOP;
    Delay_ms(50);
    return;
  }
  if ((TR_L == 0) && (TR_R == 0)) // 两边同时都没有探测到黑线
  {
    Set_Left_Speed(200);
    Set_Right_Speed(200);
    MOTOR_GO_FORWARD;
    Delay_ms(50);
    return;
  }
  if ((TR_L == 1) && (TR_R == 1)) // 左右都检测到黑线，就如视频中的那样遇到一道横的胶带
  {
    Set_Left_Speed(200);
    Set_Right_Speed(200);
    MOTOR_GO_STOP;
    Angle[0] = 45;
    Delay_ms(50);
    Angle[0] = 180;
    MOTOR_GO_BACK;
    Set_Left_Speed(120);
    Set_Right_Speed(300);
    MOTOR_GO_FORWARD;
    Delay_ms(500);
    MOTOR_GO_STOP;
    Delay_ms(50);
    return;
  }
}

/*
 * 函数名：Follow
 * 描述  ：超声波跟随模式
 * 输入  ：无
 * 输出  ：无
 * 调用  ：function中调用
 */
void Follow(void)
{
  float distance = Get_distance();
  if (distance > 300.0f)
  {
    MOTOR_GO_STOP;
  }
  // --- 电机控制逻辑（慢速版）---
  if (distance > 260.0f)
  {
    // 距离太远，缓慢前进追近
    Set_Left_Speed(145);
    Set_Right_Speed(145);
    Delay_ms(10);
    MOTOR_GO_STOP;
    Delay_ms(5);
  }
  else if (distance > 200.0f)
  {
    // 中等偏远距离，极慢前进
    Set_Left_Speed(145);
    Set_Right_Speed(145);
    Delay_ms(10);
    MOTOR_GO_STOP;
    Delay_ms(5);
  }
  else if (distance > 50.0f)
  {
    // 合适距离，微调保持
    Set_Left_Speed(140);
    Set_Right_Speed(140);
    if (distance > 140.0f)
    {
      Delay_ms(10);
    }
    else if (distance > 90.0f)
    {
      Set_Left_Speed(120);
      Set_Right_Speed(120);
      Delay_ms(10);
    }
    else
    {
      MOTOR_GO_STOP;
    }
    Delay_ms(10);
    MOTOR_GO_STOP;
    Delay_ms(10);
  }
  else
  {
    // 太近了，缓慢后退避免碰撞
    Set_Left_Speed(120);
    Set_Right_Speed(120);
    Delay_ms(10);
    MOTOR_GO_STOP;
    Delay_ms(10);
  }

  Delay_ms(10); // 主循环延时
}