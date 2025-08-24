#ifndef APPDATA_H
#define APPDATA_H
#include "stdlib.h"
#include "AppDataConfig.h"
#include "soc/rtc.h"
#include "driver/rtc_io.h"
#include <time.h>

//const float MAX_BATTERY_VOLTAGE = 4.2; // Max LiPoly voltage of a 3.7 battery is 4.2
//const float MIN_BATTERY_VOLTAGE = 3.4; // Max LiPoly voltage of a 3.7 battery is 4.2
//
//
//const char* ntpServer = "pool.ntp.org";
//const long  gmtOffset_sec = -25200;
//const int   daylightOffset_sec = 3600;

struct Sanitizer_Data {
    bool pump_initialized;
    char device_name[32];
    char device_id[25];
    uint32_t max_offline_readings;
    uint32_t bootCount;
    uint32_t offlineReadingCount;
    time_t  time_stamp_seconds[15];
    struct tm readings_temp[15];
};
#endif

