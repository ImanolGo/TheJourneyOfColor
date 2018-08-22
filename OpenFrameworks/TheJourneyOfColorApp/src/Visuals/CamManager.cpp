/*
 *  CamManager.cpp
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */



#include "ofMain.h"

#include "AppManager.h"
#include "CamManager.h"




CamManager::CamManager(): Manager(), m_prevMotionDetected(true), m_isMotionDetected(false)
{
    //m_videoPlayer = new ofxAVFVideoPlayer();
}


CamManager::~CamManager()
{
    ofLogNotice() <<"CamManager::Destructor" ;
}


void CamManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    this->setupCamera();
    this->setupCv();
    this->setupFbo();
    this->setupShaders(m_vidGrabber.getWidth(),m_vidGrabber.getHeight());
    
   
    
    ofLogNotice() <<"CamManager::initialized" ;
    
}



void CamManager::setupCamera()
{
    //m_camWidth = 1280;
    //m_camHeight = 720;
    
    m_camWidth = 320;  // try to grab at this size.
    m_camHeight =240;
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = m_vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << "CamManager::setupCamera-> " << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() <<  "CamManager::setupCamera-> " << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    m_vidGrabber.setDeviceID(0);
    //m_vidGrabber.setDesiredFrameRate(60);
    m_vidGrabber.setup(m_camWidth, m_camHeight);
}


void CamManager::setupCv()
{
    int w = m_vidGrabber.getWidth();
    int h = m_vidGrabber.getHeight();
    
    m_colorImg.allocate(w,h);
    m_grayImg.allocate(w,h);
    m_diffImg.allocate(w,h);
    m_binaryImg.allocate(w,h);
}

void CamManager::setupFbo()
{
    m_fboCamera.allocate(m_vidGrabber.getWidth(),m_vidGrabber.getHeight());
    m_fboCamera.begin(); ofClear(0); m_fboCamera.end();
    
    m_fbo.allocate(2*m_vidGrabber.getWidth(),2*m_vidGrabber.getHeight());
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void CamManager::setupShaders(float width,float height)
{
    this->setupLevels(width, height);
    this->setupBlur(width, height);
}

void CamManager::setupLevels(float width, float height)
{
    m_levels.setup(width,height);
}

void CamManager::setupBlur(float width, float height)
{
    m_blur.setup(width,height, 10, .2, 4);
    m_blur.setScale(0.0);
}

void CamManager::setBlurScale(float& value)
{
    m_blur.setScale(value);
}


void CamManager::update()
{
    this->updateCamera();
    this->updateFbo();
    
}


void CamManager::updateCamera()
{
    m_vidGrabber.update();
    
    if(m_vidGrabber.isFrameNew()){
        this->updateFboCamera();
        this->updateCv();
        //this->updateQuantization();
    }
}

void CamManager::updateFbo()
{
    int w = m_vidGrabber.getWidth();
    int h = m_vidGrabber.getHeight();
    
    m_fbo.begin();
        ofClear(0);
        m_colorImg.draw( 0, 0, w, h);
        m_grayImg.draw( w, 0, w, h);
        m_diffImg.draw( w, h,  w, h);
        m_binaryImg.draw( 0, h,  w, h);
        m_contourFinder.draw(0, h,  w, h);
    m_fbo.end();
}

void CamManager::updateFboCamera()
{
    m_fboCamera.begin();
        ofClear(0);
        this->drawCamera();
    m_fboCamera.end();
}

void CamManager::updateCv()
{
    ofPixels pixels;
    m_fboCamera.readToPixels(pixels);
    
    //m_colorImg.setFromPixels(pixels);
     m_colorImg.setFromPixels(m_vidGrabber.getPixels());
    m_colorImg.mirror( false, true );
    
    m_grayImg =  m_colorImg;
    
     m_diffImg.absDiff( m_grayImg,  m_grayPrevImg);
    
     m_grayPrevImg =  m_colorImg;
    
     m_binaryImg =  m_diffImg;
     m_binaryImg.threshold( m_imgThreshold);
    
     m_contourFinder.findContours( m_binaryImg,  m_minArea,  m_maxArea, 10, false);
    
    if ( m_contourFinder.nBlobs > 0){
        if(!m_isMotionDetected){
            AppManager::getInstance().getGuiManager().setMotionDetected(true);
        }
         m_isMotionDetected = true;
        
    }else{
        if(m_isMotionDetected){
            AppManager::getInstance().getGuiManager().setMotionDetected(false);
            this->updateQuantization();
        }
        m_isMotionDetected = false;    }
}

void CamManager::updateQuantization()
{
    ofPixels pixels;
    m_fboCamera.readToPixels(pixels);
    AppManager::getInstance().getColorManager().quantizeColors(pixels);
}

void CamManager::drawCamera()
{
    
    m_levels.begin();
    ofClear(0);
    m_vidGrabber.draw(0,0);
    m_levels.end();
    
    m_blur.begin();
    m_levels.draw();
    m_blur.end();
    
    m_blur.draw();
}



void CamManager::draw()
{
    ofClear(0);
    this->drawFbo();
}

void CamManager::drawFbo()
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
    
     m_fbo.draw(frame);
}



