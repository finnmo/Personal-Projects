
# Plantation Shutter Motor
***Author: Finn Morris
Created: 26/04/21
Modified: 13/06/21***


# Description:
Kinect Sensor to track room presence and realative position 

### Hardware: 
- Docker Container running on Personal Server
- Kinect V1 + USB  power supply
 
# Purpose:
Through HTTP protocol, the kinect v1 sensor uses infrared sensors and the NiTE skeleton tracking library to determine a human position relative to the sensor.
The program will determine if the position is within the limits of a know destination such as a bed or a desk

The Program can determine position to the centermeter in X, Y and Z coordinates  

HTTP POST: commands are sent to HomeAssistant to process automation


PROGRAM IS BASED ON NiTE SAMPLES 
