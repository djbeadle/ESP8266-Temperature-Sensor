void clearEEPROM()
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

void saveConfig(String raw_ssid, String raw_pass)
{
  clearEEPROM();

  // Copy the raw values into our char* arrays
  raw_ssid.toCharArray(ssid, 50);
  raw_pass.toCharArray(pass, 50);

  // Iterate over our char* arrays and store them in EEPROM for persistant storage
  for(int i = 0; i < raw_ssid.length(); i++)
  {
    EEPROM.write(i, ssid[i]);
    EEPROM.commit();
    
    digitalWrite(breadboardLED, HIGH);
    delay(250);
    digitalWrite(breadboardLED, LOW);
    delay(250);
  }

  for(int i = 0; i < raw_pass.length(); i++)
  {
    EEPROM.write(50 + i, pass[i]);

    digitalWrite(breadboardLED, HIGH);
    delay(250);
    digitalWrite(breadboardLED, LOW);
    delay(250);
  }

  EEPROM.commit();
  
  delay(100);
}

void readEEPROM()
{
  for(int address = 0 ; address < 50; address++)
  {
    // read a byte from the current address of the EEPROM
    char value = EEPROM.read(address);

    // if we've hit the end of the string
    if(value == '\0')
    {
     digitalWrite(breadboardLED, HIGH);
     delay(1000);
     digitalWrite(breadboardLED, LOW);
     delay(250);
      ssid[address] = '\0';
      break;
    }

    digitalWrite(breadboardLED, HIGH);
    delay(250);
    digitalWrite(breadboardLED, LOW);
    delay(250);

    ssid[address] = value;
  }

  for (int address = 0; address < 50; address++ )
  {
    // read a byte from the current address of the EEPROM
    char value = EEPROM.read(50 + address);

    // if we've hit the end of the string
    if(value == '\0')
    {
      pass[address] = '\0';
      break;
    }

    pass[address] = value;
  }
}
