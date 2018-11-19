# ESP8266-Temperature-Sensor
A stand-alone temperature sensor with basic web control panel and EEPROM configuration storage

Part of my continuing quest to make the perfect (and cheapest) Wi-Fi temperature sensor.

## Hardware:
- **ESP8266 Wi-Fi Module** I typically buy mine from [Sparkfun.com](https://www.sparkfun.com/products/13678). They can be found cheaper elsewhere, but this guarantees I'm getting decent quality components
- **Si7021 Temperature Sensor** Again, from [Sparkfun.com](https://www.sparkfun.com/products/13763). The code could be modified to support any I<sup>2</sup>C sensor, but I like this one for it's cost / accuracy
- **LD1117V33** 3.3V voltage regulator [Sparkfun.com](https://www.sparkfun.com/products/526)
- **Momentary Button**  For entering program mode [Sparkfun.com](https://www.sparkfun.com/products/97)
- **USB Male Type A** For power only [Sparkfun.com](https://www.sparkfun.com/products/437)

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
