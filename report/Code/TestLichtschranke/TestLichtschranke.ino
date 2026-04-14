/**
 * @file lichtschranke_test.ino
 * @brief Testsketch zur Erprobung einer Lichtschranke mit einem Mikrocontroller.
 *
 * Dieser Sketch liest den Zustand einer an einen GPIO-Pin angeschlossenen
 * Lichtschranke aus und signalisiert das Ergebnis ueber die eingebaute LED
 * des Mikrocontrollers sowie ueber den seriellen Monitor.
 *
 * @par Verkabelung
 * - Lichtschranke VCC  -> 5V des Mikrocontrollers
 * - Lichtschranke GND  -> GND des Mikrocontrollers
 * - Lichtschranke Signal -> GPIO-Pin (siehe @ref LICHTSCHRANKE_PIN)
 *
 * @par Testablauf
 * -# Starten der ArduinoIDE
 * -# Verbinden der Lichtschranke an die Versorgungspins (5V, GND) des Mikrocontrollers
 * -# Verbinden der Signalleitung der Lichtschranke an einen GPIO-Pin des Mikrocontrollers
 * -# Verbinden des Mikrocontrollers mit dem Rechner per USB
 * -# Oeffnen der ArduinoIDE
 * -# Oeffnen dieses Sketches zur Erprobung
 * -# Eintragen des mit der Signalleitung verbundenen GPIO-Pins bei @ref LICHTSCHRANKE_PIN
 * -# Sketch auf den Mikrocontroller spielen
 * -# Ueberpruefen der ordnungsgemaessen Funktion der Lichtschranke anhand der eingebauten LED
 *
 * @author Autor
 * @date 2025
 * @version 1.0
 */

/**
 * @brief GPIO-Pin, an dem die Signalleitung der Lichtschranke angeschlossen ist.
 *
 * Diesen Wert entsprechend der tatsaechlichen Verkabelung anpassen.
 */
#define LICHTSCHRANKE_PIN 2

/**
 * @brief Pin der eingebauten LED des Mikrocontrollers.
 *
 * Wird zur optischen Signalisierung des Lichtschrankenzustands verwendet.
 */
#define LED_PIN LED_BUILTIN

/**
 * @brief Baudrate fuer die serielle Kommunikation.
 */
#define SERIELLE_BAUDRATE 9600

/**
 * @brief Abtastintervall in Millisekunden fuer die Zustandsabfrage.
 */
#define ABTASTINTERVALL_MS 200

/**
 * @brief Initialisierung des Mikrocontrollers.
 *
 * Konfiguriert den Lichtschranken-Pin als Eingang mit internem Pull-up-Widerstand,
 * den LED-Pin als Ausgang und initialisiert die serielle Schnittstelle.
 */
void setup()
{
    pinMode(LICHTSCHRANKE_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(SERIELLE_BAUDRATE);
    while (!Serial)
    {
        /* Warten bis die serielle Verbindung steht (relevant fuer Arduino Leonardo / Micro) */
    }

    Serial.println(F("=== Lichtschranke Testsketch ==="));
    Serial.print(F("Signalpin: "));
    Serial.println(LICHTSCHRANKE_PIN);
    Serial.println(F("Starte Messung..."));
    Serial.println();
}

/**
 * @brief Liest den aktuellen Zustand der Lichtschranke ein.
 *
 * @return @c true wenn die Lichtschranke unterbrochen (ausgeloest) ist,
 *         @c false wenn der Lichtweg frei ist.
 *
 * @note Bei den meisten Lichtschrankenmodulen liegt der Ausgang im
 *       ausgeloesten Zustand auf LOW. Falls das verwendete Modul invertiert
 *       arbeitet, muss die Logik hier angepasst werden.
 */
bool lichtschrankeAusgeloest()
{
    return (digitalRead(LICHTSCHRANKE_PIN) == LOW);
}

/**
 * @brief Setzt die eingebaute LED entsprechend des uebergebenen Zustands.
 *
 * @param[in] aktiv @c true schaltet die LED ein, @c false schaltet sie aus.
 */
void setLED(bool aktiv)
{
    digitalWrite(LED_PIN, aktiv ? HIGH : LOW);
}

/**
 * @brief Gibt den aktuellen Zustand der Lichtschranke auf dem seriellen Monitor aus.
 *
 * @param[in] ausgeloest Aktueller Zustand der Lichtschranke.
 * @param[in] zustandGeaendert @c true wenn sich der Zustand seit der letzten Abfrage geaendert hat.
 */
void zustandAusgeben(bool ausgeloest, bool zustandGeaendert)
{
    if (zustandGeaendert)
    {
        if (ausgeloest)
        {
            Serial.println(F("[AUSGELOEST] Lichtschranke unterbrochen – LED AN"));
        }
        else
        {
            Serial.println(F("[FREI]       Lichtweg frei            – LED AUS"));
        }
    }
}

/**
 * @brief Hauptschleife des Programms.
 *
 * Fragt zyklisch den Zustand der Lichtschranke ab, aktualisiert die eingebaute
 * LED und gibt Zustandsaenderungen ueber die serielle Schnittstelle aus.
 */
void loop()
{
    static bool letzterZustand = false; /**< Gespeicherter vorheriger Zustand. */
    static bool ersterDurchlauf = true; /**< Flag fuer den ersten Schleifendurchlauf. */

    bool aktuellerZustand = lichtschrankeAusgeloest();
    bool geaendert = (aktuellerZustand != letzterZustand) || ersterDurchlauf;

    setLED(aktuellerZustand);
    zustandAusgeben(aktuellerZustand, geaendert);

    letzterZustand = aktuellerZustand;
    ersterDurchlauf = false;

    delay(ABTASTINTERVALL_MS);
}
