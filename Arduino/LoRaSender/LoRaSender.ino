
 /*
  
  LoRa Color Sender 
     
 Description:
 * Firmware sending Color changes usin the LoRa protocol

 Software:
 * FastLED Library 
 * RadioHead (RH_RF95) Library 
 
 Hardware:
* Adafruit Feather M0 Radio with LoRa Radio Module


 created 10th August 2018
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2018) by Imanol Gómez

 */
 

#include "LoraManager.h"
#include "SerialManager.h"


LoraManager loraManager;
SerialManager serialManager(&loraManager);


void setup() 
{
    serialManager.setup();
    loraManager.setup();
}

void loop() 
{
    loraManager.update();
    serialManager.update();
 
}
