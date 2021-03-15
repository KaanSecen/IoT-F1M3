/*  
 *   Mediacollege Duurzaam huis BO
 *   Les 4
 *   Connectie met internet  
 *   Webserver
 *   Leds On Off
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); // maak een instantie van de webserver op poort 80

//LED pins
uint8_t Led1 = D3; //LED indicator 
uint8_t Led2 = D5; //LED indicator
uint8_t Led3 = D6; //LED indicator

bool led1Status = false; //toggle led on/off
bool led2Status = false; //toggle led on/off
bool led3Status = false; //toggle led on/off

// your data
String studentName = "Kaan Secen student nummer 24114"; // jouw student nummer + naam

const char* ssid = "Medialab";
const char* password = "Mediacollege";

void wifiConnect(){
  WiFi.begin(ssid, password); // connect to wifi ssid with password 
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000); // wait 1 sec
      Serial.print("Connecting..");
      }
  //if connected
  Serial.print("\r\nConnected: SSID: "); 
  Serial.print(ssid);
  Serial.print("    IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("HTTP server started");
 }


void handleRoot() {
   server.send(200, "text/html", "<h3>Groetjes van Duurzaam Huis! mijn naam is: " 
   +  studentName + "</h3>" );
   }

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(){
  pinMode(Led1, OUTPUT);     // Initialize the pin as an output
  pinMode(Led2, OUTPUT);     // Initialize the pin as an output
  pinMode(Led3, OUTPUT);     // Initialize the pin as an output
 
  Serial.begin(115200);      //  Start de serial monitor

/*
 * Webserver functionaliteit
 *  server.on("/"
 *  server.on("/easter_egg"
 *  server.on("/led1"
 *  server.onNotFound
 */
  server.on("/", handleRoot);

  server.on("/easter_egg", [](){
        server.send(200, "text/plain", "hoera dit werkt");
    });

  server.on("/led1","/led2", "/led3", [](){
       if(!led1Status && !digitalRead(Led1)){
          server.send(200, "text/plain", "led1 on");
          digitalWrite(Led1, HIGH); // led 1 on
          led1Status = true;
        }
       if(!led2Status && !digitalRead(Led2)){
          server.send(200, "text/plain", "led2 on");
          digitalWrite(Led2, HIGH); // led 1 on
          led2Status = true;
        }
    });

  server.onNotFound(handleNotFound);
}

void loop(){
  if (WiFi.status() != WL_CONNECTED) wifiConnect();// reconnect Wifi if necessary 
  server.handleClient();
}
