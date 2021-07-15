
# Bed Sensor
***Author: Finn Morris
Created: 26/03/21
Modified: 2/05/21***

Based on code by: https://everythingsmarthome.co.uk/howto/building-a-bed-occupancy-sensor-for-home-assistant/


# Description:
Bed Sensor based on 4 connected load cells to determine the weight of bed and its occupancy state.

### Hardware: 
- 4x Load Cells
- Wemos D1 R2 Board
- 5v Power Supply (Can work with 3.3 but more inaccurate)
 
# Purpose:
Bed Sensor based on 4 connected load cells and an Arduino based ESP8266 board will measure the change resistance based on the 5v input and convert the reading into a measurable wight in kilograms. The raw measurement will be sent to HomeAssistant to process bed related automatons. 
