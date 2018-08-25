/*
 *  ImageVisual.cpp
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */


#include "AppManager.h"
#include "ResourceManager.h"

#include "ImageVisual.h"


ImageVisual::ImageVisual(): BasicVisual(), m_centred(false)
{
    //this->setResource("");
}


ImageVisual::ImageVisual(const ofVec3f& pos, const string& resourceName,bool centred):
    BasicVisual(pos, 0, 0), m_centred(centred)
{
    this->setResource(resourceName, true);
}

ImageVisual::~ImageVisual()
{
    //Intentionally left empty
}

bool ImageVisual::setResource(const string& resourceName, bool forceResize)
{
    m_texture = AppManager::getInstance().getResourceManager().getTexture(resourceName);

    m_originalWidth = m_texture->getWidth();
    m_originalHeight = m_texture->getHeight();
    
    if(forceResize)
    {
        m_width = m_texture->getWidth();
        m_height =  m_texture->getHeight();
    }
   

    return true;
}

void ImageVisual::draw()
{
     if(!m_texture->bAllocated()){
        return;
    }
    
    ofPushMatrix();
    ofPushStyle();

        ofTranslate(m_position);

        ofScale(m_scale.x, m_scale.y);

        if(m_centred){
            ofTranslate(-m_width*0.5,-m_height*0.5);
        }
    
        ofPushMatrix();
        ofTranslate(m_width*0.5,m_height*0.5, 0);//move pivot to centre
            ofRotateX(m_rotation.x);
            ofRotateY(m_rotation.y);
            ofRotateZ(m_rotation.z);//rotate from centre
        ofPushMatrix();
        ofTranslate(-m_width*0.5,-m_height*0.5,0);//move back by the centre offset
            ofSetColor(m_color);
            m_texture->draw(0,0,m_width,m_height);
        ofPopMatrix();
        ofPopMatrix();


    ofPopStyle();
    ofPopMatrix();
}

void ImageVisual::setWidth(float width, bool keepRatio)
{
    m_width = width;
    if(keepRatio){
        float ratio = m_originalWidth/m_originalHeight;
        m_height = m_width/ratio;
    }
}

void ImageVisual::setHeight(float height, bool keepRatio)
{
   m_height = height;
    if(keepRatio){
        float ratio = m_originalWidth/m_originalHeight;
        m_width = m_height*ratio;
    }

}

ofTexture & ImageVisual::getTexture()
{
    return (*m_texture.get());
}
