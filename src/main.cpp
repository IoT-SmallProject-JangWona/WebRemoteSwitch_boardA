#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

const char* ssid = "1118-2.4G";
const char* password = "kpu123456!";

#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

const int pushSW = 2;
boolean state = false;

IRAM_ATTR void buttonClicked(){
   state = true;
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
1
void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();
  WiFiClient client;
  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  if(state == true){
    if(http.begin(client, "http://172.30.1.23/Relay_toggle")){
      Serial.print("[HTTP] GET...\n");
      
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
    state = false;
  }
  delay(1000);
}