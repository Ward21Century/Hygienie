#include "appmqtt.h"
#include <string.h>

static const char *TAG = "mqtt";

static RTC_DATA_ATTR time_t now;
static RTC_DATA_ATTR struct tm *timeinfo; // Pointer to struct tm

static RTC_DATA_ATTR struct Sanitizer_Data sanitizer_data = { .pump_initialized = false,
                                                              .device_name = DEVICE_NAME,
                                                              .device_id = DEVICE_ID,
                                                              .max_offline_readings = MAX_OFFLINE_READINGS,
                                                              .bootCount = 10,
                                                              .offlineReadingCount = 0};

static const esp_mqtt_client_config_t mqtt_cfg = {
    .uri = MQTT_IP_ADDRESS_WITH_PORT,
    .username = MQTT_USERNAME,
    .client_id = MQTT_CLIENT_ID,
    .password = MQTT_PASSWORD
};


static esp_mqtt_client_handle_t client;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {

    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, "v1/devices/me/telemetry", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "v1/devices/me/telemetry", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "v1/devices/me/telemetry/qos1");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "v1/devices/me/telemetry", "data", 0, 0, 0);
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
    cJSON_AddStringToObject(root, "DEVICE_NAME", sanitizer_data.device_name);
    cJSON_AddStringToObject(root, "DEVICE_ID", sanitizer_data.device_id);
    cJSON *array = cJSON_CreateArray();
    for (uint32_t i = 0; i < AppMqttGetNumoffLineReadingCount(); i++) {
        cJSON *time = cJSON_CreateObject();
  //      AppMqttAddLocalTimeToJSON(array, sanitizer_data.readings_temp[i], time);
        cJSON_AddItemToObject(array, "Time Recorded:", cJSON_CreateNumber((double)sanitizer_data.time_stamp_seconds[i]));
    }
    cJSON_AddItemToObject(root, "TimeStamps", array);
    char *json_str = cJSON_Print(root);
    printf("%s\n", json_str);
    AppMqttPublish(json_str);
    AppMqttDestroyJson(root);
    free(json_str);
    return;
 }

void AppMqttAddTime(void) {

    // Get the current time as a Unix timestamp (seconds since the Epoch)
    time(&now);
    // Convert the timestamp to local time (tm structure)
    timeinfo = localtime(&now);

    // Print the current UTC time in seconds
    printf("Current UTC time in seconds: %ld\n", now);

    // Store the UTC timestamp
    sanitizer_data.time_stamp_seconds[sanitizer_data.offlineReadingCount] = now;
    printf("Stored timestamp: %ld\n", sanitizer_data.time_stamp_seconds[sanitizer_data.offlineReadingCount]);

    // Assuming you want to store the local time representation, you need to copy the structure
    //if (timeinfo) { // Check pointer validity
    //    sanitizer_data.readings_temp[sanitizer_data.offlineReadingCount] = *timeinfo; // Dereference pointer to copy structure
//    } else {
        // Handle error: localtime returned NULL
 //   }
    // Increment the count of readings
    AppMqttIncrementOfflineReadingCount();
    return;
}

void AppMqttClearTimeStamps(void) {
    for (int32_t i = 0; i < sanitizer_data.max_offline_readings; i++) {
         memset(&sanitizer_data.readings_temp[i], 0, sizeof(struct tm));
         sanitizer_data.time_stamp_seconds[i] = 0;
    }
}

void AppMqttIncrementOfflineReadingCount(void) {
    sanitizer_data.offlineReadingCount++;
}

void AppMqttResetOfflineReadingCount(void) {
    if (sanitizer_data.offlineReadingCount >= sanitizer_data.max_offline_readings)
        sanitizer_data.offlineReadingCount = 0;
    return;
}

int AppMqttGetNumoffLineReadingCount(void) {
    int off_line_count = sanitizer_data.offlineReadingCount;
    return off_line_count;
}

uint32_t AppmqttGetMaxOfflineReadings(void) {
   return sanitizer_data.max_offline_readings;
}


void AppMqttDestroyJson(cJSON *root) {
    cJSON_Delete(root);
    return;
}

void AppMqttPublish(char *json_str) {
    esp_mqtt_client_publish(client, "v1/devices/me/telemetry", json_str, strlen(json_str), 0, 0 );
    vTaskDelay(pdMS_TO_TICKS(500));
    return;
}

void AppMqttSetOffineReadingCount(uint32_t offlineReadingCount) {
    sanitizer_data.offlineReadingCount = offlineReadingCount;
    return;
}

void AppMqttInitNTPAndSyncTime(void) {
    AppMqttNTPinit();
    AppMqttSyncTime();
}

void AppMqttSendData(void) {
    AppMqttInit();
    AppMqttCreateJson();
    AppMqttClearTimeStamps();
    AppMqttResetOfflineReadingCount();
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

void AppMqttNTPinit(void) {
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, NTP_SERVER_1);
    sntp_setservername(1, NTP_SERVER_2);
    sntp_init();
}

void AppMqttSyncTime(void) {
    time_t nowr = 0;
    struct tm timeinfor ={0};
    int retry = 0;
    const int retry_count = 10;

    while (timeinfor.tm_year < (2024-1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(pdMS_TO_TICKS(2000));
        time(&nowr);
        ESP_LOGE(TAG, "Failed to set system time using NTP");
        char buf[64];
        localtime_r(&nowr, &timeinfor);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
        printf("Current system time is: %s\n", buf);
    }

    if(timeinfor.tm_year < (2024-1900)) {
        ESP_LOGE(TAG, "Failed to set system time using NTP");
    }
    else {
        ESP_LOGI(TAG, "System time is set using NTP");
    }
}


