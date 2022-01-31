#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "xxxx"
#define WIFI_PASSWORD "xxxx"
#define RELAY 0 // relay connected to  GPIO0

char mqttServer[]  = "xxxx"; //change it for your MQTT server IP or network name
int mqttPort  = 1883; //1883 is the default port for MQTT. Change if necessary
char deviceId[]  = "yyyy"; //every MQTT device should have a different name
char topic[]  = "cmnd/xxxx/xxxx/yyyy"; //the topic should be different for each device as well
char mqttUser[]  = "xxxx";
char mqttPassword[]  = "xxxx";

WiFiClient wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Starting");
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY,HIGH);
}

void loop() {
  if ( !client.connected() ) {
    connect();
  }
  client.loop();
}

const String unlockstring = "UNLOCK";

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  if(message == unlockstring) { 
    Serial.println("Triggering relay for specified period");
    digitalWrite(RELAY,LOW);
    delay(1600);
    digitalWrite(RELAY,HIGH);
  }
}

void connect() {
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.print("Connected to ");
      Serial.println(WIFI_SSID);
    }
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    if (client.connect(deviceId, mqttUser, mqttPassword)) {
      client.subscribe(topic);
      Serial.println("Connected to MQTT Server");
    } else {
      Serial.print("[FAILED] [ rc = ");
      Serial.print(client.state() );
      Serial.println(" : retrying in 5 seconds]");
      delay(5000);
    }
  }
}
