/*
 *  SerialManager.cpp
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */




#include "SerialManager.h"
#include "AppManager.h"


const int SerialManager::BAUD_RATE = 115200;

SerialManager::SerialManager(): Manager(), m_connected(false)
{
    //Intentionally left empty
}

SerialManager::~SerialManager()
{
    ofLogNotice() << "SerialManager::destructor";
}


//--------------------------------------------------------------

void SerialManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupHeaders();
    this->setupSerial();
    
    ofLogNotice() <<"SerialManager::initialized" ;
}


void SerialManager::setupHeaders()
{
    m_dataHeader.f1 = 0x10;
    m_dataHeader.f2 = 0x41;
    m_dataHeader.f3 = 0x37;
    m_dataHeader.size = 0;
    m_dataHeader.command = 'd';
    m_dataHeader.channel = 0;
    
    m_connectHeader.f1 = 0x10;
    m_connectHeader.f2 = 0x41;
    m_connectHeader.f3 = 0x37;
    m_connectHeader.size = 1;
    m_connectHeader.command = 'c';
    m_connectHeader.channel = 0;
}


void SerialManager::setupSerial()
{
   // ofLogNotice() <<"SerialManager::setupSerial << Autoconnecting serial port";
    //this->autoConnect();
    
    int serialPort = AppManager::getInstance().getSettingsManager().getSerialPort();
    if(serialPort<0){
        ofLogNotice() <<"SerialManager::setupSerial << Autoconnecting serial port";
        this->autoConnect();
    }
    else{
        ofLogNotice() <<"SerialManager::setupSerial << Connecting serial to port " << serialPort;
        this->connect(serialPort);
    }
}

void SerialManager::connect(int portNum)
{
    if(m_serial.setup(portNum, BAUD_RATE)) //open a device number
    {
        ofLogNotice() <<"SerialManager::connect << Arduino connected to " << portNum;
        m_connected = true;
        
    }
}

void SerialManager::autoConnect()
{
    m_serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = m_serial.getDeviceList();
    
    // this should be set to whatever com port your serial device is connected to.
    // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
    // arduino users check in arduino app....
    
    m_connected = false;
    
    for(auto device: deviceList)
    {
        if(this->checkConnection(device.getDeviceID())) //open a device number
        {
            ofLogNotice() <<"SerialManager::setupSerial << Arduino connected to port " << device.getDeviceName();
            m_connected = true;
            return;
        }
    }
}


bool SerialManager::checkConnection(int portNum)
{
    if(m_serial.setup(portNum, BAUD_RATE)) //open a device number
    {
        this->sendConnection();
        ofSleepMillis(200);
        if(this->receivedConnected()){
            ofLogNotice() <<"SerialManager::checkConnection << Arduino connected to " << portNum;
            return true;
        }
        else {
            m_serial.close();
        }
    }
    
    ofLogNotice() <<"SerialManager::checkConnection << Arduino is not connected ";
    //m_serial.setup(0, BAUD_RATE);
    return false;
    
}

void SerialManager::sendConnection()
{
    unsigned char connected = 'c';
    
    string message="";
    message+= m_connectHeader.f1; message+= m_connectHeader.f2; message+= m_connectHeader.f3;
    message+=  m_connectHeader.size;
    message+=m_connectHeader.command;
    message+=m_connectHeader.channel;
    message+=connected;
    
//    unsigned char sendMessage[7];
//    sendMessage[0]=m_connectHeader.f1;
//    sendMessage[1]=m_connectHeader.f2;
//    sendMessage[3]=m_connectHeader.f3;
//    sendMessage[4]=m_connectHeader.size;
//    sendMessage[5]=m_connectHeader.channel;
    
    ofLogNotice() <<"SerialManager::sendConnection << Sent: " << message;
    
    m_serial.writeBytes((unsigned char *) message.c_str(), message.length());
}

bool SerialManager::receivedConnected()
{
    ofLogNotice() <<"SerialManager::checkConnection << Serial Available: " << m_serial.available();
    
    
    int numBytes = m_serial.available();
    
    if ( numBytes == 0 )
    {
        return false;
    }
    
    
    unsigned char bytes[numBytes];
    
    int result = m_serial.readBytes( bytes, numBytes );
    
    string str((char *)bytes);
    
    ofLogNotice() <<"SerialManager::checkConnection << Received: " << str;
    
    // check for error code
    if ( result == OF_SERIAL_ERROR ){
        // something bad happened
        ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
    }
    else if ( result == OF_SERIAL_NO_DATA ){
        ofLog( OF_LOG_ERROR, "no data reading from serial" );
    }
    
    
    return (this->isMessage(bytes, numBytes) && this->isConnected(bytes, numBytes));
}


bool SerialManager::isMessage(unsigned char * buffer, int size)
{
    
    if(buffer[0] != m_connectHeader.f1 ){
         ofLogNotice() <<"SerialManager::buffer[0] not 0x10 ";
    }

    if(buffer[1] != m_connectHeader.f2 ){
    ofLogNotice() <<"SerialManager::buffer[1] not 0x41 ";
    }

    if(buffer[2] != m_connectHeader.f3 ){
    ofLogNotice() <<"SerialManager::buffer[2] not 0x37 ";
    }
    
    if(buffer[0] != m_connectHeader.f1  && buffer[1] != m_connectHeader.f2  && buffer[2] != m_connectHeader.f3 ){
        ofLogNotice() <<"SerialManager::isMessage -> FALSE ";
        return false;
    }
    
    
    
    
    ofLogNotice() <<"SerialManager::isMessage -> TRUE ";
    return true;
}

bool SerialManager::isConnected(unsigned char * buffer, int size)
{
    if(buffer[4] == 'c')
    {
        ofLogNotice() <<"SerialManager::isConnected -> TRUE ";
        return true;
    }
    
    
    ofLogNotice() <<"SerialManager::isConnected -> FALSE ";
    return false;
}



void SerialManager::update()
{
    //
}


void SerialManager::sendColors(vector<ofColor>& colors)
{
    if(!m_connected){
        return;
    }
    
    int ledsPerPixel = 3;
    
    string message="";
    message+= m_dataHeader.f1; message+= m_dataHeader.f2; message+= m_dataHeader.f3;
    m_dataHeader.size = ledsPerPixel*colors.size();
    message+=  m_dataHeader.size;
    message+=m_dataHeader.command;
    message+=m_dataHeader.channel;
    
    for(int i = 0; i< colors.size(); i++)
    {
        message+=colors[i].r;
        message+=colors[i].g;
        message+=colors[i].b;
    }
    
    
    
    
    m_serial.writeBytes((unsigned char *) message.c_str(), message.length());
    
}




