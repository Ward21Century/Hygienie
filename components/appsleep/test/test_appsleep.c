/* test_mean.c: Implementation of a testable component.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "unity.h"
#include "AppSleep.h"
#include <time.h>


TEST_CASE("Test AppSleep Sleep Timer", "[appsleep]")
{
    struct timeval sleep_enter_time;
    struct timeval now;
    AppSleepDeepSleepTimerInit();
    gettimeofday(&sleep_enter_time, NULL);
    AppSleepGoToDeepSleep();
    AppSleepWakeUpFromDeepSleep();
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;
    TEST_ASSERT_INT_WITHIN(1000, 10000, sleep_time_ms);

}

TEST_CASE("Test AppSleep TouchPad WakeUp", "[appsleep]")
{
    AppSleepTouchWakeUpInit();
    AppSleepGoToDeepSleep();
    AppSleepWakeUpFromDeepSleep();
    TEST_ASSERT_EQUAL(4, esp_sleep_get_wakeup_cause());

}
