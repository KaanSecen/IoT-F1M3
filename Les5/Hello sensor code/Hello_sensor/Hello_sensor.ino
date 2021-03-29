/*  
 *   Mediacollege Duurzaam huis BO
 *   Les 5
 *   Connectie met internet  
 *   Webserver
 *   DHT11 temperature + humidity sensor
*/


//        initialise temperature humidity sensor DHT11
#include "DHT.h"
#define DHTTYPE DHT11       // DHT 11 sensor
uint8_t DHTPin = D7;        // DHT Sensor data input
DHT dht(DHTPin, DHTTYPE);   // Initialize DHT sensor.    
float Temperature;          // temperature
float Humidity;             // humidity
float HeatIndex;            // Heatindex
float kaan = 23;            // GEEF CREDITS AAN: KAAN
 
//      web server
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); // maak een instantie van de webserver op poort 80

// your data
String studentName = "Kaan Secen student nummer 24114"; // jouw student nummer + naam
bool led1Status = false; //toggle led on/off

const char* ssid = "Medialab";
const char* password = "Mediacollege";

uint8_t Led1 = D3; //LED indicator Kaan


void readDHT11(){
  /* 
   *  read the sensor
   *  sampling rate DHT11 is max. 1Hz 
   *  the DHT11 is known for erroneous readings
   */
    float temperature =  round(dht.readTemperature()*10)/10; // Gets the values of the temperature
    float humidity = round(dht.readHumidity()*10)/10; // Gets the values of the humidity
    // Compute heat index in Celsius (isFahrenheit = false)
    float heatindex = round(dht.computeHeatIndex( Temperature, Humidity, false)*10)/10; 
        
    if(isnan(temperature) || isnan(humidity) || isnan(heatindex)){
        // sensor error
        Serial.println("DHT11 sensor error");
    }
    else{
        // the DHT11 readings look ok 
        Temperature = temperature;
        Humidity =  humidity ;
        HeatIndex = heatindex;
        // show in Serial Monitor
        Serial.print("Temp. ");
        Serial.print(Temperature);
        Serial.print("C. Humidity  ");
        Serial.print(humidity);
        Serial.print("% Heatindex ");
        Serial.println(heatindex);
      }
 } 

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
   server.send(200, "text/html", "<h3>Duurzaam Huis DHT11 sensor. <br> Mijn naam is: " 
   +  studentName + 
   "</h3><form action=\"/sensor\" method=\"GET\"><input type=\"submit\" value=\"Sensor Read\"></form>" );
   }

void handleNotFound(){
  String message = "I'm sorry, I don't know what to do\n\n";
  server.send(404, "text/plain", message);
}

void handleSensor(){
  server.send(200, "text/html", "<h3>Duurzaam Huis: " 
   +  studentName + "</h3>Temperature " + String(Temperature) + 
   " Celsius<br>Humidity " + String(Humidity) +  " %<br>Heatindex " + String(HeatIndex));
   if(kaan < Temperature) {
      server.send(200, "text/plain", "led1 on");
      digitalWrite(Led1, HIGH); // led 1 on Kaan
      led1Status = true;
    }
    else
    {
      server.send(200, "text/plain", "led1 off");
      digitalWrite(Led1, LOW); // Off Kaan
      led1Status = false;
    }
  }

void setup(){
  Serial.begin(115200);      //  Start de serial monitor
  pinMode(Led1, OUTPUT); // Initialize the pin as an output

/*
 * Webserver functionaliteit
 *  server.on("/"
 *  server.on("/sensor"
 *  server.onNotFound
 */
  server.on("/", handleRoot);

  server.on("/sensor", handleSensor);
 
  server.onNotFound(handleNotFound);
}

void loop(){
  if (WiFi.status() != WL_CONNECTED) wifiConnect();// reconnect Wifi if necessary 
  server.handleClient();
  delay(3500);    //Send a request every XXX ms  DHT11 needs at least 2 seconds
  readDHT11();
}
