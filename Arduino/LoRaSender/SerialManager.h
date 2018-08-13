///////////////////////////////////////////////////////////////////
// Class controlling the Serial communication
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////


#define HEADER_SIZE 6
#define SIZE_INDEX 3
#define COMMAND_INDEX 4

#pragma once
#include "Arduino.h"
#include "LoraManager.h"


class SerialManager
{
  private:
 
  LoraManager* loraManager;  ///< it sends the data to the lora manager
  

  public:
    
    SerialManager(LoraManager* loraManager);
    
    void setup();
    void update();

  private:

    void initializeSerial();
    void updateSerial();
    void parseMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isData(uint8_t* _buffer, uint8_t bufferSize);
    bool isConnection(uint8_t* _buffer, uint8_t bufferSize);
    void sendConnected();

    bool _connected;
  
};


SerialManager::SerialManager(LoraManager* loraManager)
{
    this->loraManager=loraManager;
    _connected = false;
}

void SerialManager::setup()
{
    Serial.println("LoraManager::setup");
    initializeSerial();
}

void SerialManager::initializeSerial()
{
     Serial.begin(115200);
    while (!Serial) {
      delay(1);
    }
    delay(100);
    
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
         uint8_t buf[numBytes];
         Serial.readBytes((char *) buf,numBytes);
         this->parseMessage(buf, numBytes);
    }
    
    
}


void SerialManager::parseMessage(uint8_t* buf, uint8_t len)
{
      if(this->isMessage(buf,len))
      {
          if(this->isConnection(buf, len)){
            
          }
          else if(this->isData(buf, len) && _connected){
              this->loraManager->sendMessage(buf, len);
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


bool SerialManager::isMessage(uint8_t* _buffer, uint8_t bufferSize)
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
      return true;
    }

    return false;
}



