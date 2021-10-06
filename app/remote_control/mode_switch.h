/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-02 23:46:31
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-06 14:59:40
* @FilePath     : \IRremoteESP8266\app\remote_control\mode_switch.h
* @Description  :
*******************************************************************************/
#ifndef MODE_SWITCH_H
#define MODE_SWITCH_H

#include "air_conditioner.h"

typedef enum
{
    AC_GREE_MODE  = AC_GREE_TYPE,
    AC_MIDEA_MODE = AC_MIDEA_TYPE,
    AC_LEARN_MODE = LEARN_IR_CONTROL_TYPE,
    AC_NULL_MODE,
} work_mode_t;

void sys_ac_device_switch_init(void);

work_mode_t sys_ac_device_switch(void);

work_mode_t sys_ac_device_get(void);

#endif
