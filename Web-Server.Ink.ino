#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Wifi Network Name"
#define STAPSK  "Wifi Network Password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Custom IPAddress for Arduino WebServer
IPAddress ip(198,168,100,24);
IPAddress gateway(198,200,11,8);
IPAddress subnet(198,40,46,5);

// Custom Port = 80 on which the server runs
ESP8266WebServer server(80);

const int led = 13;

struct gyrometer{
 double azimuth;
 double pitch;
 double roll;  
};

struct accelometer{
 double x;
 double y;
 double z;
};

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/post", [](){
    server.send(200, "text/plain", "this works as well");
    updateValue();
  });

  server.on("/on", []() {
    server.send(200, "text/plain", "this works as well");
    ledOn();
  });
  server.on("/off", []() {
    server.send(200, "text/plain", "this works as well");
    ledOff();
  });
 
  server.onNotFound(handleNotFound);

  pinMode(LED_BUILTIN, OUTPUT);  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}

void ledOn(void) {
   digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
}
void ledOff(void) {  
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH         
}

void updateValue(){
  struct accelometer a;
  struct gyrometer g;
  a.x = server.arg("posx").toDouble();
  a.y = server.arg("posy").toDouble();
  a.z = server.arg("posz").toDouble();
  g.azimuth = server.arg("azimuth").toDouble();
  g.pitch = server.arg("pitch").toDouble();
  g.roll = server.arg("roll").toDouble();
  
  Serial.println(a.x);
  Serial.println(a.y);
  Serial.println(a.z);
  Serial.println(g.azimuth);
  Serial.println(g.pitch);
  Serial.println(g.roll);
}
