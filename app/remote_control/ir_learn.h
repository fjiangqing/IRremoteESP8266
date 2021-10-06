/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-03 00:58:04
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-06 15:14:18
* @FilePath     : \IRremoteESP8266\app\remote_control\ir_learn.h
* @Description  : 
*******************************************************************************/

#ifndef IR_LEARN_H
#define IR_LEARN_H

#include <IRrecv.h>

typedef enum
{
    IR_LEARN = 0,
    IR_LEARN_OK,
} ir_learn_status_t;

typedef decode_results ir_code_t;


typedef struct
{
    ir_code_t temp_down_button_code;
    ir_code_t temp_up_button_code;
    ir_code_t power_button_code;
    ir_code_t mode_switch_button_code;
} ir_code_record_t;

void ir_learn_init(void);

void ir_learn_task(void);

extern ir_code_record_t ir_button_code_record;

#endif