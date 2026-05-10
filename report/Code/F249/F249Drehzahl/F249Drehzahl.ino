/**
 * @file F249Drehzahl.ino
 * @brief RPM measurement using F249 fork light barrier and slotted disk
 * @details Counts sensor pulses using an interrupt and outputs pulses per second.
 */

/// Counts detected pulses (incremented inside ISR)
volatile unsigned long pulseCount = 0;

/**
 * @brief Interrupt Service Routine (ISR)
 * @details Called on every signal edge from the light barrier sensor
 */
void countPulse()
{
    pulseCount++;
}

/**
 * @brief Arduino setup function
 * @details Initializes input pin, interrupt, and serial communication
 */
void setup()
{
    /// Input pin for the F249 sensor (DO output)
    pinMode(2, INPUT);

    /// Enable interrupt on pin 4(rising edge trigger)
    attachInterrupt(digitalPinToInterrupt(4), countPulse, RISING);

    /// Start serial communication
    Serial.begin(115200);
}

/**
 * @brief Main loop function
 * @details Calculates and prints pulses per second
 */
void loop()
{
    /// Create safe local copy of pulse counter
    unsigned long countCopy = pulseCount;

    /// Reset counter for next measurement interval
    pulseCount = 0;

    /// Output pulses per second
    Serial.print("Pulses per second: ");
    Serial.println(countCopy);

    /// Measurement interval of 1 second
    delay(1000);
}