#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "kresilnik"; // identifikator oz. ime našega WiFi usmerjevalnika
const char* password = "bled2024";

bool LED1status = LOW; // spremenljivka za status LED didode

// port 80 je prednastavljeni port za posredovanje html strani
WebServer server(80); // objekt razreda WebServer (http port 80)

const char HTML0[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Prejet ukaz za izklop LED diode.</h1>
</body>
</html>
)rawliteral";

const char HTML1[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Prejet ukaz za vklop LED diode.</h1>
</body>
</html>
)rawliteral";

const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>V brskalnik vpišite http://192.168.1.200/1 ali http://192.168.1.200/0 t.j. /1 ali /0 na koncu.</h1>
</body>
</html>
)rawliteral";

// tu določimo kaj sed ogodie, če uporabnik vpiše
// korenski ("root") naslov našega strežnika na esp32 modulu, npr. http://192.168.1.170
void handle_root(){
  server.send(200, "text/html; charset=UTF-8", HTML); // strežnik klientu pošlje kot odziv, t.j. "response"
                                                               // String "Pozdravljen svet ...", koda 200 predstavlja
                                                               // standardni odziv na uspešno http zahtevo - OK
}

void handle_led1on(){
  LED1status = HIGH;
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", HTML1); // kot odgovor posredujemo obvestilo o vklopu
}

void handle_led1off(){
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", HTML0); // kot odgovor posredujemo obvestilo o vklopu
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

  server.on("/1", handle_led1on); // če klient v Chrome-u vpiše /1, se bo sprožila funkcija handle_led1on                               
  server.on("/0", handle_led1off); // če klient v Chrome-u vpiše /0, se bo sprožila funkcija handle_led1off

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

  if(LED1status){
    digitalWrite(2, HIGH);
  }
  else{
    digitalWrite(2, LOW);
  }

}