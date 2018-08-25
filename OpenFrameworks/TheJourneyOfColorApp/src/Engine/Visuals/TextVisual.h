/*
 *  TextVisual.h
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */

#pragma once


#include "ofxTextSuite.h"
#include "BasicVisual.h"


//=========================== class TextVisual ==============================
//============================================================================
/** \class TextVisual Visual.h
 *    \brief Represents an image visual
 *    \details The class uses the OF class ofImage to draw the image in a 3D world
 */

class TextVisual: public BasicVisual
{
public:
    
    //! Constructor
    TextVisual(ofVec3f pos, float width, float height, bool centred = false);
    
    //! Constructor
    TextVisual();
    
    //! Destructor
    virtual ~TextVisual();
    
    //! Draws the text visual
    virtual void draw();
    
    //! Sets the text to be drawn
    virtual void setText(const std::string& text, const std::string& fontName, float fontSize, ofColor color = ofColor(0,0,0));
    
    //! Sets the text to be drawn
    virtual void setText(const std::string& text);
    
    //! Sets the height between lines
    virtual void  setLineHeight(float lineHeight);
    
    //! Sets the the width of the visual
    virtual void setWidth(float width);
    
    //! Gets the width of the current bounding box
    virtual float getWidth() const {return m_box.getWidth();}
    
    //! Gets the height of the current bounding box
    virtual float getHeight() {return m_box.getHeight();}
    
    virtual void setColor(const ofColor& color);
    
    const string& getText(){return m_text;}
    
    void setCentred(bool centred) {m_centred = centred;}
    
    void drawBoundingBox(bool draw){m_drawBB = draw;}
    
    void setFontSize(int value);
    
    void setFontName(const string& name);
    
    void setAligment(TextBlockAlignment& aligment) {m_alignment = aligment;}
    
private:
    
    std::string       m_text;        ///< text to be rendered
    std::string       m_fontName;    ///< saves the fontName
    float             m_fontSize;    ///< saves the font size
    float             m_lineHeight;  ///< saves the current line height;
    bool              m_centred;     ///< determines whether the visual is centred or not
    ofRectangle       m_box;         ///< the box surrounding the text
    ofVec3f           m_translation; ///< the point to which shift the origin
    bool              m_drawBB;      ///< determines whether the bounding box should be drawn or not
    
    ofxTextBlock        m_textSuite;
    TextBlockAlignment  m_alignment;  //constants for controlling state
    
    
};

