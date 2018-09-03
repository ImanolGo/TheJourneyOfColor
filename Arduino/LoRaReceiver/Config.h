#pragma once


/*************************************************************************
 Protocol
 *************************************************************************/

#define CHANNEL 0
#define HEADER_SIZE 6
#define SIZE_INDEX 3
#define COMMAND_INDEX 4
#define CHANNEL_INDEX 5
#define COLOR_INDEX 6
#define NUM_BYTES 3

/*************************************************************************
 Leds
 *************************************************************************/

#define DATA_PIN    12
#define CLOCK_PIN    A4
#define LED_TYPE    DOTSTAR
#define COLOR_ORDER BGR
#define NUM_LEDS 144
#define NUM_COLOR_PALETTE 10
#define MAX_BRIGHTNESS 100
#define TEST_DELAY 500
#define CHANNEL_WIDTH 3


/*************************************************************************
 LoRa
 *************************************************************************/

//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0
