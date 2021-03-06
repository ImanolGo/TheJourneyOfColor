/*
 *  GuiManager.h
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxDatGui.h"


//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *    \brief Class managing the application´s grapical user interface (GUI)
 *    \details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
   // static const int GUI_WIDTH;
    
public:
    
    //! Constructor
    GuiManager();
    
    //! Destructor
    ~GuiManager();
    
    //! Set-up the gui
    void setup();
    
    //! update the gui
    void update();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues(string path = "");
    
    void loadGuiValues(string path = "");
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth() {return m_gui.getWidth();}
    
    int getHeight() {return m_gui.getHeight();}
    
    ofPoint  getPosition() {return m_gui.getPosition();}
    
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    void onToggleEvent(ofxDatGuiToggleEvent e);
    
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    
    void setMotionDetected(bool value);
    
    void setSerialConnected(bool value);
    
    
private:
    
    void setupGuiParameters();
    
    void setupLevelsGui();
    
    void setupColorsGui();
    
    void setupCameraGui();
    
    void drawRectangle();
    
    void drawGui();
    
    void setupGuiEvents();
    
    
private:
    
    ofxDatGui            m_gui;
    
    ofParameter<float>    m_guiFPS;
    
    
    ofParameterGroup      m_parameters;
    
    ofParameter<float>      m_blur;
    ofParameter<float>      m_contrast;
    ofParameter<float>      m_saturation;
    ofParameter<float>      m_brightness;
    ofParameter<float>      m_gamma;
    ofParameter<float>      m_minInput;
    ofParameter<float>      m_maxInput;
    ofParameter<float>      m_minOutput;
    ofParameter<float>      m_maxOutput;
    
    ofParameter<float>        m_imgThreshold;
    ofParameter<float>        m_minArea;
    ofParameter<float>        m_maxArea;
    ofParameter<float>        m_learningTime;
    ofParameter<float>        m_backgThreshold;
    ofParameter<float>        m_roiWidth;
    ofParameter<float>        m_roiHeight;
    ofParameter<bool>           m_motionDetected;
    ofParameter<bool>           m_serialConnected;

    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


