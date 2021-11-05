#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

int sensorPin=D1;
boolean val =0;
int count = 0;

const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";

void setup() {
 unsigned short count = 0; 
 pinMode(sensorPin, INPUT);
 Serial.begin(115200);

 Serial.println();
 
 Serial.print("Connecting to ");
 Serial.println(ssid);
 
 WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
   count++;

   if (count >= 30)
      wifiRestart();
 }
 
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}


void wifiRestart(){
 Serial.println("Turning WiFi off...");
 WiFi.mode(WIFI_OFF);
 Serial.println("Sleepping for 10 seconds...");
 delay(10000);
 Serial.println("Trying to connect to WiFi...");
 WiFi.mode(WIFI_STA);
}


void loop() {
 short response_code = 0;

 
 // wait for WiFi connection
 if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    val =digitalRead(sensorPin);
    // when the sensor detects a signal above the threshold value, LED flashes
    if (val==HIGH) {
     
      Serial.print("[HTTPS] begin...\n");
      String url = "http://URL:8123/api/services/switch/toggle";
      String jsondata=("{\"entity_id\":\"switch.desk_lamp\"}");
      
      http.begin(url); 
      http.addHeader("Authorization", "Bearer <token>); 
      http.addHeader("Content-Type", "application/json"); 
    
      int httpResponseCode = http.POST(jsondata); //Send the actual POST request
    
      if(httpResponseCode>0){
        String response = http.getString();  //Get the response to the request
        Serial.println(httpResponseCode);   //Print return code
        Serial.println(response);           //Print request answer
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    
        http.end();
      }
    }
   } else {
    wifiRestart();
   }
}
