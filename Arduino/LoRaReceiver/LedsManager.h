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
    
    LedsManager();
    
    void setup();
    void update();

    void setAllBlack();
    void setColor(CRGB color);
    void setColorPalette(CRGB* palette);
    
  private:

    void setupLeds();
    void setupPositions();
    void setupColorPalettes();
    void initTest();
    void checkFps();
    void noise16();

    CRGB leds[NUM_LEDS]; 
    CRGB palette[NUM_COLOR_PALETTE];
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;

    uint16_t real_x[NUM_LEDS]; 
    uint16_t real_y[NUM_LEDS];    

    uint16_t scale;
    uint32_t speed;
    uint8_t maxChanges;
};

LedsManager::LedsManager()
{
   this->scale = 500;
   this->speed = 8;
   this->maxChanges = 24; 
}
 
void LedsManager::setup()
{
    Serial.println("LedsManager::setup");
    this->setupLeds(); 
    this->setupPositions();
    this->setupColorPalettes();
    //this->initTest();
}


void LedsManager::setupLeds()
{
   FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
 
   //FastLED.setMaxPowerInVoltsAndMilliamps (5, 2100);
   //FastLED.setDither( 0 );
   FastLED.setBrightness(MAX_BRIGHTNESS);       // set to full power
   FastLED.clear();   
   this->setAllBlack();
   Serial.println("LedsManager::setupLeds");
}

void LedsManager::setupPositions()
{
   uint16_t shift_x = random16();                         
   uint16_t shift_y = random16();        
    
   for(uint16_t i = 0; i < NUM_LEDS; i++)
   {
        float rad = float(i)*TWO_PI/NUM_LEDS;
        real_x[i] = ( (cos(rad) + 1.0)*NUM_LEDS )*this->scale; 
        real_y[i] = ( (sin(rad) + 1.0)*NUM_LEDS )*this->scale; 
   }
   
   Serial.println("LedsManager::setupPositions");
}

void LedsManager::setupColorPalettes()
{
  this->currentPalette = CRGBPalette16(
                                   CRGB::Black, CRGB::Black, CRGB::Black, CHSV(0, 255,4),
                                   CHSV(0, 255, 8), CRGB::Red, CRGB::Red, CRGB::Red,                                   
                                   CRGB::DarkOrange,CRGB::Orange, CRGB::Orange, CRGB::Orange,
                                   CRGB::Yellow, CRGB::Yellow, CRGB::Gray, CRGB::Gray);

  this->targetPalette = this->currentPalette;
}

void LedsManager::update()
{
   this->checkFps();
   this->noise16();
   nblendPaletteTowardPalette( this->currentPalette, this->targetPalette, this->maxChanges);
   FastLED.show();
    
}

void LedsManager::checkFps()
{
    EVERY_N_MILLISECONDS(FPS_CHECK_TIME_MS) 
    {
      Serial.print("LedsManager::fsp-> ");
      Serial.println(LEDS.getFPS());                       
   }
}

void LedsManager::noise16()
{
    uint32_t real_z = millis() *this->speed;                          // the z position becomes quickly incremented
    for (uint16_t i = 0; i < NUM_LEDS; i++) 
    {
      uint8_t noise = inoise16(real_x[i], real_y[i], real_z)>>8;   // get the noise data and scale it down
      uint8_t bri = noise;                           // map LED color based on noise data
      uint8_t index   = noise;
  
      leds[i] = ColorFromPalette(this->currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    
    }
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
   uint8_t ind1 = random(NUM_COLOR_PALETTE);
   uint8_t ind2 = random(NUM_COLOR_PALETTE);
   
   //this->targetPalette = CRGBPalette16(color1,color2,color3,color4);
    
fill_solid( this->targetPalette, 16, CRGB::Black);
//    // and set every fourth one to a color.
  this->targetPalette[0] = palette[0];
  this->targetPalette[4] = palette[1];
  this->targetPalette[8] = palette[2];
  this->targetPalette[12] = palette[3];

  
   //this->setColor(pal[0]);
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
  fill_solid(leds,NUM_LEDS, CRGB::Black);
   FastLED.show();
  //FastLED.setBrightness(MAX_BRIGHTNESS);
}

    

