# ESP-IDF-ThingSpeak-MQTT   

## Description    
In this project, an ``ESP32`` is used to publish data (humidity and temperature) from a ``BME280`` sensor to ``ThingSpeak`` using the ``MQTT`` protocol. An HTTP equivalent of this project can be found in one of my repos (https://github.com/MUDAL/ESP-IDF-ThingSpeak-HTTP). Both projects have the same objective, the only difference is the communication protocol used.    

## Requirements    
1. ESP32 Dev Board  
2. BME280 sensor  
3. ThingSpeak account  
4. Visual Studio Code  

## MQTT protocol   


## Important parameters for MQTT communication with ThingSpeak
1. ``Broker URI``: mqtt://mqtt3.thingspeak.com    
2. ``Port``: 1883    
3. ``Username``    
4. ``Client ID``   
5. ``Password``  

3,4 and 5 are credentials that will be automatically generated for you once you create an MQTT device in ThingSpeak.

## MQTT format to publish data to ThingSpeak broker  
Topic: ``channels/<channel_id>/publish`` e.g. ``channels/1807856/publish``
Data: ``field<x>=<val1>&field<y>=<val2>`` e.g. ``field1=55&field2=70``

## Resources to setup MQTT communication with ThingSpeak broker  
1. Publishing to ThingSpeak broker: https://www.mathworks.com/help/thingspeak/use-desktop-mqtt-client-to-publish-to-a-channel.html    
2. Getting MQTT client credentials from ThingSpeak: https://www.mathworks.com/help/thingspeak/mqtt-basics.html     
3. ESP-IDF MQTT documentation: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mqtt.html    
