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
    
    
    ofLogNotice() <<"ColorManager::initialized" ;
    
}


void ColorManager::update()
{
    //To Implement
}

void ColorManager::draw()
{
     //To Implement
}

