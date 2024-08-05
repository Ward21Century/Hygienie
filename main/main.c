#include <stdio.h>
#include <stdlib.h>
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

    esp_sleep_wakeup_cause_t wakeupCause = AppSleepGetWakeUpCause();
    if (!initialized) {
        // Perform initialization tasks here (Wi-Fi, peripherals, etc.)
        initialized = 1; // Mark as initialized
        // It's a good idea to synchronize time after initialization

        AppGraphicsAnimationCycle();
        AppWifiStart();
        AppMqttInitNTPAndSyncTime();
        AppWifiDisconnect();
    }
    switch (wakeupCause) {

        case ESP_SLEEP_WAKEUP_TIMER:
            AppSleepLog();
            if (AppMqttGetNumoffLineReadingCount() > 0 ) {
                AppWifiStart();
                AppMqttSendData();
                AppMqttInitNTPAndSyncTime();
                AppWifiDisconnect();
            }
            break;

        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            ESP_LOGI(TAG, "Wake up from touch on pad %d\n", esp_sleep_get_touchpad_wakeup_status());
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
            ESP_LOGI(TAG, "Not a deep sleep or a touch wake_up\n");
            break;
        default:
            ESP_LOGI(TAG, "ERROR with wake up cause\n");
    }
    AppSleepInit();
    AppSleepGoToDeepSleep();
}
//
//
//
//#define LCD_ENABLE_PIN 13  // Replace with your desired GPIO pin number
//#define SLEEP_DURATION 60 // Duration in seconds
//                         //
//
//void configure_gpio_for_sleep()
//{
//    rtc_gpio_init(LCD_ENABLE_PIN);
//    rtc_gpio_set_direction(LCD_ENABLE_PIN, RTC_GPIO_MODE_OUTPUT_ONLY);
//    rtc_gpio_set_level(LCD_ENABLE_PIN, 0);
//
//    // Configure reset pin as RTC GPIO and set it high
//    rtc_gpio_init(RST_PIN);
//    rtc_gpio_set_direction(RST_PIN, RTC_GPIO_MODE_OUTPUT_ONLY);
//    rtc_gpio_set_level(RST_PIN, 0);
//
//    // Configure GPIO as RTC GPIO and set it high
//    rtc_gpio_init(PIN_SDA);
//    rtc_gpio_set_direction(PIN_SDA, RTC_GPIO_MODE_OUTPUT_ONLY);
//    rtc_gpio_set_level(PIN_SDA, 1);
//
//    rtc_gpio_init(PIN_SCL);
//    rtc_gpio_set_direction(PIN_SCL, RTC_GPIO_MODE_OUTPUT_ONLY);
//    rtc_gpio_set_level(PIN_SCL, 1);
//}
//
//void configure_gpio()
//{
//    // Configuration structure
//    gpio_config_t io_conf;
//
//    // Disable interrupt
//    io_conf.intr_type = GPIO_INTR_DISABLE;
//
//    // Set as output mode
//    io_conf.mode = GPIO_MODE_OUTPUT;
//
//    io_conf.pin_bit_mask = (1ULL << LCD_ENABLE_PIN);
//
//    // Disable pull-down mode
//    io_conf.pull_down_en = 0;
//
//    // Disable pull-up mode
//    io_conf.pull_up_en = 0;
//
//    // Configure GPIO with the given settings
//    gpio_config(&io_conf);
//
//    gpio_set_level(LCD_ENABLE_PIN, 1);  // Set the pin to high
//}
//void app_main(void)
//{
//   // Configure GPIO
//
//    configure_gpio();
//  // Set the GPIO level (0 = low, 1 = high)
//
//    AppGraphicsAnimationCycle();
//
//    // Print a message to the console
//    printf("Entering deep sleep mode for %d seconds...\n", SLEEP_DURATION);
//
//    // Configure the wakeup source (timer in this case)
//
//    esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000);
//    // Optionally disable pull-ups/downs on all GPIOs except I2C and wakeup GPIO
//    configure_gpio_for_sleep();
//
//
//    esp_deep_sleep_start();
//}
//
//
//
//
//void configure_gpio_for_sleep() {
//    // Initialize and configure the LCD_ENABLE_PIN as RTC GPIO
//    rtc_gpio_init(LCD_ENABLE_PIN);
//    rtc_gpio_set_direction(LCD_ENABLE_PIN, RTC_GPIO_MODE_OUTPUT_ONLY);
//    rtc_gpio_set_level(LCD_ENABLE_PIN, 0);
//    rtc_gpio_hold_en(LCD_ENABLE_PIN);
//
//    // Initialize and configure the RST_PIN as RTC GPIO and set it high
//
//    gpio_set_level(RST_PIN, 1);
//    gpio_hold_en(RST_PIN);
//
//    // Initialize and configure PIN_SDA as RTC GPIO and set it high
//    gpio_set_level(PIN_SDA, 1);
//    gpio_hold_en(PIN_SDA);
//
//    // Initialize and configure PIN_SCL as RTC GPIO and set it high
//    gpio_set_level(PIN_SCL, 1);
//    gpio_hold_en(PIN_SCL);
//}
//
//void configure_gpio() {
//    // Configure GPIOs as regular output mode
//    gpio_config_t io_conf;
//    io_conf.intr_type = GPIO_INTR_DISABLE;
//    io_conf.mode = GPIO_MODE_OUTPUT;
//    io_conf.pin_bit_mask = (1ULL << LCD_ENABLE_PIN) | (1ULL << RST_PIN) | (1ULL << PIN_SDA) | (1ULL << PIN_SCL);
//    io_conf.pull_down_en = 0;
//    io_conf.pull_up_en = 0;
//    gpio_config(&io_conf);
//
//    // Set initial levels for the GPIOs
//    gpio_set_level(LCD_ENABLE_PIN, 1); // Set the pin to high
//}
//
//void app_main(void) {
//    // Check if the device is waking up from deep sleep
//    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
//        // Disable hold on GPIOs after waking up
//        gpio_hold_dis(RST_PIN);
//        gpio_hold_dis(PIN_SDA);
//        gpio_hold_dis(PIN_SCL);
//
//        // Reconfigure GPIOs for normal operation
//        configure_gpio();
//    } else {
//        // Initial configuration of GPIOs
//        configure_gpio();
//    }
//
//    // Simulate some work (for example, your display code)
//    // Replace `AppGraphicsAnimationCycle` with your actual function
//    AppGraphicsAnimationCycle();
//
//    // Print a message to the console
//    printf("Entering deep sleep mode for %d seconds...\n", SLEEP_DURATION);
//
//    // Configure the wakeup source (timer in this case)
//    esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000);
//
//    // Configure GPIOs for sleep mode
//    configure_gpio_for_sleep();
//
//    // Enter deep sleep
//    esp_deep_sleep_start();
//}
//#include <stdio.h>
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "esp_sleep.h"
//#include "esp_log.h"
//#include "driver/rtc_io.h"
//#include "AppGraphics.h"
//
//#define SLEEP_DURATION 20 // Duration in seconds
                          //
                          //
                          //
