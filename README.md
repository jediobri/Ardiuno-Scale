# Ardiuno-Scale
Scale that is attached to tower light


This project consists of an Arduino Nano every with an HX711 Scale amp by Sparkfun. A DC-DC buck converter as well as a MOSFET switch board.

24v in is used by the DC-DC buck converter that will step down the power to 5v for the Arduino and the Scale Amp.
The 24v goes to the mosfet board where it is used to switch on the different lights on an industrial light tower.

Since the board uses an arduino nano the pin outs are differnet than an UNO
the Dout goes to pin 21
SCK goes to pin 20

5v signal to switch on lights are wired 
red = pin 5
yellow = pin 3
 green = pin 6 
These are all on PWM pins so there is the capability to vary the intensity of the light

