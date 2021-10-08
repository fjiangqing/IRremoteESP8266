/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-07 18:20:20
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-08 15:53:51
* @FilePath     : \IRremoteESP8266\app\remote_control\ir_display.cpp
* @Description  :
*******************************************************************************/

#include "ir_display.h"

#include "air_conditioner.h"

#include <Arduino.h>

#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

typedef uint16_t   display_point_t;
typedef uint16_t   display_pixels_t;

typedef struct
{
    uint8_t          *data;
    uint8_t          word_number;
    display_point_t  begin_x;
    display_point_t  begin_y;
    display_pixels_t width;
    display_pixels_t high;
} ir_display_data_t;

ir_display_data_t display_devices;
ir_display_data_t display_mode_status;
ir_display_data_t display_work_status;
ir_display_data_t display_temp;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

uint8_t make[] = { 0x04, 0x04, 0x24, 0x04, 0x24, 0x04, 0x3F, 0xA4, 0x44, 0x24, 0x04, 0x24, 0xFF, 0xE4, 0x04, 0x24, 0x04, 0x24, 0x3F, 0xA4, 0x24, 0xA4, 0x24, 0xA4, 0x26, 0x84, 0x25, 0x04, 0x04, 0x14, 0x04, 0x08 }; /*"制",0*/
uint8_t cool[] = { 0x00, 0x40, 0x40, 0x40, 0x20, 0xA0, 0x20, 0xA0, 0x01, 0x10, 0x02, 0x48, 0x14, 0x26, 0x10, 0x20, 0x23, 0xF8, 0xE0, 0x08, 0x20, 0x10, 0x21, 0x10, 0x20, 0xA0, 0x20, 0x40, 0x20, 0x20, 0x00, 0x20 }; /*"冷",0*/

// 设备名称
uint8_t device_gree_name[] = {
    0x10, 0x80, 0x10, 0x80, 0x10, 0xF8, 0x11, 0x08, 0xFB, 0x10, 0x14, 0xA0, 0x30, 0x40, 0x38, 0xA0, 0x55, 0x18, 0x52, 0x06, 0x95, 0xF8, 0x11, 0x08, 0x11, 0x08, 0x11, 0x08, 0x11, 0xF8, 0x11, 0x08,  /*"格",0*/
    0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x7F, 0xF8, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x04, 0x08, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x88, 0x20, 0x50, 0x40, 0x20,  /*"力",1*/
};

uint8_t device_midea_name[] = {
    0x08, 0x20, 0x04, 0x40, 0x7F, 0xFC, 0x01, 0x00, 0x01, 0x00, 0x3F, 0xF8, 0x01, 0x00, 0x01, 0x00, 0xFF, 0xFE, 0x01, 0x00, 0x01, 0x00, 0x7F, 0xFC, 0x02, 0x80, 0x04, 0x40, 0x18, 0x30, 0xE0, 0x0E, /*"美",0*/
    0x10, 0x40, 0x10, 0x40, 0x20, 0x40, 0x7E, 0x7C, 0x42, 0x84, 0x42, 0x84, 0x43, 0x04, 0x42, 0x44, 0x7E, 0x24, 0x42, 0x24, 0x42, 0x04, 0x42, 0x04, 0x42, 0x04, 0x7E, 0x04, 0x42, 0x28, 0x00, 0x10, /*"的",1*/
};

uint8_t device_learn_name[] = {
    0x22, 0x08, 0x11, 0x08, 0x11, 0x10, 0x00, 0x20, 0x7F, 0xFE, 0x40, 0x02, 0x80, 0x04, 0x1F, 0xE0, 0x00, 0x40, 0x01, 0x80, 0xFF, 0xFE, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x05, 0x00, 0x02, 0x00, /*"学",0*/
    0x00, 0x00, 0x7F, 0xF8, 0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x04, 0x08, 0x02, 0x08, 0x02, 0x08, 0x00, 0x68, 0x01, 0x88, 0x0E, 0x08, 0x70, 0x08, 0x20, 0x08, 0x00, 0x08, 0x00, 0x50, 0x00, 0x20, /*"习",1*/
};

// 模式
uint8_t mode_cool_word[] = {
    0x04, 0x04, 0x24, 0x04, 0x24, 0x04, 0x3F, 0xA4, 0x44, 0x24, 0x04, 0x24, 0xFF, 0xE4, 0x04, 0x24, 0x04, 0x24, 0x3F, 0xA4, 0x24, 0xA4, 0x24, 0xA4, 0x26, 0x84, 0x25, 0x04, 0x04, 0x14, 0x04, 0x08, /*"制",0*/
    0x00, 0x40, 0x40, 0x40, 0x20, 0xA0, 0x20, 0xA0, 0x01, 0x10, 0x02, 0x48, 0x14, 0x26, 0x10, 0x20, 0x23, 0xF8, 0xE0, 0x08, 0x20, 0x10, 0x21, 0x10, 0x20, 0xA0, 0x20, 0x40, 0x20, 0x20, 0x00, 0x20, /*"冷",1*/
};

