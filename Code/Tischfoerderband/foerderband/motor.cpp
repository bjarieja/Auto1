/*
 * motor.cpp
 */

#include "motor.h"

Motor::Motor(uint8_t pinRPWM, uint8_t pinLPWM,
             uint8_t pinREN,  uint8_t pinLEN)
    : _pinRPWM(pinRPWM), _pinLPWM(pinLPWM),
      _pinREN(pinREN),   _pinLEN(pinLEN),
      _sollGeschw(0.0f), _rampGeschw(0.0f),
      _istGeschw(0.0f),  _integralFehler(0.0f),
      _letzterFehler(0.0f), _aktuellerPWM(0),
      _richtung(RICHTUNG_STOP),
      _letzteRegelung(0), _aktiviert(false)
{}

void Motor::begin() {
    pinMode(_pinRPWM, OUTPUT);
    pinMode(_pinLPWM, OUTPUT);
    pinMode(_pinREN,  OUTPUT);
    pinMode(_pinLEN,  OUTPUT);
    notStop();
}

void Motor::setSollgeschwindigkeit(float v_cms, Richtung richtung) {
    // Begrenzen auf erlaubten Bereich
    v_cms = constrain(v_cms, V_MIN_CM_S, V_MAX_CM_S);

    _sollGeschw = v_cms;
    _richtung   = richtung;
    _aktiviert  = (richtung != RICHTUNG_STOP);

    if (!_aktiviert) {
        abbremsen();
    } else {
        // Enable setzen
        digitalWrite(_pinREN, HIGH);
        digitalWrite(_pinLEN, HIGH);
        // Integralfehler zurücksetzen bei Neustart
        if (_rampGeschw < 0.1f) {
            _integralFehler = 0.0f;
            _letzterFehler  = 0.0f;
        }
    }
}

void Motor::abbremsen() {
    _sollGeschw = 0.0f;
    // Rampe läuft weiter nach unten → sanftes Abbremsen
}

void Motor::notStop() {
    digitalWrite(_pinREN, LOW);
    digitalWrite(_pinLEN, LOW);
    analogWrite(_pinRPWM, 0);
    analogWrite(_pinLPWM, 0);
    _aktuellerPWM   = 0;
    _rampGeschw     = 0.0f;
    _sollGeschw     = 0.0f;
    _integralFehler = 0.0f;
    _letzterFehler  = 0.0f;
    _aktiviert      = false;
    _richtung       = RICHTUNG_STOP;
}

void Motor::update(float istGeschwindigkeit_cms) {
    _istGeschw = istGeschwindigkeit_cms;

    uint32_t jetzt = millis();
    uint32_t dt    = jetzt - _letzteRegelung;

    if (dt < REGELUNG_INTERVALL_MS) return;
    _letzteRegelung = jetzt;

    if (!_aktiviert && _rampGeschw < 0.05f) {
        // Motor vollständig gestoppt
        motorSetzen(RICHTUNG_STOP, 0);
        return;
    }

    // 1. Rampengenerator aktualisieren
    rampeAktualisieren(dt);

    // 2. Regler ausführen (nur wenn Rampenziel > 0)
    if (_rampGeschw > 0.01f) {
        regler(_istGeschw, _rampGeschw, dt);
    } else {
        // Fahrt beendet
        motorSetzen(RICHTUNG_STOP, 0);
        if (!_aktiviert) {
            digitalWrite(_pinREN, LOW);
            digitalWrite(_pinLEN, LOW);
        }
    }
}

// ============================================================
//  Privat: Rampengenerator
//  Erhöht/Verringert _rampGeschw mit konstanter Beschleunigung
// ============================================================
void Motor::rampeAktualisieren(uint32_t dt_ms) {
    float dt_s  = dt_ms / 1000.0f;
    float delta = BESCHLEUNIGUNG * dt_s;  // cm/s Änderung pro Zyklus

    if (_rampGeschw < _sollGeschw) {
        _rampGeschw += delta;
        if (_rampGeschw > _sollGeschw) _rampGeschw = _sollGeschw;
    } else if (_rampGeschw > _sollGeschw) {
        _rampGeschw -= delta;
        if (_rampGeschw < _sollGeschw) _rampGeschw = _sollGeschw;
    }
}

// ============================================================
//  Privat: PID-Regler
// ============================================================
void Motor::regler(float ist, float soll, uint32_t dt_ms) {
    float dt_s = dt_ms / 1000.0f;

    float fehler  = soll - ist;
    _integralFehler += fehler * dt_s;

    // Anti-Windup: Integral begrenzen
    _integralFehler = constrain(_integralFehler, -50.0f, 50.0f);

    float ableitung = (fehler - _letzterFehler) / dt_s;
    _letzterFehler  = fehler;

    float ausgabe = KP * fehler
                  + KI * _integralFehler
                  + KD * ableitung;

    // Mapping: PWM-Ausgabe
    // Wir gehen davon aus, dass V_MAX_CM_S ≈ PWM 255 entspricht
    // Der Regler gibt einen Korrekturbetrag aus
    float pwmFloat = (_aktuellerPWM > 0 ? _aktuellerPWM : PWM_ANLAUF) + ausgabe;

    // Anlauf: Mindest-PWM sicherstellen
    if (pwmFloat < PWM_ANLAUF && soll > 0.1f) {
        pwmFloat = PWM_ANLAUF;
    }

    uint8_t pwm = (uint8_t)constrain((int)pwmFloat, PWM_MIN, PWM_MAX);
    _aktuellerPWM = pwm;

    motorSetzen(_richtung, pwm);
}

// ============================================================
//  Privat: Treiber ansteuern
// ============================================================
void Motor::motorSetzen(Richtung richtung, uint8_t pwm) {
    switch (richtung) {
        case RICHTUNG_RECHTS:
            analogWrite(_pinRPWM, pwm);
            analogWrite(_pinLPWM, 0);
            break;
        case RICHTUNG_LINKS:
            analogWrite(_pinRPWM, 0);
            analogWrite(_pinLPWM, pwm);
            break;
        default:  // STOP
            analogWrite(_pinRPWM, 0);
            analogWrite(_pinLPWM, 0);
            break;
    }
}

bool    Motor::istStillstand()         const { return _aktuellerPWM == 0 && _rampGeschw < 0.05f; }
float   Motor::getIstGeschwindigkeit() const { return _istGeschw;     }
float   Motor::getSollgeschwindigkeit()const { return _sollGeschw;    }
uint8_t Motor::getAktuellerPWM()       const { return _aktuellerPWM; }