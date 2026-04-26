%Diese Datei ist die Headfer Datei, in welcher alle globalen Definitionen sowie die Zusammenführung der einzelnen COdes geschehen soll 

/*
 * config.h
 * Zentrale Konfigurationsdatei – Pins, Konstanten, Enumerationen
 * Tischförderband Steuerung v1.0
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================
//  PIN-DEFINITIONEN
// ============================================================

// --- BTS7960 Motortreiber ---
// RPWM  = Rechtslauf PWM
// LPWM  = Linkslauf  PWM
// R_EN  = Rechtslauf Enable
// L_EN  = Linkslauf  Enable
#define PIN_MOTOR_RPWM      5    // PWM-fähiger Pin
#define PIN_MOTOR_LPWM      6    // PWM-fähiger Pin
#define PIN_MOTOR_R_EN      7
#define PIN_MOTOR_L_EN      8

// --- Lichtschranken E18-D80NK (NPN, LOW = unterbrochen) ---
#define PIN_LS1             2    // Lichtschranke 1 (links)
#define PIN_LS2             3    // Lichtschranke 2 (rechts)

// --- Encoder F249 Gabellichtschranke ---
#define PIN_ENCODER         18   // Interrupt-fähiger Pin (UNO R4: alle digital)

// --- Bedienelemente ---
#define PIN_TASTER          9    // Quittierungs-/Stop-Taster (LOW aktiv)
#define PIN_MODUS_L         10   // Wippschalter Position L
#define PIN_MODUS_R         11   // Wippschalter Position R
// Modus A (Automatik) = beide LOW

// --- Drehpotentiometer Geschwindigkeit ---
#define PIN_POTI            A0

// --- Status-LED (optional) ---
#define PIN_LED_FEHLER      12

// ============================================================
//  LCD-DISPLAY (I2C – LiquidCrystal_I2C)
// ============================================================
#define LCD_ADRESSE         0x27
#define LCD_SPALTEN         16
#define LCD_ZEILEN          2

// ============================================================
//  MECHANISCHE / PHYSIKALISCHE KONSTANTEN
// ============================================================

// Encoder / Lochscheibe
#define ENCODER_LOECHER     20        // Löcher auf der Lochscheibe
#define TROMMEL_UMFANG_MM   62.8f     // Umfang der Antriebstrommel in mm
                                      // (Ø 20 mm → π × 20 ≈ 62,8 mm)

// Bandgeschwindigkeit
#define V_MIN_CM_S          3.0f      // cm/s  (einstellbar ab 3 cm/s)
#define V_MAX_CM_S          8.0f      // cm/s  (max. 8 cm/s laut Spezifikation)
#define BESCHLEUNIGUNG      2.0f      // cm/s² laut Spezifikation

// ============================================================
//  ZEITKONSTANTEN  (alle in Millisekunden)
// ============================================================
#define TIMEOUT_TRANSPORT_MS        30000UL  // 30 s max. Transportzeit
#define WARTEZEIT_ZIEL_MS           5000UL   // 5 s Ruhezeit am Ziel (Automatik)
#define WARTEZEIT_START_MS          2000UL   // 2 s Wartezeit nach Erkennung
#define ENTPRELL_LS_MS              80UL     // Entprellzeit Lichtschranken
#define ENTPRELL_TASTER_MS          50UL     // Entprellzeit Taster
#define ENTPRELL_SCHALTER_MS        100UL    // Entprellzeit Wippschalter
#define MODUS_ANZEIGE_MS            3000UL   // Anzeigedauer Modusname
#define REGELUNG_INTERVALL_MS       50UL     // PID-Regelintervall
#define ENCODER_TIMEOUT_MS          500UL    // kein Signal → Geschw. = 0

// ============================================================
//  REGLER-PARAMETER  (PI-Regler für Geschwindigkeit)
// ============================================================
#define KP                  8.0f
#define KI                  2.5f
#define KD                  0.5f

// PWM-Grenzen
#define PWM_MIN             0
#define PWM_MAX             255
#define PWM_ANLAUF          60       // Mindest-PWM zum Überwinden der Haftreibung

// ============================================================
//  FEHLERCODES
// ============================================================
#define FEHLER_KEIN         0x00
#define FEHLER_TIMEOUT      0x01     // Objekt erreicht Ziel nicht rechtzeitig
#define FEHLER_GESCHW       0x02     // Zielgeschwindigkeit nicht erreichbar
#define FEHLER_SENSOR       0x03     // Sensor liefert keine Daten
#define FEHLER_UEBERHITZ    0x04     // Motor überhitzt (zukünftig via NTC)

// ============================================================
//  ZUSTÄNDE DES AUTOMATEN
// ============================================================
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

// ============================================================
//  BETRIEBSMODI
// ============================================================
enum Betriebsmodus {
    MODUS_LINKS,       // L – Transport von LS2 → LS1
    MODUS_RECHTS,      // R – Transport von LS1 → LS2
    MODUS_AUTOMATIK    // A – Pendelbetrieb
};

// ============================================================
//  RICHTUNG
// ============================================================
enum Richtung {
    RICHTUNG_STOP,
    RICHTUNG_LINKS,
    RICHTUNG_RECHTS
};

#endif // CONFIG_H

