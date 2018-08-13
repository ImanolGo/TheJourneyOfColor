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
        Byte command;
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
    
    //void onSetColor(const ofColor& color);
    
    void onSetColors(vector<ofColor>& colors);
    
    
private:
    
    void readSerialSettings();
    
    void setupSerial();
    
    void setupHeaders();
    
    void autoConnect();
    
    void connect(int portNum);
    
    bool checkConnection(int portNum);
    
    void sendConnection();
    
    bool receivedConnected();
    
    bool isMessage(unsigned char * buffer, int size);
    
    bool isConnected(unsigned char * buffer, int size);
    
private:
    
    ofSerial   m_serial;
    bool       m_connected;
    
    serial_header    m_dataHeader;
    serial_header    m_connectHeader;
    
    
};

