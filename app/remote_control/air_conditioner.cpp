/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-02 18:40:30
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-08 16:24:26
* @FilePath     : \IRremoteESP8266\app\remote_control\air_conditioner.cpp
* @Description  :
*******************************************************************************/

#include "air_conditioner.h"

#include "ir_learn.h"

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
#define RUNNING_LOG(VALUE_NAME)                                 \
    do{                                                         \
        Serial.printf("%s,%s\r\n", __FILE__, __func__);         \
        Serial.printf("%s = %d\r\n", # VALUE_NAME, VALUE_NAME); \
    }while (0)


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
    ac->ac_ir_work_status = AC_IR_WORK_RUN;
    RUNNING_LOG(ac->ac_ir_work_status);
}


void ac_control_off(ac_remote_control_t *ac)
{
    ac->control->ac_off();
    ac->ac_ir_work_status = AC_IR_WORK_CLOSE;
    RUNNING_LOG(ac->ac_ir_work_status);
}


void ac_control_temp_up(ac_remote_control_t *ac)
{
    ac->control->ac_temp_up();
}


void ac_control_temp_down(ac_remote_control_t *ac)
{
    ac->control->ac_temp_down();
}


int ac_control_temp_get(ac_remote_control_t *ac)
{
    if (ac->control->ac_temp_get != NULL)
    {
        return ac->control->ac_temp_get();
    }
    else
    {
        return 0;
    }
}


void ac_control_mode_switch(ac_remote_control_t *ac)
{
    ac->control->ac_mode_switch();
}


// todo:添加遥控器对象

const uint16_t kIrLed = 15; // ESP8266 GPIO pin to use. Recommended: 15 (D8).

IRGreeAC gree_ac(kIrLed);   // Set the GPIO to be used for sending messages.

IRMideaAC midea_ac(kIrLed);

IRsend universal_ir_send(kIrLed);

// todo:添加温度管理
static uint8_t gree_ac_temp  = 25;
static uint8_t midea_ac_temp = 25;

// todo:空调模式管理
static uint8_t gree_ac_mode  = 0;
static uint8_t midea_ac_mode = 0;


// todo:检查初始化
void gree_ac_init(void)
{
    gree_ac.begin();

    // Set up what we want to send. See ir_Gree.cpp for all the options.
    // Most things default to off.
    gree_ac.on();
    gree_ac.setFan(1);
    // kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
    gree_ac.setMode(gree_ac_mode);
    gree_ac.setTemp(gree_ac_temp); // 16-30C
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
    midea_ac.setMode(midea_ac_mode);
    midea_ac.setTemp(midea_ac_temp); // 16-30C
    midea_ac.setSleep(false);
    midea_ac.setLightToggle(true);

    Serial.println("midea_ac init");
}


void universal_ir_init(void)
{
    universal_ir_send.begin();
    Serial.printf("%s init\r\n", __func__);
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
    RUNNING_LOG(NULL);
}


void ir_learn_send(ir_code_t& code)
{
    universal_ir_send.send(code.decode_type, \
                           code.value, sizeof(code.value) * 8);
}


void universal_ir_on(void)
{
    // ir_code_t button_ir_code = ir_button_code_record.power_button_code;

    ir_learn_send(ir_button_code_record.power_button_code);

    RUNNING_LOG(NULL);
}


// todo:添加off
void gree_ac_off(void)
{
    gree_ac.off();
    gree_ac.send();
    RUNNING_LOG(NULL);
}


void midea_ac_off(void)
{
    midea_ac.off();
    midea_ac.send();
    RUNNING_LOG(NULL);
}


void universal_ir_off(void)
{
    // ir_code_t button_ir_code = ir_button_code_record.power_button_code;

    ir_learn_send(ir_button_code_record.power_button_code);

    RUNNING_LOG(NULL);
}


// todo:添加温度增加
void gree_ac_temp_up(void)
{
    gree_ac_temp++;

    gree_ac.setTemp(gree_ac_temp);

    gree_ac.send();

    gree_ac_temp = gree_ac.getTemp();

    RUNNING_LOG(gree_ac_temp);
}


