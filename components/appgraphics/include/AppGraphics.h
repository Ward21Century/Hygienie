#include <u8g2.h>
#include <stdlib.h>
#include "u8g2_esp32_hal.h"
#include "esp_log.h"
#include <driver/gpio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// SDA - GPIO21
#define PIN_SDA 21

// SCL - GPIO22
#define PIN_SCL 22

// RST - RST_PIN
#define RST_PIN 25
#define GRAPHICS_DELAY_MS CONFIG_GRAPHICS_DELAY_MS

#define network_icon 0x0119
#define upload_icon 0x008f
#define checkmark_icon 0x0073
#define star_icon 0x0102

void AppGraphicsInitDisplay(void);

void AppGraphicsAnimationCycle(void);

void AppGraphicsPrintDroplet(void);

void AppGraphicsPrintOnOled(uint32_t text, uint32_t height);

void AppGraphicsHandleText(uint32_t text);

void AppGraphicsHandleGraphics(uint32_t text);

void AppGraphicsWakeUpDisplay(void);

void AppGraphicsCloseDisplay(void);

void AppGraphicsClearBuffer(void);
