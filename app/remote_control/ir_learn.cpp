/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-03 01:17:06
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-12 10:24:05
* @FilePath     : \IRremoteESP8266\app\remote_control\ir_learn.cpp
* @Description  :
*******************************************************************************/

#include "ir_learn.h"

#include "button.h"

#include <Arduino.h>

#include <IRremoteESP8266.h>

#include <IRutils.h>

#include <EEPROM.h>

#include <string.h>


ir_code_record_t ir_button_code_record;
ir_schedule_t    ir_schedule = IR_LEARN_WAIT_BUTTON;

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.

const uint16_t kRecvPin = 13;   // D7

IRrecv irrecv(kRecvPin);


decode_results results;

const uint16_t learn_pin             = 0; // GPIO D3
const uint8_t  learn_pin_valid       = LOW;
const uint16_t learn_pin_valid_delay = 2000;


#define EEPROM_SAVA_BEGIN_ADDRESS    0

void eeprom_read_data(uint32_t address, uint8_t *data, uint16_t lenght)
{
    for (uint16_t index = 0; index < lenght; ++index)
    {
        data[index] = EEPROM.read(address + index);
    }
}


void eeprom_write_data(uint32_t address, uint8_t *data, uint16_t lenght)
{
    for (uint16_t index = 0; index < lenght; ++index)
    {
        EEPROM.write(address + index, data[index]);
    }

    if (EEPROM.commit() == true)
    {
        Serial.println("wirte to eeprom pass");
    }
    else
    {
        Serial.println("wirte to eeprom ng");
    }
}


void ir_learn_init(void)
{
    // 红外接收对线初始化，负责红外接收功能
    irrecv.enableIRIn(); // Start the receiver

    // GPIO 学习按钮初始化上拉输入
    pinMode(learn_pin, INPUT_PULLUP);

    // EEPROM初始化，存储大小为512byte
    EEPROM.begin(512);

    // 读取flash记录数据,学习按钮掉电存储位置
    eeprom_read_data(EEPROM_SAVA_BEGIN_ADDRESS, (uint8_t *)&ir_button_code_record, sizeof(ir_button_code_record));
}


ir_learn_status_t ir_learn_status = IR_LEARN_OK;
char              ir_learn_pin_name[32];

// 长按学习按钮,进入学习模式
// 按下其它按钮,标记学习存储按钮信息
// 外部遥控器发射红外数据
// 遥控器接收到红外,屏幕上提示接收数值
// 长按学习按钮,完成学习

uint16_t ir_learn_pin_list[] = { power_pin, temp_up_pin, temp_down_pin, mode_switch_pin };

void ir_code_record_flash(uint16_t& learn_button_pin, decode_results& decode)
{
    // 判断要存储在那个按钮下
    switch (learn_button_pin)
    {
    case temp_down_pin:
        // ir_button_code_record.temp_down_button_code = results.value;
        ir_button_code_record.temp_down_button_code = decode;
        break;

    case temp_up_pin:
        ir_button_code_record.temp_up_button_code = decode;
        break;

    case power_pin:
        ir_button_code_record.power_button_code = decode;
        break;

    case mode_switch_pin:
        ir_button_code_record.mode_switch_button_code = decode;
        break;

    default:
        break;
    }

    // 学习数据写入flash
    eeprom_write_data(EEPROM_SAVA_BEGIN_ADDRESS, (uint8_t *)&ir_button_code_record, sizeof(ir_button_code_record));
}


void ir_learn_task(void)
{
    static uint32_t timer_old             = 0;
    static uint8_t  pin_button_old        = 0;
    static uint8_t  pin_long_timer_button = 0;
    static uint16_t learn_button_pin      = 0;

    static uint32_t learn_wait_enter    = 0;
    static uint8_t  learn_wait_enter_ok = 0;


    // 检测按钮长按
    if (digitalRead(learn_pin) == learn_pin_valid)
    {
        if ((millis() - timer_old > learn_pin_valid_delay) &&
            (pin_button_old == 0))
        {
            pin_long_timer_button = 1;
            pin_button_old        = 1;
            Serial.println("learn long");
            // ir_learn_status = IR_LEARN;
        }
    }
    else
    {
        pin_button_old = 0;
        timer_old      = millis();
    }

    // 进入学习模式
    if ((ir_learn_status == IR_LEARN_OK) && (pin_long_timer_button == 1))
    {
        pin_long_timer_button = 0;
        ir_learn_status       = IR_LEARN;
        ir_schedule           = IR_LEARN_WAIT_BUTTON;
        learn_wait_enter      = millis();
        learn_wait_enter_ok   = 0;
    }

    // 等待学习按钮松开
    if ((ir_schedule == IR_LEARN_WAIT_BUTTON) && (digitalRead(learn_pin) == HIGH))
    {
        if (millis() - learn_wait_enter < 1000)
        {
            learn_wait_enter_ok = 0;
            return;
        }
        else
        {
            learn_wait_enter_ok = 1;
        }
    }


    if (learn_wait_enter_ok == 0)
    {
        return;
    }

    // 检测那个按钮按下，学习到的信息存储与这个按钮下
    if (ir_schedule == IR_LEARN_WAIT_BUTTON)
    {
        // 检测那个学习按钮按下
        for (uint8_t index = 0; index < sizeof(ir_learn_pin_list); ++index)
        {
            if (digitalRead(ir_learn_pin_list[index]) == LOW)
            {
                learn_button_pin = ir_learn_pin_list[index];
                ir_schedule      = IR_LEARN_WAIT_IR_VALUE;
            }
        }
    }


    // 接收红外数据,results,存储解码后的数据。
    if (irrecv.decode(&results))
    {
        // results.value：接收红外数据值
        Serial.printf("results.value = %llu\r\n", results.value);
        irrecv.resume(); // Receive the next value
        ir_schedule = IR_LEARN_FINISH;
    }

    // 学习完成检测
    if ((ir_learn_status == IR_LEARN) && (pin_long_timer_button == 1))
    {
        pin_long_timer_button = 0;
        ir_learn_status       = IR_LEARN_OK;
        // 学习完成存储信息
        ir_code_record_flash(learn_button_pin, results);
        ir_schedule = IR_LEARN_WAIT_BUTTON;
    }
}
