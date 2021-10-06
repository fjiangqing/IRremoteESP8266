/* Copyright 2016, 2018 David Conran
 *  Copyright 2020 Sadid Rafsun Tulon
 *
 * An IR LED circuit *MUST* be connected to the ESP8266 on a pin
 * as specified by kIrLed below.
 *
 * TL;DR: The IR LED needs to be driven by a transistor for a good result.
 *
 * Suggested circuit:
 *     https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending
 *
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */
#include "air_conditioner.h"

#include "mode_switch.h"

#include "button.h"

#include "ir_learn.h"

#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    delay(200);

    Serial.println("");
    Serial.println("jon remote control ...");

    ir_learn_init();

    button_init();

    sys_ac_device_switch_init();
    // 所有空调控制对象初始化
    for (ac_remote_control_t *index = ac_control_arr; index->type != AC_NULL_TYPE; ++index)
    {
        ac_control_init(index);
        if (index->type == AC_GREE_TYPE)
        {
            ac_control_use = index;
        }
    }
}


void loop()
{
    // Now send the IR signal.
    // ac_control_on(ac_control_use);

    sys_ac_device_switch();

    button_task();

    ir_learn_task();

    for (ac_remote_control_t *index = ac_control_arr; index->type != AC_NULL_TYPE; ++index)
    {
        if (index->type == sys_ac_device_get())
        {
            ac_control_use = index;
        }
    }

    // gree_ac.send();

    // delay(100);
}
