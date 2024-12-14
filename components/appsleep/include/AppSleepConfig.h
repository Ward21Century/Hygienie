#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "sdkconfig.h"
#include "soc/soc_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"
#include <inttypes.h>


#include "soc/sens_periph.h"

#ifdef CONFIG_DEEP_SLEEP_TIMER
#define DEEP_SLEEP_TIMER CONFIG_DEEP_SLEEP_TIMER
#endif

#ifdef CONFIG_IDF_TARGET_ESP32C3
#define DEFAULT_WAKEUP_PIN      CONFIG_EXAMPLE_GPIO_WAKEUP_PIN
#ifdef CONFIG_EXAMPLE_GPIO_WAKEUP_HIGH_LEVEL
#define DEFAULT_WAKEUP_LEVEL    ESP_GPIO_WAKEUP_GPIO_HIGH
#else
#define DEFAULT_WAKEUP_LEVEL    ESP_GPIO_WAKEUP_GPIO_LOW
#endif
#endif

#ifdef CONFIG_EXAMPLE_ULP_TEMPERATURE_WAKEUP
#if CONFIG_IDF_TARGET_ESP32

// Adding RTC_DATA_ATTR for sleep enter time for a later time
/*
 * Offset (in 32-bit words) in RTC Slow memory where the data is placed
 * by the ULP coprocessor. It can be chosen to be any value greater or equal
 * to ULP program size, and less than the CONFIG_ESP32_ULP_COPROC_RESERVE_MEM/4 - 6,
 * where 6 is the number of words used by the ULP coprocessor.
 */
#define ULP_DATA_OFFSET    0

// TODO: Add ULP temperature monitoring program to Appsleep module.
/**
 * @brief Start ULP temperature monitoring program
 *
 * This function loads a program into the RTC Slow memory and starts up the ULP.
 * The program monitors on-chip temperature sensor and wakes up the SoC when
 * the temperature goes lower or higher than certain thresholds.
 */
// TODO: Add ULP temperature monitoring program to Appsleep module. Will comment out until feature is needed.
//static void start_ulp_temperature_monitoring(void);

/**
 * @brief Utility function which reads data written by ULP program
 *
 * @param offset offset from ULP_DATA_OFFSET in RTC Slow memory, in words
 * @return lower 16-bit part of the word writable by the ULP
 */
static inline uint16_t ulp_data_read(size_t offset)
{
    return RTC_SLOW_ATTR[ULP_DATA_OFFSET + offset] & 0xffff;
}

/**
 * @brief Utility function which writes data to be ready by ULP program
 *
 * @param offset offset from ULP_DATA_OFFSET in RTC Slow memory, in words
 * @param value lower 16-bit part of the word to be stored
 */
static inline void ulp_data_write(size_t offset, uint16_t value)
{
    RTC_SLOW_ATTR[ULP_DATA_OFFSET + offset] = value;
}
#endif // CONFIG_IDF_TARGET_ESP32
#endif // CONFIG_EXAMPLE_ULP_TEMPERATURE_WAKEUP

#ifdef CONFIG_EXAMPLE_TOUCH_WAKEUP
#ifdef CONFIG_IDF_TARGET_ESP32
#if CONFIG_TOUCH_THRESHOLD
#endif
#endif
#endif

