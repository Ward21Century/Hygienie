/* Example application which uses testable component.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include "esp_system.h"
#include "testable.h"
#include "esp_wifi.h"
#include "driver/adc.h"
#include "AppGraphics.h"
#include "appmqtt.h"
#include "AppSleep.h"
#include "appwifi.h"

#include "ssd1306.h"
#include "u8g2_esp32_hal.h"
/* This application has a test subproject in 'test' directory, all the
 interesting things happen there. See ../test/main/example_idf_test_runner_test.c
 and the makefiles in ../test/ directory.

 This specific app_main function is provided only to illustrate the layout
 of a project.
*/

void app_main() {

    AppSleepWakeUpFromDeepSleep();
    AppGraphicsAnimationCycle();
    AppMqttAddTime();
    if (AppMqttGetNumoffLineReadingCount() < MAX_OFFLINE_READINGS-1) {
        printf("Number of readings until next transmission:\r\n");
        int num_offline_remaining = MAX_OFFLINE_READINGS - AppMqttGetNumoffLineReadingCount();
        printf("%d\r\n", num_offline_remaining);
    }
    else {
        AppWifiStart();
        AppMqttSendData();
        AppWifiDisconnect();
    }
    AppSleepInit();
    AppSleepGoToDeepSleep();
}
//#include "AppGraphics.h"
//#include <rom/ets_sys.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>
//#include <u8g2.h>
//
//#define I2C_PORT I2C_NUM_0
//#define SDA_PIN 21
//#define SCL_PIN 22
//
//u8g2_t u8g2;
//void app_main() {
//    i2c_config_t i2c_config = {
//        .mode = I2C_MODE_MASTER,
//        .sda_io_num = PIN_SDA,
//        .scl_io_num = PIN_SCL,
//        .sda_pullup_en = GPIO_PULLUP_ENABLE,
//        .scl_pullup_en = GPIO_PULLUP_ENABLE,
//        .master.clk_speed = 100000
//    };
//    i2c_param_config(I2C_PORT, &i2c_config);
//    i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
//
//    // Initialize the u8g2 library
//    u8g2_Setup_ssd1305_i2c_128x32_noname_f(&u8g2, U8G2_R1, u8x8_byte_sw_i2c, u8g2_esp32_gpio_and_delay_cb);
//    u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);  // Set the display I2C address
//
//     // Set the display power control
//     u8g2_SetPowerSave(&u8g2, 0);
//
//     // Initialize the display
//     u8g2_InitDisplay(&u8g2);
//     u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);  // Set the font
//     u8g2_ClearBuffer(&u8g2);  // Clear the display buffer
//
//     // Display text
//     u8g2_DrawStr(&u8g2, 0, 20, "Hello, u8g2!");
//
//     // Send the buffer to the display
//     u8g2_SendBuffer(&u8g2);
//     while (1) {
//        vTaskDelay(1000 / portTICK_PERIOD_MS);
//    }
//}
