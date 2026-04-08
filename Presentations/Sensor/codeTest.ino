#include <Bounce2.h>

// Pins definieren
#define LICH_SCH_PIN 2   // Lichtschranke am digitalen Pin 2
#define LED_PIN 13       // LED am Pin 13

// Bounce-Objekt für Entprellung
Bounce debouncer = Bounce();

void setup() {
  // Pins konfigurieren
  pinMode(LICH_SCH_PIN, INPUT_PULLUP);  // LOW wenn Lichtschranke unterbrochen
  pinMode(LED_PIN, OUTPUT);             // LED als Ausgang

  // Bounce konfigurieren
  debouncer.attach(LICH_SCH_PIN);
  debouncer.interval(50);               // Entprellzeit 50 ms

  // Serieller Monitor (optional)
  Serial.begin(9600);
}

void loop() {
  // Bounce aktualisieren
  debouncer.update();

  // Prüfen, ob die Lichtschranke unterbrochen ist
  if (debouncer.read() == LOW) {   // Paket erkannt
    digitalWrite(LED_PIN, HIGH);   // LED einschalten
    Serial.println("Objekt erkannt!");
  } else {
    digitalWrite(LED_PIN, LOW);    // LED ausschalten
  }
}