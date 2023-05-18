#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"


#define USE_SERIAL Serial
#define DHTPIN 2
#define DHTTYPE DHT11


const char* host = "192.168.1.156";
const int httpPort = 1337;
const int buzzer = 5;

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;
HTTPClient http;

void setup() {
  system_update_cpu_freq(40000000);
  USE_SERIAL.begin(115200);

  WiFi.forceSleepWake();

  pinMode(buzzer, OUTPUT);

  WiFiManager wifi;
  
  wifi.setTimeout(120);

  if (!wifi.autoConnect("Server")) {
    Serial.print("timeout - going to sleep");

    delay(200);
    //sleep and try again
    ESP.deepSleep(10e6);
    delay(1000);
  }




  dht.begin();
  
}

void loop()
{

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  USE_SERIAL.println(t);
  USE_SERIAL.println(h);

  if (t >= Tmax || t <= Tmin)
  {
    tone(buzzer, 2000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
  }

  httpPOST(t,h);

  ESP.deepSleep(30*1000000);
  delay(1);
}


void httpPOST(float Temperature, float Humidity){
  

  if (!client.connect(host, httpPort)){
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  http.begin(client, "http://192.168.1.156:1337/api/PUT-dashboard");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");


  DynamicJsonDocument postMessage(2048);
  DynamicJsonDocument postData(2048);

  postMessage["__component"] = "list-dashboard.dashboard";
  postMessage["device_id"] = String(ESP.getChipId());
  postMessage["device_name"] = "device_1";
  postMessage["site_name"] = "site_1";
  postMessage["local_name"] = "local_1";
  postMessage["temperature"] = Temperature;
  postMessage["humidity"] = Humidity;
  postData["data"] = postMessage;
  
  String jsonBody;
  serializeJson(postData, jsonBody);

  Serial.println(jsonBody);
  http.PUT(jsonBody);

}

// #include <Arduino.h>
// #include <ArduinoJson.h>
// #include <ESP8266WiFi.h>
// #include <WiFiManager.h>
// #include <ESP8266HTTPClient.h>
// #include "DHT.h"


// #define USE_SERIAL Serial
// #define DHTPIN 2
// #define DHTTYPE DHT22


// const char* host = "mediacaris.hopto.org";
// const int httpPort = 1337;
// const int buzzer = 5;

// DHT dht(DHTPIN, DHTTYPE);

// WiFiClient client;
// HTTPClient http;

// void setup() {
//   system_update_cpu_freq(40000000);
//   USE_SERIAL.begin(115200);

//   WiFi.disconnect();

//   WiFi.forceSleepWake();

//   pinMode(buzzer, OUTPUT);

//   WiFiManager wifi;

//   wifi.setTimeout(120);

//   if (!wifi.autoConnect("Server")) {
    
//   }




//   dht.begin();
// }

// void loop() {

//   float t = dht.readTemperature();
//   float h = dht.readHumidity();

//   if (isnan(h) || isnan(t)) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }

 

//   if (t >= 37 || t <= -6) {
//      USE_SERIAL.println(t);
//      USE_SERIAL.println(h);
//     tone(buzzer, 2000);  // Send 1KHz sound signal...
//     delay(1000);         // ...for 1 sec
//     noTone(buzzer);      // Stop sound...
//     delay(1000);         // ...for 1sec
//   }

//   httpPOST(t, h);

//   // delay(120000);
//   delay(12000);
// }


// void httpPOST(float Temperature, float Humidity) {


//   if (!client.connect(host, httpPort)) {
//     Serial.println("connection failed");
//     delay(5000);
//     return;
//   }

//   http.begin(client, "http://mediacaris.hopto.org:1337/api/PUT-dashboard");
//   http.addHeader("Content-Type", "application/json");
//   http.addHeader("Accept", "application/json");


//   DynamicJsonDocument postMessage(2048);
//   DynamicJsonDocument postData(2048);

//   postMessage["__component"] = "list-dashboard.dashboard";
//   postMessage["device_id"] = String(ESP.getChipId());
//   postMessage["device_name"] = "device_3";
//   postMessage["site_name"] = "site_3";
//   postMessage["local_name"] = "local_3";
//   postMessage["temperature"] = Temperature;
//   postMessage["humidity"] = Humidity;
//   postData["data"] = postMessage;

//   String jsonBody;
//   serializeJson(postData, jsonBody);

//   Serial.println(jsonBody);
//   http.PUT(jsonBody);
// }

////////////////////////////////////////////////////////////////////

// // #include <Arduino.h>
// #include <ArduinoJson.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>
// #include "DHT.h"
// #include "wifipassword.h"


// #define USE_SERIAL Serial
// #define DHTPIN 2
// #define DHTTYPE DHT22




// const char* ssid = "LINK-HIMMI-1";
// const char* pass = WIFI_PASSWORD;

// const char* host = "192.168.1.110";
// const int httpPort = 1337;
// const int buzzer = 9;

// DHT dht(DHTPIN, DHTTYPE);


// void setup() {
//   USE_SERIAL.begin(115200);

//   int numberOfNetworks = WiFi.scanNetworks();

//   for(int i =0; i<numberOfNetworks; i++){

//     USE_SERIAL.print("Network name: ");
//     USE_SERIAL.println(WiFi.SSID(i));
//     USE_SERIAL.print("Signal strength: ");
//     USE_SERIAL.println(WiFi.RSSI(i));
//     USE_SERIAL.println("-----------------------");

//   }


//   WiFi.begin(ssid, pass);

//   while (WiFi.status() != WL_CONNECTED) {
//     USE_SERIAL.println(".");
//     delay(500);
//   }

//   USE_SERIAL.println("");
//   USE_SERIAL.println("Wi-Fi connected");
//   USE_SERIAL.print("IP Address : ");
//   USE_SERIAL.println(WiFi.localIP());

//   pinMode(buzzer, OUTPUT);

//   dht.begin();
// }

// void loop() {
//   delay(15000);

//   float t = dht.readTemperature();

//   if (isnan(t)) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }

//   Serial.print(F("%  Temperature: "));
//   Serial.print(t);
//   Serial.print(F("°C "));

//   // if (t >= 30 || t <= -6)
//   // {
//   //   tone(buzzer, 2000); // Send 1KHz sound signal...
//   //   delay(1000);        // ...for 1 sec
//   //   noTone(buzzer);     // Stop sound...
//   //   delay(1000);        // ...for 1sec
//   // }

//   httpPOST(t);

// }

// // void httpGet(){
// //   WiFiClient client;
// //   HTTPClient http;

// //   char *url = "http://192.168.1.110:1337/api/temperatures/1";

// //   http.begin(client, url);
// //   int resCode = http.GET();

// //   Serial.println(resCode);

// //   String res = http.getString();
// //   Serial.println(res);

// //   parserMessage(res);

// //   http.end();
// // }


// void httpPOST(float Temperature){
//   WiFiClient client;
//   HTTPClient http;

//   if (!client.connect(host, httpPort)){
//     Serial.println("connection failed");
//     delay(5000);
//     return;
//   }

//   http.begin(client, "http://192.168.1.110:1337/api/temperatures");
//   http.addHeader("Content-Type", "application/json");
//   http.addHeader("Accept", "application/json");


//   DynamicJsonDocument postMessage(2048);
//   postMessage["data"]["Name"] = "test";
//   postMessage["data"]["TemperatureC"] = Temperature;

//   String jsonBody;
//   serializeJson(postMessage, jsonBody);

//   USE_SERIAL.println(jsonBody);

//   int resCode = http.POST(jsonBody);

//   USE_SERIAL.println(resCode);

//   String res = http.getString();
//   USE_SERIAL.println(res);

//   // parserMessage(res);

//   http.end();
// }


// // void parserMessage(String res){
// //   DynamicJsonDocument doc(2048);
// //   deserializeJson(doc, res);
// //   String _data = doc["data"]["attributes"]["TemperatureC"];
// //   USE_SERIAL.println(_data);
// // }
