
 /*
  
  LoRa Color Receiver 
     
 Description:
 * Firmware receiving Color changes usin the LoRa protocol

 Software:
 * FastLED Library 
 * RadioHead (RH_RF95) Library 
 
 Hardware:
* Adafruit Feather M0 Radio with LoRa Radio Module
* DotStars Led Strip


 created 10th August 2018
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2018) by Imanol Gómez

 */
 
#include "LoraManager.h"
#include "LedsManager.h"
#include "SerialManager.h"
#include "Config.h"


LedsManager ledsManager;
LoraManager loraManager(&ledsManager);
SerialManager serialManager(&ledsManager);


void setup() 
{
    Serial.begin(115200);
    delay(200);
    Serial.println("Starting Software!!!!");

    ledsManager.setup();
    loraManager.setup();
    serialManager.setup();
}

void loop() 
{
    ledsManager.update();
    loraManager.update();
    serialManager.update();
 
}
