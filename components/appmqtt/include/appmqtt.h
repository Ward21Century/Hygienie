#include <stdio.h>
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
