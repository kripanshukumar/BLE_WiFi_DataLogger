//===========================LIBRARIES================================
#include <WiFiClientSecure.h>

//============================OBJECTS=================================
WiFiClientSecure client;

//===========================FUNCTIONS================================
String host(const char* url) {
  String output = url;
  output = output.substring(8, output.indexOf('/', 8));
  Serial.println(output);
  return output;
}

String destination(const char* url) {
  String output = url;
  output = output.substring(output.indexOf('/', 8), output.length());
  return output;
}

String addHeader(const char* header, const char* value) {
  String output = String(header) + ": " + String(value);
  return output;
}


void init_response() {
  if (client) {
    //client.setCACert(test_root_ca);
    client.setInsecure();
  }
}
//---------------------------------------------------------------------------------

String construct_json(Linked_List<BLE_DEVICE>& obj) {
  String output = "";
  if (obj.size() > 0) {
    output += "{\"TIMESTAMP\":\"";
    output += timestamp + "\",\"DATA\":[";
    for (int i = 0; i < obj.size(); i++) {
      BLE_DEVICE tmp = obj.get(i);
      output += tmp.json();
      if (i != obj.size() - 1) {
        output += ',';
      }
    }
    output += "]}";
  }
  return output;
}

void upload_data_to_cloud() {
  if (millis() - upload_timer >= DATA_UPLOADING_INTERVAL) {
    DLINEDASH
    Serial.println("In the upload section....");
    //print_json(&construct_json(BLE_LIST)[0]);
    Serial.print("Establishing connection with: "); Serial.println(URL.c_str());
    if (WIFI_FLAG) {
      String json = construct_json(BLE_LIST);
      client.setInsecure();
      if (!client.connect("api.thingspeak.com", 443))
        Serial.println("Connection failed!");
      else {
        Serial.println("Connected to server!");
        // Make a HTTP request:
        client.print("POST " + URL + " HTTP/1.0");
        client.println("Host: " + host(URL.c_str()));
        client.println("Content-Type: application/json");
        client.println("X_API_KEY: " + KEY);
        client.println("Connection: close");
        client.println(json.c_str());
        
        //Serial.println(json.c_str());

        while (client.connected()) {
          String line = client.readStringUntil('\n');
          if (line == "\r") {
            Serial.println("\n\nheaders received:\n");
            break;
          }
        }
        // if there are incoming bytes available
        // from the server, read them and print them:
        while (client.available()) {
          char c = client.read();
          Serial.write(c);
        }

        client.stop();
      }
    }
    BLE_LIST.clear();
    upload_timer = millis();
    DLINEDASH
  }
}
