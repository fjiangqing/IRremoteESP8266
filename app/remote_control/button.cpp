/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-03 01:57:44
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-12 10:30:57
* @FilePath     : \IRremoteESP8266\app\remote_control\button.cpp
* @Description  :
*******************************************************************************/
#include "button.h"

#include "air_conditioner.h"

#include "ir_learn.h"

#include <Arduino.h>

#include <string.h>

#include <stddef.h>

// typedef void (*function_call_t)(void);

// typedef struct
// {
//     const char      *name;
//     function_call_t call;
// } button_ac_call_t;


// button_ac_call_t button_ac_call[] =
// {
//     { POWER_PIN_NAME,       ac_control_use->control->ac_on        },
//     { TEMP_UP_PIN_NAME,     ac_control_use->control->ac_temp_up   },
//     { TEMP_DWON_PIN_NAME,   ac_control_use->control->ac_temp_down },
//     { FAN_PIN_NAME,         NULL                                  },
//     { MODE_SWITCH_PIN_NAME, NULL                                  },
//     { NULL,                 NULL                                  },
// };

char button_down_name[32];


// const uint16_t power_pin       = 16;
// const uint16_t temp_up_pin     = 0;
// const uint16_t temp_down_pin   = 14;
// const uint16_t mode_switch_pin = 12;
// const uint16_t fan_pin         = 10;

const uint16_t button_delay = 150;
const uint8_t  button_valid = LOW;

void button_init(void)
{
    // 控制按钮初始化
    pinMode(power_pin, INPUT_PULLUP);
    pinMode(temp_up_pin, INPUT_PULLUP);
    pinMode(temp_down_pin, INPUT_PULLUP);
    pinMode(mode_switch_pin, INPUT_PULLUP);
    // pinMode(fan_pin, INPUT_PULLUP);
}


typedef  enum
{
    BUTTON_STATUS_LOW  = 0,
    BUTTON_STATUS_HIGH = 1,
    BUTTON_STATUS_RISE = 2,
    BUTTON_STATUS_FALL = 3,
    BUTTON_NULL,
} button_status_t;

int button_fall_check(uint16_t pin, uint32_t& delay_old, uint8_t& pin_status_old)
{
    // 按钮按下检查,只在按下时候有效。下降沿检测
    if (digitalRead(pin) == LOW)
    {
        if ((millis() - delay_old > button_delay) &&
            (pin_status_old == 0))
        {
            pin_status_old = 1;

            delay_old = millis();

            return BUTTON_STATUS_FALL;
        }
    }
    else
    {
        pin_status_old = 0;

        delay_old = millis();
    }

    return BUTTON_NULL;
}


void button_task(void)
{
    static uint32_t power_delay     = 0;
    static uint32_t temp_up_delay   = 0;
    static uint32_t temp_down_delay = 0;
    static uint32_t mode_delay      = 0;
    // static uint32_t fan_delay       = 0;

    static uint8_t power_pin_old     = 0;
    static uint8_t temp_up_pin_old   = 0;
    static uint8_t temp_down_pin_old = 0;
    static uint8_t mode_pin_old      = 0;

    // 学习中模式下不进行发射红外
    if(ir_learn_status == IR_LEARN)
    {
        return ;
    }
    // 按钮按下检查
    if (button_fall_check(power_pin, power_delay, power_pin_old) == BUTTON_STATUS_FALL)
    {
        // todo:
        Serial.printf("power_pin down = %d\r\n", __LINE__);
        // 更具控制电源按钮,开机按下按钮关机，关机按下按钮开机。
        if (ac_control_use->ac_ir_work_status == AC_IR_WORK_CLOSE)
        {
            // 调用具体控制空调红外代码
            ac_control_on(ac_control_use);
        }
        else
        {
            ac_control_off(ac_control_use);
        }

        Serial.printf("power_pin down = %d\r\n", __LINE__);
    }

    // 温度增加控制
    if (button_fall_check(temp_up_pin, temp_up_delay, temp_up_pin_old) == BUTTON_STATUS_FALL)
    {
        // todo:
        Serial.println("temp_up_pin down");

        ac_control_temp_up(ac_control_use);
    }

    // 温度减少控制
    if (button_fall_check(temp_down_pin, temp_down_delay, temp_down_pin_old) == BUTTON_STATUS_FALL)
    {
        // todo:
        Serial.println("temp_down_pin down");

        ac_control_temp_down(ac_control_use);
    }

    // 模式切换控制
    if (button_fall_check(mode_switch_pin, mode_delay, mode_pin_old) == BUTTON_STATUS_FALL)
    {
        // todo:
        Serial.println("mode_switch_pin down");

        ac_control_mode_switch(ac_control_use);
    }
}
