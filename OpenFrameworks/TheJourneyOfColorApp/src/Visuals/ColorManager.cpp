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
    this->setupQuantizer();
    
    
    ofLogNotice() <<"ColorManager::initialized" ;
    
}


void ColorManager::setupColors()
{
    for(int i=0; i<NUM_COLORS; i++ )
    {
        m_colorPalette.push_back(ofColor::black);
    }
}

void ColorManager::setupQuantizer()
{
    m_colorQuantizer.setNumColors(NUM_COLORS);
}

void ColorManager::update()
{
    //To Implement
}


void ColorManager::quantizeColors(const ofPixels & pixels)
{
    m_colorQuantizer.quantize(pixels);
    m_colorPalette = m_colorQuantizer.getColors();
    
    // using default comparison (operator <):
    //std::sort (m_colorPalette.begin(), m_colorPalette.end());
    
    std::sort(m_colorPalette.begin(),
              m_colorPalette.end(),
              [](const ofColor& lhs, const ofColor& rhs)
    {
        return lhs.getHue() < rhs.getHue();
    });
    
//    for(auto& color: m_colorPalette){
//        color.setSaturation(255);
//    }
}

void ColorManager::draw()
{
    //this->drawColor();
    this->drawPalette();
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

void ColorManager::drawPalette()
{
    string name = "Color";
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    
    float width = rect->getWidth()/m_colorPalette.size();
    float height = rect->getHeight();
    
    ofPushStyle();
    ofFill();
    for(int i=0; i<m_colorPalette.size(); i++)
    {
        ofSetColor(m_colorPalette[i]);
        ofDrawRectangle(i*width,0, width,height);
    }
    
     ofPopStyle();
    
}


void ColorManager::setColor(ofColor& color, int index)
{
    if(index>=0 && index<m_colorPalette.size()){
        m_colorPalette[index] = color;
        ofLogNotice() <<"ColorManager::selected color index: " << index;
        ofLogNotice() <<"ColorManager::selected color: " << (int) m_colorPalette[index].r << ", " <<  (int)m_colorPalette[index].g << ", " <<  (int)m_colorPalette[index].b;
        AppManager::getInstance().getSerialManager().sendColors(m_colorPalette);
    }
}

ofColor ColorManager::getColor(int index)
{
    if(index>0 && index<m_colorPalette.size()){
        return m_colorPalette[index];
    }
    
    return ofColor::black;
}


