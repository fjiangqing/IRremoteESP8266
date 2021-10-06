/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-03 01:57:04
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-06 11:46:40
* @FilePath     : \IRremoteESP8266\app\remote_control\button.h
* @Description  :
*******************************************************************************/
#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

#define POWER_PIN_NAME            "power_pin"
#define TEMP_UP_PIN_NAME          "temp_up_pin"
#define TEMP_DWON_PIN_NAME        "temp_down_pin"
#define FAN_PIN_NAME              "fan_pin"
#define MODE_SWITCH_PIN_NAME      "mode_switch_pin"
#define LEARN_PIN_NAME            "learn_pin"
#define DEVICE_SWITCH_PIN_NAME    "device_switch_pin"

void button_init(void);
void button_task(void);

// extern const uint16_t power_pin;
// extern const uint16_t temp_up_pin;
// extern const uint16_t temp_down_pin;
// extern const uint16_t mode_switch_pin;

#define power_pin          16
#define temp_up_pin        0
#define temp_down_pin      14
#define mode_switch_pin    12
// const uint16_t fan_pin         = 10;

#endif
