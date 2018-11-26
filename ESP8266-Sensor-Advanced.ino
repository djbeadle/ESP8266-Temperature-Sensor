#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// Custom header file with prototypes for functions in other files
#include "helpers.h"

// My custom library for feeding to InfluxDB
#include <ESP8266InfluxClient.h>

// For the Si7021 temperature sensor:
#include <Wire.h>
#include "SparkFun_Si7021_Breakout_Library.h"

/* Repurpose the RX & TX pins as extra GPIO */
int breadboardLED = 1;
int breadboardButton = 3;
int buttonState = 0;

// Wi-Fi credentials, max of 50 characters each
char ssid[50];
char pass[50];

// InfluxDB settings, max of 25 characters each
char influx_address[25];
char influx_port[25];
char database_name[25];
char measurement_name[25];
char host_name[25];
char region[25];

// How often should the sensor update (in milliseconds)
int update_interval = 1000;

// Set the details of your InfluxDB server here
const char *host = "192.168.1.218"; // ip or domain name
uint16_t port = 8086;               // Set the port your server is listening for data on. Default is 8086

// Measurement Structs
Measurement temp_config;
Measurement humidity_config;

// Initialize the Si7021 Temp & Humidity sensor
Weather sensor;

// Initialize Wi-Fi connection
ESP8266WiFiMulti WiFiMulti;

// Create the web server object used for setting the configuration variables
ESP8266WebServer server(80);

// Declare the database object. Initalize it in setup once we've loaded settings
// from the EEPROM
ESP8266InfluxClient influx_server;

// State 0 = EEPROM is clear, either because this is the first run or it was reset
// State 1 = We've got Wi-Fi credentials in the EEPROM
int state = -1;

void setup()
{
  delay(1000);

  // Initialize our settings with null values
  memset(ssid, 0, sizeof(ssid));
  memset(pass, 0, sizeof(pass));
  memset(influx_address, 0, sizeof(influx_address));
  memset(influx_port, 0, sizeof(influx_port));
  memset(database_name, 0, sizeof(database_name));
  memset(measurement_name, 0, sizeof(measurement_name));
  memset(host_name, 0, sizeof(host_name));
  memset(region, 0, sizeof(region));

  // Store Wi-Fi and database settings the ESP8266's flash storage
  // (Unlike on Arduinos it isn't REAL EEPROM , it just pretends to be)
  // 512 bytes are enough for our purposes, but up to 4096 is available.
  EEPROM.begin(512);

  // Repurpose RX & TX as GPIO
  pinMode(breadboardLED, OUTPUT);
  digitalWrite(breadboardLED, LOW);
  pinMode(breadboardButton, INPUT_PULLUP);

  // Check if the reset button is being pressed
  buttonState = digitalRead(breadboardButton);
  if (buttonState == LOW)
  {
    clearEEPROM();

    // State 0 means Wi-Fi config mode
    state = 0;
  }

  // Start Wi-Fi Network
  if (state == 0)
  {
    WiFi.softAP("SensorSetup");
    IPAddress myIP = WiFi.softAPIP();

    server.on("/", handleRoot);
    server.on("/info", handleInfo);
    server.begin();
  }

  // Connect to Wi-Fi Network with settings in EEPROM
  else
  {
    state = 1;

    // Read the configuration settings from the EEPROM
    readEEPROM();

    influx_server = ESP8266InfluxClient(host, port);

    // Fire up the sensor
    sensor.begin();

    // Connect to Wi-Fi network
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, pass);

    while (WiFiMulti.run() != WL_CONNECTED)
    {
      delay(500);
    }

    load_config();

    // Intalize the structs with our InfluxDB settings
    temp_config = {
      database_name,
      measurement_name,
      "fahrenheit",
      host_name,
      region
    }

    humidity_config = {
      database_name,
      measurement_name,
      "relative_humidity",
      host_name,
      region
    }
  }
}

void loop()
{
  if (state == 0)
  {
    while (true)
    {
      server.handleClient();
    }
  }
  else if (state == 1)
  {
    while (true)
    {
      influx_server.update(temp_config, read_temperature());
      influx_server.update(humidity_config, read_humidity());

      // Pause for update_interval
      delay(update_interval);
    }
  }
}
