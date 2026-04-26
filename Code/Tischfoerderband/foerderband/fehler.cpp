/*
 * fehler.cpp
 */

#include "fehler.h"

Fehlerverwaltung::Fehlerverwaltung(uint8_t pinLED)
    : _pinLED(pinLED), _fehlerCode(FEHLER_KEIN),
      _aktiv(false), _letztesBlinken(0), _ledZustand(false)
{}

void Fehlerverwaltung::begin() {
    pinMode(_pinLED, OUTPUT);
    digitalWrite(_pinLED, LOW);
}

void Fehlerverwaltung::setze(uint8_t fehlerCode) {
    _fehlerCode = fehlerCode;
    _aktiv      = true;
}

void Fehlerverwaltung::quittiere() {
    _fehlerCode = FEHLER_KEIN;
    _aktiv      = false;
    digitalWrite(_pinLED, LOW);
    _ledZustand = false;
}

bool    Fehlerverwaltung::istFehlerAktiv()      const { return _aktiv;     }
uint8_t Fehlerverwaltung::getAktuellerFehler()  const { return _fehlerCode; }

const char* Fehlerverwaltung::getFehlertext() const {
    switch (_fehlerCode) {
        case FEHLER_TIMEOUT:   return "Fehler: Timeout";
        case FEHLER_GESCHW:    return "Fehler: Geschw.";
        case FEHLER_SENSOR:    return "Fehler: Sensor";
        case FEHLER_UEBERHITZ: return "Fehler: Ueberhitz";
        default:               return "Kein Fehler";
    }
}

void Fehlerverwaltung::update() {
    if (!_aktiv) return;
    // Fehler-LED im 500ms-Takt blinken
    if (millis() - _letztesBlinken >= 500) {
        _letztesBlinken = millis();
        _ledZustand     = !_ledZustand;
        digitalWrite(_pinLED, _ledZustand ? HIGH : LOW);
    }
}
