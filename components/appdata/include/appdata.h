#include "soc/rtc.h"
#include "driver/rtc_io.h"
#include <time.h>

#define MAX_OFFLINE_READINGS CONFIG_MAX_OFFLINE_READINGS
#define facility_name "testfacility"
//const float MAX_BATTERY_VOLTAGE = 4.2; // Max LiPoly voltage of a 3.7 battery is 4.2
//const float MIN_BATTERY_VOLTAGE = 3.4; // Max LiPoly voltage of a 3.7 battery is 4.2
//
//
//const char* ntpServer = "pool.ntp.org";
//const long  gmtOffset_sec = -25200;
//const int   daylightOffset_sec = 3600;

struct Sanitizer_Data {
    bool pump_initialized;
    char PUMP_ID[25];
    int max_offline_readings;
    int bootCount;
    unsigned int offlineReadingCount;
    struct tm  readings_temp[15];
};

