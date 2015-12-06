/*
  "Christmas Tree Lights"
  JormaSnow--2015-12-05

  Free to use with or without attibution--there isn't anything special here.

  I'm not much of a Christmas person really, but some friends comissioned
  me to design and make a decoration. The easiest part of this is was festive
  Arduino-controlled lights--thanks to the wonderful Adafruit NeoPixel library.

  The operation is simple: 
  
  A switch statement is used to select any combination of R, G & B based on a 
  simple random() function. There is an approximately equal chance for any 
  combination of R, G & B, based on the "fairness" of random().
  
  The ratio between how many LEDs are on or off is determined by the likelyhood 
  of hitting the default case in the switch statement, which turns an LED off.
  Selecting the default case is determined by the range of the random() function
  that falls outside of the cases which select combinations of R, G & B values.

  changeColor() is then called on a random() LED between 0 and the max number of
  LEDs on the strand (LED_COUNT below).

  changeColor is also fed random values for each of the channels that it is 
  going to change, gets the current value of all channels and loops to 
  increment or decrement the current value toward the new value until new and 
  current values are equal.
  
  There are "parameters" near the top of the code in the #defines which adjust
  some values to change brightness, speed, etc. without tweaking the logic.

  It is important to ensure that the number of LEDs and the data pin are
  correct. The other defaults should work without adjustment.

  Requires the Adafruit NeoPixel library.
  https://github.com/adafruit/Adafruit_NeoPixel
  
  Compiled and run on AdaFruit Trinket 8MHz using Arduino 1.6.6, USBtinyISP
  
  Tested with 60 Breadboardable WS2812 LEDs from SparkFun--should work with
  any of the many flavors of WS2812 LED.
*/

#include <Adafruit_NeoPixel.h>

// IMPORTANT control pin and LED count need to match the physical circuit in order for the code to work correctly

#define PIXEL_PIN 0  // the NeoPixel control pin
#define LED_COUNT 60 // the total number of LEDs on the string--NeoPixel contructor argument

// PARAMETERS--these values may be varied to adjust the operation of the LEDs
#define VMAX       65   // the maximum value that can be sent to any one of the RGB channels--255 is the upper limit
#define VMIN       15   // the minimum non-zero value that can be sent to any one of the RGB channels
#define SLOW       1500 // slows down the transition between values when changing LED RGB values in microseconds
#define SWITCH_MAX 13   // the max value for the switch statement--should be >7. See comment below
#define SWITCH_MIN 1    // the minimum for the switch statement--should be 1. See comment below

/*
  --------------------------SWITCH_MIN and SWITCH_MAX--------------------------
  The main switch function below choses which RGB channels to change using 
  random(SWITCH_MIN, SWITCH_MAX). Values 1 through 7 (see constants below) 
  select combination of channels to change. values outside that range will hit
  the switch default case and turn the LED off.
  Therefor a SWITCH_MIN > 1 or SWITCH_MAX < 8 will omit cases.
  SWITCH_MIN 1 and SWITCH_MAX 8 would mean that the default case should never 
  get called; The greater the value of SWITCH_MAX above 8 determines a greater 
  probability of the default case gettign hit. 
  With SWITCH_MIN at 1 the ratio of the probability of an LED being turned off
  is approximately (SWITCH_MAX-7)/7:1
*/

// constants for the switch statement
#define RED            1
#define GREEN          2
#define RED_AND_GREEN  3
#define BLUE           4
#define RED_AND_BLUE   5
#define GREEN_AND_BLUE 6
#define RGB            7

// Create an instance of the Adafruit_NeoPixel class called "leds"
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIXEL_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  pinMode(PIXEL_PIN, OUTPUT);
  leds.begin();
  leds.show(); //clears the leds
}

void loop() {
  // switches on the range of values representing: Red(1), Green(2) and Blue(4) and their sums
  // to determin the new color of a random LED. Values greater than 7 represent BLACK and are
  // handled by the default condition, allowing for the possiblity for an approzimate percentage
  // of leds to be black at any time.
  switch (random(SWITCH_MIN, SWITCH_MAX)) {
    case RED: // sends random value to the red led and 0 to others
      changeColor(random(LED_COUNT), (uint8_t)random(VMIN, VMAX), 0x00, 0x00);
      break;
    case GREEN:// sends random value to the green led and 0 to others
      changeColor(random(LED_COUNT), 0x00, (uint8_t)random(VMIN, VMAX), 0x00);
      break;
    case RED_AND_GREEN: // sends random value to the red and green leds and 0 to blue
      changeColor(random(LED_COUNT), (uint8_t)random(VMIN, VMAX), (uint8_t)random(VMIN, VMAX), 0x00);
      break;
    case BLUE: // sends random value to the blue led and 0 to others
      changeColor(random(LED_COUNT), 0x00, 0x00, (uint8_t)random(VMIN, VMAX));
      break;
    case RED_AND_BLUE: // sends random value to the red and blue leds and 0 to green
      changeColor(random(LED_COUNT), (uint8_t)random(VMIN, VMAX), 0x00, (uint8_t)random(VMIN, VMAX));
      break;
    case GREEN_AND_BLUE: // sends random value to the green and blue leds and 0 to red
      changeColor(random(LED_COUNT), 0x00, (uint8_t)random(VMIN, VMAX), (uint8_t)random(VMIN, VMAX));
      break;
    case RGB: // sends random value to the each of the leds
      changeColor(random(LED_COUNT), (uint8_t)random(VMIN, VMAX), (uint8_t)random(VMIN, VMAX), (uint8_t)random(VMIN, VMAX));
      break;
    default:
      changeColor(random(LED_COUNT), 0x00, 0x00, 0x00);
      break;
  }
}

void changeColor(long led, uint8_t newR, uint8_t newG, uint8_t newB) {
  //capture the current state of the led
  uint8_t currentR = (leds.getPixelColor(led) >> 16);
  uint8_t currentG = (leds.getPixelColor(led) >>  8);
  uint8_t currentB = (leds.getPixelColor(led)      );

  boolean changing = true;
  while (changing) {
    changing = false;
    if (currentR < newR) {
      currentR++;
      leds.setPixelColor(led, currentR, currentG, currentB);
      changing = true;
    } else if (currentR > newR) {
      currentR--;
      leds.setPixelColor(led, currentR, currentG, currentB);
      changing = true;
    }

    if (currentG < newG) {
      currentG++;
      leds.setPixelColor(led, currentR, currentG, currentB);
      changing = true;
    } else if (currentG > newG) {
      currentG--;
      leds.setPixelColor(led, currentR, currentG, currentB);
      changing = true;
    }

    if (currentB < newB) {
      currentB++;
      leds.setPixelColor(led, currentR, currentG, currentB);
      changing = true;
    } else if (currentB > newB) {
      currentB--;
      leds.setPixelColor(led, currentR, currentG, currentB);
      changing = true;
    }
  leds.show();
  delayMicroseconds(SLOW); // pauses breifly to slow the overall transition
  }
}

