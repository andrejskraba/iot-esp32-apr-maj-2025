#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "kresilnik"; // identifikator oz. ime našega WiFi usmerjevalnika
const char* password = "bled2024";

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT); // pin (nožica) št. 2 bo uporabljena kot digitalni izhod

  Serial.begin(115200); // hitrost komunikacije preko serijskega vmesnika

  WiFi.begin(ssid, password); // poženemo WiFi

  while(WiFi.status() != WL_CONNECTED){ // WifiLibrary - status CONNECTED
    delay(500);
    Serial.println("Povezovanje na WiFi omrežje...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljna");
  Serial.print("IP naslov esp32 modula: ");
  Serial.println(WiFi.localIP());

  // ta del kode uporabimo kot indikator, da se je program prenesel na modul, LED 2x utripne
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (HIGH)
  delay(250); // zakasnitev v milisekundah
  digitalWrite(2, LOW); //  na pin 2 zapišemo nizko vrednost (LOW)
  delay(250); // zakasnitev v milisekundah
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (HIGH)
  delay(250); // zakasnitev v milisekundah
  digitalWrite(2, LOW); //  na pin 2 zapišemo nizko vrednost (LOW)
  delay(250); // zakasnitev v milisekundah
}

void loop() {
  // put your main code here, to run repeatedly:
}