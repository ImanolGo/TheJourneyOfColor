///////////////////////////////////////////////////////////////////
// Class controlling the LoRa Radio communication
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////


#define CHANNEL 0
#define HEADER_SIZE 6
#define SIZE_INDEX 3
#define COMMAND_INDEX 4
#define CHANNEL_INDEX 5
#define COLOR_INDEX 6
#define NUM_BYTES 3

#pragma once
#include "Arduino.h"
#include <SPI.h>
#include <RH_RF95.h>
#include "FastLED.h"
#include "LedsManager.h"


//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0


class LoraManager
{
  private:
 
  LedsManager* ledsManager;  ///< it sends the commands to the leds manager
  RH_RF95*     rf95;        ///< the lora library
  

  public:
    
    LoraManager(LedsManager* ledsManager);
    
    void setup();
    void update();

  private:

    void initializeLora();
    void updateLora();
    void parseMessage();
    bool isMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isData(uint8_t* _buffer, uint8_t bufferSize);
    bool isChannel(uint8_t* _buffer, uint8_t bufferSize);
    int BtoI(byte a, byte b);
    CRGB  getRGBColor(uint8_t* _buffer, uint8_t bufferSize);
    CRGB* getColorPalette(unsigned char* pbuff, int count);
 
};


LoraManager::LoraManager(LedsManager* ledsManager)
{
    this->ledsManager=ledsManager;
    this->rf95 = new RH_RF95(RFM95_CS, RFM95_INT);
  
}

void LoraManager::setup()
{
    Serial.println("LoraManager::setup");
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
      Serial.println("LoraManager::LoRa radio init failed");
      while (1);
    }

    Serial.println("LoraManager::LoRa radio init OK!");

    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    if (!this->rf95->setFrequency(RF95_FREQ)) {
      Serial.println("LoraManager::setFrequency failed");
      while (1);
    }
    Serial.print("LoraManager::Set Freq to: "); Serial.println(RF95_FREQ);
    
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    
    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 5 to 23 dBm:
    this->rf95->setTxPower(23, false);

    
}





void LoraManager::update()
{
    updateLora();
}

void LoraManager::updateLora()
{
      if (this->rf95->available())
      {
        // Should be a message for us now
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
      
        if (this->rf95->recv(buf, &len))
        {
            if(this->isMessage(buf,len) && this->isData(buf, len) && this->isChannel(buf, len))
            {
                this->ledsManager->setColorPalette(this->getColorPalette(buf,len));
                
            }
        }
        else
        {
          Serial.println("LoraManager::Receive failed");
        }
      }
}

CRGB LoraManager::getRGBColor(uint8_t* _buffer, uint8_t bufferSize)
{       
        if(bufferSize <= COLOR_INDEX + NUM_BYTES){
           return CRGB::Black;
        }

        return CRGB(_buffer[COLOR_INDEX], _buffer[COLOR_INDEX+1], _buffer[COLOR_INDEX+2]);
        
}


CRGB* LoraManager::getColorPalette(unsigned char* pbuff, int count) 
{
    int numLeds = count/CHANNEL_WIDTH;
  
    if(numLeds > NUM_COLOR_PALETTE){
       numLeds = NUM_COLOR_PALETTE;
    }
  
    CRGB palette[NUM_COLOR_PALETTE];
    
    int offset = 0; //reset RGB channel assignment to 0 each time through loop.
    for (int i = 0; i < numLeds; i++) //loop to assign 3 channels to each pixel
    {
        palette[i] = CRGB(pbuff[HEADER_SIZE + offset], pbuff[HEADER_SIZE + (offset +1)], pbuff[HEADER_SIZE + (offset +2)]);
        offset +=CHANNEL_WIDTH; //increase last channel number by channel width
    }
  
    return palette;

}


bool LoraManager::isMessage(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[0] == 0x10 && _buffer[1] == 0x41 && _buffer[2] == 0x37) 
    { 
        uint8_t data_size = _buffer[SIZE_INDEX];
        if ( (bufferSize-HEADER_SIZE) == data_size ) 
        {
          return true; 
        }
    }

    return false;
}


bool LoraManager::isData(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[COMMAND_INDEX] == 'd') { 
      return true;
    }

    return false;
}


bool LoraManager::isChannel(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[CHANNEL_INDEX] == CHANNEL) { 
      return true;
    }

    return false;
}



int LoraManager::BtoI(byte a, byte b)
{
  return (a<<8)+b;
}
