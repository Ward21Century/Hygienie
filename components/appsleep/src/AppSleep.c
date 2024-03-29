#include "AppSleep.h"
#include <stdio.h>
#include <stdlib.h>


static struct timeval now;
static const char *TAG = "Sleep";
static void calibrate_touch_pad(touch_pad_t pad)
{
    uint32_t avg = 0;
    const size_t calibration_count = 128;
    for (uint32_t i = 0; i < calibration_count; ++i) {
        uint16_t val;
        touch_pad_read(pad, &val);
        avg += val;
    }
    avg /= calibration_count;
    const uint32_t min_reading = 1500;
    if (avg < min_reading) {
        ESP_LOGI(TAG, "Touch pad #%d average reading is too low: %d (expecting at least %d). "
               "using %d for deep sleep wakeup.\n", pad, avg, min_reading, min_reading);
        touch_pad_config(pad, min_reading);
    } else {
        ESP_LOGI(TAG, "Touch pad #%d average: %d, wakeup threshold set to %d.\n", pad, avg, avg);
        touch_pad_config(pad, avg);
    }
}

void AppSleepInit() {
    AppSleepDeepSleepTimerInit();
    AppSleepTouchWakeUpInit();
}

void AppSleepGoToDeepSleep() {
    AppSleepRecordEnterTime();
    esp_deep_sleep_start();
}

void AppSleepRecordEnterTime() {
    gettimeofday(&sleep_enter_time, NULL); // Record current time before sleep
}


esp_sleep_wakeup_cause_t AppSleepWakeUpFromDeepSleep() {
   return AppSleepGetWakeUpCause();
}

void AppSleepLog() {
    gettimeofday(&now, NULL);
    uint32_t sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;
    ESP_LOGD(TAG, "Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
}

esp_sleep_wakeup_cause_t AppSleepGetWakeUpCause() {
     return esp_sleep_get_wakeup_cause();
}

void AppSleepDeepSleepTimerInit() {
    uint32_t wakeup_time_sec;
    wakeup_time_sec = DEEP_SLEEP_TIMER*60;
        // If the time is within allowed hours, handle accordingly
        // For example, you might want to delay sleep or perform some data transmission here
        // Proceed to deep sleep
    #ifdef DEEP_SLEEP_TIMER
        wakeup_time_sec = DEEP_SLEEP_TIMER*60;
    #else
        wakeup_time_sec = 600;
    #endif

    ESP_LOGI(TAG, "Enabling timer wakeup, %ds\n", wakeup_time_sec);
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);
}

void AppSleepTouchWakeUpInit() {
    ESP_ERROR_CHECK(touch_pad_init());
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_set_voltage(TOUCH_HVOLT_2V4, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

    touch_pad_config(TOUCH_PAD_NUM7, TOUCH_THRESH_NO_USE);
    calibrate_touch_pad(TOUCH_PAD_NUM7);
    esp_sleep_enable_touchpad_wakeup();
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
}

