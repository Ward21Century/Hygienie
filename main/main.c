#include <stdio.h>
#include <stdlib.h>
#include <esp_intr_types.h>
#include "esp_system.h"
#include "testable.h"
#include "esp_wifi.h"
#include "driver/adc.h"
#include "AppGraphics.h"
#include "Appmqtt.h"
#include "AppSleep.h"
#include "appwifi.h"

static const char *TAG = "App Main";
RTC_DATA_ATTR static uint8_t initialized = 0;
void app_main() {
    AppSleepConfigureGpioForSleep();
    esp_sleep_wakeup_cause_t wakeupCause = AppSleepGetWakeUpCause();
    ESP_LOGI(TAG, "Wake-up cause: %" PRIu32, (uint32_t)wakeupCause);
    if (!initialized) {
        /* Perform initialization tasks here (Wi-Fi, peripherals, etc.) */
        ESP_LOGI(TAG, "Initialization values: %" PRIu8, initialized);
        ESP_LOGI(TAG, "System initializing...");
        /* Perform initialization tasks here (Wi-Fi, peripherals, etc.) */
        initialized = 1; // Mark as initialized
       /* It's a good idea to synchronize time after initialization */
        AppGraphicsAnimationCycle();
        AppWifiStart();
        AppMqttInitNTPAndSyncTime();
        AppWifiDisconnect();
        ESP_LOGI(TAG, "System initialized.");
    }
    switch (wakeupCause) {

        case ESP_SLEEP_WAKEUP_TIMER:
            ESP_LOGI(TAG, "Wake up from sleep timer.\r\n");
            AppSleepLog();
            ESP_LOGI(TAG, "Woke up from timer.");
            if (AppMqttGetNumoffLineReadingCount() > 0 ) {
                AppWifiStart();
                AppMqttSendData();
                AppMqttInitNTPAndSyncTime();
                AppWifiDisconnect();
                ESP_LOGI(TAG, "Data sent after wake up.");
            }

            break;

         case ESP_SLEEP_WAKEUP_EXT0:
            ESP_LOGI(TAG, "Wake up from external GPIO: %" PRIu32, (uint32_t)esp_sleep_get_touchpad_wakeup_status());
            AppMqttAddTime();
            AppGraphicsAnimationCycle();
            if (AppMqttGetNumoffLineReadingCount() >= MAX_OFFLINE_READINGS) {
                AppWifiStart();
                AppMqttSendData();
                AppMqttInitNTPAndSyncTime();
                AppWifiDisconnect();
            }
            break;

        case ESP_SLEEP_WAKEUP_UNDEFINED:
            ESP_LOGE(TAG, "Error: Unhandled wake up cause: %" PRIu32, (uint32_t)wakeupCause);
            break;
        default:
            ESP_LOGI(TAG, "ERROR with wake up cause\n");
    }

    ESP_LOGI(TAG, "Going to Sleep\r\n");
    AppSleepInit();
    AppSleepGoToDeepSleep();
}

