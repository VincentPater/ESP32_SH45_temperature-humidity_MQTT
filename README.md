# ESP32_SH45_temperature-humidity_MQTT
Uses an ESP32 C6 and an Adafruit SH45 to measure and send temperature and humidity values to a MQTT broker


# Circuit Diagram
Connections:
| ESP32 | SHT45 |
|-------|-------|
| 3.3 V | VIN |
| -  | 3Vo |
| G | GND|
| GPIO 22 | SCL |
| GPIO 23 | SDA |

I've tried to get other pins working, but it appears that pins 22 and 23 are baked-in somewhere.

![alt text](https://github.com/VincentPater/ESP32_SH45_temperature-humidity_MQTT/blob/main/assets/ESP32C6%20SHT45%20pinout.PNG "Circuit diagram")


# Libraries Needed
- Adafruit_SHT4x
- PubSubClient
- WiFi
- string.h (installed by default)

These are all availible from the build-in library explorer of the Arduino IDE

# YAML Config for Home Assistent
```
mqtt:
  sensor:
    - name: My Temperature Sensor
      unique_id: "myid_temperature"
      state_topic: "root/location/climate/temperature"
      device_class: temperature
      unit_of_measurement: "°C"
      value_template: "{{ value|float }}"
      
    - name: Vincent Schuur  Humidity 
      unique_id: "myid_humidity"
      state_topic: "root/location/climate//humidity"
      device_class: humidity
      unit_of_measurement: "%"
      value_template: "{{ value|float }}"
```

# Contributions
Some of this code was taken from my [ESP32_DSMR_MQTT](https://github.com/VincentPater/ESP32_DSMR_MQTT) project which is itself a fork of [bartwo's repo](https://github.com/bartwo/esp32_p1meter/tree/master).
