/**
 * @file BTS7960Test.ino
 * @brief Motor control with BTS7960, Arduino UNO R4, and potentiometer.
 *
 * The potentiometer controls the motor speed.
 * A direction variable selects the rotation direction:
 * - 1 = clockwise
 * - 0 = counterclockwise
 *
 * Connections:
 * - Potentiometer center pin -> A0
 * - BTS7960 RPWM -> D5
 * - BTS7960 LPWM -> D6
 * - BTS7960 R_EN -> 5V
 * - BTS7960 L_EN -> 5V
 * - BTS7960 VCC -> 5V
 * - BTS7960 GND -> GND
 */
/** @brief Analog input pin for potentiometer */
const int POT_PIN = A0;

/** @brief PWM pin for clockwise rotation */
const int RPWM_PIN = 5;

/** @brief PWM pin for counterclockwise rotation */
const int LPWM_PIN = 6;
/**
 * @brief Motor direction variable.
 *
 * 1 = clockwise
 * 0 = counterclockwise
 */
int motorDirection = 1;
/**
 * @brief Initializes pin configuration.
 */
void setup()
{
    pinMode(RPWM_PIN, OUTPUT);
    pinMode(LPWM_PIN, OUTPUT);
}
/**
 * @brief Controls motor speed and direction.
 *
 * The potentiometer value (0 to 1023) is mapped
 * to a PWM value (0 to 255).
 */
void loop()
{
    // Read potentiometer value
    int potValue = analogRead(POT_PIN);
    // Convert analog value to PWM range
    int pwmValue = map(potValue, 0, 1023, 0, 255);
    // Set motor direction
    if (motorDirection == 1)
    {
        // Clockwise rotation
        analogWrite(RPWM_PIN, pwmValue);
        analogWrite(LPWM_PIN, 0);
    }
    else
    {
        // Counterclockwise rotation
        analogWrite(RPWM_PIN, 0);
        analogWrite(LPWM_PIN, pwmValue);
    }
}