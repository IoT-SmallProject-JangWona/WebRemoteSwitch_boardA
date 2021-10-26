#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

const char* ssid = "1118-2.4G";
const char* password = "kpu123456!"; 

#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

const int pushSW = 2;
boolean state_on = false;
boolean state_off= true;

IRAM_ATTR void buttonClicked(){
   state_on = !state_on;
   state_off = !state_off;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pushSW,INPUT_PULLUP);
  attachInterrupt(pushSW,buttonClicked,FALLING);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.println("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("Connected to "); 
  Serial.println(ssid);

  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP());

   if (MDNS.begin("wonazz")){
    Serial.println("MDNS responder started");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();
  WiFiClient client;
  HTTPClient http;
    
    if(state_on == true && state_off == false){ // Only state_on is true
      Serial.print("[HTTP] begin...\n");
      if(http.begin(client, "http://172.30.1.8/Relay_on")){
        int httpCode = http.GET();
        if(httpCode > 0){
        Serial.printf("[HTTP] GET ... code: %d\n",httpCode);
        if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
          String payload = http.getString();
          Serial.println(payload);
        } 
      }else {
        Serial.printf("[HTTP] GET....failed, error: %s\n",http.errorToString(httpCode).c_str());
      } 
      http.end();
    } else {
      Serial.printf("[HTTP] Unable to connect\n");
    }
    state_on = false; // state_on value reset
    }

    if(state_on == true && state_off == true){ // state_on, state_off both true
      if(http.begin(client, "http://172.30.1.8/Relay_off")){
      int httpCode = http.GET();
      if(httpCode > 0){
        Serial.printf("[HTTP] GET ... code: %d\n",httpCode);
        if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
          String payload = http.getString();
          Serial.println(payload);
        }
      }else {
        Serial.printf("[HTTP] GET....failed, error: %s\n",http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP] Unable to connect\n");
    }
    state_on = false; // state_on value reset
    }
    
}