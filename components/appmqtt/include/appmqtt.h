#include <stdio.h>
#include "appmqtt.h"
#include "json.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"
#include "appdata.h"
#include "time.h"


void AppMqttInit(void);

void AppMqttPublish(void);

void AppMqttDestory(void);

void AppMqttCreateJson(void);

void AddMqttAddTime(time_t time);

void AppMqttDestoryJson(cJson *root);

void AppMqttPublish(char *json_str);

void AppMqttSetOfflineReadingCount(int offlineReadingCount);
