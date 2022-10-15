#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "wifi_conn.h"
#include "bme280.h"
#include "thingspeak_mqtt.h"

/**
 * @brief Task to handle periodic transmission of BME280
 * sensor data to Thingspeak server via the MQTT protocol.
 * 
 * @param arg 
 */
void upload_sensor_data_to_thingspeak(void* arg)
{
    thingspeak_mqtt_client_t mqtt_client;
    bme280_data_t bme280_data = {};
    thingspeak_mqtt_client_init(&mqtt_client);
    TickType_t previous_tick = xTaskGetTickCount();
    while(1)
    {
        bme280_get_data(&bme280_data);
        if(wifi_connected() && wifi_got_ip())
        {
            ESP_LOGI("SEND","uploading data to cloud\n");
            thingspeak_set_field(&mqtt_client,THINGSPEAK_FIELD1,bme280_data.temperature);
            thingspeak_set_field(&mqtt_client,THINGSPEAK_FIELD2,bme280_data.humidity);
            thingspeak_mqtt_client_publish(&mqtt_client);
        }
        vTaskDelayUntil(&previous_tick,pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    nvs_flash_init();
    wifi_config();
    bme280_init();
    xTaskCreate(upload_sensor_data_to_thingspeak,"sensor-data-to-cloud",4000,NULL,configMAX_PRIORITIES,NULL);
    TickType_t previous_tick = xTaskGetTickCount();
    while(1)
    {
        if(wifi_disconnected_after_max_retries())
        {
            wifi_reconnect_after_specified_time(3000);
        }
        vTaskDelayUntil(&previous_tick,pdMS_TO_TICKS(50));
    }    
}