void midea_ac_temp_up(void)
{
    midea_ac_temp++;

    midea_ac.setTemp(midea_ac_temp);

    midea_ac.send();

    midea_ac_temp = midea_ac.getTemp();

    RUNNING_LOG(midea_ac_temp);
}


void universal_ir_temp_up(void)
{
    // ir_code_t button_ir_code = ir_button_code_record.power_button_code;

    ir_learn_send(ir_button_code_record.temp_up_button_code);

    RUNNING_LOG(NULL);
}


// todo:添加温度减少
void gree_ac_temp_down(void)
{
    gree_ac_temp--;

    gree_ac.setTemp(gree_ac_temp);

    gree_ac.send();

    gree_ac_temp = gree_ac.getTemp();

    RUNNING_LOG(gree_ac_temp);
}


void midea_ac_temp_down(void)
{
    midea_ac_temp--;

    midea_ac.setTemp(midea_ac_temp);

    midea_ac.send();

    midea_ac_temp = midea_ac.getTemp();

    RUNNING_LOG(midea_ac_temp);
}


// 温度获取

int gree_temp_get()
{
    int temp = gree_ac.getTemp();

    RUNNING_LOG(temp);

    return temp;
}


int midea_temp_get()
{
    int temp = midea_ac.getTemp();

    RUNNING_LOG(temp);

    return temp;
}


void universal_ir_temp_down(void)
{
    // ir_code_t button_ir_code = ir_button_code_record.power_button_code;

    ir_learn_send(ir_button_code_record.temp_down_button_code);

    RUNNING_LOG(NULL);
}


// todo: 空调模式切换
void gree_ac_mode_switch(void)
{
    if (gree_ac_mode < kGreeHeat)
    {
        ++gree_ac_mode;
    }
    else
    {
        gree_ac_mode = kGreeAuto;
    }

    gree_ac.setMode(gree_ac_mode);

    gree_ac.send();

    RUNNING_LOG(gree_ac_mode);
}


void midea_ac_mode_switch(void)
{
    if (midea_ac_mode < kMideaACFan)
    {
        ++midea_ac_mode;
    }
    else
    {
        midea_ac_mode = kMideaACCool;
    }

    midea_ac.setMode(midea_ac_mode);

    midea_ac.send();

    RUNNING_LOG(midea_ac_mode);
}


void universal_ir_mode_switch(void)
{
    // ir_code_t button_ir_code = ir_button_code_record.power_button_code;

    ir_learn_send(ir_button_code_record.mode_switch_button_code);

    RUNNING_LOG(NULL);
}


// todo: 添加构造函数初始化
ac_control_t gree_ac_control =
{
    .ac_init        = gree_ac_init,
    .ac_on          = gree_ac_on,
    .ac_off         = gree_ac_off,
    .ac_temp_up     = gree_ac_temp_up,
    .ac_temp_down   = gree_ac_temp_down,
    .ac_mode_switch = gree_ac_mode_switch,
    .ac_temp_get    = gree_temp_get,
};

ac_control_t midea_ac_control =
{
    .ac_init        = midea_ac_init,
    .ac_on          = midea_ac_on,
    .ac_off         = midea_ac_off,
    .ac_temp_up     = midea_ac_temp_up,
    .ac_temp_down   = midea_ac_temp_down,
    .ac_mode_switch = midea_ac_mode_switch,
    .ac_temp_get    = midea_temp_get,
};

ac_control_t universal_ir_control =
{
    .ac_init        = universal_ir_init,
    .ac_on          = universal_ir_on,
    .ac_off         = universal_ir_off,
    .ac_temp_up     = universal_ir_temp_up,
    .ac_temp_down   = universal_ir_temp_down,
    .ac_mode_switch = universal_ir_mode_switch,
};

// todo:列表切换空调遥控器
ac_remote_control_t ac_control_arr[] =
{
    { AC_GREE_TYPE,          &gree_ac_control,      AC_IR_WORK_CLOSE },
    { AC_MIDEA_TYPE,         &midea_ac_control,     AC_IR_WORK_CLOSE },
    { LEARN_IR_CONTROL_TYPE, &universal_ir_control, AC_IR_WORK_CLOSE },
    { AC_NULL_TYPE,          NULL,                  AC_IR_WORK_CLOSE }
};
