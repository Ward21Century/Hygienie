#include <u8g2.h>
#include "u8g2_esp32_hal.h"
#include <driver/gpio.h>

// SDA - GPIO21
#define PIN_SDA 21

// SCL - GPIO22
#define PIN_SCL 22

void AppGraphicsInitDisplay(void);

void AppGraphicsPrintDroplet(void);

void AppGraphicsPrintOnOled(int text, int height);

void AppGraphicsHandleText(int text);

void AppGraphicsHandleGraphics(int text);

void AppGraphicsWakeUpDisplay(void);

void AppGraphicsCloseDisplay(void);

void AppGraphicsClearBuffer(void);
