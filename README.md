# ESP-01-01S-MQTT-control
Simple MQTT-controlled ESP-01/01S firmware. Intended to be used for opening the door remotely via home assistant, but can be easily modified also for other use-cases.

## Description 
When value "UNLOCK" is recieved via specified MQTT topic, the relay will close the relay for 1.6 seconds.
