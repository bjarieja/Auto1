/**
 * @file lichtschranke_basic.ino
 * @brief Simple example for connecting and reading a light barrier (E18-D80NK) with an Arduino Uno R4
 *
 * @details
 * This program demonstrates the basic integration of a light barrier with
 * an NPN open-collector output. The sensor is connected to a digital input pin
 * and is read cyclically.
 *
 * The light barrier provides an inverted signal:
 * - HIGH  -> no object detected
 * - LOW   -> object detected (light beam interrupted)
 *
 * The internal pull-up resistor of the Arduino is used, so no external resistor is required.
 *
 * @hardware
 * - Arduino Uno R4
 * - Light barrier E18-D80NK
 *
 * @connection
 * - Brown (VCC)  -> 5V Arduino
 * - Blue  (GND)  -> GND Arduino
 * - Black (OUT)  -> Digital pin 2 Arduino
 *
 * @author
 * Own code
 *
 * @date
 * 2026
 */

/**
 * @brief Digital input pin for the light barrier
 */
const int sensorPin = 8;

/**
 * @brief Hardware initialization
 *
 * @details
 * The sensor pin is configured as an input with internal pull-up resistor.
 * Additionally, the serial interface is started for debug output.
 */
void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);

  // Set sensor pin as input with internal pull-up resistor
  pinMode(sensorPin, INPUT_PULLUP);

  // Startup message
  Serial.println("Light barrier initialized");
}

/**
 * @brief Main program loop
 *
 * @details
 * The state of the light barrier is continuously read.
 * Depending on the signal state, it outputs whether an object is detected.
 */
void loop() {
  // Read current sensor state
  int sensorState = digitalRead(sensorPin);

  // Check if an object is detected
  if (sensorState == LOW) {
    // Light beam interrupted -> object detected
    Serial.println("Object detected");
  } else {
    // Light beam free -> no object
    Serial.println("No object");
  }

  // Small delay to stabilize output
  delay(100);
}