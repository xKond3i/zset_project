/*
 * 
 * SCHOOL / FUN PROJECT
 * PROGRAMMABLE LEDS MANAGED WITH WIFI
 * SOFTWARE: KONRAD CEGLARSKI
 * HARDWARE: MATEUSZ KĘDZIERSKI
 * 
 */

// include all modules
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESPAsyncWiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include <Ticker.h>               // LEDs status

// define asynchronous server and DNS
AsyncWebServer server(80);
DNSServer dns;

// modes count
#define JLEN 1024
#define MLEN 16
#define OLEN 6
#define LEDS 300

// DATA
bool state = true;
String modes[MLEN] = {"rainbow", "solid", "breathing", "filling", "pulse"};
int mode = 0;
bool options[MLEN][OLEN] = {{true, true, true, true, true, true}, {true, true, true, true, true, true}, {true, true, true, true, true, true}, {true, true, true, true, true, true}, {true, true, true, true, true, true}};
int hue = 180;
int sat = 50;
int val = 50;
int bright = 50;
int spd = 2500;
// DATA END

Ticker ticker;

void tick() { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); }

void configModeCallback(AsyncWiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager -> getConfigPortalSSID());
  ticker.attach(0.2, tick);
}

void setup() {
  WiFi.mode(WIFI_STA);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  SPIFFS.begin();

  ticker.attach(0.6, tick);
  AsyncWiFiManager wm(&server, &dns);
  wm.setAPCallback(configModeCallback);

  // WiFi Manager connection loop
  if (!wm.autoConnect()) {
    Serial.println("Connection timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
  // When pass the loop You have been connected

  Serial.println("Connected successfully!");
  Serial.print("Network SSID: ");
  Serial.println(WiFi.SSID());
  ticker.detach();
  
  // Keep the WiFi module LED on.
  digitalWrite(LED, LOW);

  Serial.print("App IP Address: ");
  Serial.println(WiFi.localIP());

  // ROUTES
  // Root
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/index.html", String(), false);
  });
  // Stylesheets
  server.on("/fonts.css", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/fonts.css", "text/css");
  });
  server.on("/style.css", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/responsive.css", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/responsive.css", "text/css");
  });
  // JavaScript
  server.on("/src/angular.min.js", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/src/angular.min.js", "application/javascript");
  });
  server.on("/app.js", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/app.js", "application/javascript");
  });
  // Files and fonts
  server.on("/src/icon.ico", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/src/icon.ico", "image/x-icon");
  });
  server.on("/src/logo/angular-logo-white.png", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/src/logo/angular-logo-white.png", "image/png");
  });
  server.on("/src/logo/arduino-logo-white.png", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/src/logo/arduino-logo-white.png", "image/png");
  });
  server.on("/src/logo/logo-white.png", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/src/logo/logo-white.png", "image/png");
  });
  // Arduino -> JS
  server.on("/dataget", HTTP_GET, [] (AsyncWebServerRequest * request) {
    StaticJsonBuffer<JLEN> jb;
    JsonObject& obj = jb.createObject();

    obj["status"] = state;
    JsonArray& objmodes = obj.createNestedArray("modes");
    for (int i = 0; i < (sizeof(modes)/sizeof(*modes)); i++) { 
      if (modes[i] != "") { 
        objmodes.add(modes[i]); 
      } else {
        break;
      }
    }
    obj["mode"] = mode;
    JsonArray& objoptions = obj.createNestedArray("options");
    for (int i = 0; i < (sizeof(options[mode])/sizeof(*options[mode])); i++) { 
        objoptions.add(options[i]);
    }
    obj["hue"] = hue;
    obj["sat"] = sat;
    obj["val"] = val;
    obj["bright"] = bright;
    obj["speed"] = spd;

    char jmsg[JLEN];
    obj.printTo(jmsg);

    request -> send(200, "application/json", jmsg);
  });
  // JS -> Arduino
  server.on("/datapost", HTTP_POST, [](AsyncWebServerRequest * request) {}, NULL,
  [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

    String temp = (char*)data;

    StaticJsonBuffer<JLEN> jb;
    JsonObject& obj = jb.parseObject(temp);
    
    state = obj["status"];
    mode = obj["mode"];
    hue = obj["hue"];
    sat = obj["sat"];
    val = obj["val"];
    bright = obj["bright"];
    spd = obj["speed"];

    request -> send(200);
  });
  // ROUTES END

  // Start the server.
  server.begin();
}

void loop() {
  // * from here the LEDs should be controlled !
  if (state) {
    switch (mode) {
      case 0: // rainbow
        // do stuff
        break;
      case 1: // solid
        // do stuff
        break;
      case 2: // breathing
        // do stuff
        break;
      case 3: // filling
        // do stuff
        break;
      case 4: // pulse
        // do stuff
        break;
    }
  } else {
    // turn LEDs off
  }
  delay(250);
}
