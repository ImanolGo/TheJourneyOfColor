///////////////////////////////////////////////////////////////////
// Class controlling the leds and its effects
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"
#include "FastLED.h"
#include "Config.h"

class LedsManager{

  public:
    
    LedsManager() {}
    
    void setup();
    void update();

    void setAllBlack();
    void setColor(CRGB color);
    void setColorPalette(CRGB* palette);
    
  private:

    void setupLeds();
    void initTest();

    CRGB leds[NUM_LEDS]; 
    CRGB palette[NUM_COLOR_PALETTE];    
};

void LedsManager::setup()
{
    Serial.println("LedsManager::setup");
    this->setupLeds(); 
    this->initTest();
}


void LedsManager::setupLeds()
{
   FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
 
   //FastLED.setMaxPowerInVoltsAndMilliamps (5, 2100);
   //FastLED.setDither( 0 );
   FastLED.clear();   
   this->setAllBlack();
   Serial.println("LedsManager::setupLeds");
}

void LedsManager::update()
{
    
}

void LedsManager::setAllBlack()
{ 
  fill_solid(leds,NUM_LEDS, CRGB::Black);
  FastLED.show();
 
}


void LedsManager::setColor(CRGB color)
{
  fill_solid(leds,NUM_LEDS, color);
  Serial.print("LedsManager::setColor, r = ");
  Serial.print(color.r);
  Serial.print(", g = ");
  Serial.print(color.g);
  Serial.print(", b = ");
  Serial.println(color.b);
  FastLED.show();
}



void LedsManager::setColorPalette(CRGB* palette)
{
   CRGB* pal = palette;  
   this->setColor(pal[0]);
}


void LedsManager::initTest() //runs at board boot to make sure pixels are working
{
  FastLED.setBrightness(MAX_BRIGHTNESS);       // set to full power
  
  FastLED.clear(); // on power up, one or more leds can be in a funky state.
  
  
  Serial.println ("Red:");
  fill_solid(leds,NUM_LEDS, CRGB::Red);
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::Green);
  Serial.println ("Green");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::Blue);
  Serial.println ("Blue");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::Yellow);
  Serial.println ("Yellow");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::Violet);
  Serial.println ("Violet");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::White);
  Serial.println ("White - Check Power!");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  FastLED.clear();
  Serial.println ("Rainbows!");
  fill_rainbow(leds,NUM_LEDS, CRGB::White);
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  Serial.println("Show Time...");
  FastLED.clear();
  //FastLED.setBrightness(MAX_BRIGHTNESS);
}

    

