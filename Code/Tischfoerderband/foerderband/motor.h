/*
 * motor.h
 * Motorsteuerung über BTS7960
 * – Sanfte Beschleunigung / Abbremsung (Rampengenerator)
 * – PI(D)-Geschwindigkeitsregelung
 */

#ifndef MOTOR_H
#define MOTOR_H

#include "config.h"

class Motor {
public:
    Motor(uint8_t pinRPWM, uint8_t pinLPWM,
          uint8_t pinREN,  uint8_t pinLEN);

    void  begin();
    void  update(float istGeschwindigkeit_cms); // Muss zyklisch aufgerufen werden

    // Sollwert setzen (positive Werte = rechts, negative = links)
    void  setSollgeschwindigkeit(float v_cms, Richtung richtung);

    // Sofortiger Not-Stop (PWM = 0, Enable LOW)
    void  notStop();

    // Sanftes Abbremsen (Zielgeschwindigkeit = 0)
    void  abbremsen();

    bool  istStillstand() const;
    float getIstGeschwindigkeit() const;
    float getSollgeschwindigkeit() const;
    uint8_t getAktuellerPWM() const;

private:
    uint8_t _pinRPWM, _pinLPWM, _pinREN, _pinLEN;

    float    _sollGeschw;       // cm/s (Zielgeschwindigkeit)
    float    _rampGeschw;       // cm/s (aktuell angestrebter Rampenwert)
    float    _istGeschw;        // cm/s (gemessen vom Encoder)
    float    _integralFehler;
    float    _letzterFehler;
    uint8_t  _aktuellerPWM;
    Richtung _richtung;
    uint32_t _letzteRegelung;
    bool     _aktiviert;

    void  motorSetzen(Richtung richtung, uint8_t pwm);
    void  regler(float ist, float soll, uint32_t dt_ms);
    void  rampeAktualisieren(uint32_t dt_ms);
};

#endif // MOTOR_H