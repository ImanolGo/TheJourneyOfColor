/*
 *  SerialManager.h
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */



#pragma once

#include "ofMain.h"
#include "Manager.h"


//========================== class SerialManager =======================================
//==============================================================================
/** \class SerialManager SerialManager.h
 *    \brief class for managing the serial connection
 *    \details  It connects the Adafruit Feather M0 board to communicate color changes on the RGB LEDs.
 */


class SerialManager: public Manager
{
    static const int BAUD_RATE;
    
    struct serial_header {
        Byte f1;
        Byte f2;
        Byte f3;
        Byte size;
        Byte  channel;
    };
    
public:
    //! Constructor
    SerialManager();
    
    //! Destructor
    virtual ~SerialManager();
    
    //! setups the udp manager
    void setup();
    
    //! updates the udp manager
    void update();
    
    void onSetColor(const ofColor& color);
    
    
private:
    
    void readSerialSettings();
    
    void setupSerial();
    
    void autoConnect();
    
    void connect(int portNum);
    
    bool checkConnection(int portNum);
    
    void sendPin();
    
    bool receivedOk();
    
private:
    
    ofSerial   m_serial;
    bool       m_connected;
    
    
};

