#ifndef HEADER_HELPERFUNC
  #define HEADER_HELPERFUNC

    void clearEEPROM();
    void saveConfig(String, String);
    void readEEPROM();

    float read_temperature();
    float read_humidity();

    void handleRoot();
    void returnFail();
    void handleSubmit();
    void returnOK();
    void handleNotFound();

#endif
