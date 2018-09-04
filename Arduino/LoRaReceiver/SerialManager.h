///////////////////////////////////////////////////////////////////
// Class controlling the Serial communication
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////


#pragma once
#include "Arduino.h"
#include "FastLED.h"
#include "LedsManager.h"
#include "Config.h"

class SerialManager
{
  private:

  LedsManager* ledsManager;  ///< it sends the commands to the leds manager
 
  public:
    
    SerialManager(LedsManager* ledsManager);
    
    void setup();
    void update();

  private:

    void initializeSerial();
    void updateSerial();
    void sendConnected();
    void parseMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isData(uint8_t* _buffer, uint8_t bufferSize);
    bool isConnection(uint8_t* _buffer, uint8_t bufferSize);
    bool isChannel(uint8_t* _buffer, uint8_t bufferSize);
    int BtoI(byte a, byte b);
    CRGB  getRGBColor(uint8_t* _buffer, uint8_t bufferSize);
    CRGB* getColorPalette(unsigned char* pbuff, int count);
    CRGB palette[NUM_COLOR_PALETTE];

    bool _connected;
  
};


SerialManager::SerialManager(LedsManager* ledsManager)
{
    this->ledsManager=ledsManager;
    _connected = false;
}

void SerialManager::setup()
{
    initializeSerial();
    //Serial.println("SerialManager::setup");
    //Serial.print("OK");
}

void SerialManager::initializeSerial()
{
    Serial.begin(115200);
    delay(200);
    
}


void SerialManager::update()
{
    updateSerial();
}

void SerialManager::updateSerial()
{     

    uint8_t numBytes = Serial.available();
    
    if (numBytes > 0) 
    {
         //Serial.print("SerialManager::received -> ");
         //Serial.print(numBytes);
         //Serial.println(numBytes);
         //Serial.print("OK"); 
         uint8_t buf[numBytes];
         Serial.readBytes((char *) buf,numBytes);
         //Serial.print("OK");
         this->parseMessage(buf, numBytes);
    }
    
}


void SerialManager::parseMessage(uint8_t* buf, uint8_t len)
{
      if(this->isMessage(buf,len))
      {
          if(this->isConnection(buf, len)){
            this->sendConnected();
            
          }
          else if(this->isData(buf, len) && this->isChannel(buf, len)){
              this->ledsManager->setColorPalette(this->getColorPalette(buf,len));
          } 
         
      }
}


void SerialManager::sendConnected()
{
      int len = HEADER_SIZE+1;
      byte buf[len];
      buf[0] = 0x10;
      buf[1] = 0x41;
      buf[2] = 0x37;
      buf[3] = 1;
      buf[4] = 'c';
      buf[5] = 0;
      buf[6] = 'c';
      
      Serial.write(buf, len);
      _connected = true;
}


CRGB SerialManager::getRGBColor(uint8_t* _buffer, uint8_t bufferSize)
{       
        if(bufferSize <= COLOR_INDEX + NUM_BYTES){
           return CRGB::Black;
        }

        return CRGB(_buffer[COLOR_INDEX], _buffer[COLOR_INDEX+1], _buffer[COLOR_INDEX+2]);
        
}


CRGB* SerialManager::getColorPalette(unsigned char* pbuff, int count) 
{
    int numLeds = count/CHANNEL_WIDTH;
  
    if(numLeds > NUM_COLOR_PALETTE){
       numLeds = NUM_COLOR_PALETTE;
    }
    
    int offset = 0; //reset RGB channel assignment to 0 each time through loop.
    for (int i = 0; i < numLeds; i++) //loop to assign 3 channels to each pixel
    {
        this->palette[i] = CRGB(pbuff[HEADER_SIZE + offset], pbuff[HEADER_SIZE + (offset +1)], pbuff[HEADER_SIZE + (offset +2)]);
        offset +=CHANNEL_WIDTH; //increase last channel number by channel width

          Serial.print("SerialManager::getColorPalette, r = ");
          Serial.print(this->palette[i].r);
          Serial.print(", g = ");
          Serial.print(this->palette[i].g);
          Serial.print(", b = ");
          Serial.println(this->palette[i].b);
    }
  
    return palette;

}

bool SerialManager::isMessage(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[0] == 0x10 && _buffer[1] == 0x41 && _buffer[2] == 0x37) 
    { 
        uint8_t data_size = _buffer[SIZE_INDEX];
        if ( (bufferSize-HEADER_SIZE) == data_size ) 
        {
           //Serial.println("SerialManager::isMessage -> true");
          return true; 
        }
    }

    //Serial.println("SerialManager::isMessage -> false");
    return false;
}


bool SerialManager::isData(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[COMMAND_INDEX] == 'd') { 
      return true;
    }

    return false;
}


bool SerialManager::isConnection(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[COMMAND_INDEX] == 'c') { 
       //Serial.println("SerialManager::isConnection -> true");
      return true;
    }

    //Serial.println("SerialManager::isConnection -> false");
    return false;
}


bool SerialManager::isChannel(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[CHANNEL_INDEX] == CHANNEL) { 
      return true;
    }

    return false;
}






