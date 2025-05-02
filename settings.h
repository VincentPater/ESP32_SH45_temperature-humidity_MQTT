// For logs to Serial Monitor
#define DEBUG
#define STATUSLED
#define RGB_BRIGHTNESS 25

// WiFi Settings
#define WIFI_SSID_STRING "ssid"
#define WIFI_PASSWORD "password"

// Serial configuration
#define BAUD_RATE 115200


// MQTT setup
#define HOSTNAME "hostname"
#define MQTT_MAX_RECONNECT_TRIES 10
#define MQTT_ROOT_TOPIC "root/location/climate"
#define MQTT_TIMEOUT 120

#define MQTT_SERVER_IP "ip adress"
#define MQTT_PORT_NUM "1883"
#define MQTT_USER_NAME "mqtt-user"
#define MQTT_PASSWORD "mqtt-password"

// Time is in microseconds!
#define UPDATE_FULL_INTERVAL 30000000  // 30 seconds