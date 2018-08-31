#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <fauxmoESP.h>

/* Network credentials */
#define WIFI_SSID "ChampagneCasa"
#define WIFI_PASS "welcomehome"    

/* Belkin WeMo emulation */
fauxmoESP fauxmo;
#define DEVICE_NAME "My Switch" 

/* Set Relay Pins */
#define RELAY_1 D1

#define DEBUG_FAUXMO 1

void setup() {
   Serial.begin(115200);
   Serial.println("");
   
   Serial.println("calling wifiSetup");
   wifiSetup();
   Serial.println("wifiSetup complete");
 
   Serial.println("Initializing Relay");
   pinMode(RELAY_1, OUTPUT);
   digitalWrite(RELAY_1, LOW);   

   Serial.println("Configuring Fauxmo");
   fauxmo.addDevice(DEVICE_NAME);
   fauxmo.onMessage(callback); 

   Serial.println("Entering Loop");
}

void loop() {
  fauxmo.handle();
}

/* ---------------------------------------------------------------------------
 Device Callback
 ----------------------------------------------------------------------------*/
void callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.print("Device "); 
  Serial.print(device_name); 
  Serial.println(" state: " + state ? "ON" : "OFF");
  
  if(device_name == DEVICE_NAME){ 
      digitalWrite(RELAY_1, state ? HIGH : LOW);
  } else {
    Serial.print("Device has no functionality or not configured"); 
  }
}
    
void wifiSetup() {
   // Set WIFI module to STA mode
   WiFi.mode(WIFI_STA);

   // Connect
   Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Wait
   while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
   }
   Serial.print(" ==> CONNECTED!" );
   Serial.println();

   // Connected!
   Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
}
