/*
 * steuerung.cpp
 * Zustandsautomat – Kernlogik des Tischförderbands
 */

#include "steuerung.h"

Steuerung::Steuerung()
    : _motor(PIN_MOTOR_RPWM, PIN_MOTOR_LPWM,
             PIN_MOTOR_R_EN,  PIN_MOTOR_L_EN),
      _encoder(PIN_ENCODER),
      _poti(PIN_POTI),
      _ls1(PIN_LS1, true, ENTPRELL_LS_MS),
      _ls2(PIN_LS2, true, ENTPRELL_LS_MS),
      _taster(PIN_TASTER, true, ENTPRELL_TASTER_MS),
      _wippschalter(PIN_MODUS_L, PIN_MODUS_R),
      _fehler(PIN_LED_FEHLER),
      _zustand(ZUSTAND_INIT),
      _modus(MODUS_AUTOMATIK),
      _fahrtrichtung(RICHTUNG_STOP),
      _autoZuletzt_ls1(false),
      _zeitstempel(0)
{}

void Steuerung::begin() {
    _motor.begin();
    _encoder.begin();
    _poti.begin();
    _ls1.begin();
    _ls2.begin();
    _taster.begin();
    _wippschalter.begin();
    _display.begin();
    _fehler.begin();

    _zustandsuebergang(ZUSTAND_STARTBILDSCHIRM);
}

// ============================================================
//  Hauptschleife
// ============================================================
void Steuerung::update() {
    // --- Alle Eingaben aktualisieren ---
    _ls1.update();
    _ls2.update();
    _taster.update();
    _wippschalter.update();
    _encoder.update();
    _fehler.update();

    // --- Motorregelung ---
    _motor.update(_encoder.getGeschwindigkeit_cms());

    // --- Moduswechsel prüfen (außer bei Fehler und Init) ---
    if (_zustand != ZUSTAND_INIT &&
        _zustand != ZUSTAND_STARTBILDSCHIRM &&
        _zustand != ZUSTAND_WARTE_FREIGABE &&
        _zustand != ZUSTAND_FEHLER) {
        _pruefeModuswechsel();
    }

    // --- Zustandsautomat ---
    _verarbeiteZustand();
}

// ============================================================
//  Zustandsübergang
// ============================================================
void Steuerung::_zustandsuebergang(Systemzustand neu) {
    _zustand    = neu;
    _zeitstempel = millis();

    // Eintrittsaktionen
    switch (_zustand) {
        case ZUSTAND_STARTBILDSCHIRM:
            _display.zeigeStartbildschirm();
            break;
        case ZUSTAND_WARTE_FREIGABE:
            _display.zeigeFreigabeAbfrage();
            break;
        case ZUSTAND_HAUPTMENUE:
        case ZUSTAND_WARTE_OBJEKT:
            _display.zeigeWarteObjekt(_modus);
            break;
        case ZUSTAND_FEHLER:
            _motor.notStop();
            _display.zeigeFehler(_fehler.getFehlertext());
            break;
        case ZUSTAND_ZIEL_ERREICHT:
            _motor.abbremsen();
            _display.zeigeZielErreicht();
            break;
        case ZUSTAND_MODUSWECHSEL:
            _motor.abbremsen();
            _display.zeigeModuswechselHinweis();
            break;
        case ZUSTAND_STOP:
            _motor.abbremsen();
            break;
        default:
            break;
    }
}

