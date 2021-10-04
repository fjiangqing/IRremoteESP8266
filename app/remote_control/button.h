/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-03 01:57:04
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-03 02:50:57
* @FilePath     : \IRremoteESP8266\app\remote_control\button.h
* @Description  : 
*******************************************************************************/
#ifndef BUTTON_H
#define BUTTON_H

#define POWER_PIN_NAME            "power_pin"
#define TEMP_UP_PIN_NAME          "temp_up_pin"
#define TEMP_DWON_PIN_NAME        "temp_down_pin"
#define FAN_PIN_NAME              "fan_pin"
#define MODE_SWITCH_PIN_NAME      "mode_switch_pin"
#define LEARN_PIN_NAME            "learn_pin"
#define DEVICE_SWITCH_PIN_NAME    "device_switch_pin"

void button_init(void);
void button_task(void);
#endif