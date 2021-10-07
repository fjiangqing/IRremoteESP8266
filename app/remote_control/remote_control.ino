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

#include <Wire.h>

#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

uint8_t make[] = { 0x04, 0x04, 0x24, 0x04, 0x24, 0x04, 0x3F, 0xA4, 0x44, 0x24, 0x04, 0x24, 0xFF, 0xE4, 0x04, 0x24, 0x04, 0x24, 0x3F, 0xA4, 0x24, 0xA4, 0x24, 0xA4, 0x26, 0x84, 0x25, 0x04, 0x04, 0x14, 0x04, 0x08 }; /*"制",0*/
uint8_t cool[] = { 0x00, 0x40, 0x40, 0x40, 0x20, 0xA0, 0x20, 0xA0, 0x01, 0x10, 0x02, 0x48, 0x14, 0x26, 0x10, 0x20, 0x23, 0xF8, 0xE0, 0x08, 0x20, 0x10, 0x21, 0x10, 0x20, 0xA0, 0x20, 0x40, 0x20, 0x20, 0x00, 0x20 }; /*"冷",0*/


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


    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.display();
    delay(1000);

    // Clear the buffer.
    display.clearDisplay();
    display.display();

    // text display tests
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
//   display.setCursor(0,0);
//   display.print("Connecting to SSID\n'adafruit':");
//   display.print("connected!");
//   display.setTextSize(2);
//   display.println("IP: 10.0.1.23");
//   display.println("Sending val #0");
    display.setCursor(0, 0);
    
    display.drawBitmap(0, 0, make, 2 * 8, 2 * 8, SSD1306_WHITE);

    display.drawBitmap(2 * 8, 0, cool, 2 * 8, 2 * 8, SSD1306_WHITE);
    // display.fillCircle(20, 20, 10, SSD1306_WHITE);
    display.display(); // actually display all of the above
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
