# Invisible Light Switch
***Author: Finn Morris
Created: 15/06/21
Modified: 15/06/21***


# Description:
Invisible Light Switch using Capacative touch component and Arduino Nano with ESP8266 module

### Hardware: 
- Arduino Nano + ESP8266 Module
- Sparkfun Capacitive touch breakout board - at42qt1011
- 5v-3.3V Power Supply
- 6cm x 10cm aluminum foil
- 4x Jumper leads
 
# Purpose:
A toggle switch created using capacaitive touch principle through an extended touch pad of aluminium foil connected by a jumper cable to the AT42QT1011 breakout board, transmitting HIGH or LOW state to ESP8266 capable Arduino board. 
Toggle states are 2 touches with less than 2 seconds between, once triggered an update is sent to HomeAssistant to process the light state.
