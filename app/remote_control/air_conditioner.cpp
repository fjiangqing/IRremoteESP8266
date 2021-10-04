/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-02 18:40:30
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-04 23:13:39
* @FilePath     : \IRremoteESP8266\app\remote_control\air_conditioner.cpp
* @Description  :
*******************************************************************************/

#include "air_conditioner.h"

#include <Arduino.h>

#include <IRremoteESP8266.h>

#include <IRsend.h>

#include <ir_Gree.h>

#include <ir_Midea.h>

#include <stddef.h>

// 空调是初始化
// 模式
// 温度
// 开机

ac_remote_control_t *ac_control_use = NULL;
// {
//     .ac_init      = NULL,
//     .ac_on        = NULL,
//     .ac_off       = NULL,
//     .ac_temp_up   = NULL,
//     .ac_temp_down = NULL,
// };


void ac_control_init(ac_remote_control_t *ac)
{
    ac->control->ac_init();
}


void ac_control_on(ac_remote_control_t *ac)
{
    ac->control->ac_on();
}


void ac_control_off(ac_remote_control_t *ac)
{
    ac->control->ac_off();
}


void ac_control_temp_up(ac_remote_control_t *ac)
{
    ac->control->ac_temp_up();
}


void ac_control_temp_down(ac_remote_control_t *ac)
{
    ac->control->ac_temp_down();
}


// todo:添加遥控器对象

const uint16_t kIrLed = 15; // ESP8266 GPIO pin to use. Recommended: 15 (D8).

IRGreeAC gree_ac(kIrLed);   // Set the GPIO to be used for sending messages.

IRMideaAC midea_ac(kIrLed);


// todo:添加温度管理
static uint8_t gree_ac_temp  = 25;
static uint8_t midea_ac_temp = 25;

// todo:检查初始化
void gree_ac_init(void)
{
    gree_ac.begin();

    // Set up what we want to send. See ir_Gree.cpp for all the options.
    // Most things default to off.
    gree_ac.on();
    gree_ac.setFan(1);
    // kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
    gree_ac.setMode(kGreeCool);
    gree_ac.setTemp(25); // 16-30C
    gree_ac.setSwingVertical(true, kGreeSwingAuto);
    gree_ac.setXFan(false);
    gree_ac.setLight(true);
    gree_ac.setSleep(false);
    gree_ac.setTurbo(false);
    Serial.println("gree_ac init");
}


void midea_ac_init(void)
{
    midea_ac.begin();
    midea_ac.on();
    midea_ac.setFan(1);
    // kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
    midea_ac.setMode(kMideaACCool);
    midea_ac.setTemp(midea_ac_temp); // 16-30C
    midea_ac.setSleep(false);
    Serial.println("midea_ac init");
}


// todo:添加on
void gree_ac_on(void)
{
    gree_ac.on();
    gree_ac.send();
    Serial.printf("FILE = %s,LINE = %d,function = %s\r\n", __FILE__, __LINE__, __func__);
}


void midea_ac_on(void)
{
    midea_ac.on();
    midea_ac.send();
}


// todo:添加off
void gree_ac_off(void)
{
    gree_ac.off();
    gree_ac.send();
}


void midea_ac_off(void)
{
    midea_ac.off();
    midea_ac.send();
}


// todo:添加温度增加
void gree_ac_temp_up(void)
{
    gree_ac_temp++;

    gree_ac.setTemp(gree_ac_temp);

    gree_ac.send();

    gree_ac_temp = gree_ac.getTemp();
}


void midea_ac_temp_up(void)
{
    midea_ac_temp++;

    midea_ac.setTemp(midea_ac_temp);

    midea_ac.send();

    midea_ac_temp = midea_ac.getTemp();
}


// todo:添加温度减少
void gree_ac_temp_down(void)
{
    gree_ac_temp--;

    gree_ac.setTemp(gree_ac_temp);

    gree_ac.send();

    gree_ac_temp = gree_ac.getTemp();
}


void midea_ac_temp_down(void)
{
    midea_ac_temp--;

    midea_ac.setTemp(midea_ac_temp);

    midea_ac.send();

    midea_ac_temp = midea_ac.getTemp();
}


// todo: 添加构造函数初始化
ac_control_t gree_ac_control =
{
    .ac_init      = gree_ac_init,
    .ac_on        = gree_ac_on,
    .ac_off       = gree_ac_off,
    .ac_temp_up   = gree_ac_temp_up,
    .ac_temp_down = gree_ac_temp_down,
};

ac_control_t midea_ac_control =
{
    .ac_init      = midea_ac_init,
    .ac_on        = midea_ac_on,
    .ac_off       = midea_ac_off,
    .ac_temp_up   = midea_ac_temp_up,
    .ac_temp_down = midea_ac_temp_down,
};

// todo:列表切换空调遥控器
ac_remote_control_t ac_control_arr[] =
{
    { AC_GREE_TYPE,  &gree_ac_control  },
    { AC_MIDEA_TYPE, &midea_ac_control },
    { AC_NULL_TYPE,  NULL              }
};
