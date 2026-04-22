/*
 * ============================================================
 *  Foerderband.ino
 *  Elektrisches Tischförderband – Hauptdatei
 *
 *  Hardware:
 *    – Arduino UNO R4
 *    – BTS7960 Motortreiber
 *    – Motor: Motraxx S555-1
 *    – 2× E18-D80NK Lichtschranken
 *    – F249 Gabellichtschranke (Encoder)
 *    – 3-Positions-Wippschalter
 *    – Quittierungs-/Stop-Taster
 *    – Drehpotentiometer
 *    – LCD 16×2 I2C
 *
 *  Bibliotheken (Arduino IDE Library Manager):
 *    – LiquidCrystal_I2C  (Frank de Brabander)
 *
 *  Hochschule Emden/Leer – Maschinenbau & Design
 * ============================================================
 */

#include "steuerung.h"

// Globale Instanz des Steuerungsautomaten
Steuerung foerderband;

void setup() {
    Serial.begin(115200);
    Serial.println("Tischfoerderband v1.0 – Start");
    foerderband.begin();
}

void loop() {
    foerderband.update();
}