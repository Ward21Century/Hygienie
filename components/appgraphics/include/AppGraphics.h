#ifndef APPGRAPHICS_H
#define APPGRAPHICS_H
#include <u8g2.h>
#include <stdlib.h>
#include "u8g2_esp32_hal.h"
#include "esp_log.h"
#include <driver/gpio.h>
#include <inttypes.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// SDA - GPIO21
#define PIN_SDA 21

// SCL - GPIO22
#define PIN_SCL 22

// RST - RST_PIN
#define RST_PIN 25

#define LCD_ENABLE_PIN 13

#define GRAPHICS_DELAY_MS CONFIG_GRAPHICS_DELAY_MS

// Different graphic images
#define network_icon 0x0119
#define upload_icon 0x008f
#define checkmark_icon 0x0073
#define star_icon 0x0102
#define display_address 0x0078
#define tear_drop 0x0098

// Box of display boundaries
#define x_upper_left 20
#define y_upper_left 80
#define width_of_box 20
#define height_of_box 30


// Glyph coordinates
#define glpyh_x_coordinate 10
#define glyph_y_coordinate 60


// Str coordinates
#define string_x_coordinate 30
#define string_y_coordinate 60
// Address within u8g2 graphics library
// Address within u8g2 graphics library
#define start_address_of_animals 0x0030

void AppGraphicsInitDisplay(void);

void AppGraphicsAnimationCycle(void);

void AppGraphicsPrintDroplet(void);

void AppGraphicsPrintOnOled(int32_t text, int32_t height);

void AppGraphicsHandleText(uint32_t text);

void AppGraphicsHandleGraphics(uint32_t text);

void AppGraphicsWakeUpDisplay(void);

void AppGraphicsCloseDisplay(void);

void AppGraphicsClearBuffer(void);
#endif
