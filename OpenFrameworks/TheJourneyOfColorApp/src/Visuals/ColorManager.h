/*
 *  ColorManager.h
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */


#pragma once

#include "Manager.h"

#include "ofxOpenCv.h"
#include "ofxColorQuantizer.h"


//========================== class ColorManager ==============================
//============================================================================
/** \class ColorManager ColorManager.h
 *	\brief Class managing the color palettes
 *	\details it reads from the camera feed
 */


class ColorManager: public Manager
{
    
    static const int NUM_COLORS;
    
    public:

        //! Constructor
        ColorManager();

        //! Destructor
        ~ColorManager();

        //! Setup the Color Manager
        void setup();
    
        //! Update the Color Manager
        void update();
    
        //! Draw the Color Manager
        void draw();
    
        int getNumberOfColors() {return m_colorPalette.size();}
    
        void setColor(ofColor& color, int index);
    
        ofColor getColor(int index);
    
        void quantizeColors(const ofPixels & pixels);
    
    
    private:
    
        void setupColors();
    
        void setupQuantizer();
    
        void drawColor();
    
        void drawPalette();
    
    
    private:
    
        typedef  vector< ofColor> ColorPalette;  ///< defines a vector of ofColors
    
        ColorPalette            m_colorPalette;
        ofxColorQuantizer       m_colorQuantizer;
    

};




