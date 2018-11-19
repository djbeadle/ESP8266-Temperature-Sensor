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

// How often should the sensor update (in milliseconds)
int update_interval = 1000;

// Set the details of your InfluxDB server here
const char *host = "192.168.1.218"; // ip or domain name
uint16_t port = 8086;               // Set the port your server is listening for data on. Default is 8086

// Measurement Settings
// These are stored in a struct. If you're logging multiple measurements,
// create multiple structs.
Measurement basement_si7021_temp = {
    "FortyTwoFairchild",   // Database name
    "DanielsRoom",          // Measurement name
    "fahrenheit",          // Field key
    "esp8266-DanielsRoom1", // Host name (A name or unique id for the sensor)
    "us-east"              // Region
};
Measurement basement_si7021_humidity = {
    "FortyTwoFairchild",   // Database name
    "DanielsRoom",          // Measurement name
    "relative_humidity",   // Field key
    "esp8266-DanielsRoom1", // Host name (A name or unique id for the sensor)
    "us-east"              // Region
};

// Initialize the Si7021 Temp & Humidity sensor
Weather sensor;

// For connecting to the Wi-Fi Network
ESP8266WiFiMulti WiFiMulti;

// Create the database object
ESP8266InfluxClient influx_server = ESP8266InfluxClient(host, port);

// Create the web server object used for setting the configuration variables
ESP8266WebServer server(80);


// State 0 = EEPROM is clear, either because this is the first run or it was reset
// State 1 = We've got Wi-Fi credentials in the EEPROM
int state = -1;

void setup()
{
  delay(1000);

  memset(ssid, 0, sizeof(ssid));
  memset(pass, 0, sizeof(pass));

  // Store Wi-Fi and database settings the ESP8266's flash storage
  // (Unlike on Arduinos it isn't REAL EEprom, it just pretends to be)
  // 512 bytes are enough for our purposes.
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

    // Read the WiFi settings from the EEPROM
    readEEPROM();

    // Fire up the sensor
    sensor.begin();

    // Connect to Wi-Fi network
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, pass);

    while (WiFiMulti.run() != WL_CONNECTED)
    {
      delay(500);
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
      influx_server.update(basement_si7021_temp, read_temperature());
      influx_server.update(basement_si7021_humidity, read_humidity());

      // Pause for update_interval
      delay(update_interval);
    }
  }
}
