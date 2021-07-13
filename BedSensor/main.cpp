#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <HX711.h>
#include <PubSubClient.h>
#include "config.h"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

HX711 scale;

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(HOSTNAME, MQTTUSER, MQTTPASS)) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish(AVAILABILITY_TOPIC, "online");
            client.subscribe(TOPIC_TARE);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

// callback function for handling new messages on subscribed topics
void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, TOPIC_TARE) == 0) {
    Serial.println("starting tare...");
    scale.set_scale(CALIBRATION);
    //scale.set_offset(OFFSET);
    scale.tare();       //Reset scale to zero
    Serial.println("Scale reset to zero");
  }
}

void setupOTA() {
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(HOSTNAME);

    ArduinoOTA.onStart([]() { Serial.println("Starting"); });
    ArduinoOTA.onEnd([]() { Serial.println("\nEnd"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();
}

void setup() {
    Serial.begin(115200); delay(10);
    Serial.println();
    Serial.println("Starting load cells...");

    scale.begin(HX711_DT, HX711_SCK);

    //Set conversion factor
    scale.set_scale(CALIBRATION);
    
    // zero out scale
    scale.tare();
    Serial.println("Scale torn!");

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connecting...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    client.setServer(MQTT_SERVER, 1883);
    client.setCallback(callback);
    setupOTA();

    // Setting up heartbeat
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    static uint32_t timer   = 0; // sample timer
    static uint8_t  samples = 0; // number of samples taken
    static float    avg     = 0; // load average
    static float    oldAvg  = 0; // stores the past average
    int value;

    if (!client.connected()) {
        reconnect();
    }
    // take sample
    if (millis() - timer >= SAMPLE_PERIOD)
    {
        avg += scale.get_units();
        samples++;

        if (samples >= NUM_SAMPLES)
        {
            // buffer for the sample as a string
           char result[RESULT_WIDTH + 1]; 
           //float result;
            // average the samples
            avg /= samples;

            // transmit only if value has changed
            if (abs(avg - oldAvg) > PRECISION)
            {
                // convert the float to a string
                dtostrf(avg, RESULT_WIDTH, RESOLUTION, result);

                // publish string
                Serial.println("Uploading new result: " );
                Serial.println(result);
                client.publish(TOPIC_LOAD, result);

                // store past transmission
                oldAvg = avg;
            }

            // clear variables
            avg     = 0;
            samples = 0;
        }

        // reset timer
        timer = millis();
    }

    //int value;
    //value = scale.read_average(5);
    //Serial.println(value);

    
    // Handle OTA
    ArduinoOTA.handle();

    // Process MQTT tasks
    client.loop();

    // Heartbeat
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    //delay(250);

    //scale.power_down(); // put the ADC in sleep mode
    //delay(1000);
    //scale.power_up();
}
