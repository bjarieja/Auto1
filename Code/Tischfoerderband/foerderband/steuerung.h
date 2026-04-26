/*
 * steuerung.h
 * Zentraler Zustandsautomat des Tischförderbands
 */

#ifndef STEUERUNG_H
#define STEUERUNG_H

#include "config.h"
#include "motor.h"
#include "sensoren.h"
#include "display.h"
#include "fehler.h"

class Steuerung {
public:
    Steuerung();
    void begin();
    void update();  // Wird in loop() aufgerufen

private:
    // --- Hardware-Objekte ---
    Motor              _motor;
    Encoder            _encoder;
    Potentiometer      _poti;
    DigitalEntprellt   _ls1;
    DigitalEntprellt   _ls2;
    DigitalEntprellt   _taster;
    Wippschalter       _wippschalter;
    Display            _display;
    Fehlerverwaltung   _fehler;

    // --- Zustand ---
    Systemzustand      _zustand;
    Betriebsmodus      _modus;
    Richtung           _fahrtrichtung;

    // --- Automatik-Pendelzustand ---
    // Merkt sich, wohin die letzte Fahrt ging
    bool               _autoZuletzt_ls1;  // true = zuletzt von LS2→LS1 gefahren

    // --- Zeitstempel ---
    uint32_t           _zeitstempel;      // Mehrzweck-Timer

    // --- Hilfsmethoden ---
    void _zustandsuebergang(Systemzustand neu);
    void _verarbeiteZustand();
    bool _tasterGedrueckt();   // Steigende Flanke mit Mehrfachschutz
    void _starteMotor(Richtung richtung, float sollGeschw);
    void _stoppeMotor();
    void _pruefeTimeout();
    void _pruefeModuswechsel();
    void _aktualisiereAnzeige();
};

#endif // STEUERUNG_H