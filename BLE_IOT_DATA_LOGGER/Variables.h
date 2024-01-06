#include <ArduinoJson.h>

//===================================GLOBAL MACROS==============================================

#define LINEBREAK Serial.println(FPSTR(""))       // Blank line break macro allows easy line breaks by a clear command
#define DLINEDASH Serial.println(F("\n============================================================================="));
#define LINEDASH Serial.println(FPSTR("\n-----------------------------------------------------------------------------"));

#define BLE_DATA_ACCUMULATION_INTERVAL  2000                 // Minimum vlaue is 1000 ms
#define DATA_UPLOADING_INTERVAL 5000                        // Minimum value is 10000ms
#define LED2 16
#define LED1 17
#define CONFIG_PIN 25
#define LED_BLINK_DELAY 500
//======================================ENUM Variables==========================================
enum LED_STATE{OFF, CONFIG_FOUND, WIFI_CONNECTING, CONFIG_MODE_ON, WIFI_NOT_CONNECTED, WIFI_CONNECTED} led_state ,pled_state;
//=====================================Global Variables=========================================
String frm_ver = "XPR_16.06.22";
String mac_id = "";
String device_id = "";
String CUS_DEV_ID = "";

String AP_ID = "";
String AP_PSWD = "";

String ssid     = "Alpha 4G";
String password = "kissmyass";

String URL = "";
String KEY = "";

boolean storageMount = false;
boolean val_updated = false;
boolean exit_ap = true;

uint32_t timer = 0;
uint32_t timer2 = 0;
uint32_t LED_TIMER = 0;

const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

boolean WIFI_FLAG = 0;
boolean json_clear_flag = 0;

String timestamp = "XX_XX_XX_XX_XX_XX";
