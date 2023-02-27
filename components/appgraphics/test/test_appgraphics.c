/* test_mean.c: Implementation of a testable component.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "unity.h"
#include "AppGraphics.h"
#define network_icon 0x0119
#define upload_icon 0x008f
#define checkmark_icon 0x0073
#define star_icon 0x0102


TEST_CASE("Test Display Init", "[appgraphics]")
{
    AppGraphicsInitDisplay();
    AppGraphicsWakeUpDisplay();
    AppGraphicsClearBuffer();
    AppGraphicsCloseDisplay();
    TEST_ASSERT_EQUAL(0, 0);
}

TEST_CASE("Test Print Droplet", "[appgraphics]")
{
    AppGraphicsInitDisplay();
    AppGraphicsWakeUpDisplay();
    AppGraphicsPrintDroplet();
    TEST_ASSERT_EQUAL(0, 0);
}

TEST_CASE("Test Print Animal", "[appgraphics]")
{
    AppGraphicsInitDisplay();
    AppGraphicsWakeUpDisplay();
    AppGraphicsPrintOnOled(-1, -1);
    AppGraphicsClearBuffer();
    AppGraphicsCloseDisplay();
    TEST_ASSERT_EQUAL(0, 0);
}

TEST_CASE("Test Print Check Mark", "[appgraphics]")
{
    AppGraphicsInitDisplay();
    AppGraphicsWakeUpDisplay();
    AppGraphicsPrintOnOled(checkmark_icon, 70);
    AppGraphicsClearBuffer();
    AppGraphicsCloseDisplay();
    TEST_ASSERT_EQUAL(1, 1);
}

TEST_CASE("Test Print Upload Icon", "[appgraphics]")
{
    AppGraphicsInitDisplay();
    AppGraphicsWakeUpDisplay();
    AppGraphicsPrintOnOled(upload_icon, 70);
    AppGraphicsClearBuffer();
    AppGraphicsCloseDisplay();
    TEST_ASSERT_EQUAL(1, 1);
}

TEST_CASE("Test Print Network Icon", "[appgraphics]")
{
    AppGraphicsInitDisplay();
    AppGraphicsWakeUpDisplay();
    AppGraphicsPrintOnOled(network_icon, 70);
    AppGraphicsClearBuffer();
    AppGraphicsCloseDisplay();
    TEST_ASSERT_EQUAL(1, 1);
}

TEST_CASE("Test Print Star Icon", "[appgraphics]")
{
    AppGraphicsInitDisplay();
    AppGraphicsWakeUpDisplay();
    AppGraphicsPrintOnOled(star_icon, 70);
    AppGraphicsClearBuffer();
    AppGraphicsCloseDisplay();
    TEST_ASSERT_EQUAL(1, 1);
}

