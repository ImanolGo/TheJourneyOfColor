/*
 *  CamManager.h
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxPSLevels.h"
#include "ofxBlur.h"
#include "ofxOpenCv.h"
#include "ofxTiming.h"

//========================== class CamManager ==============================
//============================================================================
/** \class CamManager CamManager.h
 *	\brief Class managing the camera processing
 *	\details it reads the freames from the web camera and process them
 */


class CamManager: public Manager
{
    
    
public:
    
    //! Constructor
    CamManager();
    
    //! Destructor
    ~CamManager();
    
    //! Setup the Video Manager
    void setup();
    
    //! Update the Video Manager
    void update();
    
    //! Draw the Video Manager
    void draw();
    
    void setContrast(float& value) {m_levels.contrast = value;}
    
    void setSaturation(float& value) {m_levels.saturation = value;}
    
    void setBrightness(float& value) {m_levels.brightness = value;}
    
    void setGamma(float& value) {m_levels.gamma = value;}
    
    void setMinInput(float& value) {m_levels.minInput = value;}
    
    void setMaxInput(float& value) {m_levels.maxInput = value;}
    
    void setMinOutput(float& value) {m_levels.minOutput = value;}
    
    void setMaxOutput(float& value) {m_levels.maxOutput = value;}
    
    void setImageThreshold(float& value) {m_threshold = value;}
    
    void setMinArea(float& value) {m_minArea = value;}
    
    void setMaxArea(float& value) {m_maxArea = value;}
    
    void setRoiWidth(float& value);
    
    void setRoiHeight(float& value);
    
    void setBlurScale(float& value);
    
    const ofFbo& getFbo() const{return m_fbo;}
    
private:
    
    
    void setupFbo();
    
    void setupCv();
    
    void setupCamera();
    
    void updateCamera();
    
    void updateCv();
    
    void updateFbo();
    
    void updateFboCamera();
    
    void updateQuantization();
    
    void drawCamera();
    
    void drawRectangle();
    
    void drawFbo();
    
    void setupShaders(float width,float height);
    
    void setupBlur(float width, float height);
    
    void setupLevels(float width, float height);
    
    void reallocateRoi();
    
    
    
private:
    
    
    ofVideoGrabber      m_vidGrabber;
    int                 m_camWidth;
    int                 m_camHeight;
    
    ofxCvColorImage            m_colorImg;
    ofxCvGrayscaleImage        m_grayImg;
    ofxCvGrayscaleImage        m_grayPrevImg;
    ofxCvGrayscaleImage        m_diffImg;
    ofxCvGrayscaleImage        m_binaryImg;
    ofxCvContourFinder         m_contourFinder;
    
//    ofxCv::ContourFinder        m_contourFinder;
//    ofxCv::RunningBackground    m_background;
    ofImage                     m_thresholded;
    
    ofxPSLevels     m_levels;
    ofxBlur         m_blur;
    
    ofFbo           m_fboCamera;
    ofFbo           m_fbo;
    ofFbo           m_fboRoi;
    ofFbo           m_fboThresholded;
    
    ofRectangle     m_roiRectangle;
    
    Hysteresis      m_hysteresis;
    
    float           m_threshold;
    float           m_minArea;
    float           m_maxArea;
    float           m_learningTime;
    
    bool            m_isMotionDetected;
    bool            m_prevMotionDetected;

};




