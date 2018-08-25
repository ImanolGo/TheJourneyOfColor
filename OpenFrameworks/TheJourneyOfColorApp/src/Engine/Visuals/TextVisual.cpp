/*
 *  TextVisual.cpp
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */


#include "TextVisual.h"

TextVisual::TextVisual(ofVec3f pos, float width, float height, bool centred): BasicVisual(pos, width, height),m_fontSize(0), m_centred(centred), m_drawBB(false),m_lineHeight(1.0),m_alignment(OF_TEXT_ALIGN_LEFT)
{
    //Intentionally left empty
}

TextVisual::TextVisual(): BasicVisual(),m_fontSize(0), m_centred(false), m_drawBB(false), m_lineHeight(1.0),m_alignment(OF_TEXT_ALIGN_LEFT)
{
    //Intentionally left empty
}

TextVisual::~TextVisual()
{
    //Intentionally left empty
}

void TextVisual::setText(const std::string& text, const std::string& fontName, float fontSize, ofColor color)
{
    m_text = text;
    m_fontSize = fontSize;
    m_fontName = fontName;
    m_color = color;
    
    this->setText(m_text);
}

void TextVisual::setText(const std::string& text)
{
    if (m_text == text) {
        return;
    }
    
    m_text = text;
    
    m_textSuite.init(m_fontName,m_fontSize);
    m_textSuite.setText(m_text);
    m_textSuite.setLineHeight(m_lineHeight);
    m_textSuite.setColor(m_color.r, m_color.g, m_color.g, m_color.b);
    
    m_box = ofRectangle(0,0,m_textSuite.getWidth(), m_textSuite.getHeight());
    
    m_translation = ofVec3f(0);
    if(m_centred){
        m_translation.x -= m_box.getWidth()*0.5;
        m_translation.y -= m_box.getHeight()*0.5;
    }
}

void  TextVisual::setWidth(float width)
{
    m_width = width;
    this->setText(m_text);
}

void  TextVisual::setFontSize(int value)
{
    m_fontSize = value;
    this->setText(m_text);
}

void  TextVisual::setFontName(const string& name)
{
    m_fontName = name;
    this->setText(m_text);
}

void TextVisual::setLineHeight(float lineHeight)
{
    m_lineHeight= lineHeight;
    this->setText(m_text);
}

void TextVisual::setColor(const ofColor& color)
{
    m_color = color;
    this->setText(m_text);
}

void TextVisual::draw()
{
    
    ofPushMatrix();
    ofPushStyle();
    
    ofScale(m_scale.x, m_scale.y);
    
    switch (m_alignment) {
            
        case OF_TEXT_ALIGN_LEFT:
            m_textSuite.draw(m_position.x,m_position.y);
            break;
        case OF_TEXT_ALIGN_RIGHT:
            m_textSuite.drawRight(m_position.x + m_box.getWidth(), m_position.y);
            break;
        case OF_TEXT_ALIGN_CENTER:
            m_textSuite.drawCenter(m_position.x,m_position.y - m_box.getHeight());
            break;
        case OF_TEXT_ALIGN_JUSTIFIED:
            m_textSuite.drawJustified(m_position.x,m_position.y, m_width);
            break;
            
    }
    
    ofPopStyle();
    ofPopMatrix();   // recall the pushed style
}

