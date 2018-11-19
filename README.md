# ESP8266-Temperature-Sensor
A stand-alone temperature sensor with basic web control panel and EEPROM configuration storage

Part of my continuing quest to make the perfect (and cheapest) Wi-Fi temperature sensor...

## Usage:
1. Flash the software
2. Enter configuration mode by pressing and holding the button while powering on
3. Join the sensor's ad-hoc Wi-Fi network, "SensorSetup"
4. Navigate to the sensor's IP address, 192.168.4.1
5. Enter the SSID and password of the network you want the sensor to join and press "submit"
6. Check that you entered the right thing at 192.168.4.1/info
7. Power cycle the sensor (don't hold the button this time)
8. Watch as data starts flowing in to your InfluxDB!

## To-Do:
- Security!
- Configure InfluxDB settings in control panel
- Get PCBs made!
