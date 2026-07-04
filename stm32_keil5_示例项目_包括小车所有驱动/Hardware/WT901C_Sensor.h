#ifndef __WT901C_H_
#define __WT901C_H_

void WT901C_Init(void);
void SensorProcess(void);
void AutoScanSensor(void);
void SensorUartSend(uint8_t *p_data, uint32_t uiSize);
void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
void Delayms(uint16_t ucMs);

// 添加获取传感器数据的函数声明
void GetSensorAngle(float *angle);
void GetSensorAcc(float *acc);
void GetSensorGyro(float *gyro);

#endif
