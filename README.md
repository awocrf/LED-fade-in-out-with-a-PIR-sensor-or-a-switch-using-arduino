# LED fade in out with a PIR sensor or a switch using arduino

Code for arduino that makes fade in/out effect after receiving signal from switch, PIR sensor, etc.

Default pinout:

Pin 9 - main output. That's where you connect your LEDs. Of course you can change that but remember: not all pins on arduino support PWM!

Pin 13 - RLED output. When you connect LED here (But you don't have to sice most arduinos or arduino compatible boards have a LED already connected to this pin.) it'll make a short flash after void setup() is done. I added that because of a large delay going on here. This delays function is explained in one of the comments.

Pin 3 - signal input. This is where you connect whatever is sending it. Signals maximum length is dependent on value of dly (delay). If it'll be longer than dly value * 255 (I think) + how much time arduino needs to change to next PWM value and complete analogWrite, the LED will do a "breathing effect" (Fade in and out without stopping.).

Other things are explained it the code comments.
