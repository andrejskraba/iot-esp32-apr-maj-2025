#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT); // pin (nožica) št. 2 bo uporabljena kot digitalni izhod
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (HIGH)
  delay(250); // zakasnitev v milisekundah
  digitalWrite(2, LOW); //  na pin 2 zapišemo nizko vrednost (LOW)
  delay(250); // zakasnitev v milisekundah
}