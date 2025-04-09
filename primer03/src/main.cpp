#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "kresilnik"; // identifikator oz. ime našega WiFi usmerjevalnika
const char* password = "bled2024";

// port 80 je prednastavljeni port za posredovanje html strani
WebServer server(80); // objekt razreda WebServer (http port 80)

// tu določimo kaj sed ogodie, če uporabnik vpiše
// korenski ("root") naslov našega strežnika na esp32 modulu, npr. http://192.168.1.170
void handle_root(){
  server.send(200, "text/html; charset=UTF-8", "Andrej Š. - pozdravljen iz esp32!"); // strežnik klientu pošlje kot odziv, t.j. "response"
                                                               // String "Pozdravljen svet ...", koda 200 predstavlja
                                                               // standardni odziv na uspešno http zahtevo - OK
}

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT); // pin (nožica) št. 2 bo uporabljena kot digitalni izhod

  Serial.begin(115200); // hitrost komunikacije preko serijskega vmesnika

  WiFi.begin(ssid, password); // poženemo WiFi

  while(WiFi.status() != WL_CONNECTED){ // WifiLibrary - status CONNECTED
    delay(500);
    Serial.println("Povezovanje na WiFi omrežje...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljna.");
  Serial.print("IP naslov esp32 modula: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root); // ko vpišemo IP naslov v brskalnik in pritisnemo Enter, tedaj pokličemo funkcijo handle_root,
                               // ki vrne kratko sporočilo klientu ("Pozdravljen, ...")

  server.begin(); // zagon spletnega strežnika na modulu esp32                               

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
  server.handleClient(); // spremljamo prisotnost klientov in poslušamo njihove html zahteve ("request")
}