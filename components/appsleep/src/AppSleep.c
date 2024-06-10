#include "AppSleep.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define GPIO_WAKEUP_PIN 27

static struct timeval now;
static const char *TAG = "Sleep";

// Forward declaration of the interrupt handler
static void IRAM_ATTR gpio_isr_handler(void* arg);

void AppSleepInit() {
    AppSleepDeepSleepTimerInit();
    AppSleepGPIOWakeUpInit(); // Modified to use GPIO instead of touchpad
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
    #ifdef DEEP_SLEEP_TIMER
        wakeup_time_sec = DEEP_SLEEP_TIMER * 60;
    #else
        wakeup_time_sec = 600; // Default to 10 minutes if DEEP_SLEEP_TIMER is not defined
    #endif

    ESP_LOGI(TAG, "Enabling timer wakeup, %ds\n", wakeup_time_sec);
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);
}

void AppSleepGPIOWakeUpInit() {
    gpio_config_t io_config;
    io_config.intr_type = GPIO_INTR_NEGEDGE;
    io_config.pin_bit_mask = (1ULL << GPIO_WAKEUP_PIN);
    io_config.mode = GPIO_MODE_INPUT;
    io_config.pull_up_en = 1;
    gpio_config(&io_config);

    // Install ISR service and attach interrupt handler
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_WAKEUP_PIN, gpio_isr_handler, (void*) GPIO_WAKEUP_PIN);

    // Enable wakeup from GPIO
    esp_sleep_enable_ext0_wakeup(GPIO_WAKEUP_PIN, 0); // 0 = LOW
}

// Interrupt Service Routine (ISR) for GPIO
static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    ESP_LOGI(TAG, "GPIO interrupt triggered on pin %d\r\n", gpio_num);
    // Handle GPIO interrupt (e.g., wake up from sleep)
}

// If you have app_main function, ensure it's properly defined and used
// void app_main() {
//     AppSleepInit();
//     // Other initializations and main loop code
// }
