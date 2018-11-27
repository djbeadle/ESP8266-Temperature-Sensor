#ifndef HEADER_HELPERFUNC
  #define HEADER_HELPERFUNC

    void clear_config();
    void save_config(String, String, String, String, String, String, String, String);
    void load_config();
    void writeSetting(int, String);
    void readSetting(int, int, char*);
    void readEEPROM();

    float read_temperature();
    float read_humidity();

    void handleRoot();
    void returnFail();
    void handleSubmit();
    void returnOK();
    void handleNotFound();

#endif
