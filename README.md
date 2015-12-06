# NeoPixel_ChristmasLights
A simple implementation of twinkling Christmas lights for Arduino using WS2812 (NeoPixel) LEDS

(copied from the opening comments in the source code)

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
