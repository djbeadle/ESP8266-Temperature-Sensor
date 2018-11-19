const char INDEX_HTML[] =
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>ESP8266 Web Form Demo</title>"
  "<style>"
  "\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
  "</style>"
  "</head>"
  "<body>"
  "<h1>ESP8266 Web Form Demo</h1>"
  "<FORM action=\"/\" method=\"post\">"
  "<div>"
  "<label for=\"ssid\">Wifi SSID:</label>"
  "<input type=\"text\" id=\"ssid\" name=\"ssid\">"
  "</div>"
  "<div>"
  "<label for=\"pass\">Wifi Password:</label>"
  "<input type=\"text\" id=\"pass\" name=\"pass\">"
  "</div>"
  "<div class=\"button\">"
  "<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
  "</div>"
  "</form>"
  "</body>"
  "</html>";

void handleRoot()
{
  if (server.hasArg("ssid")) {
    handleSubmit();
  }
  else {
    server.send(200, "text/html", INDEX_HTML);
  }
}

void handleInfo()
{
  readEEPROM();
  
  server.send(200, "text/html", "ssid: " + String(ssid) + " pass: " + String(pass));
}


void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handleSubmit()
{
  if (!server.hasArg("ssid")) return returnFail("BAD ARGS");
  
  saveConfig(server.arg("ssid"), server.arg("pass"));
}

void returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
