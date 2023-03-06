#include <u8g2.h>
#include "u8g2_esp32_hal.h"
#include <driver/gpio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// SDA - GPIO21
#define PIN_SDA 21

// SCL - GPIO22
#define PIN_SCL 22

#define GRAPHICS_DELAY_MS CONFIG_GRAPHICS_DELAY_MS

#define network_icon 0x0119
#define upload_icon 0x008f
#define checkmark_icon 0x0073
#define star_icon 0x0102

void AppGraphicsInitDisplay(void);

void AppGraphicsAnimationCycle(void);

void AppGraphicsPrintDroplet(void);

void AppGraphicsPrintOnOled(int text, int height);

void AppGraphicsHandleText(int text);

void AppGraphicsHandleGraphics(int text);

void AppGraphicsWakeUpDisplay(void);

void AppGraphicsCloseDisplay(void);

void AppGraphicsClearBuffer(void);
