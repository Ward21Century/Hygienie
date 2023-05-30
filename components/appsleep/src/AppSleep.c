#include "AppSleep.h"
#include <stdio.h>
#include <stdlib.h>


static struct timeval now;

static void calibrate_touch_pad(touch_pad_t pad)
{
    int avg = 0;
    const size_t calibration_count = 128;
    for (int i = 0; i < calibration_count; ++i) {
        uint16_t val;
        touch_pad_read(pad, &val);
        avg += val;
    }
    avg /= calibration_count;
    const int min_reading = 300;
    if (avg < min_reading) {
        printf("Touch pad #%d average reading is too low: %d (expecting at least %d). "
               "Not using for deep sleep wakeup.\n", pad, avg, min_reading);
        touch_pad_config(pad, 1000);
    } else {
//        int threshold = 1000;
        printf("Touch pad #%d average: %d, wakeup threshold set to %d.\n", pad, avg, avg);
        touch_pad_config(pad, 1600);
    }
}

void AppSleepInit()
{
    AppSleepDeepSleepTimerInit();
    AppSleepTouchWakeUpInit();
}
void AppSleepGoToDeepSleep() {
    esp_deep_sleep_start();
}

void AppSleepWakeUpFromDeepSleep() {
   AppSleepGetWakeUpCause();
}

void AppSleepGetWakeUpCause() {
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;

    switch (esp_sleep_get_wakeup_cause()) {

        case ESP_SLEEP_WAKEUP_TIMER: {
            printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
            break;
        }

        case ESP_SLEEP_WAKEUP_TOUCHPAD: {
            printf("Wake up from touch on pad %d\n", esp_sleep_get_touchpad_wakeup_status());
            break;
        }

        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            printf("Not a deep sleep reset\n");
    }
}

void AppSleepDeepSleepTimerInit() {

    int wakeup_time_sec;
#ifdef DEEP_SLEEP_TIMER
    wakeup_time_sec = DEEP_SLEEP_TIMER*60;
#else
    wakeup_time_sec = 600;
#endif

    printf("Enabling timer wakeup, %ds\n", wakeup_time_sec);
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);
}

void AppSleepTouchWakeUpInit() {
    ESP_ERROR_CHECK(touch_pad_init());
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_set_voltage(TOUCH_HVOLT_2V4, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

//    touch_pad_sleep_channel_set_work_time(1000, TOUCH_PAD_MEASURE_CYCLE_DEFAULT);
    touch_pad_config(TOUCH_PAD_NUM7, TOUCH_THRESH_NO_USE);
    calibrate_touch_pad(TOUCH_PAD_NUM7);
    esp_sleep_enable_touchpad_wakeup();
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
}

