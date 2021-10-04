/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-03 01:17:06
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-03 01:58:02
* @FilePath     : \IRremoteESP8266\app\remote_control\ir_learn.cpp
* @Description  :
*******************************************************************************/

#include "ir_learn.h"

#include <Arduino.h>

#include <IRremoteESP8266.h>

#include <IRrecv.h>

#include <IRutils.h>

#include <string.h>

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 14;

IRrecv irrecv(kRecvPin);

decode_results results;

const uint16_t learn_pin             = 12; // GPIO D6
const uint8_t  learn_pin_valid       = LOW;
const uint16_t learn_pin_valid_delay = 500;


void ir_learn_init(void)
{
    irrecv.enableIRIn(); // Start the receiver

    pinMode(learn_pin, INPUT_PULLUP);
}


ir_learn_status_t ir_learn_status = IR_LEARN_OK;
char              ir_learn_pin_name[32];

void ir_learn_task(void)
{
    static uint32_t timer_old = 0;

    if (digitalRead(learn_pin) == learn_pin_valid)
    {
        if (millis() - timer_old > learn_pin_valid_delay)
        {
            ir_learn_status = IR_LEARN;
        }
    }
    else
    {
        timer_old = millis();
    }

    if (irrecv.decode(&results))
    {
        // print() & println() can't handle printing long longs. (uint64_t)
        // serialPrintUint64(results.value, HEX);
        // Serial.println(results.value);
        Serial.printf("results.value = %llu\r\n", results.value);
        irrecv.resume(); // Receive the next value
    }
}
