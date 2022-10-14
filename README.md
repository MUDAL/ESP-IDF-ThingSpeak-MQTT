# ESP-IDF-ThingSpeak-MQTT   

## Description    
In this project, an ``ESP32`` is used to publish data (humidity and temperature) from a ``BME280`` sensor to ``ThingSpeak`` using the ``MQTT`` protocol. An HTTP equivalent of this project can be found in one of my repos (https://github.com/MUDAL/ESP-IDF-ThingSpeak-HTTP). Both projects have the same objective, the only difference is the communication protocol used.    

## Requirements    
1. ESP32 Dev Board  
2. BME280 sensor  
3. ThingSpeak account  
4. Visual Studio Code  

## MQTT protocol   
MQTT involves communication between a ``client`` and a ``broker``. You can think of the broker as a server that receives data from client(s) and also sends data to the client(s). The term used to describe data transmission from a client to a broker is called ``publishing`` i.e. the client ``publishes`` data to a ``topic`` of the broker. When a client ``subscribes`` to a ``topic``, it means the client will receive data from the broker based on that topic. See https://mqtt.org/ for more information.  

## Important parameters for MQTT communication with ThingSpeak
1. ``Broker URI``: mqtt://mqtt3.thingspeak.com    
2. ``Port``: 1883    
3. ``Username``    
4. ``Client ID``   
5. ``Password``  

3,4 and 5 are credentials that will be automatically generated for you once you create an MQTT device in ThingSpeak. Save these credentials as they'd be used in the code to communicate with the broker.  

## How to create an MQTT device in ThingSpeak  
1. Click on the ``Devices`` tab and click on ``MQTT``  

![Screenshot (270)](https://user-images.githubusercontent.com/46250887/195920624-40248e79-d28b-4f98-976e-9f6ccf819af7.png)  

2. Click on ``Add a new device``  

![Screenshot (271)](https://user-images.githubusercontent.com/46250887/195920789-2b783723-e224-472b-8210-b34632e3d1eb.png)

3. Enter a name for the device  

![Screenshot (272)](https://user-images.githubusercontent.com/46250887/195920902-fe5a3d45-97bc-4fc5-a40a-378782c7df1e.png)

4. Associate a channel with the device  

![Screenshot (273)](https://user-images.githubusercontent.com/46250887/195921133-c5c7e6b4-af54-4d48-ba9c-8aa79f350122.png)  

5. Click on ``Add Channel``  

![Screenshot (274)](https://user-images.githubusercontent.com/46250887/195921339-8d65c45f-0c32-44ce-ae90-24ad588bf7a9.png)  

6. Click on ``Download Credentials``    

![Screenshot (275)](https://user-images.githubusercontent.com/46250887/195921518-9e6ee483-b41b-4078-bb2e-51815bbc32c6.png)  

7. Select ``Plain Text (*.txt)``   

![Screenshot (276)](https://user-images.githubusercontent.com/46250887/195922301-02d008b8-8691-458f-8e38-ec9c7bfd2025.png)    

8. Click ``Done``

![Screenshot (277)](https://user-images.githubusercontent.com/46250887/195922356-801d63c0-52e2-42ce-951b-561e458a28e0.png)   

9. View the ``MQTT device`` you just created, confirm its name  

![Screenshot (278)](https://user-images.githubusercontent.com/46250887/195921988-7fdc9fb4-5729-46ce-ab13-ef64d2ef9274.png)  

## MQTT format to publish data to ThingSpeak broker  
1. Topic: ``channels/<channel_id>/publish`` e.g. ``channels/1807856/publish``  
2. Data: ``field<x>=<val1>&field<y>=<val2>`` e.g. ``field1=55&field2=70``  
The ``Topic`` and ``Data`` strings above will be passed as arguments to the ``esp_mqtt_client_publish()`` function in order to publish data to the broker.  

## Visualizing data from the sensor  
The sensor readings can be visualized in the ``channel`` that was associated with the ``MQTT device`` created. 

## Additional resources    
1. Publishing to ThingSpeak broker: https://www.mathworks.com/help/thingspeak/use-desktop-mqtt-client-to-publish-to-a-channel.html    
2. Getting MQTT client credentials from ThingSpeak: https://www.mathworks.com/help/thingspeak/mqtt-basics.html     
3. ESP-IDF MQTT documentation: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mqtt.html    

