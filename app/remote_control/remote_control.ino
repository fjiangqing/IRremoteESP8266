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

#include "ir_display.h"

void setup()
{
    // 串口初始化，波特率：115200,无校验，一位停止位。
    Serial.begin(115200);
    delay(200);

    // 打印一条日志提示系统运行
    Serial.println("");
    Serial.println("jon remote control ...");

    // 调用红外学习，按钮，模式切换初始化
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
    // 显示功能初始化
    display_init();
}


void loop()
{
    // Now send the IR signal.
    // ac_control_on(ac_control_use);
    // 模式切换
    sys_ac_device_switch();
    
    // 按钮控制响应
    button_task();
    
    //学习红外处理 
    ir_learn_task();

    // 模式切换后更换红外发射控制
    for (ac_remote_control_t *index = ac_control_arr; index->type != AC_NULL_TYPE; ++index)
    {
        if (index->type == sys_ac_device_get())
        {
            ac_control_use = index;
        }
    }

    // 显示函数
    display_task();
    // gree_ac.send();

    // delay(100);
}
