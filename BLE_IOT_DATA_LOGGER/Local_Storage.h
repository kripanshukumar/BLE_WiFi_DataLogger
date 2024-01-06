#include "FS.h"
#include "SPIFFS.h"

#define FORMAT_SPIFFS_IF_FAILED true

const char* filename = "/data.txt";

//===============================Function Declarations======================================

void checkFiles();
void load_vars();

//------------------------------------------------------------------------------------------

void storage_init() {
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    storageMount = false;
    return;
  } else {

    Serial.println("SPIFFS Mounted..");
    storageMount = true;
  }
  checkFiles();
  load_vars();
}

//------------------------------------------------------------------------------------------

void checkFiles() {
  if (storageMount == true) {
    Serial.println("Checking for the required filed in the flash:");
    File file = SPIFFS.open(filename, "r");
    if (file) {
      Serial.print("Found: "); Serial.println(filename);
      led_state = CONFIG_FOUND;
      /*
      Serial.print("Contents of the file: ");
      while (file.available()) {
        Serial.write(file.read());
      }
      Serial.println("********");
      */
      file.close();
    }
    else {
      Serial.print("Creating: "); Serial.println(filename);
      file = SPIFFS.open(filename, "w");
      if (file) {
        Serial.println("File created successfully...");
        file.close();
      }
      else {
        Serial.println("Unable to create the file...");
      }
    }
  }
}

void update_file() {
  String str = CUS_DEV_ID + '\n'
               + ssid + '\n'
               + password + '\n'
               + URL + '\n'
               + KEY + '\n'
               + AP_PSWD + '\n';
  File file = SPIFFS.open(filename, "w");
  if (file) {
    Serial.println("File created successfully...");
    file.print(str);
    file.close();
  }
  else {
    Serial.println("Unable to create the file...");
  }
}

void load_vars() {
  Serial.println("Loading variable values from the Flash");
  String temp = "";
  File file = SPIFFS.open(filename, "r");
  if (file) {
    Serial.print("Found: "); Serial.println(filename);
    while (file.available()) {
      temp.concat(char(file.read()));
    }
    int arr[10] = {0};
    arr[0] = temp.indexOf('\n');
    for (int i = 1; i < 6; i++) {
      arr[i] = temp.indexOf('\n', arr[i-1] + 1);
    }
    CUS_DEV_ID = temp.substring(0,arr[0]);
    ssid = temp.substring(arr[0] + 1,arr[1]);
    password = temp.substring(arr[1] + 1,arr[2]);
    URL = temp.substring(arr[2] + 1,arr[3]);
    KEY = temp.substring(arr[3] + 1,arr[4]);
    AP_PSWD = temp.substring(arr[4] + 1,arr[5]);

    Serial.println("\n============================================================");
    Serial.print("Custom Device ID======>");Serial.println(CUS_DEV_ID);
    Serial.print("WIFI SSID=============>");Serial.println(ssid);
    Serial.print("WIFI PSWD=============>");Serial.println(password);
    Serial.print("URL===================>");Serial.println(URL);
    Serial.print("KEY===================>");Serial.println(KEY);
    Serial.print("AP PSWD===============>");Serial.println(AP_PSWD);
    Serial.println();
    file.close();
  }
  else {
    Serial.println("Failed to open the file");
  }
}
