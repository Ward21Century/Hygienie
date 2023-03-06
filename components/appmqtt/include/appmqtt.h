#include <stdio.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "cJSON.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"
#include "appdata.h"
#include "time.h"
#include "mqtt_client.h"

void AppMqttInit(void);

void AppMqttDestory(void);

void AppMqttCreateJson(void);

void AppMqttAddTime(void);

int AppMqttGetNumoffLineReadingCount(void);

void AppMqttDestroyJson(cJSON *root);

void AppMqttPublish(char *json_str);

void AppMqttSetOfflineReadingCount(int offlineReadingCount);

void AppMqttSendData(void);

void AppMqttAddLocalTimeToJSON(cJSON *array, struct tm timeinfo, cJSON*time);