// ============================================================
//  Zustandsverarbeitung
// ============================================================
void Steuerung::_verarbeiteZustand() {
    switch (_zustand) {

        // -------------------------------------------------------
        case ZUSTAND_STARTBILDSCHIRM:
            // 2 Sekunden warten, dann Freigabe abfragen
            if (millis() - _zeitstempel >= 2000) {
                _zustandsuebergang(ZUSTAND_WARTE_FREIGABE);
            }
            break;

        // -------------------------------------------------------
        case ZUSTAND_WARTE_FREIGABE:
            // Warte auf Quittierung (beide LS müssen frei sein)
            if (_taster.steigende()) {
                if (!_ls1.istAktiv() && !_ls2.istAktiv()) {
                    _modus = _wippschalter.getModus();
                    _zustandsuebergang(ZUSTAND_WARTE_OBJEKT);
                } else {
                    // LS noch blockiert → Fehlermeldung
                    _display.zeigeNachricht("LS noch belegt!",
                                            "Band leeren!   ");
                }
            }
            break;

        // -------------------------------------------------------
        case ZUSTAND_WARTE_OBJEKT:
            _aktualisiereAnzeige();

            switch (_modus) {
                case MODUS_LINKS:
                    // Warte auf Objekt in LS2 (rechte Seite → nach links)
                    if (_ls2.steigende()) {
                        _fahrtrichtung = RICHTUNG_LINKS;
                        _zustandsuebergang(ZUSTAND_ANLAUF);
                    }
                    break;

                case MODUS_RECHTS:
                    // Warte auf Objekt in LS1 (linke Seite → nach rechts)
                    if (_ls1.steigende()) {
                        _fahrtrichtung = RICHTUNG_RECHTS;
                        _zustandsuebergang(ZUSTAND_ANLAUF);
                    }
                    break;

                case MODUS_AUTOMATIK:
                    // Warte auf Objekt in LS1 oder LS2
                    if (_ls1.steigende()) {
                        _fahrtrichtung    = RICHTUNG_RECHTS;
                        _autoZuletzt_ls1  = true;
                        _zustandsuebergang(ZUSTAND_ANLAUF);
                    } else if (_ls2.steigende()) {
                        _fahrtrichtung    = RICHTUNG_LINKS;
                        _autoZuletzt_ls1  = false;
                        _zustandsuebergang(ZUSTAND_ANLAUF);
                    }
                    break;
            }

            // Stop-Taster (wenn kein Fehler)
            if (_taster.steigende() && !_fehler.istFehlerAktiv()) {
                _zustandsuebergang(ZUSTAND_STOP);
            }
            break;

        // -------------------------------------------------------
        case ZUSTAND_ANLAUF:
            // 2 Sekunden warten, dann losfahren
            if (millis() - _zeitstempel >= WARTEZEIT_START_MS) {
                float soll = _poti.getSollgeschwindigkeit_cms();
                _starteMotor(_fahrtrichtung, soll);
                _zustandsuebergang(ZUSTAND_FAHRT);
            }
            break;

        // -------------------------------------------------------
        case ZUSTAND_FAHRT:
            // Sollgeschwindigkeit nachführen (Poti kann während Fahrt geändert werden)
            {
                float soll = _poti.getSollgeschwindigkeit_cms();
                _motor.setSollgeschwindigkeit(soll, _fahrtrichtung);
            }

            _aktualisiereAnzeige();
            _pruefeTimeout();

            // Ziel-Lichtschranke prüfen
            switch (_modus) {
                case MODUS_LINKS:
                    // Ziel: LS1 (links) – Objekt verlässt das Band
                    if (_ls1.steigende()) {
                        // Objekt hat LS1 erreicht → Band noch kurz weiterfahren
                        // bis Objekt das Band verlässt (fallende Flanke LS1)
                    }
                    if (_ls1.fallende() && !_ls1.istAktiv()) {
                        // Objekt hat LS1 passiert → Band stoppen
                        _zustandsuebergang(ZUSTAND_ABBREMS);
                    }
                    break;

                case MODUS_RECHTS:
                    if (_ls2.steigende()) { /* Objekt nähert sich LS2 */ }
                    if (_ls2.fallende() && !_ls2.istAktiv()) {
                        _zustandsuebergang(ZUSTAND_ABBREMS);
                    }
                    break;

                case MODUS_AUTOMATIK:
                    // Ziel-LS abhängig von Fahrtrichtung
                    if (_fahrtrichtung == RICHTUNG_LINKS && _ls1.steigende()) {
                        _zustandsuebergang(ZUSTAND_ZIEL_ERREICHT);
                    } else if (_fahrtrichtung == RICHTUNG_RECHTS && _ls2.steigende()) {
                        _zustandsuebergang(ZUSTAND_ZIEL_ERREICHT);
                    }
                    break;
            }

            // Stop-Taster
            if (_taster.steigende() && !_fehler.istFehlerAktiv()) {
                _zustandsuebergang(ZUSTAND_STOP);
            }
            break;

        // -------------------------------------------------------
        case ZUSTAND_ABBREMS:
            // Warte bis Motor stillsteht
            if (_motor.istStillstand()) {
                _zustandsuebergang(ZUSTAND_WARTE_OBJEKT);
            }
            break;

        // -------------------------------------------------------
        case ZUSTAND_ZIEL_ERREICHT:
            // Automatik: Band hält an, 5 Sekunden warten
            if (_motor.istStillstand()) {
                if (millis() - _zeitstempel >= WARTEZEIT_ZIEL_MS) {
                    // Nach 5 Sekunden: Rückfahrt oder auf neues Objekt warten
                    if (_ls1.istAktiv() || _ls2.istAktiv()) {
                        // Objekt noch vorhanden → Rückfahrt
                        _fahrtrichtung = (_fahrtrichtung == RICHTUNG_LINKS)
                                         ? RICHTUNG_RECHTS : RICHTUNG_LINKS;
                        float soll = _poti.getSollgeschwindigkeit_cms();
                        _starteMotor(_fahrtrichtung, soll);
                        _zustandsuebergang(ZUSTAND_FAHRT);
                    } else {
                        // Objekt entfernt → neues Warten
                        _zustandsuebergang(ZUSTAND_WARTE_OBJEKT);
                    }
                }
            }
            break;

        // -------------------------------------------------------
        case ZUSTAND_STOP:
            _aktualisiereAnzeige();
            // Warte auf Quittierung
            if (_taster.steigende()) {
                _zustandsuebergang(ZUSTAND_WARTE_OBJEKT);
            }
            break;

        // -------------------------------------------------------
        case ZUSTAND_FEHLER:
            // Motor ist bereits gestoppt (notStop in Eintrittsaktion)
            // Warte auf Quittierung
            if (_taster.steigende()) {
                _fehler.quittiere();
                _zustandsuebergang(ZUSTAND_WARTE_OBJEKT);
            }
            break;

        // -------------------------------------------------------
        case ZUSTAND_MODUSWECHSEL:
            // Warte bis Band stillsteht, dann Quittierung verlangen
            if (_motor.istStillstand()) {
                if (_taster.steigende()) {
                    _modus = _wippschalter.getModus();
                    // Modus kurz anzeigen
                    _display.zeigeModus(_modus);
                    _zeitstempel = millis();
                    // Kurz warten, dann in WARTE_OBJEKT
                    while (millis() - _zeitstempel < MODUS_ANZEIGE_MS) {
                        // Einfache Blockierphase (kurz, da nur Anzeige)
                        delay(10);
                    }
                    _zustandsuebergang(ZUSTAND_WARTE_OBJEKT);
                }
            }
            break;

        default:
            _zustandsuebergang(ZUSTAND_STARTBILDSCHIRM);
            break;
    }
}

