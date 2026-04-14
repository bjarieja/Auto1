/**
 * @file lichtschranke_test.ino
 * @brief Testsketch zur Erprobung einer Lichtschranke mit einem Arduino UNO R4.
 *
 * Dieser Sketch liest den Zustand einer an einen GPIO-Pin angeschlossenen
 * Lichtschranke aus und signalisiert das Ergebnis ueber die eingebaute LED
 * des Arduino UNO R4 sowie ueber den seriellen Monitor.
 *
 * @par Hardware
 * - Mikrocontroller: Arduino UNO R4 (Minima oder WiFi)
 * - Prozessor: Renesas RA4M1 (Arm Cortex-M4, 48 MHz)
 * - Betriebsspannung Logik: 3,3 V (5-V-tolerante GPIO-Pins)
 * - Versorgung Lichtschranke: 5 V ueber den 5V-Pin des Boards
 *
 * @par Verkabelung
 * | Lichtschranke | Arduino UNO R4 |
 * |---------------|----------------|
 * | VCC           | 5V             |
 * | GND           | GND            |
 * | Signal        | GPIO-Pin (siehe @ref LICHTSCHRANKE_PIN) |
 *
 * @par Testablauf
 * -# Starten der ArduinoIDE
 * -# Verbinden der Lichtschranke an die Versorgungspins (5V, GND) des Arduino UNO R4
 * -# Verbinden der Signalleitung der Lichtschranke an einen GPIO-Pin des Arduino UNO R4
 * -# Verbinden des Arduino UNO R4 mit dem Rechner per USB-C
 * -# Oeffnen der ArduinoIDE
 * -# Oeffnen dieses Sketches zur Erprobung
 * -# Eintragen des mit der Signalleitung verbundenen GPIO-Pins bei @ref LICHTSCHRANKE_PIN
 * -# Sketch auf den Arduino UNO R4 spielen
 * -# Ueberpruefen der ordnungsgemaessen Funktion der Lichtschranke anhand der eingebauten LED
 *
 * @note Beim Arduino UNO R4 ist der USB-Anschluss als USB-C ausgefuehrt.
 *       In der ArduinoIDE muss unter Werkzeuge > Board das Board
 *       "Arduino UNO R4 Minima" bzw. "Arduino UNO R4 WiFi" ausgewaehlt werden.
 *       Das zugehoerige Board-Paket "Arduino UNO R4 Boards" muss ueber den
 *       Boardverwalter installiert sein.
 *
 * @author Autor
 * @date 2025
 * @version 1.0
 */

/* ============================================================== */
/*  Konfiguration                                                  */
/* ============================================================== */

/**
 * @brief GPIO-Pin, an dem die Signalleitung der Lichtschranke angeschlossen ist.
 *
 * Diesen Wert entsprechend der tatsaechlichen Verkabelung anpassen.
 * Gueltige digitale Pins beim Arduino UNO R4: 0 – 13, A0 – A5.
 *
 * @note Pin 0 und 1 werden fuer die serielle Kommunikation (UART) verwendet
 *       und sollten daher nicht belegt werden.
 */
#define LICHTSCHRANKE_PIN 2

/**
 * @brief Pin der eingebauten LED des Arduino UNO R4.
 *
 * Beim Arduino UNO R4 liegt die eingebaute LED auf Pin 13 (LED_BUILTIN).
 * Sie wird zur optischen Signalisierung des Lichtschrankenzustands verwendet.
 */
#define LED_PIN LED_BUILTIN

/**
 * @brief Baudrate fuer die serielle Kommunikation ueber USB-C.
 */
#define SERIELLE_BAUDRATE 115200

/**
 * @brief Abtastintervall in Millisekunden fuer die Zustandsabfrage.
 */
#define ABTASTINTERVALL_MS 200

/* ============================================================== */
/*  Setup                                                          */
/* ============================================================== */

/**
 * @brief Initialisierung des Arduino UNO R4.
 *
 * Konfiguriert den Lichtschranken-Pin als digitalen Eingang mit internem
 * Pull-up-Widerstand, den LED-Pin als Ausgang und initialisiert die
 * serielle Schnittstelle ueber USB-C.
 */
