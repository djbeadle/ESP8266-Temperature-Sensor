/**
  Erase the entire EEPROM
*/
void clear_config()
{
  // Blink twice to acknowledge
  digitalWrite(breadboardLED, HIGH);
  delay(250);
  digitalWrite(breadboardLED, LOW);
  delay(250);
  digitalWrite(breadboardLED, HIGH);
  delay(250);
  digitalWrite(breadboardLED, LOW);
  delay(250);

  // Clear the EEPROM values
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++)
  {
    EEPROM.write(i, '\0');
  }
  EEPROM.commit();
}

/**
  Save the configuration variables in the EEPROM
*/
void save_config(String raw_ssid, String raw_pass, String influx_address, String influx_port, String database_name, String measurement_name, String host_name, String region, int interval)
{
  clear_config();

  writeSetting(0, raw_ssid);
  writeSetting(50, raw_pass);
  writeSetting(100, influx_address);
  writeSetting(125, influx_port);
  writeSetting(150, database_name);
  writeSetting(175, measurement_name);
  writeSetting(200, host_name);
  writeSetting(225, region);
  EEPROM.put(250, interval);

  EEPROM.commit();
  delay(100);
}

/**
  Load the configuration variables from the EEPROM
*/
void load_config()
{
  readSetting(0, 50, ssid);
  readSetting(50, 50, pass);
  readSetting(100, 25, influx_address);
  readSetting(125, 25, influx_port);
  readSetting(150, 25, database_name);
  readSetting(175, 25, measurement_name);
  readSetting(200, 25, host_name);
  readSetting(225, 25, region);
  EEPROM.get(250, update_interval);
}

/**
  Write a String to a specific location in the EEPROM

  @param start_address The byte to start writing at
  @param value The String to save in the EEPROM
*/
void writeSetting(int start_address, String value)
{
  for (int i = 0; i < value.length(); i++)
  {
    EEPROM.write(start_address + i, value.charAt(i));
  }

  EEPROM.commit();
  delay(100);
}

/**
  Read a String from the EEPROM and store it in a char array

  @param base_address The starting address of the string
  @param max_length The maximum possible length of the string
  @param output The char array to put the output in
*/
void readSetting(int base_address, int max_length, char* output)
{
  for (int i = 0; i < max_length; i++)
  {
    // read a byte from the current address of the EEPROM
    char value = EEPROM.read(base_address + i);

    // if we've hit the end of the string
    if (value == '\0')
    {
      output[i] = '\0';
      break;
    }

    output[i] = value;
  }
}
