#include "Variables.h"
#include "Local_Storage.h"
#include "HTML.h"
#include "Linked_List.h"
#include "BLE_DEVICE.h"
#include "BLE_SCAN.h"
#include "Wireless.h"
#include "WebServer.h"
#include "httpRequest.h"
#include "main.h"



void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("\n\n\n==================================================================================");
  storage_init();
  config_params();
  init_pins();
  check_config_pin();
  setup_server();
  BLE_INIT();
  init_time();
  init_response();


  xTaskCreatePinnedToCore(
    BLE_SCAN
    ,  "BLE_HANDLER"   // A name just for humans
    ,  20000  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - timer > 100) {
    exit_access_point();
    timer = millis();
  }

  if (millis() - timer2 > 1000) {
    //Serial.print("\nFree Heap: "); Serial.println(ESP.getFreeHeap());
    update_time();
    upload_data_to_cloud();
    timer2 = millis();
  }
  delay(10);

}
