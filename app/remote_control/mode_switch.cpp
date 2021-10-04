/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-02 23:47:01
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-03 00:56:20
* @FilePath     : \IRremoteESP8266\app\remote_control\mode_switch.cpp
* @Description  :
*******************************************************************************/

#include "mode_switch.h"

#include <Arduino.h>

#include <stdint.h>

const uint16_t mode_switch_pin   = 5; // GPIO D1
const int      mode_switch_valid = LOW;
const uint16_t button_delay      = 1000;

void work_mode_init(void)
{
    pinMode(mode_switch_pin, INPUT_PULLUP);
}


work_mode_t sys_work_mode = AC_GREE_MODE;

work_mode_t sys_work_mode_get(void)
{
    return sys_work_mode;
}


work_mode_t work_mode_switch(void)
{
    static uint32_t timer_old = 0;

    if ((digitalRead(mode_switch_pin) == mode_switch_valid))
    {
        if (millis() - timer_old > button_delay)
        {
            timer_old = millis();
            Serial.printf("__LINE__ = %d\r\n", __LINE__);

            if (sys_work_mode < AC_LEARN_MODE)
            {
                Serial.printf("__LINE__ = %d\r\n", __LINE__);

                sys_work_mode = (work_mode_t)(sys_work_mode + 1);
            }
            else
            {
                Serial.printf("__LINE__ = %d\r\n", __LINE__);

                sys_work_mode = AC_GREE_MODE;
            }
        }
    }
    else
    {
        timer_old = millis();
    }

    return sys_work_mode;
}
