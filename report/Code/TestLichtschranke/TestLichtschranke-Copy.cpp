/**
 * @brief GPIO pin for the light barrier signal.
 */
#define LICHTSCHRANKE_PIN 2

/**
 * @brief Built-in LED pin of the Arduino UNO R4.
 */
#define LED_PIN LED_BUILTIN

/**
 * @brief Baud rate for serial communication.
 */
#define SERIELLE_BAUDRATE 115200 // Only needed for initial upload to the microcontroller

/**
 * @brief Sampling interval in milliseconds.
 */
#define ABTASTINTERVALL_MS 200

/**
 * @brief Initializes hardware components.
 *
 * @details
 * Configures the light barrier input with internal pull-up,
 * sets the LED as output, and starts serial communication.
 */
void setup()
{
    pinMode(LICHTSCHRANKE_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(SERIELLE_BAUDRATE);
    while (!Serial)
    {
    }
    Serial.print(F("Signal pin:       "));
    Serial.println(LICHTSCHRANKE_PIN);
    Serial.print(F("LED pin:          "));
    Serial.println(LED_PIN);
    Serial.print(F("Sampling interval: "));
    Serial.print(ABTASTINTERVALL_MS);
    Serial.println(F(" ms"));
    Serial.println();
    Serial.println(F("Starting measurement..."));
    Serial.println();
}

/**
 * @brief Reads the current state of the light barrier.
 *
 * @return true  if beam is interrupted (LOW)
 * @return false if beam is clear (HIGH)
 */
bool lichtschrankeAusgeloest()
{
    return (digitalRead(LICHTSCHRANKE_PIN) == LOW);
}

/**
 * @brief Controls the built-in LED.
 *
 * @param aktiv true = LED ON, false = LED OFF
 */
void setLED(bool aktiv)
{
    digitalWrite(LED_PIN, aktiv ? HIGH : LOW);
}

/**
 * @brief Outputs the sensor state to the serial monitor.
 *
 * @param ausgeloest       Current sensor state
 * @param zustandGeaendert True if state has changed
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
            Serial.println("Triggered");
        }
        else
        {
            Serial.println("not triggered");
        }
    }
}

/**
 * @brief Main loop for cyclic sensor polling.
 *
 * @details
 * Reads sensor state, updates LED, and prints changes.
 */
void loop()
{
    static bool letzterZustand = false;
    static bool ersterDurchlauf = true;

    bool aktuellerZustand = lichtschrankeAusgeloest();
    bool geaendert = (aktuellerZustand != letzterZustand) || ersterDurchlauf;

    setLED(aktuellerZustand);
    zustandAusgeben(aktuellerZustand, geaendert);

    letzterZustand = aktuellerZustand;
    ersterDurchlauf = false;

    delay(ABTASTINTERVALL_MS);
}