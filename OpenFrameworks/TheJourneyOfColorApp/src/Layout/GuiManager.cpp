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
    //this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    
    this->setupGuiParameters();
    this->setupColorsGui();
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
    
}


void GuiManager::setupLevelsGui()
{
    auto cameraManager = &AppManager::getInstance().getCameraManager();
   
    m_contrast.set("Contrast", 1.0, 0.0, 2.0);
    m_contrast.addListener(cameraManager, &CameraManager::setContrast);
    m_parameters.add(m_contrast);
    
    m_saturation.set("Saturation", 1.0, 0.0, 2.0);
    m_saturation.addListener(cameraManager, &CameraManager::setSaturation);
    m_parameters.add(m_saturation);
    
    m_brightness.set("Brightness", 1.0, 0.0, 2.0);
    m_brightness.addListener(cameraManager, &CameraManager::setBrightness);
    m_parameters.add(m_brightness);
    
    m_gamma.set("Gamma", 1.0, 0.0, 2.0);
    m_gamma.addListener(cameraManager, &CameraManager::setGamma);
    m_parameters.add(m_gamma);
    
    m_blur.set("Blur", 0.0, 0.0, 10.0);
    m_blur.addListener(cameraManager, &CameraManager::setBlurScale);
    m_parameters.add(m_blur);
    
    m_minInput.set("MinInput", 0.0, 0.0, 1.0);
    m_minInput.addListener(cameraManager, &CameraManager::setMinInput);
    m_parameters.add(m_minInput);
    
    m_maxInput.set("MaxInput", 1.0, 0.0, 1.0);
    m_maxInput.addListener(cameraManager, &CameraManager::setMaxInput);
    m_parameters.add(m_maxInput);
    
    m_minOutput.set("MinOutput", 0.0, 0.0, 1.0);
    m_minOutput.addListener(cameraManager, &CameraManager::setMinOutput);
    m_parameters.add(m_minOutput);
    
    m_maxOutput.set("MaxOutput", 1.0, 0.0, 1.0);
    m_maxOutput.addListener(cameraManager, &CameraManager::setMaxOutput);
    m_parameters.add(m_maxOutput);

    
    ofxDatGuiFolder* folder = m_gui.addFolder("GENERAL", ofColor::purple);
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
    //ofSerialize(xml, m_parameters);
    xml.serialize(m_parameters);
    
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
    
    xml.deserialize(m_parameters);
    //ofDeserialize(xml, m_parameters);
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