uint8_t mode_hot_word[] = {
    0x04, 0x04, 0x24, 0x04, 0x24, 0x04, 0x3F, 0xA4, 0x44, 0x24, 0x04, 0x24, 0xFF, 0xE4, 0x04, 0x24, 0x04, 0x24, 0x3F, 0xA4, 0x24, 0xA4, 0x24, 0xA4, 0x26, 0x84, 0x25, 0x04, 0x04, 0x14, 0x04, 0x08, /*"制",0*/
    0x10, 0x40, 0x10, 0x40, 0x10, 0x40, 0xFD, 0xF8, 0x10, 0x48, 0x10, 0x48, 0x1C, 0xC8, 0x30, 0x48, 0xD0, 0xAA, 0x10, 0xAA, 0x51, 0x06, 0x22, 0x02, 0x00, 0x00, 0x48, 0x88, 0x44, 0x44, 0x84, 0x44, /*"热",1*/
};

uint8_t mode_auto_word[] = {
    0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x1F, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10, /*"自",0*/
    0x00, 0x40, 0x00, 0x40, 0x7C, 0x40, 0x00, 0x40, 0x01, 0xFC, 0x00, 0x44, 0xFE, 0x44, 0x20, 0x44, 0x20, 0x44, 0x20, 0x84, 0x48, 0x84, 0x44, 0x84, 0xFD, 0x04, 0x45, 0x04, 0x02, 0x28, 0x04, 0x10, /*"动",1*/
};

uint8_t mode_fan_word[] = {
    0x00, 0x80, 0x00, 0x80, 0x78, 0x80, 0x48, 0xFC, 0x49, 0x04, 0x49, 0x08, 0x4A, 0x40, 0x4C, 0x40, 0x48, 0x40, 0x48, 0xA0, 0x78, 0xA0, 0x49, 0x10, 0x01, 0x10, 0x02, 0x08, 0x04, 0x04, 0x08, 0x02, /*"吹",0*/
    0x00, 0x00, 0x3F, 0xF0, 0x20, 0x10, 0x20, 0x10, 0x28, 0x50, 0x24, 0x50, 0x22, 0x90, 0x22, 0x90, 0x21, 0x10, 0x21, 0x10, 0x22, 0x90, 0x22, 0x92, 0x24, 0x4A, 0x48, 0x4A, 0x40, 0x06, 0x80, 0x02, /*"风",1*/
};

// 温度符合
uint8_t temp_code_word[] = {
    0x60, 0x00, 0x91, 0xF4, 0x96, 0x0C, 0x6C, 0x04, 0x08, 0x04, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x08, 0x00, 0x0C, 0x04, 0x06, 0x08, 0x01, 0xF0, 0x00, 0x00, /*"℃",0*/
};

// 运行
uint8_t ir_run_word[] = {
    0x00, 0x00, 0x23, 0xF8, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00, 0x07, 0xFC, 0xF0, 0x40, 0x10, 0x80, 0x11, 0x10, 0x12, 0x08, 0x17, 0xFC, 0x12, 0x04, 0x10, 0x00, 0x28, 0x00, 0x47, 0xFE, 0x00, 0x00, /*"运",0*/
    0x08, 0x00, 0x09, 0xFC, 0x10, 0x00, 0x20, 0x00, 0x48, 0x00, 0x08, 0x00, 0x13, 0xFE, 0x30, 0x20, 0x50, 0x20, 0x90, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0xA0, 0x10, 0x40, /*"行",1*/
};

// 运行
uint8_t ir_close_word[] = {
    0x10, 0x10, 0x08, 0x10, 0x08, 0x20, 0x00, 0x00, 0x3F, 0xF8, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0xFF, 0xFE, 0x01, 0x00, 0x02, 0x80, 0x02, 0x80, 0x04, 0x40, 0x08, 0x20, 0x30, 0x18, 0xC0, 0x06, /*"关",0*/
    0x10, 0x00, 0x11, 0xF0, 0x11, 0x10, 0x11, 0x10, 0xFD, 0x10, 0x11, 0x10, 0x31, 0x10, 0x39, 0x10, 0x55, 0x10, 0x55, 0x10, 0x91, 0x10, 0x11, 0x12, 0x11, 0x12, 0x12, 0x12, 0x12, 0x0E, 0x14, 0x00, /*"机",1*/
};

#define CHINESE_16BIT_1WORD_PIXELS_WIDTH    16
#define CHINESE_16BIT_2WORD_PIXELS_WIDTH    (CHINESE_16BIT_1WORD_PIXELS_WIDTH * 2)
#define CHINESE_16BIT_1WORD_PIXELS_HIGH     16

void display_chinese_16bit(uint8_t *word, uint8_t word_number, display_point_t x, display_point_t y);
void display_work_mode_switch(void);
void ir_display_chinese_data(ir_display_data_t& display_data);

