
#include "AppSleep.h"
#include <stdio.h>
#include <stdlib.h>


static struct timeval now;
//static const char *TAG = "Sleep";

void AppSleepInit() {
    AppSleepDeepSleepTimerInit();
    AppSleepTouchWakeUpInit();
}

void AppSleepGoToDeepSleep() {
    esp_deep_sleep_start();
}

esp_sleep_wakeup_cause_t AppSleepWakeUpFromDeepSleep() {
    return AppSleepGetWakeUpCause();
}

void AppSleepLog() {
    gettimeofday(&now, NULL);
    // uint32_t sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;
    // ESP_LOGD(TAG, "Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
}

esp_sleep_wakeup_cause_t AppSleepGetWakeUpCause() {
    return esp_sleep_get_wakeup_cause();
}

void AppSleepDeepSleepTimerInit() {
    uint32_t wakeup_time_sec;
    #ifdef DEEP_SLEEP_TIMER
        wakeup_time_sec = DEEP_SLEEP_TIMER * 60;
    #else
        wakeup_time_sec = 600;
    #endif

    // ESP_LOGI(TAG, "Enabling timer wakeup, %ds\n", wakeup_time_sec);
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);
}

void AppSleepConfigureGpioForSleep(void) {
 gpio_config_t io_conf;   // Configure GPIO 13 as an output
    io_conf.intr_type = GPIO_INTR_DISABLE;      // Disable interrupt (handled by sleep wake-up)
    io_conf.mode = GPIO_MODE_INPUT;             // Set as input mode
    io_conf.pin_bit_mask = (1ULL << 27);        // Pin 27 mask
    io_conf.pull_down_en = 0;                   // Disable pull-down mode
    io_conf.pull_up_en = 1;                     // Enable pull-up mode
    gpio_config(&io_conf);
}

void AppSleepTouchWakeUpInit() {
    // Configure GPIO 27 as an external wakeup pin

    AppSleepConfigureGpioForSleep();
    // Enable ext0 wakeup on GPIO 27, triggering on a low level (0)
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);

    // Configure power domain to keep RTC peripherals on during deep sleep
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
}

