/**
 * @file lichtschranke_basic.ino
 * @brief Simple example for connecting and reading a light barrier (E18-D80NK) with an Arduino Uno R4
 *
 * @details
 * NPN Light Barrier, Output and Test PRogramm 
 * The light barrier provides an inverted signal:
 * - HIGH  -> no object detected
 * - LOW   -> object detected (light beam interrupted)
 * External Pull Up Resistor(10K) is needed 
 * @hardware
 * - Arduino Uno R4
 * - Light barrier E18-D80NK
 * - Rsistor 10K 
 * @connection
 * - Brown (VCC)  -> 5V Arduino
 * - Blue  (GND)  -> GND Arduino
 * - Black (OUT)  -> Digital pin 2 Arduino
 *
 * @author
 * Bjarne Janssen 
 *
 * @date
 * 2026-13-04
 */

/**
 * @brief Digital input pin for the light barrier
 */
const int sensorPin = 8;

/**
 * @brief Hardware initialization
 *
 * @details
 * The sensor pin is configured as an input with extern pull-up resistor.
 * Additionally, the serial interface is started f.
 */
void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(115200); // Only for uploading to the microcontroller needed 

  // Set sensor pin as input with internal pull-up resistor
  pinMode(sensorPin, INPUT_PULLUP);

  // Startup message
  Serial.println("Light barrier initialized");
}

/**
 * @brief Main program loop
 *
 * @details
 * The state of the light barrier is  read continiuosly
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