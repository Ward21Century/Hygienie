
#include "AppSleepConfig.h"

void AppSleepInit(void);

void AppSleepGoToDeepSleep(void);

void AppSleepRecordEnterTime(void);

esp_sleep_wakeup_cause_t AppSleepWakeUpFromDeepSleep(void);

void AppSleepLog(void);

esp_sleep_wakeup_cause_t AppSleepGetWakeUpCause(void);

void AppMqttEraseTimeStamps(void);

void AppSleepDeepSleepTimerInit(void);

void AppSleepTouchWakeUpInit(void);
