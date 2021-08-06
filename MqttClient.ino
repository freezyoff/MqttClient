#include <Arduino.h>
#include "tls/UserTrust.CA.h"
#include "arduino_esp8266/Mqtt.cpp"

#include <TZ.h>
#include <ESP8266WiFi.h>

#define WAN_CONFIG_SSID "AA8"
#define WAN_CONFIG_PWD "sadana devica"
#define WAN_CONFIG_NTP1 "id.pool.ntp.org"
#define WAN_CONFIG_NTP2 "pool.ntp.org"
#define WAN_CONFIG_NTP3 "time.google.com"

#define MQTT_CONFIG_CID  "NamaSayaMusa"
#define MQTT_CONFIG_HOST "mqtt.kosan.co.id"
#define MQTT_CONFIG_PORT 8883
#define MQTT_CONFIG_USR "jacc"
#define MQTT_CONFIG_PWD "kosansukses!"

unsigned long timer = 0;
bool call_connect = false;
const char* subTopic[2] = {"jacc/s/#", "jacc/p/#"}; 

MqttSecureClient mqttClient;

void syncTime(){
  if (esp8266::coreVersionNumeric() >= 20700000){
    configTime(TZ_Asia_Jakarta, WAN_CONFIG_NTP1, WAN_CONFIG_NTP2, WAN_CONFIG_NTP3);
  }
  else{
    setenv("TZ", "WIB-7" , 1);
    configTime(0, 0, WAN_CONFIG_NTP1, WAN_CONFIG_NTP2, WAN_CONFIG_NTP3);
  }
}

bool isTimeSync(){ return time(NULL) > 1627750749; }

void onMqttConnected(bool connected){
  Serial.println(F("onMqttConnected"));
}

void onMqttSubscribe(uint16_t pid, std::vector<uint8_t>* qos){}

void onMqttUnsubscribe(uint16_t pid){}

void onMqttPublish(uint16_t pid){}

void onMqttMessage(uint16_t pid, const char* topic, MqttMsg* msg){ 
  Serial.println(F("onMqttMessage")); 
  Serial.print(F("F size:"));Serial.println(msg->size());
  Serial.print(F("Topic :"));Serial.println(topic);
  while (msg->available()){
    Serial.print((char) msg->read());
  }
  Serial.println("");
}

void setup(){
  Serial.begin(115200);
  Serial.println("");
  
  LittleFS.begin();
  
  WiFi.begin(WAN_CONFIG_SSID, WAN_CONFIG_PWD);
  Serial.print("Connecting to ");Serial.print(WAN_CONFIG_SSID); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
    if (i > 30) ESP.restart();
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  
  if (!isTimeSync()){
    syncTime();
    Serial.print("Sync Time ...");
    i=0;
    while (!isTimeSync()) {
      yield();
      delay(1000);
      if (i > 60) ESP.restart();
    }
  }
  Serial.print("success: ");Serial.println(time(NULL));
  
  mqttClient.setServer(MQTT_CONFIG_HOST, MQTT_CONFIG_PORT);
  mqttClient.setCACert(USERTRUST_CA_DER, USERTRUST_CA_LEN);
  mqttClient.setOnConnectedCallback(onMqttConnected);
  mqttClient.setOnSubscribeCallback(onMqttSubscribe);
  mqttClient.setOnUnsubscribeCallback(onMqttUnsubscribe);
  mqttClient.setOnPublishCallback(onMqttPublish);
  mqttClient.setOnMessageCallback(onMqttMessage);
  mqttClient.connect(MQTT_CONFIG_CID, MQTT_CONFIG_USR, MQTT_CONFIG_PWD);

}

bool subscribeOK = false;

void loop() {
  
  if (!mqttClient.connected() && millis()-timer > 3000) { 
    timer = millis();
    mqttClient.connect(MQTT_CONFIG_CID, MQTT_CONFIG_USR, MQTT_CONFIG_PWD);
  }
  
  if (!mqttClient.connected()) return;
    
  if (mqttClient.loop()) {
    yield();
  }
  
  if (millis() - timer > 2000){
    timer = millis();
    if ( !subscribeOK ){
      //const char* subTopic[2] = {"jacc/s/#", "jacc/p/#"};
      uint16_t sid = 123;
      subscribeOK = mqttClient.subscribe(&sid, subTopic, (uint8_t) 0, 2);
    }
    else{
      uint16_t sid = 124;
      subscribeOK = mqttClient.unsubscribe(&sid, subTopic, 2);
    }
    
    uint16_t mid = 99;
    const char* msg = "halo apakabar?";
    mqttClient.publish(&mid, "jacc/p/abc", (uint8_t*) msg, strlen(msg), 0, false, false);
    --mid;
    const char* msg2 = "Saya Baik loh!";
    mqttClient.publish(&mid, "jacc/p/xxx", (uint8_t*) msg2, strlen(msg2), 0, false, false);
  }

}
