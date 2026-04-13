#include <Bounce2.h>

// Pins definieren
#define LICH_SCH_PIN 2 
#define LED_PIN 13 

// Bounce-objekt zur Entprellung des Eingangssignals
// Die Klasse Bounce aus der Bibliothek Bounce2 sorgt dafuer,
// dass Stoerungen (Prellen) am Eingangssignal gefiltert werden.
Bounce debouncer = Bounce();

// Initialisierung der Hardware und seriellen Kommunikation
void setup() {
    pinMode(LICH_SCH_PIN, INPUT_PULLUP);  // Eingang mit Pullup
    pinMode(LED_PIN, OUTPUT);             // LED als Ausgang
    
    debouncer.attach(LICH_SCH_PIN);       // Pin dem Debouncer zuweisen
    debouncer.interval(50);               // 50 ms Entprellzeit
    
    Serial.begin(9600);                   // Start Serial-Monitor
}

// Hauptprogrammschleife
// Prueft kontinuierlich den Zustand der Lichtschranke
// und steuert die LED sowie die serielle Ausgabe.
void loop() {
    debouncer.update();  // Entprellung aktualisieren
    
    if (debouncer.read() == LOW) {     
        digitalWrite(LED_PIN, HIGH);   // LED an
        Serial.println("Objekt erkannt!");  // Ausgabe im Serial-Monitor
    } else {
        digitalWrite(LED_PIN, LOW);    // LED aus
    }
}