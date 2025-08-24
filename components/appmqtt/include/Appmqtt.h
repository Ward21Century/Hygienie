#ifndef APPMQTT_H
#define APPMQTT_H

#include <stdio.h>
#include <stdlib.h>
#include "AppMqttConfig.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "cJSON.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"
#include "AppData.h"
#include "time.h"
#include "lwip/apps/sntp.h"
#include "mqtt_client.h"
#include <inttypes.h>

#define NTP_SERVER_1 "pool.ntp.org"
#define NTP_SERVER_2 "ntp.ubuntu.org"

void AppMqttInit(void);

void AppMqttDestroy(void);

void AppMqttCreateJson(void);

void AppMqttAddTime(void);

void AppMqttIncrementOfflineReadingCount(void);

void AppMqttResetOfflineReadingCount(void);

void AppMqttClearTimeStamps(void);

uint8_t AppMqttGetNumoffLineReadingCount(void);

void AppMqttDestroyJson(cJSON *root);

void AppMqttPublish(char *json_str);

void AppMqttSetOfflineReadingCount(uint32_t offlineReadingCount);

void AppMqttInitNTPAndSyncTime(void);

void AppMqttSendData(void);

void AppMqttAddLocalTimeToJSON(cJSON *array, struct tm timeinfo, cJSON*time);

void AppMqttNTPinit(void);

void AppMqttSyncTime(void);
#endif
