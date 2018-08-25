/*
 *  GuiManager.cpp
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */


#include "ofMain.h"

#include "AppManager.h"

#include "GuiManager.h"
#include "GuiTheme.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "GUI";
//const int GuiManager::GUI_WIDTH = 350;


GuiManager::GuiManager(): Manager(), m_showGui(true)
{
    //Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    
    this->setupGuiParameters();
    this->setupColorsGui();
    this->setupCameraGui();
    this->setupLevelsGui();

    this->setupGuiEvents();
    this->loadGuiValues();

    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    
    ofxDatGuiLog::quiet();
    
    //m_gui.setPosition(ofxDatGuiAnchor::TOP_LEFT);
    m_gui.setPosition(0,0);
    //m_gui.setAssetPath(ofToDataPath("fonts/"));
    //m_gui.setAssetPath(ofToDataPath("fonts/"));
    //m_gui.setAssetPath("../Resources/data/fonts/");
    m_gui.setTheme(new GuiTheme());
    
    
    int margin =  LayoutManager::MARGIN;
    m_gui.setAutoDraw(false);
    auto pos = m_gui.getPosition();
    m_gui.setPosition(pos.x + margin, pos.y + margin);
    m_gui.addHeader(GUI_SETTINGS_NAME, false);
    
    m_gui.addFRM(0.1);
    
    m_gui.addBreak();
}

void GuiManager::setupColorsGui()
{
    ofxDatGuiFolder* folder = m_gui.addFolder("PALETTE", ofColor::pink);
    int numColors = AppManager::getInstance().getColorManager().getNumberOfColors();
    for(int i = 0; i < numColors; i++)
    {
        auto color = AppManager::getInstance().getColorManager().getColor(i);
        string colorName = "COLOR " + ofToString(i);
        folder->addColorPicker(colorName, color);
    }
    // f1->expand();
    m_gui.addBreak();
    
}


void GuiManager::setupLevelsGui()
{
    auto camManager = &AppManager::getInstance().getCamManager();
   
    m_contrast.set("Contrast", 1.0, 0.0, 2.0);
    m_contrast.addListener(camManager, &CamManager::setContrast);
    m_parameters.add(m_contrast);
    
    m_saturation.set("Saturation", 1.0, 0.0, 2.0);
    m_saturation.addListener(camManager, &CamManager::setSaturation);
    m_parameters.add(m_saturation);
    
    m_brightness.set("Brightness", 1.0, 0.0, 2.0);
    m_brightness.addListener(camManager, &CamManager::setBrightness);
    m_parameters.add(m_brightness);
    
    m_gamma.set("Gamma", 1.0, 0.0, 2.0);
    m_gamma.addListener(camManager, &CamManager::setGamma);
    m_parameters.add(m_gamma);
    
    m_blur.set("Blur", 0.0, 0.0, 10.0);
    m_blur.addListener(camManager, &CamManager::setBlurScale);
    m_parameters.add(m_blur);
    
    m_minInput.set("MinInput", 0.0, 0.0, 1.0);
    m_minInput.addListener(camManager, &CamManager::setMinInput);
    m_parameters.add(m_minInput);
    
    m_maxInput.set("MaxInput", 1.0, 0.0, 1.0);
    m_maxInput.addListener(camManager, &CamManager::setMaxInput);
    m_parameters.add(m_maxInput);
    
    m_minOutput.set("MinOutput", 0.0, 0.0, 1.0);
    m_minOutput.addListener(camManager, &CamManager::setMinOutput);
    m_parameters.add(m_minOutput);
    
    m_maxOutput.set("MaxOutput", 1.0, 0.0, 1.0);
    m_maxOutput.addListener(camManager, &CamManager::setMaxOutput);
    m_parameters.add(m_maxOutput);

    
    ofxDatGuiFolder* folder = m_gui.addFolder("LEVELS", ofColor::purple);
    folder->addSlider(m_contrast);
    folder->addSlider(m_saturation);
    folder->addSlider(m_brightness);
    folder->addSlider(m_gamma);
    folder->addSlider(m_blur);
    folder->addSlider(m_minInput);
    folder->addSlider(m_maxInput);
    folder->addSlider(m_minOutput);
    folder->addSlider(m_maxOutput);
    folder->expand();

    m_gui.addBreak();
}


