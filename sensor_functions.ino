float read_temperature()
{
  // Measure Temperature from the HTU21D or Si7021
  float tempf = 0;
  tempf = sensor.getTempF();

  return tempf;
}

float read_humidity()
{
    float humidity = 0;
    
    // Measure Relative Humidity from the HTU21D or Si7021
    humidity = sensor.getRH();

    return humidity / 100;
}