#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

void setup_server() {
  // Send web page with WiFi network input fields to client at <AP_IP>/WiFiConfig
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", (const uint8_t*) &HTML_INDEX, sizeof(HTML_INDEX));
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest * request) {
    String payload = String(WIFI_FLAG);
    payload += "http://" + String(WiFi.localIP().toString());
    request->send_P(200, "text/html", (const uint8_t*)payload.c_str(), payload.length());
  });

  // Send web page with WiFi network input fields to client at <AP_IP>/WiFiConfig
  server.on("/get_params", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("Asnc Client: "); Serial.print(request->client()->remoteIP());
    String payload = frm_ver + '\n'
                     + device_id + '\n'
                     + mac_id + '\n'
                     + CUS_DEV_ID + '\n'
                     + ssid + '\n'
                     + "********\n"
                     + URL + '\n'
                     + KEY + '\n'
                     + "********\n";
    request->send_P(200, "text/html", (const uint8_t*)payload.c_str(), payload.length());
  });

  server.on("/update_credentials", HTTP_POST, [] (AsyncWebServerRequest * request) {
    Serial.println();
    // GET Values From Web Form
    int paramNumber = request->params();
    for (int i = 0; i < paramNumber; i++) {
      AsyncWebParameter* p = request->getParam(i);
      Serial.print(p->name()); Serial.print(" : "); Serial.println(p->value());

      if (p->name() == "DEVID") {
        CUS_DEV_ID = p->value();
      }
      if (p->name() == "WFNM") {
        ssid = p->value();
      }
      if (p->name() == "WFPW") {
        if (p->value() != "********") {
          Serial.println("WIFI PASSWORD is changed!!!");
          password = p->value();
        }
      }
      if (p->name() == "WCURL") {
        URL = p->value();
      }
      if (p->name() == "WPAS") {
        KEY = p->value();
      }
      if (p->name() == "APPW") {
        if (p->value() != "********") {
          Serial.println("AP PASSWORD is changed!!!");
          AP_PSWD = p->value();
        }
      }
    }
    val_updated = true;
    WIFI_FLAG = 0;
    String address = "http://" + String(WiFi.localIP().toString());
    request->send_P(200, "text/html", (const uint8_t*)address.c_str(), address.length());
  });

  server.begin();
}
