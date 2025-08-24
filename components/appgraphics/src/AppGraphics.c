#include "AppGraphics.h"
#include <inttypes.h>
#include <rom/ets_sys.h>

static const char *TAG = "AppGraphics";
static u8g2_t u8g2;
/* I2C address (0x3C << 1) */
#define OLED_ADDR 0x78

static void configure_gpio() {
    gpio_config_t io_conf;


    gpio_hold_dis(RST_PIN);
    gpio_hold_dis(PIN_SCL);
    gpio_hold_dis(PIN_SDA);
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << LCD_ENABLE_PIN) | (1ULL << RST_PIN) | (1ULL << PIN_SCL) | (1ULL << PIN_SDA);
    io_conf.pull_down_en = 1;
    io_conf.pull_up_en = 0;

    gpio_set_level(LCD_ENABLE_PIN, 1);
    gpio_config(&io_conf);
}

static void configure_gpio_for_sleep()
{
    gpio_set_level(PIN_SDA, 1);
    gpio_hold_en(PIN_SDA);
    gpio_set_level(PIN_SCL, 1);
    gpio_hold_en(PIN_SCL);
    gpio_set_level(RST_PIN, 1);
    gpio_hold_en(RST_PIN);

}
void AppGraphicsInitDisplay() {

    /* Initialize the I2C bus */

    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_esp32_hal.sda = PIN_SDA;
    u8g2_esp32_hal.scl = PIN_SCL;
    u8g2_esp32_hal.reset = RST_PIN;
    u8g2_esp32_hal_init(u8g2_esp32_hal);
    ESP_LOGD(TAG, "AppGraphics has been configured.");
    u8g2_Setup_ssd1305_i2c_128x32_noname_f(&u8g2, U8G2_R0, u8g2_esp32_i2c_byte_cb, u8g2_esp32_gpio_and_delay_cb);
      ESP_LOGI(TAG, "Driver Connected");
    u8x8_SetI2CAddress(&u8g2.u8x8, OLED_ADDR);
    /* Send init sequence to the display, display is in */
    u8g2_InitDisplay(&u8g2);  // send init sequence to the display, display is in
  return;
}

void AppGraphicsAnimationCycle() {
    configure_gpio();
    AppGraphicsInitDisplay();
    AppGraphicsWakeUpDisplay();
    u8g2_DrawBox(&u8g2, x_upper_left, y_upper_left, width_of_box, height_of_box);
    AppGraphicsPrintDroplet();
    AppGraphicsClearBuffer();
    AppGraphicsPrintOnOled(-1, -1);
    AppGraphicsClearBuffer();
    vTaskDelay(GRAPHICS_DELAY_MS/portTICK_PERIOD_MS);
    AppGraphicsPrintOnOled(checkmark_icon, 70);
    vTaskDelay(GRAPHICS_DELAY_MS/portTICK_PERIOD_MS);
    AppGraphicsClearBuffer();
    AppGraphicsCloseDisplay();
    configure_gpio_for_sleep();
    return;
}

void AppGraphicsPrintDroplet() {
   for (uint32_t i=0; i<100; i+=5){
       AppGraphicsPrintOnOled(tear_drop, i+29);
    }
   return;
}

void AppGraphicsPrintOnOled(int32_t text, int32_t height) {
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetDisplayRotation(&u8g2, U8G2_R1);
    if (text == -1)
        AppGraphicsHandleText(text);

    else if (height == -1)
        AppGraphicsHandleGraphics(height);

    else {
        u8g2_SetFont(&u8g2, u8g2_font_open_iconic_all_2x_t);
        u8g2_DrawGlyph(&u8g2, glpyh_x_coordinate, height, text);
   }
   u8g2_SendBuffer(&u8g2);
   u8g2_ClearBuffer(&u8g2);
   return;
 }

void AppGraphicsHandleText(uint32_t text) {
    u8g2_SetFont(&u8g2, u8g2_font_streamline_pet_animals_t);

    /* This displays a random animal by selecting a random number from 1 to 10 */
    srand(time(NULL));
    uint32_t mod = (rand() % 10) + 1;
    text = start_address_of_animals + mod;
    u8g2_DrawGlyph(&u8g2, glpyh_x_coordinate, glyph_y_coordinate, text);
    return;
}

void AppGraphicsHandleGraphics(uint32_t text) {
    char string[20];
    sprintf(string, "%ld", text);
    u8g2_SetFont(&u8g2, u8g2_font_logisoso16_tr);
    u8g2_DrawStr(&u8g2, string_x_coordinate, string_y_coordinate, string);
    return;
}

void AppGraphicsWakeUpDisplay() {
    u8g2_SetPowerSave(&u8g2, 0);
    return;
}

void AppGraphicsCloseDisplay() {
    u8g2_SetPowerSave(&u8g2, 1);
    return;
}

void AppGraphicsClearBuffer() {
    u8g2_ClearBuffer(&u8g2);
    return;
}

