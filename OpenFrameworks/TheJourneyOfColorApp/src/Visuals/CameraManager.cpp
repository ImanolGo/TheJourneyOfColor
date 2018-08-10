/*
 *  CameraManager.cpp
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */



#include "ofMain.h"

#include "CameraManager.h"
#include "AppManager.h"



CameraManager::CameraManager(): Manager()
{
    //m_videoPlayer = new ofxAVFVideoPlayer();
}


CameraManager::~CameraManager()
{
    ofLogNotice() <<"CameraManager::Destructor" ;
}


void CameraManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    this->setupCamera();
    this->setupShaders(width,height);
    
   
    
    ofLogNotice() <<"CameraManager::initialized" ;
    
}



void CameraManager::setupCamera()
{
    m_camWidth = 320;  // try to grab at this size.
    m_camHeight = 240;
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = m_vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    m_vidGrabber.setDeviceID(0);
    m_vidGrabber.setDesiredFrameRate(60);
    m_vidGrabber.initGrabber(m_camWidth, m_camHeight);
}

void CameraManager::setupShaders(float width,float height)
{
    this->setupLevels(width, height);
    this->setupBlur(width, height);
}

void CameraManager::setupLevels(float width, float height)
{
    m_levels.setup(width,height);
}

void CameraManager::setupBlur(float width, float height)
{
    m_blur.setup(width,height, 10, .2, 4);
    m_blur.setScale(0.0);
}




void CameraManager::updateCamera()
{
    m_vidGrabber.update();
    
    if(m_vidGrabber.isFrameNew()){
        //Do something
    }
}


void CameraManager::drawCamera()
{
    string name = "Camera";
    ofRectangle frame;
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    float ratio = m_vidGrabber.getWidth()/ m_vidGrabber.getHeight();
    frame.height = rect->getHeight();
    frame.width = frame.height*ratio;
    
    if( frame.width > rect->getWidth() ){
        frame.width = rect->getWidth();
        frame.height = frame.width/ratio;
    }
    
    frame.x = rect->getWidth()*0.5 - frame.width*0.5;
    frame.y = rect->getHeight()*0.5 - frame.height*0.5;
    
    
    m_levels.begin();
    ofClear(0);
    m_vidGrabber.draw(0,0);
    m_levels.end();
    
    m_blur.begin();
    m_levels.draw();
    m_blur.end();
    
    m_blur.draw(frame);
}



void CameraManager::draw()
{
    this->drawCamera();
}

