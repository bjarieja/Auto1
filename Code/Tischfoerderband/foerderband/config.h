/**
 * @file config.h
 * @brief Zentrale Konfigurationsdatei für die Tischfoerderband-Steuerung.
 *
 * Diese Datei enthaelt:
 * - Pinbelegungen
 * - Mechanische Konstanten
 * - Zeitkonstanten
 * - Reglerparameter
 * - Fehlercodes
 * - Zustands- und Modusdefinitionen
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// 
//  PIN-DEFINITIONEN
// 

/** @brief PWM-Pin für Rechtslauf des Motors. */
#define PIN_MOTOR_RPWM      5

/** @brief PWM-Pin für Linkslauf des Motors. */
#define PIN_MOTOR_LPWM      6

/** @brief Enable-Pin für Rechtslauf. */
#define PIN_MOTOR_R_EN      7

/** @brief Enable-Pin für Linkslauf. */
#define PIN_MOTOR_L_EN      8

/** @brief Lichtschranke 1 (linke Position). */
#define PIN_LS1             2

/** @brief Lichtschranke 2 (rechte Position). */
#define PIN_LS2             3

/**
 * @brief Encoder-Eingang.
 *
 * Muss interruptfähig sein.
 */
#define PIN_ENCODER         18

/** @brief Quittierungs- bzw. Stopptaster. */
#define PIN_TASTER          9

/** @brief Wippschalter Position Links. */
#define PIN_MODUS_L         10

/** @brief Wippschalter Position Rechts. */
#define PIN_MODUS_R         11

/** @brief Analogeingang für Geschwindigkeits-Potentiometer. */
#define PIN_POTI            A0

/** @brief Fehler- oder Status-LED. */
#define PIN_LED_FEHLER      12

//  LCD-DISPLAY (I2C – LiquidCrystal_I2C)


/** @brief I2C-Adresse des LCD-Displays. */
#define LCD_ADRESSE         0x27

/** @brief Anzahl der LCD-Spalten. */
#define LCD_SPALTEN         16

/** @brief Anzahl der LCD-Zeilen. */
#define LCD_ZEILEN          2

// 
//  MECHANISCHE / PHYSIKALISCHE KONSTANTEN
// 

/** @brief Anzahl der Loecher der Encoderscheibe. */
#define ENCODER_LOECHER     20

/**
 * @brief Umfang der Antriebstrommel in Millimetern.
 *
 * Berechnet aus einem Trommeldurchmesser von 20 mm.
 */
#define TROMMEL_UMFANG_MM   62.8f

/** @brief Minimale Bandgeschwindigkeit in cm/s. */
#define V_MIN_CM_S          3.0f

/** @brief Maximale Bandgeschwindigkeit in cm/s. */
#define V_MAX_CM_S          8.0f

/** @brief Beschleunigung des Foerderbandes in cm/s². */
#define BESCHLEUNIGUNG      2.0f

// 
//  ZEITKONSTANTEN
//

/** @brief Maximale Transportdauer bis Timeout. */
#define TIMEOUT_TRANSPORT_MS        30000UL

/** @brief Wartezeit am Ziel in Millisekunden. */
#define WARTEZEIT_ZIEL_MS           5000UL

/** @brief Wartezeit nach Objekterkennung. */
#define WARTEZEIT_START_MS          2000UL

/** @brief Entprellzeit der Lichtschranken. */
#define ENTPRELL_LS_MS              80UL

/** @brief Entprellzeit des Tasters. */
#define ENTPRELL_TASTER_MS          50UL

/** @brief Entprellzeit der Wippschalter. */
#define ENTPRELL_SCHALTER_MS        100UL

/** @brief Dauer der Modusanzeige auf dem Display. */
#define MODUS_ANZEIGE_MS            3000UL

/** @brief Intervall der PI-Regelung. */
#define REGELUNG_INTERVALL_MS       50UL

/** @brief Timeout für fehlende Encoder-Signale. */
#define ENCODER_TIMEOUT_MS          500UL

// 
//  REGLER-PARAMETER
// 
/** @brief Proportionalanteil des Reglers. */
#define KP                  8.0f

/** @brief Integralanteil des Reglers. */
#define KI                  2.5f

/** @brief Differentialanteil des Reglers. */
#define KD                  0.5f

/** @brief Minimaler PWM-Wert. */
#define PWM_MIN             0

/** @brief Maximaler PWM-Wert. */
#define PWM_MAX             255

/** @brief Mindest-PWM zum Überwinden der Haftreibung. */
#define PWM_ANLAUF          60

// 
//  FEHLERCODES
//

/** @brief Kein Fehler vorhanden. */
#define FEHLER_KEIN         0x00

/** @brief Objekt erreicht Ziel nicht rechtzeitig. */
#define FEHLER_TIMEOUT      0x01

/** @brief Zielgeschwindigkeit nicht erreichbar. */
#define FEHLER_GESCHW       0x02

/** @brief Sensorfehler erkannt. */
#define FEHLER_SENSOR       0x03

/** @brief Motorueberhitzung erkannt. */
#define FEHLER_UEBERHITZ    0x04

// 
//  ZUSTÄNDE DES AUTOMATEN
// 
/**
 * @enum Systemzustand
 * @brief Zustaende der Anlagensteuerung.
 */
enum Systemzustand {
    ZUSTAND_INIT,
    ZUSTAND_STARTBILDSCHIRM,
    ZUSTAND_WARTE_FREIGABE,
    ZUSTAND_HAUPTMENUE,
    ZUSTAND_WARTE_OBJEKT,
    ZUSTAND_ANLAUF,
    ZUSTAND_FAHRT,
    ZUSTAND_ZIEL_ERREICHT,
    ZUSTAND_ABBREMS,
    ZUSTAND_STOP,
    ZUSTAND_FEHLER,
    ZUSTAND_MODUSWECHSEL
};

// 
//  BETRIEBSMODI
// 

/**
 * @enum Betriebsmodus
 * @brief Unterstuetzte Betriebsarten der Anlage.
 */
enum Betriebsmodus {
    MODUS_LINKS,
    MODUS_RECHTS,
    MODUS_AUTOMATIK
};

// 
//  RICHTUNG
// 
/**
 * @enum Richtung
 * @brief Bewegungsrichtungen des Foerderbandes.
 */
enum Richtung {
    RICHTUNG_STOP,
    RICHTUNG_LINKS,
    RICHTUNG_RECHTS
};

#endif // CONFIG_H