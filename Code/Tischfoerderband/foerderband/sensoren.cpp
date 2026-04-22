/*
 * sensoren.cpp
 */

#include "sensoren.h"

// ============================================================
//  DigitalEntprellt
// ============================================================

DigitalEntprellt::DigitalEntprellt(uint8_t pin,
                                   bool aktivLow,
                                   uint32_t entprellMs)
    : _pin(pin), _aktivLow(aktivLow), _entprellMs(entprellMs),
      _zustand(false), _letzterZustand(false),
      _steigende(false), _fallende(false),
      _letzteAenderung(0)
{}

void DigitalEntprellt::begin() {
    pinMode(_pin, _aktivLow ? INPUT_PULLUP : INPUT);
    // Initialzustand einlesen
    bool roh = (digitalRead(_pin) == LOW);
    _zustand        = _aktivLow ? roh : !roh;
    _letzterZustand = _zustand;
}

void DigitalEntprellt::update() {
    _steigende = false;
    _fallende  = false;

    bool rohAktiv = (digitalRead(_pin) == (_aktivLow ? LOW : HIGH));

    // Nur nach Ablauf der Entprellzeit übernehmen
    if (rohAktiv != _letzterZustand) {
        _letzteAenderung = millis();
        _letzterZustand  = rohAktiv;
    }

    if ((millis() - _letzteAenderung) >= _entprellMs) {
        if (rohAktiv != _zustand) {
            bool alt    = _zustand;
            _zustand    = rohAktiv;
            _steigende  = (!alt &&  _zustand);
            _fallende   = ( alt && !_zustand);
        }
    }
}

bool DigitalEntprellt::istAktiv()  const { return _zustand;   }
bool DigitalEntprellt::steigende() const { return _steigende; }
bool DigitalEntprellt::fallende()  const { return _fallende;  }

// ============================================================
//  Encoder
// ============================================================

volatile uint32_t Encoder::_impulse = 0;

void IRAM_ATTR Encoder::isrImpuls() {
    Encoder::_impulse++;
}

Encoder::Encoder(uint8_t pin)
    : _pin(pin), _letzteBerechnung(0), _impulseVorher(0),
      _geschwindigkeit_cms(0.0f), _gesamtImpulse(0)
{}

void Encoder::begin() {
    pinMode(_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_pin),
                    Encoder::isrImpuls,
                    RISING);
}

void Encoder::update() {
    uint32_t jetzt = millis();
    uint32_t dt    = jetzt - _letzteBerechnung;

    if (dt >= REGELUNG_INTERVALL_MS) {
        noInterrupts();
        uint32_t aktuelleImpulse = _impulse;
        interrupts();

        uint32_t delta = aktuelleImpulse - _impulseVorher;
        _gesamtImpulse  = aktuelleImpulse;
        _impulseVorher  = aktuelleImpulse;

        if (dt < ENCODER_TIMEOUT_MS) {
            // Strecke pro Impuls = Trommelumfang / Lochanzahl [mm]
            // Geschwindigkeit [cm/s] = (delta * mm_pro_Impuls) / dt[s] / 10
            float mmProImpuls   = (float)TROMMEL_UMFANG_MM / ENCODER_LOECHER;
            float strecke_mm    = delta * mmProImpuls;
            float dt_s          = dt / 1000.0f;
            _geschwindigkeit_cms = (strecke_mm / 10.0f) / dt_s;
        } else {
            _geschwindigkeit_cms = 0.0f;
        }

        _letzteBerechnung = jetzt;
    }

    // Timeout-Prüfung: kein Impuls seit ENCODER_TIMEOUT_MS → Stillstand
    noInterrupts();
    uint32_t imp = _impulse;
    interrupts();
    if (imp == _impulseVorher &&
        (jetzt - _letzteBerechnung) >= ENCODER_TIMEOUT_MS) {
        _geschwindigkeit_cms = 0.0f;
    }
}

float    Encoder::getGeschwindigkeit_cms()  { return _geschwindigkeit_cms; }
uint32_t Encoder::getGesamtImpulse() const  { return _gesamtImpulse; }

// ============================================================
//  Potentiometer
// ============================================================

Potentiometer::Potentiometer(uint8_t pin) : _pin(pin) {}

void Potentiometer::begin() {
    pinMode(_pin, INPUT);
}

float Potentiometer::getSollgeschwindigkeit_cms() {
    // Mehrfachsampling für stabilere Messung
    uint32_t summe = 0;
    for (uint8_t i = 0; i < 8; i++) {
        summe += analogRead(_pin);
        delayMicroseconds(100);
    }
    float wert = summe / 8.0f;  // 0 … 1023

    // Mapping: 0…1023 → V_MIN_CM_S … V_MAX_CM_S
    float v = V_MIN_CM_S +
              (wert / 1023.0f) * (V_MAX_CM_S - V_MIN_CM_S);
    return v;
}

// ============================================================
//  Wippschalter
// ============================================================

Wippschalter::Wippschalter(uint8_t pinL, uint8_t pinR)
    : _switchL(pinL, true, ENTPRELL_SCHALTER_MS),
      _switchR(pinR, true, ENTPRELL_SCHALTER_MS),
      _modus(MODUS_AUTOMATIK),
      _letzterModus(MODUS_AUTOMATIK),
      _geaendert(false)
{}

void Wippschalter::begin() {
    _switchL.begin();
    _switchR.begin();
    update();
    _letzterModus = _modus;
    _geaendert    = false;
}

void Wippschalter::update() {
    _switchL.update();
    _switchR.update();

    Betriebsmodus neu;
    if (_switchL.istAktiv() && !_switchR.istAktiv()) {
        neu = MODUS_LINKS;
    } else if (!_switchL.istAktiv() && _switchR.istAktiv()) {
        neu = MODUS_RECHTS;
    } else {
        neu = MODUS_AUTOMATIK;
    }

    _geaendert = (neu != _letzterModus);
    _modus     = neu;
    _letzterModus = _modus;
}

Betriebsmodus Wippschalter::getModus()        const { return _modus;    }
bool          Wippschalter::modusGeaendert()  const { return _geaendert; }