void display_init(void)
{
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
    // display.println("℃");
    display.setCursor(0, 0);

    // display.drawBitmap(0, 0, make, 2 * 8, 2 * 8, SSD1306_WHITE);

    // display.drawBitmap(2 * 8, 0, cool, 2 * 8, 2 * 8, SSD1306_WHITE);
    // display.fillCircle(20, 20, 10, SSD1306_WHITE);

    // display.drawBitmap(0, 0, device_gree_name, 2 * 16, 1 * 16, SSD1306_WHITE);
    // display_chinese_16bit(temp_code_word, 1, IR_DEVICE_BEGIN_POINT_X, IR_DEVICE_BEGIN_POINT_Y);
    display.display(); // actually display all of the above


    // display_chinese_16bit(device_gree_name, 2, 0, 0);
    // display_chinese_16bit(mode_cool_word, 2, 0, 40);

    display_work_mode_switch();
    ir_display_chinese_data(display_devices);

    ir_display_chinese_data(display_mode_status);

    display.drawFastVLine(IR_VERTICAL_BEGIN_LINE_POINT_X, IR_VERTICAL_BEGIN_LINE_POINT_Y, IR_VERTICAL_BEGIN_LINE_POINT_LEN, SSD1306_WHITE);
    display.display();

    display.drawFastHLine(IR_HORIZONTAL_LINE_BEGIN_POINT_X, IR_HORIZONTAL_LINE_BEGIN_POINT_Y, 128 - IR_HORIZONTAL_LINE_BEGIN_POINT_X, SSD1306_WHITE);
    display.display();

    ir_display_chinese_data(display_work_status);

    
    display.setCursor(IR_TEMP_BEGIN_POINT_X, IR_TEMP_BEGIN_POINT_Y);
    display.setTextSize(3);
    display.println("25");
    display.display();

    display_chinese_16bit(temp_code_word, 1, IR_TEMP_CODE_BEGIN_POINT_X, IR_TEMP_CODE_BEGIN_POINT_Y);
    
    display.display();
}


void display_chinese_16bit(uint8_t *word, uint8_t word_number, display_point_t x, display_point_t y)
{
    uint16_t word_pixels = 16;

    for (uint16_t begin_x = x, begin_y = y, word_index = 0; word_index < word_number; ++word_index)
    {
        display.drawBitmap(begin_x, begin_y, word + (word_pixels * 2 * word_index), word_pixels, word_pixels, SSD1306_WHITE);

        begin_x += word_pixels;
        // todo：添加换行
        Serial.printf("__func__ = %s\r\n", __func__);
        // begin_y += word_pixels;
    }
    display.display();
}


void ir_display_chinese_data(ir_display_data_t& display_data)
{
    // display.setCursor(display_data.begin_x, display_data.begin_y);
    display_chinese_16bit(display_data.data, display_data.word_number, display_data.begin_x, display_data.begin_y);
}


void display_devices_name_set(uint8_t *device_name, uint8_t word_number = 2)
{
    display_devices.data        = device_name;
    display_devices.word_number = word_number;

    display_devices.begin_x = IR_DEVICE_BEGIN_POINT_X;
    display_devices.begin_y = IR_DEVICE_BEGIN_POINT_Y;

    display_devices.width = 0;
    display_devices.high  = 0;
}


void display_mode_name_set(uint8_t *mode_name, uint8_t word_number = 2)
{
    display_mode_status.data        = mode_name;
    display_mode_status.word_number = word_number;

    display_mode_status.begin_x = IR_AC_MODE_BEGIN_POINT_X;
    display_mode_status.begin_y = IR_AC_MODE_BEGIN_POINT_Y;

    display_mode_status.width = 0;
    display_mode_status.high  = 0;
}


void display_work_status_name_set(uint8_t *mode_name, uint8_t word_number = 2)
{
    display_work_status.data        = mode_name;
    display_work_status.word_number = word_number;

    display_work_status.begin_x = IR_WORK_BEGIN_POINT_X;
    display_work_status.begin_y = IR_WORK_BEGIN_POINT_Y;

    display_work_status.width = 0;
    display_work_status.high  = 0;
}


void display_work_mode_switch(void)
{
    Serial.printf("ac_control_use->type = %d\r\n", ac_control_use->type);
    switch (ac_control_use->type)
    {
    case AC_GREE_TYPE:

        display_devices_name_set(device_gree_name);
        display_mode_name_set(mode_cool_word);
        break;

    case AC_MIDEA_TYPE:
        /* code */
        break;

    case LEARN_IR_CONTROL_TYPE:
        /* code */
        break;

    default:
        break;
    }

    if (ac_control_use->ac_ir_work_status == AC_IR_WORK_RUN)
    {
        display_work_status_name_set(ir_run_word);
    }
    else
    {
        display_work_status_name_set(ir_close_word);
    }
}


void display_task(void)
{
    return;

    #if 0
    static uint32_t timer_old = 0;

    if (millis() - timer_old > 500)
    {
        Serial.printf("__func__ = %s\r\n", __func__);

        timer_old = millis();

        // display.clearDisplay();
        // display.display();


        display_work_mode_switch();

        ir_display_chinese_data(display_devices);

        // ir_display_chinese_data(display_mode_status);
    }
    #endif
}
