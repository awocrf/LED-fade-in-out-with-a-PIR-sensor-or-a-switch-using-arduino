# LED fade in out with a PIR sensor or a switch using arduino

Code for arduino that makes fade in/out effect after receiving signal from switch, PIR sensor, etc.

Default pinout:

Pin 9 - main output. That's where you connect your LEDs. Of course you can change that but remember: not all pins on arduino support PWM!

Pin 13 - RLED output. When you connect LED here (But you don't have to sice most arduinos or arduino compatible boards have a LED already connected to this pin.) it'll make a short flash after void setup() is done. I added that because of a large delay going on here. This delays function is explained in one of the comments.

Pin 3 - signal input. This is where you connect whatever is sending it. The code (from version 1.3) is detecting rising slope using an interrupt so there shouldn't be any problem with the signal being long.

Pin A0 - delay potentiometer input. After connecting it here and enabling it in the code you can change main delay (dly) in real time (not including when LED in turning on or off).

Pin A1 - max brightness potentiometer. By connecting it and enabling its support in the code you can change maximum brightness to which the LED will go up an then stop.

Pin A2 - min brightness potentiometer. Same thing as above but minimum, not maximum.

Other things are explained it the code comments.

Disclaimer: there might be some stupid things in the code that can be done way better and that I don't see because my programming skills aren't so good.
