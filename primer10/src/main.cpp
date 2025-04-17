#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

const char* ssid = "kibernetika"; // identifikator oz. ime našega WiFi usmerjevalnika
const char* password = "norbert1948";

// port 80 je prednastavljeni port za posredovanje html strani
WebServer server(80); // objekt razreda WebServer (http port 80)

// na vratih 81 ustvarimo strežnik spletnih vtičnikov - WebSocketsServer
WebSocketsServer webSocket = WebSocketsServer(81);

const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Primer s spletnim vtičnikom WebSocket.</h1>
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

// ****************************************************************************************
// Definicija funkcije onWebSocketEvent, ki se izvede kadarkoli dobimo sporočilo
// preko spletnega vtišnika - preko WebSocket-a (na vtičniku se dogodi dogodek - "event")
// ****************************************************************************************
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  // določimo funkcionalnost glede na tip dogodka na spletnem vtičniku (WebSocket-u)
  switch(type){
    case WStype_DISCONNECTED:
    {
      Serial.printf("[%u] Klient odklopljen!\n", num);
    }
    break;

    case WStype_CONNECTED:
    {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Vzpostavljena WebSocket povezava iz IP naslova!\n", num);
      Serial.println(ip.toString());
    }
    break;

    case WStype_TEXT:
    {
      Serial.printf("[%u] Besedilo sporočila: %s\n", num, payload); // izpis vsebine sporočila v serijski monitor
    }

    default:
      break;
  } // KONEC switch stavka
} // KONEC funkcije onWebSocketEvent()


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
  Serial.println("HTTP strežnik je zagnan, vpišite IP naslov v brskalnik, npr.: http://192.168.1.158");

  // poženemo WebSocket strežnik in določimo "callback"
  webSocket.begin(); // zagon WebSocket strežnika
  webSocket.onEvent(onWebSocketEvent); // kadarkoli se bo dogodil določen dogodek preko spletnega vtičnika
                                       // WebSocket bomo izvedli funkcijo onWebSocketEvent, ki smo jo def. zgoraj

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
  // delovanje spletnega strežnika
  server.handleClient(); // spremljamo prisotnost klientov in poslušamo njihove html zahteve ("request")

  // delovanje WebSocket strežnika
  webSocket.loop(); // poslušamo na spletnem vtičniku - WebSocket za podatke in dogodke
}