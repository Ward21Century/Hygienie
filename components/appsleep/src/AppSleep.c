#include "driver/rtc_io.h"
#include "AppSleep.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define GPIO_WAKEUP_PIN 27

static struct timeval now;

void AppSleepInit() {
    AppSleepDeepSleepTimerInit();
    AppSleepWakeUpInit();
}

void AppSleepGoToDeepSleep() {
    esp_deep_sleep_start();
}

esp_sleep_wakeup_cause_t AppSleepWakeUpFromDeepSleep() {
    return AppSleepGetWakeUpCause();
}

void AppSleepLog() {
    gettimeofday(&now, NULL);
}

esp_sleep_wakeup_cause_t AppSleepGetWakeUpCause() {
    return esp_sleep_get_wakeup_cause();
}

void AppSleepDeepSleepTimerInit() {
    uint32_t wakeup_time_sec;
    #ifdef DEEP_SLEEP_TIMER
        wakeup_time_sec = DEEP_SLEEP_TIMER * 60;
    #else
     /* Default to 10 minutes if DEEP_SLEEP_TIMER is not defined */
        wakeup_time_sec = 600; // Default to 10 minutes if DEEP_SLEEP_TIMER is not defined
    #endif
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);
}

void AppSleepConfigureGpioForSleep(void) {


    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << 27);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

}

void AppSleepWakeUpInit() {
    /* Configure GPIO 27 as an external wakeup pin */
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_ON);

   /* Configure power domain to keep RTC peripherals on during deep sleep */
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
}
