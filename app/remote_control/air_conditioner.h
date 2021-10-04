/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-02 18:33:41
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-03 00:12:10
* @FilePath     : \IRremoteESP8266\app\remote_control\air_conditioner.h
* @Description  :
*******************************************************************************/

#ifndef AIR_CONDITIONER_H
#define AIR_CONDITIONER_H

#include <stdint.h>

// 遥控流程
// 初始化
// 修改某个参数
// 发送红外数据

typedef void (*ac_init_call_t)(void); //空调遥控初始化
typedef void (*ac_on_call_t)(void);   //空调开控制
typedef void (*ac_off_call_t)(void);  //空调关控制
typedef void (*ac_temp_call_t)(void); //空调温度控制


typedef struct
{
    ac_init_call_t ac_init;
    ac_on_call_t   ac_on;
    ac_off_call_t  ac_off;
    ac_temp_call_t ac_temp_up;
    ac_temp_call_t ac_temp_down;
} ac_control_t;

typedef enum
{
    AC_GREE_TYPE = 0,
    AC_MIDEA_TYPE,
    AC_NULL_TYPE,
} ac_type_t;

typedef struct
{
    ac_type_t    type;
    ac_control_t *control;
} ac_remote_control_t;

void ac_control_init(ac_remote_control_t *ac);
void ac_control_on(ac_remote_control_t *ac);
void ac_control_off(ac_remote_control_t *ac);
void ac_control_temp_up(ac_remote_control_t *ac);
void ac_control_temp_down(ac_remote_control_t *ac);

extern ac_remote_control_t *ac_control_use;
extern ac_remote_control_t ac_control_arr[];

#endif
