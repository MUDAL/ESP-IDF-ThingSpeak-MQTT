#ifndef THINGSPEAK_MQTT_H
#define THINGSPEAK_MQTT_H

#include "credentials.h"
//ThingSpeak channel fields
typedef enum 
{
  THINGSPEAK_FIELD1 = 1,
  THINGSPEAK_FIELD2,
  THINGSPEAK_FIELD3,
  THINGSPEAK_FIELD4,
  THINGSPEAK_FIELD5,
  THINGSPEAK_FIELD6,
  THINGSPEAK_FIELD7,
  THINGSPEAK_FIELD8
}thingspeak_field_t;

/**
 * @brief MQTT client struct. Contains the following:  
 * 1. MQTT client handle (Required to publish data to the ThingSpeak broker).  
 *    It holds some MQTT configuration data required to perform publishing of data to  
 *    a topic.    
 * 2. Client data buffer (holds the data to be sent to the broker)  
 * 3. Number of set fields (this is a counter to prevent   
 *    overflow of the client data buffer).  
 * 4. Index of first field number (index of client data buffer that contains
 *    the number of the first data field in the client data buffer).  
 * 5. Topic length: length of the topic. The client data buffer is split into  
 *    two strings internally ('topic' string and 'data field' string).   
 */

typedef struct
{
  esp_mqtt_client_handle_t handle;
  char client_data[300];
  uint8_t num_of_set_fields;
  uint8_t index_of_first_field_num;
  uint8_t topic_length;
}thingspeak_mqtt_client_t;
/**
 * @brief Initializes ThingSpeak MQTT client.  
 * 
 * @param client_struct pointer to MQTT client struct.
 */
void thingspeak_mqtt_client_init(thingspeak_mqtt_client_t* client_struct);
/**
 * @brief Sets up field number and value before publishing to the broker.  
 * 
 * @param client_struct pointer to MQTT client struct.  
 * @param field_num Field in which data is to be written.  
 * @param field_value Value to be written to the field.  
 */
void thingspeak_set_field(thingspeak_mqtt_client_t* client_struct,uint8_t field_num,uint32_t field_value);
/**
 * @brief Publishes client data to ThingSpeak broker.  
 * 
 * @param client_struct pointer to MQTT client struct.  
 */
void thingspeak_mqtt_client_publish(thingspeak_mqtt_client_t* client_struct);

#endif