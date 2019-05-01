# rgbRing

This Arduino sketch displays some patterns on a ring of Neopixel
RGB LEDs, under the control of a pot and a push-button switch.

## Library Usage

This sketch requires the use of the 'FastLED' library.
The library provides all the code to efficiently drive the LED string.

## Hardware Setup

The LED ring is connected to Arduino digital pin 3 (change this in the
code by changing 'DATA_PIN').
There's an analog pot connected to Arduino analog pin 0 for setting
hues, brightness and so on.
The mode selection is done by a push-button switch on Arduino digital
pin 2 ('BUTTON_PIN' in the code).
The button has a pull-up resistor connected, and it grounds the input pin
when pressed.