void setup()
{
    pinMode(LICHTSCHRANKE_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(SERIELLE_BAUDRATE);
    while (!Serial)
    {
        /* Warten bis die serielle USB-C-Verbindung steht */
    }

    Serial.println(F("============================================="));
    Serial.println(F("  Lichtschranke – Testsketch"));
    Serial.println(F("  Board: Arduino UNO R4"));
    Serial.println(F("============================================="));
    Serial.print(F("Signalpin:       "));
    Serial.println(LICHTSCHRANKE_PIN);
    Serial.print(F("LED-Pin:         "));
    Serial.println(LED_PIN);
    Serial.print(F("Abtastintervall: "));
    Serial.print(ABTASTINTERVALL_MS);
    Serial.println(F(" ms"));
    Serial.println();
    Serial.println(F("Starte Messung..."));
    Serial.println();
}

/* ============================================================== */
/*  Hilfsfunktionen                                                */
/* ============================================================== */

/**
 * @brief Liest den aktuellen Zustand der Lichtschranke ein.
 *
 * @return @c true  wenn die Lichtschranke unterbrochen (ausgeloest) ist,
 *         @c false wenn der Lichtweg frei ist.
 *
 * @note Bei den meisten Lichtschrankenmodulen liegt der Signalausgang im
 *       ausgeloesten Zustand auf LOW. Falls das verwendete Modul invertiert
 *       arbeitet, muss die Logik in dieser Funktion angepasst werden.
 */
bool lichtschrankeAusgeloest()
{
    return (digitalRead(LICHTSCHRANKE_PIN) == LOW);
}

/**
 * @brief Schaltet die eingebaute LED des Arduino UNO R4 ein oder aus.
 *
 * @param[in] aktiv @c true schaltet die LED ein,
 *                  @c false schaltet die LED aus.
 */
void setLED(bool aktiv)
{
    digitalWrite(LED_PIN, aktiv ? HIGH : LOW);
}

/**
 * @brief Gibt den aktuellen Zustand der Lichtschranke auf dem seriellen Monitor aus.
 *
 * Eine Ausgabe erfolgt nur bei einer Zustandsaenderung gegenueber dem vorherigen
 * Abtastzyklus, um den seriellen Monitor nicht zu ueberfluten.
 *
 * @param[in] ausgeloest       Aktueller Zustand der Lichtschranke.
 * @param[in] zustandGeaendert @c true wenn sich der Zustand seit der letzten
 *                             Abfrage geaendert hat.
 */
void zustandAusgeben(bool ausgeloest, bool zustandGeaendert)
{
    if (zustandGeaendert)
    {
        unsigned long zeitstempel = millis();

        Serial.print(F("["));
        Serial.print(zeitstempel);
        Serial.print(F(" ms] "));

        if (ausgeloest)
        {
            Serial.println(F("AUSGELOEST – Lichtschranke unterbrochen – LED AN"));
        }
        else
        {
            Serial.println(F("FREI       – Lichtweg frei             – LED AUS"));
        }
    }
}

/* ============================================================== */
/*  Hauptschleife                                                  */
/* ============================================================== */

/**
 * @brief Hauptschleife des Programms.
 *
 * Fragt zyklisch den Zustand der Lichtschranke ab, aktualisiert die eingebaute
 * LED (Pin 13) und gibt Zustandsaenderungen mit Zeitstempel ueber die serielle
 * Schnittstelle (USB-C) aus.
 */
void loop()
{
    /** @brief Gespeicherter vorheriger Zustand der Lichtschranke. */
    static bool letzterZustand = false;

    /** @brief Flag fuer den ersten Schleifendurchlauf. */
    static bool ersterDurchlauf = true;

    bool aktuellerZustand = lichtschrankeAusgeloest();
    bool geaendert = (aktuellerZustand != letzterZustand) || ersterDurchlauf;

    setLED(aktuellerZustand);
    zustandAusgeben(aktuellerZustand, geaendert);

    letzterZustand = aktuellerZustand;
    ersterDurchlauf = false;

    delay(ABTASTINTERVALL_MS);
}