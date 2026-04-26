/*
 * sensoren.h
 * Verwaltung aller Sensoren:
 *   – E18-D80NK Lichtschranken (entprellt)
 *   – F249 Encoder (Interrupt-basiert)
 *   – Potentiometer Geschwindigkeitssollwert
 *   – Taster / Wippschalter (entprellt)
 */

#ifndef SENSOREN_H
#define SENSOREN_H

#include "config.h"

// ============================================================
//  KLASSE: Entprellter Digitaleingang
// ============================================================
class DigitalEntprellt {
public:
    /**
     * @param pin       Arduino-Pin
     * @param aktivLow  true = Signal aktiv bei LOW (NPN-Sensor, Taster gegen GND)
     * @param entprellMs Entprellzeit in Millisekunden
     */
    DigitalEntprellt(uint8_t pin, bool aktivLow, uint32_t entprellMs);

    void    begin();
    void    update();          // Muss regelmäßig in loop() aufgerufen werden

    bool    istAktiv()  const; // Aktueller (entprellter) Zustand
    bool    steigende() const; // Flanke: inaktiv → aktiv
    bool    fallende()  const; // Flanke: aktiv  → inaktiv

private:
    uint8_t  _pin;
    bool     _aktivLow;
    uint32_t _entprellMs;
    bool     _zustand;
    bool     _letzterZustand;
    bool     _steigende;
    bool     _fallende;
    uint32_t _letzteAenderung;
};

// ============================================================
//  KLASSE: Encoder
// ============================================================
class Encoder {
public:
    Encoder(uint8_t pin);
    void    begin();

    // ISR – muss als friend oder statisch aufgerufen werden
    static void IRAM_ATTR isrImpuls();

    void    update();                  // Geschwindigkeit berechnen
    float   getGeschwindigkeit_cms();  // cm/s
    uint32_t getGesamtImpulse() const;

    static volatile uint32_t _impulse;

private:
    uint8_t  _pin;
    uint32_t _letzteBerechnung;
    uint32_t _impulseVorher;
    float    _geschwindigkeit_cms;
    uint32_t _gesamtImpulse;
};

// ============================================================
//  KLASSE: Potentiometer → Geschwindigkeitssollwert
// ============================================================
class Potentiometer {
public:
    Potentiometer(uint8_t pin);
    void  begin();
    float getSollgeschwindigkeit_cms();  // Gibt V_MIN_CM_S … V_MAX_CM_S zurück

private:
    uint8_t _pin;
};

// ============================================================
//  KLASSE: 3-Positions-Wippschalter
// ============================================================
class Wippschalter {
public:
    Wippschalter(uint8_t pinL, uint8_t pinR);
    void           begin();
    void           update();
    Betriebsmodus  getModus() const;
    bool           modusGeaendert() const;

private:
    DigitalEntprellt _switchL;
    DigitalEntprellt _switchR;
    Betriebsmodus    _modus;
    Betriebsmodus    _letzterModus;
    bool             _geaendert;
};

#endif // SENSOREN_H
