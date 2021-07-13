#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h> 


// FONT IS Ano Half W00 Upper Lower - -its not but its similar 


//SETUP WIFI
const char* ssid = "SAVANNAH";
const char* password =  "finnashycaely";


//MQTT
const char* mqttServer = "10.1.3.178";
const int mqttPort = 1883;
const char* mqttUser = "HassioMQTT";
const char* mqttPassword = "finnreed";
const char* stateTopic = "home/bedroom/clock";
const char* availableTopic = "home/bedroom/clock/available";
const char* hostName = "wemosD1Mini-esp8266-master-clock";

//SERVO SETUP
//BLACk = GROUND
//RED = POWER (5V)
//WHITE = CONTROL PIN
const int PIN = D4; // WHITE
int lastAngle = 0;
int val;  
int max_angle = 180; 
int cur_val;
int pos;



WiFiClient espClient;
PubSubClient client(espClient);
Servo myservo;
 
void setup() {
 
  Serial.begin(115200);
  myservo.attach(D4);    
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  client.subscribe(stateTopic);
  Serial.print("Subscribed");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  Serial.print(" Rotation angle is:");
  Serial.print("   ");
  for(int i=0;i<length;i++)
  {
    if((int)payload[i]>194||(int)payload[i]<0)
    break;
    myservo.write((int)payload[i]);// tell servo to go to position in variable '(int)payload[i]'
    Serial.print((int)payload[i]);//print the rotation angle
    Serial.print("]");
    Serial.println();
  }
}//end callback

void loop() {
  client.loop();
}
