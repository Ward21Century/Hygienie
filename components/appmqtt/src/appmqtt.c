#include "appmqtt.h"
#include <string.h>

static const char *TAG = "mqtt application";

static RTC_DATA_ATTR struct Sanitizer_Data sanitizer_data = { .pump_initialized = false,
                                                              .PUMP_ID = "test_id",
                                                              .max_offline_readings = MAX_OFFLINE_READINGS,
                                                              .bootCount = 10,
                                                              .offlineReadingCount = 0};

static const esp_mqtt_client_config_t mqtt_cfg = {
    .uri = "mqtt://mqtt.eclipseprojects.io",
//.event_handle = mqtt_event_handler,
};


static esp_mqtt_client_handle_t client;

//static double read_battery_level() {
//    rawValue = 0;
//    double voltageLevel = (rawValue / 4095.0) * 2 * 1.1 * 3.3; // calculate voltage level
//    double batteryFraction = ( voltageLevel - MIN_BATTERY_VOLTAGE ) / ( MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE );
//
//    return batteryFraction;
//
//}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {

    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
            ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            ESP_LOGI(TAG, "Last captured errno : %d (%s)",  event->error_handle->esp_transport_sock_errno,
                     strerror(event->error_handle->esp_transport_sock_errno));
        } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
            ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
        } else {
            ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void AppMqttInit() {
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    client = esp_mqtt_client_init(&mqtt_cfg);
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
    return;
}

void AppMqttDestroy() {
    esp_mqtt_client_destroy(client);
    return;
}

void AppMqttCreateJson() {

    cJSON *root =cJSON_CreateObject();
//    double batteryFraction = read_battery_level();
 //   cJSON_AddNumberToObject(root, "Battery_Fraction", batteryFraction);
    cJSON_AddStringToObject(root, "FacilityName", facility_name);
    cJSON *array = cJSON_CreateArray();
    for (int i = 0; i < MAX_OFFLINE_READINGS; i++) {
        cJSON *time = cJSON_CreateObject();
        AppMqttAddLocalTimeToJSON(array, sanitizer_data.readings_temp[i], time);
    }
    cJSON_AddItemToObject(root, "TimeStamps", array);
    char *json_str = cJSON_Print(root);
    printf("%s\n", json_str);
    AppMqttPublish(json_str);
    AppMqttDestroyJson(root);
    free(json_str);
    return;
 }

void AppMqttAddTime() {
    //time( &raw_time );
    time_t now;
    //struct tm timeinfo;
    char strftime_buf[64];
    time(&now);
// Set timezone to China Standard Time
    //setenv("TZ", "MT", 1);
    //tzset();
    //localtime_r(&now, &timeinfo);
    //strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
   //printf("The current date/time in Calgary is: %s", strftime_buf);


    struct tm * timeinfo = localtime( &now );
    sanitizer_data.readings_temp[sanitizer_data.offlineReadingCount] = *timeinfo;
    sanitizer_data.offlineReadingCount++;
    if (sanitizer_data.offlineReadingCount >= 10)
        sanitizer_data.offlineReadingCount = 0;
    return;
}

int AppMqttGetNumoffLineReadingCount() {
    int off_line_count = sanitizer_data.offlineReadingCount;
    return off_line_count;
}

void AppMqttDestroyJson(cJSON *root) {
    cJSON_Delete(root);
    return;
}

void AppMqttPublish(char *json_str) {
    esp_mqtt_client_publish(client, "example/topic", json_str, strlen(json_str), 0, 0 );
    return;
}

void AppMqttSetOffineReadingCount(int offlineReadingCount) {
    sanitizer_data.offlineReadingCount = offlineReadingCount;
    return;
}

void AppMqttSendData(void) {
    AppMqttInit();
    AppMqttCreateJson();
    AppMqttDestroy();
}

void AppMqttAddLocalTimeToJSON(cJSON *array, struct tm timeinfo, cJSON *time) {
   cJSON_AddNumberToObject(time, "tm_sec", timeinfo.tm_sec);
   cJSON_AddNumberToObject(time, "tm_min", timeinfo.tm_min);
   cJSON_AddNumberToObject(time, "tm_hour", timeinfo.tm_hour);
   cJSON_AddNumberToObject(time, "tm_mday", timeinfo.tm_mday);
   cJSON_AddNumberToObject(time, "tm_mon", timeinfo.tm_mon);
   cJSON_AddNumberToObject(time, "tm_year", timeinfo.tm_year);
   cJSON_AddNumberToObject(time, "tm_wday", timeinfo.tm_wday);
   cJSON_AddNumberToObject(time, "tm_yday", timeinfo.tm_yday);
   cJSON_AddNumberToObject(time, "tm_isdst", timeinfo.tm_isdst);
   cJSON_AddItemToArray(array, time);
   return;
}
