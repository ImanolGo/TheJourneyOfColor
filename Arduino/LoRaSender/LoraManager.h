///////////////////////////////////////////////////////////////////
// Class controlling the LoRa Radio communication
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////


#pragma once
#include "Arduino.h"
#include <SPI.h>
#include <RH_RF95.h>


//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0


class LoraManager
{
  private:
 
  RH_RF95*     rf95;        ///< the lora library
  

  public:
    
    LoraManager();
    
    void setup();
    void update();

    bool sendMessage(uint8_t* _buffer, uint8_t bufferSize);

  private:

    void initializeLora();
 
};


LoraManager::LoraManager()
{
    this->rf95 = new RH_RF95(RFM95_CS, RFM95_INT); 
}

void LoraManager::setup()
{
    //Serial.println("LoraManager::setup");
    initializeLora();
}

void LoraManager::initializeLora()
{
    // manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
  
    while (!this->rf95->init()) {
      //Serial.println("LoraManager::LoRa radio init failed");
      while (1);
    }

    //Serial.println("LoraManager::LoRa radio init OK!");

    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    if (!this->rf95->setFrequency(RF95_FREQ)) {
      //Serial.println("LoraManager::setFrequency failed");
      while (1);
    }
    //Serial.print("LoraManager::Set Freq to: "); //Serial.println(RF95_FREQ);
    
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    
    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 5 to 23 dBm:
    this->rf95->setTxPower(23, false);
}





void LoraManager::update()
{
}

bool LoraManager::sendMessage(uint8_t* _buffer, uint8_t bufferSize)
{       
    return this->rf95->send(_buffer, bufferSize);    
}

