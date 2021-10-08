/*******************************************************************************
* @Author       : Jon.Fang
* @Date         : 2021-10-07 18:19:31
* @LastEditors  : Jon.Fang
* @LastEditTime : 2021-10-08 15:56:57
* @FilePath     : \IRremoteESP8266\app\remote_control\ir_display.h
* @Description  :
*******************************************************************************/

#ifndef IR_DISPLAY_H
#define IR_DISPLAY_H

// 横轴X，纵轴Y
#define IR_DEVICE_BEGIN_POINT_X             6
#define IR_DEVICE_BEGIN_POINT_Y             6

#define IR_AC_MODE_BEGIN_POINT_X            6
#define IR_AC_MODE_BEGIN_POINT_Y            40

#define IR_VERTICAL_BEGIN_LINE_POINT_X      40
#define IR_VERTICAL_BEGIN_LINE_POINT_Y      0
#define IR_VERTICAL_BEGIN_LINE_POINT_LEN    64

#define IR_HORIZONTAL_LINE_BEGIN_POINT_X    40
#define IR_HORIZONTAL_LINE_BEGIN_POINT_Y    27

#define IR_WORK_BEGIN_POINT_X               73
#define IR_WORK_BEGIN_POINT_Y               6

#define IR_TEMP_BEGIN_POINT_X               65
#define IR_TEMP_BEGIN_POINT_Y               38

#define IR_TEMP_CODE_BEGIN_POINT_X          100
#define IR_TEMP_CODE_BEGIN_POINT_Y          43

// todo 温度图标跟随温度后续

void display_init(void);
void display_task(void);

#endif
