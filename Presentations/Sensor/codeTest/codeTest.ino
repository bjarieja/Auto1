#include <Bounce2.h>

/**
 * @file codeTest.ino
 * @brief Steuerung eines Lichtschranken-Sensors mit Entprellung und LED-Ausgabe.
 *
 * Dieses Programm liest das Signal einer Lichtschranke ein,
 * entprellt es mithilfe der Bounce2-Bibliothek und steuert
 * eine LED entsprechend. Zusaetzlich erfolgt eine Ausgabe
 * ueber den Serial-Monitor.
 */

// Pins definieren

/** 
 * @def LICH_SCH_PIN
 * @brief GPIO-Pin fuer das Signal der Lichtschranke.
 */
#define LICH_SCH_PIN 2 

/** 
 * @def LED_PIN
 * @brief GPIO-Pin fuer die LED-Ausgabe.
 */
#define LED_PIN 13 

/**
 * @brief Bounce-Objekt zur Entprellung des Eingangssignals.
 *
 * Die Klasse Bounce aus der Bibliothek Bounce2 sorgt dafuer,
 * dass Stoerungen (Prellen) am Eingangssignal gefiltert werden.
 */
Bounce debouncer = Bounce();

/**
 * @brief Initialisierung der Hardware und seriellen Kommunikation.
 */
void setup() {

    pinMode(LICH_SCH_PIN, INPUT_PULLUP);  ///< Eingang mit Pullup
    pinMode(LED_PIN, OUTPUT);             ///< LED als Ausgang
    
    debouncer.attach(LICH_SCH_PIN);       ///< Pin dem Debouncer zuweisen
    debouncer.interval(50);               ///< 50 ms Entprellzeit
    
    Serial.begin(9600);                   ///< Start Serial-Monitor
}

/**
 * @brief Hauptprogrammschleife.
 *
 * Prueft kontinuierlich den Zustand der Lichtschranke
 * und steuert die LED sowie die serielle Ausgabe.
 */
void loop() {

    debouncer.update();  ///< Entprellung aktualisieren
    
    if (debouncer.read() == LOW) {     
        digitalWrite(LED_PIN, HIGH);   ///< LED an
        
        Serial.println("Objekt erkannt!");  ///< Ausgabe im Serial-Monitor
    } else {
        digitalWrite(LED_PIN, LOW);    ///< LED aus
    }
}