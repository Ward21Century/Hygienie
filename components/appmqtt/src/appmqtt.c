#include "appmqtt.h"
#include <string.h>

static const char *TAG = "mqtt application";

static RTC_DATA_ATTR struct Sanitizer_Data sanitizer_data = { .pump_initialized = false,
                                                              .PUMP_ID = "test_id",
                                                              .max_offline_readings = MAX_OFFLINE_READINGS,
                                                              .bootCount = 10,
                                                              .offlineReadingCount = 0};

static const esp_mqtt_client_config_t mqtt_cfg = {
    .uri = CONFIG_MQTT_URL,
    .username = CONFIG_USERNAME,
    .password = CONFIG_PASSWORD,
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
    time_t now;
    char strftime_buf[64];
    time(&now);

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
    esp_mqtt_client_publish(client, "v1/devices/me/telemetry", json_str, strlen(json_str), 0, 0 );

    vTaskDelay(pdMS_TO_TICKS(500));
    return;
}

void AppMqttSetOffineReadingCount(int offlineReadingCount) {
    sanitizer_data.offlineReadingCount = offlineReadingCount;
    return;
}

void AppMqttSendData(void) {
    AppMqttNTPinit();
    AppMqttSyncTime();
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

void AppMqttNTPinit(void) {
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, NTP_SERVER_1);
    sntp_setservername(1, NTP_SERVER_2);
    sntp_init();
}

void AppMqttSyncTime(void) {
    time_t now = 0;
    struct tm timeinfo ={0};
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (2023-1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(pdMS_TO_TICKS(2000));
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if(timeinfo.tm_year < (2023-1900)) {
        ESP_LOGE(TAG, "Failed to set system time using NTP");
    }
    else {
        ESP_LOGI(TAG, "System time is set using NTP");
    }
}