void GuiManager::setupCameraGui()
{
    auto camManager = &AppManager::getInstance().getCamManager();
    
    m_motionDetected.set("Motion", false);
    
    m_imgThreshold.set("Image Threshold", 128, 0, 255);
    m_imgThreshold.addListener(camManager, &CamManager::setImageThreshold);
    m_parameters.add(m_imgThreshold);
    
    m_minArea.set("Min Area", 10, 1, 100);
    m_minArea.addListener(camManager, &CamManager::setMinArea);
    m_parameters.add(m_minArea);
    
    m_maxArea.set("Max Area", 200, 1, 500);
    m_maxArea.addListener(camManager, &CamManager::setMaxArea);
    m_parameters.add(m_maxArea);
    
    m_learningTime.set("LearningTime", 2, 0, 10);
    //m_learningTime.addListener(camManager, &CamManager::setLearningTime);
    m_parameters.add(m_learningTime);
    
    m_backgThreshold.set("Background Threshold", 10, 0, 255);
   // m_backgThreshold.addListener(camManager, &CamManager::setThresholdBackground);
    m_parameters.add(m_backgThreshold);
    
    m_roiWidth.set("Roi Width", 0.5, 0.1, 1.0);
    m_roiWidth.addListener(camManager, &CamManager::setRoiWidth);
    m_parameters.add(m_roiWidth);
    
    m_roiHeight.set("Roi Height", 0.5, 0.1, 1.0);
    m_roiHeight.addListener(camManager, &CamManager::setRoiHeight);
    m_parameters.add(m_roiHeight);
    
    
    ofxDatGuiFolder* folder = m_gui.addFolder("CAMERA", ofColor::green);
    folder->addToggle("Motion");
    folder->addSlider(m_imgThreshold);
    //folder->addSlider(m_backgThreshold);
    //folder->addSlider(m_learningTime);
    folder->addSlider(m_minArea);
    folder->addSlider(m_maxArea);
    folder->addSlider(m_roiWidth);
    folder->addSlider(m_roiHeight);
    
    folder->expand();
    
    m_gui.addBreak();
}


void GuiManager::update()
{
    m_gui.update();
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    //this->drawRectangle();
    this->drawGui();
}


void GuiManager::drawGui()
{
    ofEnableAlphaBlending();
    m_gui.draw();
    ofDisableAlphaBlending();
}

void GuiManager::setupGuiEvents()
{
    //m_gui.onDropdownEvent(this, &GuiManager::onDropdownEvent);
    m_gui.onColorPickerEvent(this, &GuiManager::onColorPickerEvent);
    m_gui.onButtonEvent(this, &GuiManager::onButtonEvent);
    //m_gui.onToggleEvent(this, &GuiManager::onToggleEvent);
    //m_gui.onMatrixEvent(this, &GuiManager::onMatrixEvent);
}


void GuiManager::saveGuiValues(string path)
{
    ofXml xml;
    ofSerialize(xml, m_parameters);
    //xml.serialize(m_parameters);
    
    if(path.empty()){
        xml.save(GUI_SETTINGS_FILE_NAME);
    }
    else{
        xml.save(path);
    }
    
    
}

void GuiManager::loadGuiValues(string path)
{
    ofXml xml;
    if(path.empty()){
         xml.load(GUI_SETTINGS_FILE_NAME);
    }
    else{
         xml.load(path);
    }
    
    //xml.deserialize(m_parameters);
    ofDeserialize(xml, m_parameters);
}



void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::drawRectangle()
{
    int margin =  LayoutManager::MARGIN;
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( m_gui.getPosition().x - margin, 0, m_gui.getWidth() + 2*margin, ofGetHeight());
    ofPopStyle();
}



void GuiManager::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getName() << " Selected" << endl;
    
}

void GuiManager::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getName() << " Selected" << endl;
    
    int numColors = AppManager::getInstance().getColorManager().getNumberOfColors();
    
    for(int i=0; i<numColors; i++ ){
        string name = "COLOR " + ofToString(i);
        if (e.target->getName() == name){
            AppManager::getInstance().getColorManager().setColor(e.color, i);
        }
    }
   
}

void GuiManager::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getName() << " Selected" << endl;
    
}


void GuiManager::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getName() << " Selected" << endl;
    
}

void GuiManager::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    cout << "onMatrixEvent " << e.child << " : " << e.enabled << endl;
}

void GuiManager::setMotionDetected(bool value)
{
    m_gui.getToggle("Motion")->setChecked(value);
}

