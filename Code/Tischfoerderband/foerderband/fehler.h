/*
 * fehler.h
 * Fehlerverwaltung: Setzen, Auslesen, Quittieren
 */

#ifndef FEHLER_H
#define FEHLER_H

#include "config.h"

class Fehlerverwaltung {
public:
    Fehlerverwaltung(uint8_t pinLED);
    void  begin();

    void  setze(uint8_t fehlerCode);
    void  quittiere();
    bool  istFehlerAktiv()     const;
    uint8_t getAktuellerFehler() const;
    const char* getFehlertext()  const;

    void  update();  // LED blinken lassen

private:
    uint8_t  _pinLED;
    uint8_t  _fehlerCode;
    bool     _aktiv;
    uint32_t _letztesBlinken;
    bool     _ledZustand;
};

#endif // FEHLER_H