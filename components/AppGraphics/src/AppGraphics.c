#include "AppGraphics.h"


static  u8g2_t u8g2;


void AppGraphicsPrintDroplet(void) {
   for (int i=0; i<100; i=i+5){
       AppGraphicsPrintOnOled(0x0098, i+29);

}

void AppGraphicsPrintOnOled(int text, int height) {
    u8g2_ClearBuffer(&u8g2);
    u8g2_setDisplayRotation(&u8g2, U8G2_R1);
    if (text == -1)
        AppGraphicsHandleText(text);

    else if (height == -1)
        AppGraphicsHandleGraphics(int height)

    else {
        u8g2_SetFont(&u8g2, u8g2_font_open_iconic_all_2x_t);
        u8g2_DrawGlyph(&u8g2, 8, height,text);
   }
   u8g2_SendBuffer(&u8g2);
   delay(100);
   u8g2_ClearBuffer(&u8g2);
 }

void AppGraphicsHandleText(int text) {
    u8g2_SetFont(&u8g2, u8g2_font_streamline_pet_animals_t);
    int mod = random(10);
    text = 0x0030 + mod;
    u8g2_DrawGlyph(&u8g2, 8, 60, text);
}

void AppGraphicsHandleGraphics(int height) {
    char string[20];
    u8g2_SetFont(&u8g2, u8g2_font_logisoso16_tr);
    u8g2_DrawStr(&u8g2, 1,60,string);
}
