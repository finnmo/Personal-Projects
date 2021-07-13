
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h> 

#define DEBOUNCE 100

//SETUP WIFI
const char* ssid = "SAVANNAH";
const char* password =  "finnashycaely";


//MQTT
const char* mqttServer = "10.1.3.178";
const int mqttPort = 1883;
const char* mqttUser = "HassioMQTT";
const char* mqttPassword = "finnreed";
const char* stateTopic = "home/bedroom/lamp";
const char* availableTopic = "home/bedroom/lamp/available";
const char* hostName = "ESP8266CapTouch";


// Constants
const int TOUCH_BUTTON_PIN = 4;  // Input pin for touch state

// Global Variables
int touchState = 0;             // Variable for reading button
int touchCounter = 0;   // counter for the number of button presses
int lastTouchState = 0;     // previous state of the button
int toggle = 1;

String touchCounterString;
char touchMqtt[50];

//Timer
unsigned long timer =0;
unsigned long lastChange =0;

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {

  setup_wifi();
  client.setServer(mqttServer, 1883);

  // Configure button pin as input 
  pinMode(TOUCH_BUTTON_PIN, INPUT);

  timer = millis();   // start delay

  Serial.begin(9600);

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read the state of the capacitive touch board
  touchState = digitalRead(TOUCH_BUTTON_PIN);

  if ((touchState != lastTouchState) && ((millis()-lastChange) > DEBOUNCE)) {
     lastTouchState = touchState;
     lastChange = millis();
    if (touchState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      Serial.println("TOUCH ON");
      touchCounter++;
      Serial.print("number of touches: ");
      Serial.println(touchCounter);
      if(touchCounter == 1){
        timer = millis();
      }
      if (millis()-timer<=1500){
        if(touchCounter == 2){
          toggle++;
          Serial.println("Toggle added:" + toggle);
          touchCounter = 0;
          touchCounterString = String(toggle);
          touchCounterString.toCharArray(touchMqtt, touchCounterString.length() + 1);
          client.publish(stateTopic, touchMqtt);
          }
      }else{
        touchCounter = 0;
      }
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("TOUCH OFF");
    }
  }
    
    if(toggle > 100){
      toggle = 1;
    }
    delay(20);
}
