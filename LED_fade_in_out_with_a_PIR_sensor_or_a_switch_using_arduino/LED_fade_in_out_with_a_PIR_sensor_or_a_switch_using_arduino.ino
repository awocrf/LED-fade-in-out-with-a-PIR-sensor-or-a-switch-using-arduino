/*
 Code made by awocrf. However, erial disabling was not done by me (Source: https://forum.arduino.cc/t/why-the-code-using-serial-println-compiles-without-serial-begin/413929/10).
 If you want to save some resources by disabling serial communication comment the line below.
 If you want to enable setting fade in/out delay changing using potentiometer uncomment line 17.
 If you also want to hide potpin value in serial monitor with potentiometer support enabled comment lines 71 to 74.
 */

// Serial disabling stuff.
#define DEBUG  // Comment this line to disable serial prints and save resources.

#ifndef DEBUG
  #define DEBUG_BEGIN
#else
  #define DEBUG_BEGIN Serial.begin(115200)
#endif

//#define POTENTIOMETER  // Uncomment this if you want to enable potentiometer support.

#ifndef POTENTIOMETER
  #define POTENTIOMETER_ENABLE
#else
  #define POTENTIOMETER_ENABLE
#endif
 
// Pins
const int LED = 9;      // LED output. WARNING! If you want to use other pin make sure that it's a PWM one. Otherwise it won't work.
const int RLED = 13;    // Readiness LED pin.
const int pirpin = 3;   // PIR sensor pin. It can be replaced with a push button.

#ifdef POTENTIOMETER
const int potpin = A0;  // Potentiometer pin.
#endif

// Values
int dly = 15;           // Delay between writing new value to LED pin. Default is 15.
const int dly2 = 100;   // Delay between lew loops. It is used to prevent spam in serial monitor. WARNING! If this delay gets too high arduino can miss PIR sensor's signal! Default is 100.
const int dly3 = 3100;  // For some reason my PIR sensor is sending high signal after connecting to power causing LEDs to turn on so to eliminate that I'm using a delay. Default is 3100.
const int dly4 = 100;   // Readiness LED's flash time. Default is 100.
int pir = 0;            // PIR sensor value. Don't change this!
int state = 0;          // Variable used to define whether LED is on or off to avoid "breathing LED" effect. Don't change this too!
 
void setup()  {
  DEBUG_BEGIN;
  
  POTENTIOMETER_ENABLE;
   
  pinMode(LED, OUTPUT);   // Declaring LED pin as output.
  pinMode(RLED, OUTPUT);  // Declaring RLED pin as output.
  pinMode(pirpin, INPUT); // Declaring PIR sensor pin as input.
  
  delay(dly3); // For some reason my PIR sensor is sending HIGH signal after connecting to power causing LEDs to turn on so to eliminate that I'm using a delay.

  digitalWrite(RLED, HIGH); // Readiness LED flash. Indicates that setup is done.
  delay(dly4);
  digitalWrite(RLED, LOW);
}

 
void loop()  {
  pir = digitalRead(pirpin); // Setting "pir" as pirpin's current value.
  
  #ifdef POTENTIOMETER
  dly = map(potpin, 0, 1023, 0, 1050); // Changing range from 0-1023 to 0-50.
  #endif

  // Serial monitor communication.
  #ifdef DEBUG
  Serial.print(F("NEW LOOP, LED state = "));
  Serial.print(state);
  
  #ifdef POTENTIOMETER
  Serial.print(", potpin = ");
  Serial.print(potpin);
  #endif
  
  Serial.print(F(", LED brgt dly = "));
  Serial.print(dly);
  Serial.print(F(", PIR = "));
  Serial.print(pir);
  Serial.print(F(", Ldly = "));
  Serial.println(dly2);
  #endif
  
 if (pir == HIGH && state == 0) // Checking whether pir is HIGH and "state" value.
 {
  state = 1; // Setting "state" to 1.
  
  #ifdef DEBUG
  Serial.print(F("LED IS TURNING ON, brgt dly = ")); // Indicating that LED is turning on in serial monitor.
  Serial.println(dly);
  #endif

  for (int brightness=0; brightness<=255; brightness++)  // Fade in using for loop.
    {
      analogWrite(LED, brightness);  // Writing new value to a LED pin.
      delay(dly); // Delay to see fade effect.
    }
 }
 pir = digitalRead(pirpin); // Setting "pir" as pirpin's current value.

 if (pir == HIGH && state == 1) // Checking whether pir is HIGH and "state" value.
 {
    state = 0; // Setting "state" to 0
    
    #ifdef DEBUG
    Serial.print(F("LED IS TURNING OFF, brgt dly = ")); // Indicating that LED is turning off in serial monitor.
    Serial.println(dly);
    #endif
    
  for (int brightness=255; brightness>=0; brightness--) // Fade out using for loop.
    {
      analogWrite(LED, brightness); // Writing new value to a LED pin.
      delay(dly); // Delay to see fade effect.
    }
 }
 delay(dly2); // Delay to prevent spam in serial monitor.
}
