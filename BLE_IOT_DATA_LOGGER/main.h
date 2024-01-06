int LED_COUNTER = 0;
void LED_HANDLER (void *pvParameters);

void init_pins() {
  pinMode(CONFIG_PIN, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  xTaskCreatePinnedToCore(
    LED_HANDLER
    ,  "LED_HANDLER"   // A name just for humans
    ,  2000  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  1);
}

void check_config_pin() {
  if (digitalRead(CONFIG_PIN) == 0) {
    led_state = CONFIG_MODE_ON;
    LINEDASH
    Serial.println("CONFIG PIN ASSERTED");
    Serial.println("Starting device in AP mode");
    start_AP();
  }
  else {
    for (int i = 0; i < 3; i++) {
      if (connect_wifi()) {
        break;
      }
    }
    if(WiFi.status() != WL_CONNECTED){
      start_AP();
    }
    else{
      exit_ap = true;
    }
  }
}

void LED_HANDLER (void *pvParameters) {

  while (1) {
    if ((millis() - LED_TIMER > LED_BLINK_DELAY) || (pled_state != led_state)) {
      switch (led_state) {
        case OFF:
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          delay(500);
          break;

        case CONFIG_FOUND:
          //Serial.println
          digitalWrite(LED1, HIGH);
          delay(200);
          digitalWrite(LED1, LOW);
          delay(1000);
          break;

        case WIFI_CONNECTING:
          digitalWrite(LED2, !digitalRead(LED2));
          delay(500);
          break;

        case CONFIG_MODE_ON:
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          delay(500);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          delay(500);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          delay(500);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          delay(500);
          led_state = OFF;
          break;

        case WIFI_NOT_CONNECTED:
          digitalWrite(LED1, LOW);
          delay(500);
          break;

        case WIFI_CONNECTED:
          digitalWrite(LED2, HIGH);
          delay(500);
          break;

        default:
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          delay(500);
          break;
      }
      pled_state = led_state;
      LED_TIMER = millis();
    }
  }
}
