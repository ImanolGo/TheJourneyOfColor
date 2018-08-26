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
    this->setupFbo();
    this->setupCv();
    this->setupShaders(m_vidGrabber.getWidth(),m_vidGrabber.getHeight());
    
   
    
    ofLogNotice() <<"CamManager::initialized" ;
    
}



void CamManager::setupCamera()
{
    m_camWidth = 1920;
    m_camHeight = 1080;
    
    //m_camWidth = 960;  // try to grab at this size.
    //m_camHeight =540;
    
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
    m_vidGrabber.videoSettings();
}


void CamManager::setupCv()
{
    int w = m_fboRoi.getWidth();
    int h = m_fboRoi.getHeight();
    
    //m_contourFinder.setFindHoles(false);
    
    m_colorImg.allocate(w,h);
    m_grayImg.allocate(w,h);
    m_diffImg.allocate(w,h);
    m_binaryImg.allocate(w,h);
    m_grayPrevImg.allocate(w,h);
    
    m_hysteresis.setDelay(0.2, 0.2);
}

void CamManager::setupFbo()
{
    m_fboCamera.allocate(m_vidGrabber.getWidth(),m_vidGrabber.getHeight(),GL_RGB);
    m_fboCamera.begin(); ofClear(0); m_fboCamera.end();
    
    m_fboCamera.getTexture().getTextureData().bFlipTexture = true;
    
    
    m_fbo.allocate(2*m_vidGrabber.getWidth(),2*m_vidGrabber.getHeight());
    m_fbo.begin(); ofClear(0); m_fbo.end();
    
    m_roiRectangle.setFromCenter(m_fboCamera.getWidth()*0.5, m_fboCamera.getHeight()*0.5,m_fboCamera.getWidth()*0.25 , m_fboCamera.getHeight()*0.25);
    
    this->reallocateRoi();
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
        ofSetColor(255);
        m_fboCamera.draw( 0, 0, w, h);
        m_fboRoi.draw( w, 0, w, h);
        m_diffImg.draw( w, h,  w, h);
        //m_fboThresholded.draw( 0, h,  w, h);
        m_binaryImg.draw( 0, h,  w, h);
        m_contourFinder.draw(0, h,  w, h);
        this->drawRectangle();
    m_fbo.end();
}

void CamManager::drawRectangle()
{
    ofPushStyle();
    ofNoFill();
    ofSetColor(255);
    ofSetLineWidth(5);
    ofDrawRectangle(m_roiRectangle);
    ofPopStyle();
}

void CamManager::updateFboCamera()
{
    m_fboCamera.begin();
        ofClear(0);
        this->drawCamera();
    m_fboCamera.end();
    
    m_fboRoi.begin();
    ofClear(0);
    m_fboCamera.draw(-m_roiRectangle.x,-m_roiRectangle.y);
    m_fboRoi.end();
    
//    m_fboThresholded.begin();
//    ofClear(0);
//    m_thresholded.draw(0,0);
//    m_contourFinder.draw();
//    m_fboThresholded.end();
}

void CamManager::updateCv()
{
    ofPixels pixels;
    m_fboRoi.readToPixels(pixels);
    
    //m_colorImg.setFromPixels(pixels);
    m_colorImg.setFromPixels(pixels);
    //m_colorImg.mirror( false, true );
    
    m_grayImg =  m_colorImg;
    
    m_diffImg.absDiff( m_grayImg,  m_grayPrevImg);
    
    m_grayPrevImg =  m_colorImg;
    
    m_binaryImg =  m_diffImg;
    m_binaryImg.threshold( m_threshold);
    
    m_contourFinder.findContours( m_binaryImg,  m_minArea,  m_maxArea, 10, false);
    
    m_hysteresis.update(m_contourFinder.blobs.size()  > 0);
    
    if ( m_hysteresis.get()){
        
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
    m_fboRoi.readToPixels(pixels);
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

void CamManager::setRoiWidth(float& value)
{
    m_roiRectangle.width = value*m_fboCamera.getWidth();
    this->reallocateRoi();
}

void CamManager::setRoiHeight(float& value)
{
    m_roiRectangle.height = value*m_fboCamera.getHeight();
    this->reallocateRoi();
}

void CamManager::reallocateRoi()
{
    m_roiRectangle.setFromCenter(m_fboCamera.getWidth()*0.5, m_fboCamera.getHeight()*0.5,m_roiRectangle.getWidth(),m_roiRectangle.getHeight());
    
    m_fboRoi.allocate(m_roiRectangle.getWidth(),m_roiRectangle.getHeight(), GL_RGB);
    m_fboRoi.begin(); ofClear(0); m_fboRoi.end();
    
    m_fboThresholded.allocate(m_roiRectangle.getWidth(),m_roiRectangle.getHeight());
    m_fboThresholded.begin(); ofClear(0); m_fboThresholded.end();
    
    this->setupCv();
    
   // m_background.reset();
}
