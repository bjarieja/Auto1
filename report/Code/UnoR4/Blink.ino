/*
* This Arduino UNO R4 code was developed by newbiely.com
*
* This Arduino UNO R4 code is made available for public use without any restriction
*
* For comprehensive instructions and wiring diagrams, please visit:
* https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-blink-led
*/

define LED-PIN 13  // The Arduino UNO R4 pin connected to the LED
// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin 13 as an output.
	pinMode(LED-PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
	digitalWrite(LED-PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
	delay(500);                   // wait for 500 milliseconds
	digitalWrite(LED-PIN, LOW);   // turn the LED off by making the voltage LOW
	delay(500);                   // wait for 500 milliseconds
}
