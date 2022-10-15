#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "thingspeak_mqtt.h"
#include <string.h>

#define MAX_NUMBER_OF_FIELDS  8

static void integer_to_string(uint32_t integer, char* string)
{
	if (integer == 0)
	{//Edge case  
		string[0] = '0';
		return;
	}
	uint32_t copy_of_integer = integer;
	uint8_t number_of_digits = 0;

	while(copy_of_integer > 0)
	{
		copy_of_integer /= 10;
		number_of_digits++;
	}
	while (integer > 0)
	{
		string[number_of_digits - 1] = '0' + (integer % 10);
		integer /= 10;
		number_of_digits--;
	}
}
 
/**
 * @brief 
 * 
 * @param client_struct 
 */
static void prevent_client_data_buffer_overflow(thingspeak_mqtt_client_t* client_struct)
{
  if(client_struct->num_of_set_fields >= MAX_NUMBER_OF_FIELDS)
  {
    ESP_LOGI("RESET BROKER DATA","resetting after %d\n fields were set\n",client_struct->num_of_set_fields);
    //Resetting buffer containing data to send to broker
    client_struct->client_data[client_struct->index_of_first_field_num + 2] = '\0';
    client_struct->num_of_set_fields = 0;
    ESP_LOGI("BROKER DATA","after resetting, data to broker = %s\n",client_struct->client_data);
  }
  else
  {
    (client_struct->num_of_set_fields)++;
    ESP_LOGI("NUMBER OF FIELDS","%d field(s) have been added\n",client_struct->num_of_set_fields);
  }
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
  switch (event->event_id) 
  {
      case MQTT_EVENT_ERROR:
          ESP_LOGI("MQTT", "MQTT_EVENT_ERROR\n");
          break;
      case MQTT_EVENT_CONNECTED:
          ESP_LOGI("MQTT", "MQTT_EVENT_CONNECTED\n");
          break;
      case MQTT_EVENT_DISCONNECTED:
          ESP_LOGI("MQTT", "MQTT_EVENT_DISCONNECTED\n");
          break;
      case MQTT_EVENT_PUBLISHED:
          ESP_LOGI("MQTT", "MQTT_EVENT_PUBLISHED, msg_id=%d\n", event->msg_id);
          break;
      case MQTT_EVENT_BEFORE_CONNECT:
          ESP_LOGI("MQTT", "MQTT_EVENT_BEFORE_CONNECT\n");
          break;
      default:
          ESP_LOGI("MQTT", "Other event id:%d\n", event->event_id);
          break;
  }
  return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) 
{
  ESP_LOGD("MQTT", "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
  mqtt_event_handler_cb(event_data);
}

void thingspeak_mqtt_client_init(thingspeak_mqtt_client_t* client_struct)
{
  client_struct->topic_length = 0;
  client_struct->index_of_first_field_num = 0;
  client_struct->num_of_set_fields = 0;
  memset(client_struct->client_data,'\0',strlen(client_struct->client_data));//Clear data buffer before initializing
  //Initialize data to be transmitted to broker
  strcat(client_struct->client_data,"channels/");
  strcat(client_struct->client_data,CHANNEL_ID);
  strcat(client_struct->client_data,"/publish");
  client_struct->topic_length = strlen(client_struct->client_data);
  ESP_LOGI("TOPIC STRING LEN","len = %d\n",client_struct->topic_length);
  strcat(client_struct->client_data,"field1=");
  ESP_LOGI("INITIAL BROKER DATA","data to broker = %s\n",client_struct->client_data);
  //Get index of first field number [default field number = 1]
  for(uint8_t i = 0; i < strlen(client_struct->client_data); i++)
  {
    if(client_struct->client_data[i] == '=')
    {
      client_struct->index_of_first_field_num = i - 1;
      ESP_LOGI("FIRST FIELD NUM","index = %d\n",client_struct->index_of_first_field_num);
      break;
    }
  }
  //ESP-MQTT setup
  esp_mqtt_client_config_t mqtt_cfg = 
  {
      .uri = URI,
      .port = PORT,
      .username = USERNAME,
      .client_id = CLIENT_ID,
      .password = MQTT_PASSWORD
  };
  client_struct->handle = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_register_event(client_struct->handle,
                                  ESP_EVENT_ANY_ID,
                                  mqtt_event_handler,
                                  client_struct->handle);
  esp_mqtt_client_start(client_struct->handle);
}

void thingspeak_set_field(thingspeak_mqtt_client_t* client_struct,uint8_t field_num,uint32_t field_value)
{
  char field_value_string[11] = {0}; //10 digits max
  integer_to_string(field_value,field_value_string);
  if(strlen(client_struct->client_data) == (client_struct->index_of_first_field_num + 2))
  {
    ESP_LOGI("SET FIELD","First field to be set\n");
    client_struct->client_data[client_struct->index_of_first_field_num] = field_num + '0';
    strcat(client_struct->client_data,field_value_string);
    ESP_LOGI("AFTER SETTING FIRST FIELD","data to broker is now = %s\n",client_struct->client_data);
  }
  else if(strlen(client_struct->client_data) > (client_struct->index_of_first_field_num + 2))
  {
    ESP_LOGI("SET FIELD","Subsequent field to be set\n");
    char new_field_data[20] = "&field1=";
    new_field_data[6] = field_num + '0';
    strcat(new_field_data,field_value_string);
    strcat(client_struct->client_data,new_field_data);
    ESP_LOGI("AFTER SETTING ANOTHER FIELD","data to broker is now = %s\n",client_struct->client_data);
  }
  else
  {
    ESP_LOGI("SET FIELD","Forbidden\n");
    return;
  }
  prevent_client_data_buffer_overflow(client_struct);
}

void thingspeak_mqtt_client_publish(thingspeak_mqtt_client_t* client_struct)
{
  //Split client data buffer into 'topic' and 'field data' strings
  char topic[50] = {0};
  char field_data[200] = {0};
  for(uint8_t i = 0; i < strlen(client_struct->client_data); i++)
  { 
    if(i < client_struct->topic_length)
    {
      topic[i] = client_struct->client_data[i]; //Get topic 
    }
    else
    {
      field_data[i - client_struct->topic_length] = client_struct->client_data[i]; //Get field data
    }
  }
  //Test the results
  ESP_LOGI("FINAL STRINGS","topic = %s and field data = %s\n",topic,field_data);
  esp_mqtt_client_publish(client_struct->handle,topic,field_data,0,1,0);
  //Resetting client data buffer
  client_struct->client_data[client_struct->index_of_first_field_num + 2] = '\0';
  client_struct->num_of_set_fields = 0;
}