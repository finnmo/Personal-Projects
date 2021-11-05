
# Click Detector Lamp
***Author: Finn Morris
Created: 05/11/21
Modified: 05/11/21***

# Description:
A lamp that toggles with a click of your fingers

### Hardware: 
- Arduino Sound Sensor
- Wemos D1 mini
- Lamp
 
# Purpose:
The carefully hidden sound sensor detects a click and sends a request to Homeassistant to toggle the lamp.


# Process:

###Prerequisite 
- HTTP (API) enabled Homeassistant

### Step 1:
Upload the code to the Wemos D1 mini with configured HTTP request
Make sure to test that sound will toggle your device, adjusting the potentiometer may be necessary until you find a good level 

### Step 2:
Attach the sound meter to the inside of your lamp and thread wires through to the Wemos D1 Mini.

![Test Image1](https://github.com/finnmo/Personal-Projects/blob/main/ClickDetectorLamp/Inside_Lamp.jpg)