// ============================================================
//  Hilfsmethoden
// ============================================================

void Steuerung::_starteMotor(Richtung richtung, float sollGeschw) {
    _motor.setSollgeschwindigkeit(sollGeschw, richtung);
}

void Steuerung::_stoppeMotor() {
    _motor.abbremsen();
}

void Steuerung::_pruefeTimeout() {
    if (_zustand != ZUSTAND_FAHRT) return;
    if (millis() - _zeitstempel >= TIMEOUT_TRANSPORT_MS) {
        _fehler.setze(FEHLER_TIMEOUT);
        _zustandsuebergang(ZUSTAND_FEHLER);
    }
}

void Steuerung::_pruefeModuswechsel() {
    _wippschalter.update();
    if (_wippschalter.modusGeaendert()) {
        // Nur reagieren wenn sich Modus tatsächlich geändert hat
        if (_wippschalter.getModus() != _modus) {
            _zustandsuebergang(ZUSTAND_MODUSWECHSEL);
        }
    }
}

void Steuerung::_aktualisiereAnzeige() {
    static uint32_t letzteAnzeige = 0;
    if (millis() - letzteAnzeige >= 250) {  // Anzeige 4× pro Sekunde
        letzteAnzeige = millis();
        _display.zeigeHauptmenue(
            _ls1.istAktiv(),
            _ls2.istAktiv(),
            _modus,
            _encoder.getGeschwindigkeit_cms(),
            _poti.getSollgeschwindigkeit_cms()
        );
    }
}
