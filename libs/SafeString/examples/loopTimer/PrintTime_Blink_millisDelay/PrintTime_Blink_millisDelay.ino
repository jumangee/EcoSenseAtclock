// PrintTime_Blink_millisDelay.ino
// Print and Blink without delay, (good)
/*
   (c)2019 Forward Computing and Control Pty. Ltd.
   NSW Australia, www.forward.com.au
   This code is not warranted to be fit for any purpose. You may only use it at your own risk.
   This generated code may be freely used for both private and commercial use
   provided this copyright is maintained.
*/
#include <loopTimer.h>
// install the loopTimer library from https://www.forward.com.au/pfod/ArduinoProgramming/RealTimeArduino/TimingDelaysInArduino.html
// loopTimer.h also needs the millisDelay library installed from https://www.forward.com.au/pfod/ArduinoProgramming/TimingDelaysInArduino.html

int led = 13;
// Pin 13 has an led connected on most Arduino boards.
bool ledOn = false; // keep track of the led state
millisDelay ledDelay;
millisDelay printDelay;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  for (int i = 10; i > 0; i--) {
    Serial.println(i);
    delay(500);
  }
  // initialize digital pin led as an output.
  pinMode(led, OUTPUT);

  ledDelay.start(1000); // start the ledDelay, toggle every 1000mS
  printDelay.start(5000); // start the printDelay, print every 5000mS
}

// the task method
void blinkLed13() {
  if (ledDelay.justFinished()) {   // check if delay has timed out
    ledDelay.repeat(); // start delay again without drift
    ledOn = !ledOn;     // toggle the led
    digitalWrite(led, ledOn?HIGH:LOW); // turn led on/off
  } // else nothing to do this call just return, quickly
}

// the task method
void print_mS() {
  if (printDelay.justFinished()) {
    printDelay.repeat(); // start delay again without drift
    Serial.println(millis());   // print the current mS
  } // else nothing to do this call just return, quickly
}

// the loop function runs over and over again forever
void loop() {
  loopTimer.check(Serial);
  blinkLed13(); // call the method to blink the led
  print_mS(); // print the time
}
