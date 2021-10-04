/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-03 00:58:04
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-03 01:41:35
* @FilePath     : \IRremoteESP8266\app\remote_control\ir_learn.h
* @Description  : 
*******************************************************************************/

#ifndef IR_LEARN_H
#define IR_LEARN_H

typedef enum
{
    IR_LEARN = 0,
    IR_LEARN_OK,
} ir_learn_status_t;

void ir_learn_init(void);

void ir_recv_task(void);

#endif