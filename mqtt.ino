# include <string.h>

void sendMQTTMessage(const char *topic, char *payload) {
  bool result = mqttClient.publish(topic, payload, false);
}

bool mqttReconnect() {
  int MQTT_RECONNECT_RETRIES = 0;

  while (!mqttClient.connected() && MQTT_RECONNECT_RETRIES < MQTT_MAX_RECONNECT_TRIES) {
    MQTT_RECONNECT_RETRIES++;

    #ifdef STATUSLED
    rgbLedWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red
    #endif
    #ifdef DEBUG
      Serial.println("Connection Lost to MQTT server");
    #endif

    if (mqttClient.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {
      // char *message = new char[16 + strlen(HOSTNAME) + 1];
      // strcpy(message, "Sensor Alive: ");
      // strcat(message, HOSTNAME);
      // mqttClient.publish("hass/status", message);

    } else {
      delay(1000);
    }
  }

  if (MQTT_RECONNECT_RETRIES >= MQTT_MAX_RECONNECT_TRIES) {
    return false;
  }

  // Connection is re-established
  #ifdef DEBUG
  Serial.println("Reconnected to MQTT server");
  #endif
  #ifdef STATUSLED
  rgbLedWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0);  // Green
  #endif

  return true;
}

void sendMetric(String name, char *metric) {
  char output[20];
  memset(output, 0, 20); //need to reset to 0 or somehow eol '/0' is not present.
  memcpy(output, metric, strlen(metric));

  String topic = String(MQTT_ROOT_TOPIC) + "/" + name;
  sendMQTTMessage(topic.c_str(), output);
}



void sendDataToBroker(float temperatureValue, float humidityValue){
  char tempCharArray[10];
  memset(tempCharArray, 0, 10);
  dtostrf(temperatureValue, 4, 2, tempCharArray);
  sendMetric("temperature", tempCharArray);

  char rhCharArray[10];
  memset(rhCharArray, 0, 10);
  dtostrf(humidityValue, 4, 2, rhCharArray);
  sendMetric("humidity", rhCharArray);

}
