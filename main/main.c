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
