#include <WiFi.h>

#include "time.h"

const char* ntpServer = "pool.ntp.org";
uint32_t upload_timer = 0;

//====================================Function Declaration=================================
void load_params();
void start_AP();
//=========================================================================================

boolean connect_wifi() {
  if(ssid.length() > 0){
    Serial.print("\nConnecting to: ");Serial.println(ssid);
    int itr = 0;
    WiFi.begin(ssid.c_str(),password.c_str());
    while(WiFi.status() != WL_CONNECTED){
      itr++;
      delay(500);
      Serial.print('.');
      if(itr > 12){
        break;
      }
    }
    if(WiFi.status() == WL_CONNECTED){
      WIFI_FLAG = 1;
      led_state = WIFI_CONNECTED;
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      return true;
    }
    else{
      Serial.println("\nWiFi Connection Failed...");
      WIFI_FLAG = 0;
      led_state = WIFI_NOT_CONNECTED;
      return false;
    }
  }
  else{
    Serial.println("\nSSID not configured.....");
    return false;
  }
}

void start_AP() {
  Serial.println();
  Serial.println("Starting Access Point:");
  Serial.print("AP SSID: "); Serial.println(AP_ID);
  WiFi.softAP(AP_ID.c_str(), AP_PSWD.c_str());
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  exit_ap = false;
}

void config_params() {
  mac_id = WiFi.macAddress();
  mac_id.replace(":", "");
  device_id = "XP_" + mac_id;
  AP_ID = "XPT_" + mac_id.substring(mac_id.length() - 6,mac_id.length());
}

void exit_access_point() {
  if (val_updated == true) {
    Serial.println("\n\nNew Value found............\n\n");
    connect_wifi();
    update_file();
    if (exit_ap == true) {
      WiFi.softAPdisconnect(true);
    }
    val_updated = false;
  }
}

void init_time() {
  if(WIFI_FLAG){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  }
}

void update_time(){
  if(WIFI_FLAG){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  timestamp = String(timeinfo.tm_mday)+'_' + String(timeinfo.tm_mon)+'_' + String(timeinfo.tm_year-100)+'_' + 
              String(timeinfo.tm_hour)+'_' + String(timeinfo.tm_min)+'_' + String(timeinfo.tm_sec);
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  //sprintf(timestamp,"%B %d %Y %H:%M:%S",&timeinfo);
  //Serial.print("\nCurrent time: ");Serial.println(timestamp);
  }
}
