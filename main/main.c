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
 * interesting things happen there. See ../test/main/example_idf_test_runner_test.c
 * and the makefiles in ../test/ directory.
 *
 * This specific app_main function is provided only to illustrate the layout
 * of a project.
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

