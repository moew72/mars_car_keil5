#include "stm32f10x.h" // Device header

#include "wit_c_sdk.h"
#include "WT901C_Sensor.h"
#include "SENSOR.h"
#include "dht11.h"

#include "include.h"

void Sensor_App(void)
{
    float angles[3];

    menu_command_callback(BUFFER_CLEAR);

    while (1)
    {
        GetSensorAngle(angles);
        SensorProcess();

        OLED_ShowString(0, 0, "Sensor Data", OLED_8X16);
        OLED_DrawLine(0, 16, 127, 16);

        OLED_ShowString(0, 20, "X:", OLED_6X8);
        OLED_ShowFloatNum(18, 20, angles[0], 3, 1, OLED_6X8);
        OLED_ShowString(48, 20, "deg", OLED_6X8);

        OLED_ShowString(0, 30, "Y:", OLED_6X8);
        OLED_ShowFloatNum(18, 30, angles[1], 3, 1, OLED_6X8);
        OLED_ShowString(48, 30, "deg", OLED_6X8);

        OLED_ShowString(0, 40, "Z:", OLED_6X8);
        OLED_ShowFloatNum(18, 40, angles[2], 3, 1, OLED_6X8);
        OLED_ShowString(48, 40, "deg", OLED_6X8);

        OLED_ShowString(0, 55, "Press OK 2", OLED_6X8);
        OLED_ShowString(0, 63, "return", OLED_6X8);

        OLED_Update();

        Delay_ms(100);
        if (menu_command_callback(GET_EVENT_ENTER))
            return;
    }
}

void DHT11_App(void)
{
    u8 temperature, humidity;
    menu_command_callback(BUFFER_CLEAR);

    while (1)
    {

        OLED_Clear();

        OLED_ShowString(0, 0, "DHT11 Sensor", OLED_8X16);
        OLED_DrawLine(0, 16, 127, 16); 
        OLED_ShowString(0, 50, "Press OK 2", OLED_6X8);
        OLED_ShowString(70, 50, "return", OLED_6X8);
        if (DHT11_Read_Data(&temperature, &humidity) == 0)
        {

            OLED_ShowString(0, 20, "Temperature:", OLED_6X8);
            OLED_ShowNum(78, 20, temperature, 2, OLED_6X8);
            OLED_ShowString(90, 20, "C", OLED_6X8);

            OLED_ShowString(0, 30, "Humidity:", OLED_6X8);
            OLED_ShowNum(78, 30, humidity, 2, OLED_6X8);
            OLED_ShowString(90, 30, "%", OLED_6X8);
            OLED_Update();
            Delay_ms(500);
        }

        if (menu_command_callback(GET_EVENT_ENTER))
            return;
    }
}
