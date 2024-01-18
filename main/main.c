#include <stdio.h>
#include <stdlib.h>
#include "esp_system.h"
#include "testable.h"
#include "esp_wifi.h"
#include "driver/adc.h"
#include "AppGraphics.h"
#include "appmqtt.h"
#include "AppSleep.h"
#include "appwifi.h"

static const char *TAG = "App Main";

void app_main() {

    AppSleepWakeUpFromDeepSleep();
    AppGraphicsAnimationCycle();
    AppMqttAddTime();
    if (AppMqttGetNumoffLineReadingCount() < MAX_OFFLINE_READINGS-1) {
        uint32_t num_offline_remaining = MAX_OFFLINE_READINGS - AppMqttGetNumoffLineReadingCount();
        ESP_LOGI(TAG, "Number of readings until next transmission:%d \r\n", num_offline_remaining);
    }
    else {
        AppWifiStart();
        AppMqttSendData();
        AppWifiDisconnect();
    }
    AppSleepInit();
    AppSleepGoToDeepSleep();
}
