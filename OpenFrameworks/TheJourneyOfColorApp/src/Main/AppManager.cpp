/*
 *  AppManager.cpp
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */

#include "ofMain.h"

#include "AppManager.h"

AppManager& AppManager::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static AppManager    m_instance;
    return m_instance;

}

AppManager::AppManager(): Manager(), m_debugMode(false), m_initialized(false)
{
   //Intentionally left empty
}


AppManager::~AppManager()
{
    ofLogNotice() <<"AppManager::Destructor";
}


void AppManager::setup()
{
	if(m_initialized)
		return;
    
    ofLogNotice() << "AppManager::initialized";

	Manager::setup();
    
    this->setupOF();
	this->setupManagers();
    
    m_initialized = true;
    
}

void AppManager::setupOF()
{
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetVerticalSync(true);
    ofSetEscapeQuitsApp(false);
    //ofSetDataPathRoot("../Resources/data/");
    ofSetBackgroundAuto(true);
}


void AppManager::setupManagers()
{
    m_settingsManager.setup();
    m_resourceManager.setup();
    m_viewManager.setup();
    m_visualEffectsManager.setup();
    m_layoutManager.setup();
    m_colorManager.setup();
    m_cameraManager.setup();
    m_serialManager.setup();
    m_keyboardManager.setup();
    
    m_guiManager.setup();
}

void AppManager::update()
{
    if(!m_initialized)
        return;
    
    m_guiManager.update();
    m_visualEffectsManager.update();
    m_viewManager.update();
    m_cameraManager.update();
    m_colorManager.update();
    m_serialManager.update();
    m_layoutManager.update();
}


void AppManager::draw()
{
    
    if(!m_initialized)
        return;
    
    
    ofBackground(17,17,17);
    m_layoutManager.draw();
    m_guiManager.draw();    
}

void AppManager::toggleDebugMode()
{
    m_debugMode = !m_debugMode;
    setDebugMode(m_debugMode);
}


void AppManager::setDebugMode(bool showDebug)
{
    m_debugMode = showDebug;
    
    ofLogNotice()<<"AppManager::setDebugMode-> " << m_debugMode;
    
    if(m_debugMode){
        //ofSetLogLevel(OF_LOG_VERBOSE);
    }
    else{
        ofSetLogLevel(OF_LOG_NOTICE);
    }
    

    m_guiManager.showGui(m_debugMode);
    
}



