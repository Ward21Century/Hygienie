#include "AppGraphics.h"
#include <rom/ets_sys.h>
#include <stdlib.h>

#define RST_PIN 25
#define I2C_PORT I2C_NUM_0
static const char *TAG = "AppGraphics Application";
static u8g2_t u8g2;

void AppGraphicsInitDisplay() {

    // Initialize the I2C bus
    //i2c_config_t i2c_config = {
    //    .mode = I2C_MODE_MASTER,
    //    .sda_io_num = PIN_SDA,
    //    .scl_io_num = PIN_SCL,
    //    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    //    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    //    .master.clk_speed = 100000
    //};
    //i2c_param_config(I2C_PORT, &i2c_config);
    //i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);

    //// Initialize the u8g2 library
    //u8g2_Setup_ssd1305_i2c_128x32_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8g2_esp32_gpio_and_delay_cb);
    //u8x8_SetI2CAddress(&u8g2.u8x8, 0x3C);  // Set the display I2C address

    // // Set the display power control
    // u8g2_SetPowerSave(&u8g2, 0);

    // // Initialize the display
    // u8g2_InitDisplay(&u8g2);
    // u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);  // Set the font
    // u8g2_ClearBuffer(&u8g2);  // Clear the display buffer

     // Display text
    // u8g2_DrawStr(&u8g2, 0, 60, "Hello, u8g2!");

     // Send the buffer to the display
     //u8g2_SendBuffer(&u8g2);
     //while (1) {
     //   vTaskDelay(1000 / portTICK_PERIOD_MS);
    //}
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_esp32_hal.bus.i2c.sda = PIN_SDA;
    u8g2_esp32_hal.bus.i2c.scl = PIN_SCL;
    u8g2_esp32_hal.reset = RST_PIN;
    u8g2_esp32_hal_init(u8g2_esp32_hal);
    ESP_LOGD(TAG, "AppGraphics has been configured.");
    u8g2_Setup_ssd1305_i2c_128x32_noname_f(&u8g2, U8G2_R1, u8x8_byte_sw_i2c, u8g2_esp32_gpio_and_delay_cb);
    //u8g2_Setup_ssd1306_i2c_128x32_univision_f(
    //  &u8g2, U8G2_R1,
    //  u8x8_byte_sw_i2c,
//  //    u8g2_esp32_i2c_byte_cb,
    //  u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure
      ESP_LOGI(TAG, "Driver Connected");
    u8x8_SetI2CAddress(&u8g2.u8x8, 0x3C);
    u8g2_InitDisplay(&u8g2);  // send init sequence to the display, display is in
  return;
}

void AppGraphicsAnimationCycle() {
    AppGraphicsInitDisplay();
    AppGraphicsWakeUpDisplay();
    u8g2_DrawBox(&u8g2, 10, 20, 20, 30);
    AppGraphicsPrintDroplet();
    AppGraphicsClearBuffer();
    AppGraphicsPrintOnOled(-1, -1);
    AppGraphicsClearBuffer();
    vTaskDelay(GRAPHICS_DELAY_MS/portTICK_PERIOD_MS);
    AppGraphicsPrintOnOled(checkmark_icon, 70);
    AppGraphicsClearBuffer();
    AppGraphicsCloseDisplay();
    return;
}

void AppGraphicsPrintDroplet() {
   for (int i=0; i<100; i+=5){
       AppGraphicsPrintOnOled(0x0098, i+29);
    }
   return;
}

void AppGraphicsPrintOnOled(int text, int height) {
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetDisplayRotation(&u8g2, U8G2_R1);
    if (text == -1)
        AppGraphicsHandleText(text);

    else if (height == -1)
        AppGraphicsHandleGraphics(height);

    else {
        u8g2_SetFont(&u8g2, u8g2_font_open_iconic_all_2x_t);
        u8g2_DrawGlyph(&u8g2, 20, height, text);
   }
   u8g2_SendBuffer(&u8g2);
   u8g2_ClearBuffer(&u8g2);
   return;
 }

void AppGraphicsHandleText(int text) {
    u8g2_SetFont(&u8g2, u8g2_font_streamline_pet_animals_t);
    int mod = (rand() % 10) + 1;
    text = 0x0030 + mod;
    u8g2_DrawGlyph(&u8g2, 20, 60, text);
    return;
}

void AppGraphicsHandleGraphics(int height) {
    char string[20];
    u8g2_SetFont(&u8g2, u8g2_font_logisoso16_tr);
    u8g2_DrawStr(&u8g2, 1, 60, string);
    return;
}

void AppGraphicsWakeUpDisplay() {
    u8g2_SetPowerSave(&u8g2, 0);  // wake up display
    return;
}

void AppGraphicsCloseDisplay() {
    u8g2_SetPowerSave(&u8g2, 1); // put display to sleep
    return;
}

void AppGraphicsClearBuffer() {
    u8g2_ClearBuffer(&u8g2);
    return;
}

