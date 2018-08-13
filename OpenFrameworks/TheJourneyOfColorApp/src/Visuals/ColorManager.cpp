/*
 *  ColorManager.cpp
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */



#include "ofMain.h"

#include <time.h>

#include "ColorManager.h"
#include "AppManager.h"


const int ColorManager::NUM_COLORS = 5;

ColorManager::ColorManager(): Manager()
{
	//Intentionally left empty
}


ColorManager::~ColorManager()
{
    ofLogNotice() <<"ColorManager::Destructor" ;
}


void ColorManager::setup()
{
	if(m_initialized)
		return;


	Manager::setup();
    this->setupColors();
    
    
    ofLogNotice() <<"ColorManager::initialized" ;
    
}


void ColorManager::setupColors()
{
    for(int i=0; i<NUM_COLORS; i++ )
    {
        m_colorPalette.push_back(ofColor::black);
    }
}


void ColorManager::update()
{
    //To Implement
}

void ColorManager::draw()
{
    this->drawColor();
}

void ColorManager::drawColor()
{
    string name = "Color";
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    
    
    ofPushStyle();
        ofFill();
        ofSetColor(m_colorPalette.front());
        ofDrawRectangle(0,0, rect->getWidth(), rect->getHeight());
    ofPopStyle();
}

void ColorManager::setColor(ofColor& color, int index)
{
    if(index>=0 && index<m_colorPalette.size()){
        m_colorPalette[index] = color;
        ofLogNotice() <<"ColorManager::selected color index: " << index;
        ofLogNotice() <<"ColorManager::selected color: " << m_colorPalette[index].r << ", " <<  m_colorPalette[index].g << ", " <<  m_colorPalette[index].b;
    }
}

ofColor ColorManager::getColor(int index)
{
    if(index>0 && index<m_colorPalette.size()){
        return m_colorPalette[index];
    }
    
    return ofColor::black;
}


