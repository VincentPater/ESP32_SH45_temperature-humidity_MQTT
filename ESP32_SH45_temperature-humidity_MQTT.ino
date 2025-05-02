#include "settings.h"
#include <Adafruit_SHT4x.h>
#include <PubSubClient.h>
#include <WiFi.h>



/***********************************
            Create various Objects
 ***********************************/
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
WiFiClient espClient;
PubSubClient mqttClient(espClient);


/***********************************
            Global Variables
 ***********************************/
uint64_t LAST_RECONNECT_ATTEMPT = 0;
uint64_t LAST_FULL_UPDATE_SENT = 0;

char WIFI_SSID[32] = WIFI_SSID_STRING;
char WIFI_PASS[32] = WIFI_PASSWORD;

char MQTT_HOST[64] = MQTT_SERVER_IP;
char MQTT_PORT[6] = MQTT_PORT_NUM;
char MQTT_USER[32] = MQTT_USER_NAME;
char MQTT_PASS[32] = MQTT_PASSWORD;


uint64_t currentTime = 0; // Variable for time keeping (microseconds)



/***********************************
            Main Setup
 ***********************************/
void setup() {

  // Setup Serial
  Serial.begin(115200);


  // Setup the Adafruit SHT 45
  if (!sht4.begin()) {
    #ifdef DEBUG
    Serial.println("Couldn't find SHT4x");
    #endif
    while (true){
      delay(1);
    }
  }

  #ifdef DEBUG
  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);
  #endif

  // Set precision to high
  sht4.setPrecision(SHT4X_HIGH_PRECISION);

  // Humidity should stay at normal levels, say <80% so no heater should be needed
  sht4.setHeater(SHT4X_NO_HEATER);



  // Setup WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {

#ifdef DEBUG
    Serial.println("Connection Failed! Rebooting...");
#endif
    delay(5000);
    ESP.restart();
  }

  delay(3000);


  // Setup MQTT
  mqttClient.setServer(MQTT_HOST, atoi(MQTT_PORT));
  mqttClient.setSocketTimeout(MQTT_TIMEOUT);
  mqttClient.setKeepAlive(MQTT_TIMEOUT);


#ifdef DEBUG
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif

}




/***********************************
            Main Loop
 ***********************************/
void loop() {

  // Time keeping
  currentTime = esp_timer_get_time(); //time in microseconds


  // Check WiFi Status
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      #ifdef DEBUG
        Serial.println("Connection Failed! Rebooting...");
      #endif
      delay(5000);
      ESP.restart();
    }
  }

  // Check if new MQTT messages are available (does not send data)
  if (!mqttClient.connected()) {
    if (currentTime - LAST_RECONNECT_ATTEMPT > 5000000) {
      LAST_RECONNECT_ATTEMPT = currentTime;

      if (!mqttReconnect()) {
        #ifdef DEBUG
            Serial.println("Connection to MQTT Failed! Rebooting...");
        #endif
        delay(5000);
        ESP.restart();
      } else {
        LAST_RECONNECT_ATTEMPT = 0;
      }
    }
  } else {
    mqttClient.loop();
  }


  // Read SH45 sensor data and Publish it to the MQTT broker
  if (currentTime - LAST_FULL_UPDATE_SENT >= UPDATE_FULL_INTERVAL){
    // Reading sensors
    sensors_event_t humidity, temp;
    sht4.getEvent(&humidity, &temp);

    #ifdef DEBUG
    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degrees C");

    Serial.print("Humidity: ");
    Serial.print(humidity.relative_humidity);
    Serial.println("% rH");
    #endif

    sendDataToBroker(temp.temperature, humidity.relative_humidity);

    LAST_FULL_UPDATE_SENT = currentTime;
  }
}
