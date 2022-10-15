#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "wifi_conn.h"

static uint8_t retry_num;
static bool is_connected;
static bool got_ip;

static void wifi_event_handler(void *event_handler_arg, 
                               esp_event_base_t event_base, 
                               int32_t event_id, 
                               void *event_data)
{
  switch (event_id)
  {
    case WIFI_EVENT_STA_START:
      esp_wifi_connect();
      ESP_LOGI("WI-FI","WiFi connecting ... \n");
      break;
    case WIFI_EVENT_STA_CONNECTED:
      retry_num = 0;
      is_connected = true;
      ESP_LOGI("WI-FI","WiFi connected ... \n");
      break;
    case WIFI_EVENT_STA_DISCONNECTED:
      is_connected = false;
      got_ip = false;
      if(retry_num < MAXIMUM_RETRIES)
      {
          esp_wifi_connect();
          retry_num++;
          ESP_LOGI("WI-FI","Retry connection to the AP\n");
      }
      break;
    case IP_EVENT_STA_GOT_IP:
      ESP_LOGI("WI-FI","WiFi got IP ... \n\n");
      got_ip = true;
      break;
    default:
      break;
  }
}

void wifi_config(void)
{
  // 1 - Wi-Fi/LwIP Init Phase
  esp_netif_init();                    // TCP/IP initiation 					s1.1
  esp_event_loop_create_default();     // event loop 			                s1.2
  esp_netif_create_default_wifi_sta(); // WiFi station 	                    s1.3
  wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&wifi_initiation); // 					                    s1.4
  // 2 - Wi-Fi Configuration Phase
  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
  esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
  wifi_config_t wifi_configuration = 
  {
    .sta = 
    {
      .ssid = SSID,
      .password = PASS
    }
  };
  esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
  // 3 - Wi-Fi Start Phase
  esp_wifi_start();
}

bool wifi_connected(void)
{
  return is_connected;
}

bool wifi_got_ip(void)
{
  return got_ip;
}

bool wifi_disconnected_after_max_retries(void)
{
  return (!is_connected && retry_num >= MAXIMUM_RETRIES);
}

void wifi_reconnect_after_specified_time(uint32_t time_milli_secs)
{
  ESP_LOGI("RECONNECT","Connection failed, restarting in %d milliseconds\n",time_milli_secs);
  vTaskDelay(pdMS_TO_TICKS(time_milli_secs));
  retry_num = 0;
  esp_wifi_connect();
}