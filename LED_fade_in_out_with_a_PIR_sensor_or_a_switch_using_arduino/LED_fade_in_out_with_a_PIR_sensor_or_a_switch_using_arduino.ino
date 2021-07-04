/*
 Code made by awocrf. However, erial disabling was not done by me (Source: https://forum.arduino.cc/t/why-the-code-using-serial-println-compiles-without-serial-begin/413929/10).
 If you want to save some resources by disabling serial communication comment line 10.
 If you want to enable setting fade in/out PWM step delay changing using potentiometer uncomment line 16.
 If you also want to hide potpin value in serial monitor with potentiometer support enabled comment lines 101 to 104.
 Simmilar thing with setting max and min LED brightness values using potentiometer.
 */

// Serial disabling stuff.
#define DEBUG  // Comment this line to disable serial prints and save resources.

#ifdef DEBUG
  #define DEBUG_BEGIN Serial.begin(115200)
#endif

//#define DLY_POTENTIOMETER  // Uncomment this if you want to enable potentiometer support.

//#define MAXBRTPOT // Uncomment this if you want to enable setting max LED brightness using potentiometer.

//#define MINBRTPOT // Uncomment this if you want to enable setting min LED brightness using potentiometer.
 
// Pins
const int LED = 9;      // LED output. WARNING! If you want to use other pin make sure that it's a PWM one. Otherwise it won't work.
const int RLED = 13;    // Readiness LED pin.
const int pirpin = 3;   // PIR sensor pin. It can be replaced with a push button.

#ifdef DLY_POTENTIOMETER
const int dlypotpin = A0;  // Delay potentiometer pin.
#endif

#ifdef MAXBRTPOT
const int maxbrtpotpin = A1;  // Max brightness potentiometer pin.
#endif

#ifdef MINBRTPOT
const int minbrtpotpin = A2;  // Min brightness potentiometer pin.
#endif

// Values
int dly = 15;             // Delay between writing new value to LED pin. Default is 15.
const int dly2 = 100;     // Delay between lew loops. It is used to prevent spam in serial monitor. WARNING! If this delay gets too high arduino can miss PIR sensor's signal! Default is 100.
const int dly3 = 3100;    // For some reason my PIR sensor is sending high signal after connecting to power causing LEDs to turn on so to eliminate that I'm using a delay. Default is 3100.
const int dly4 = 100;     // Readiness LED's flash time. Default is 100.
int pir = 0;              // PIR sensor value. Don't change this!
int state = 0;            // Variable used to define whether LED is on or off to avoid "breathing LED" effect. Don't change this too!
int maxbrt = 255;         // Maximum LED brightness value.
int minbrt = 0;           // Minimum LED brightness value.
int maxbrtpotval = 1023;  // Max brightness potentiometer value.
int minbrtpotval = 0;     // Min brightness potentiometer value.
int brightness = 0;       // Current LED brightness.

#ifdef DLY_POTENTIOMETER
int dlypot = 60; // Potentiometer value
#endif
 
void setup()  {
  DEBUG_BEGIN;
  
  pinMode(LED, OUTPUT);   // Declaring LED pin as output.
  pinMode(RLED, OUTPUT);  // Declaring RLED pin as output.
  pinMode(pirpin, INPUT); // Declaring PIR sensor pin as input.
  
  delay(dly3); // For some reason my PIR sensor is sending HIGH signal after connecting to power causing LEDs to turn on so to eliminate that I'm using a delay.

  digitalWrite(RLED, HIGH); // Readiness LED flash. Indicates that setup is done.
  delay(dly4);
  digitalWrite(RLED, LOW);
}

 
void loop()  {
  attachInterrupt(digitalPinToInterrupt(pirpin), highpirpin, RISING);  // Detecting high signal on pirpin pin is based on interrupt because the PIR sensor's signal is quite long and often causes breathing effect when some loops are too short.

  if(brightness<0) {  // For some reason brightness' value can go up to 256 (which is interpreted as 0) or down to -1 (which is interpreted as 255). This results in flashes at the start of fade in or fade out effect.
    brightness = 0;
  }

  if(brightness>255) {
    brightness = 255;
  }
  
  #ifdef DLY_POTENTIOMETER
  dlypot = analogRead(dlypotpin);               // Setting dlypotpin as dlypots value
  dly = map(dlypot, 0, 1023, 0, 50);            // Changing range from 0-1023 to 0-50.
  #endif

  #ifdef MAXBRTPOT
  maxbrtpotval = analogRead(maxbrtpotpin);      // Setting dlypotpin as dlypots value
  maxbrt = map(maxbrtpotval, 0, 1023, 0, 255);  // Changing range from 0-1023 to 0-50.
  #endif

  #ifdef MINBRTPOT
  minbrtpotval = analogRead(minbrtpotpin);      // Setting potpin as pots value
  minbrt = map(minbrtpotval, 0, 1023, 0, 255);  // Changing range from 0-1023 to 0-50.
  #endif
  
  // Serial monitor communication.
  #ifdef DEBUG
  Serial.print(F("NEW LOOP, LED state = "));
  Serial.print(state);
  #ifdef DLY_POTENTIOMETER
  Serial.print(", dlypot = ");
  Serial.print(dlypot);
  #endif
  Serial.print(F(", LED brgt dly = "));
  Serial.print(dly);
  Serial.print(F(", PIR = "));
  Serial.print(pir);
  #ifdef MAXBRTPOT
  Serial.print(F(", maxbrt = "));
  Serial.print(maxbrt);
  #endif
  #ifdef MINBRTPOT
  Serial.print(F(", minbrt = "));
  Serial.print(minbrt);
  #endif
  Serial.print(F(", Ldly = "));
  Serial.print(dly2);
  Serial.print(F(", currbrt = "));
  Serial.println(brightness);
  #endif
  
 if (pir == 1 && state == 0) // Checking whether pir is HIGH and "state" value.
 {
  state = 1; // Setting "state" to 1.
  pir = 0;   // Setting "pir" to 0.
  
  #ifdef DEBUG
  Serial.print(F("LED IS TURNING ON, brgt dly = ")); // Indicating that LED is turning on in serial monitor.
  Serial.println(dly);
  #endif

  for (brightness; brightness<=maxbrt; brightness++)  // Fade in using for loop.
    {
      analogWrite(LED, brightness);  // Writing new value to a LED pin.
      delay(dly); // Delay to see fade effect.
    }
 }
 
 if (pir == 1 && state == 1) // Checking whether pir is HIGH and "state" value.
 {
    state = 0; // Setting "state" to 0
    pir = 0;   // Setting "pir" to 0.
    
    #ifdef DEBUG
    Serial.print(F("LED IS TURNING OFF, brgt dly = ")); // Indicating that LED is turning off in serial monitor.
    Serial.println(dly);
    #endif
    
  for (brightness; brightness>=minbrt; brightness--) // Fade out using for loop.
    {
      analogWrite(LED, brightness); // Writing new value to a LED pin.
      delay(dly); // Delay to see fade effect.
    }
 }
 delay(dly2); // Delay to prevent spam in serial monitor.
}

void highpirpin() {
  pir = 1;  // Setting "pir" to 1.
}
