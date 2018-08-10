/*
 *  CameraManager.h
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxPSLevels.h"
#include "ofxBlur.h"

//========================== class CameraManager ==============================
//============================================================================
/** \class CameraManager CameraManager.h
 *	\brief Class managing the camera processing
 *	\details it reads the freames from the web camera and process them
 */


class CameraManager: public Manager
{
    
    
public:
    
    //! Constructor
    CameraManager();
    
    //! Destructor
    ~CameraManager();
    
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
    
    void setBlurScale(float& value);
    
private:
    
    void setupCamera();
    
    void updateCamera();
    
    void drawCamera();
    
    void setupShaders(float width,float height);
    
    void setupBlur(float width, float height);
    
    void setupLevels(float width, float height);
    
private:
    
    
    ofVideoGrabber      m_vidGrabber;
    int                 m_camWidth;
    int                 m_camHeight;
    
    ofxPSLevels     m_levels;
    ofxBlur         m_blur;

};