//void configure_gpio()
//{
//
//    gpio_config_t io_conf;
//
//    gpio_hold_dis(RST_PIN);
//    gpio_hold_dis(PIN_SCL);
//
//    // Disable interrupt
//    io_conf.intr_type = GPIO_INTR_DISABLE;
//
//    // Set as output mode
//    io_conf.mode = GPIO_MODE_OUTPUT;
//
//    io_conf.pin_bit_mask = (1ULL << 13);
//
//    // Disable pull-down mode
//    io_conf.pull_down_en = 0;
//
//    // Disable pull-up mode
//    io_conf.pull_up_en = 0;
//
//    // Configure GPIO with the given settings
//    gpio_config(&io_conf);
//
//}
//                          //
//void configure_gpio_for_sleep()
//{
//    gpio_set_level(RST_PIN, 1);
//    gpio_hold_en(RST_PIN);
//
//    gpio_set_level(PIN_SDA, 1);
//    gpio_hold_en(PIN_SDA);
//    gpio_set_level(PIN_SCL, 1);
//    gpio_hold_en(PIN_SCL);
//}
//
//void app_main(void)
//{
//
//    configure_gpio();
//
//    // Set the GPIO level (0 = low, 1 = high)
//    gpio_set_level(13, 1);  // Set the pin to high
//    AppGraphicsAnimationCycle();
//
//    gpio_set_level(13, 0);
//
//    // Print a message to the console
//    printf("Entering deep sleep mode for %d seconds...\n", SLEEP_DURATION);
//
//    // Configure the wakeup source (timer in this case)
//
//    configure_gpio_for_sleep();
//
//    //gpio_set_level(PIN_SDA, 1);
//    //gpio_hold_en(PIN_SDA);
//
//    //gpio_set_level(PIN_SCL, 1);
//    //gpio_hold_en(PIN_SCL);
//    esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000);
//    // Optionally disable pull-ups/downs on all GPIOs except I2C and wakeup GPIO
//    // Go to deep sleep
//    //
//    esp_deep_sleep_start();
//}
