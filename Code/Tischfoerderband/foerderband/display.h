/*
 * display.h
 * LCD-Anzeige (16×2, I2C)
 * Zeigt Startbildschirm, Hauptmenü, Fehlermeldungen
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include "config.h"

class Display {
public:
    Display();
    void begin();

    // Bildschirme
    void zeigeStartbildschirm();
    void zeigeFreigabeAbfrage();
    void zeigeHauptmenue(bool ls1, bool ls2,
                         Betriebsmodus modus,
                         float istGeschw,
                         float sollGeschw);
    void zeigeModus(Betriebsmodus modus);
    void zeigeWarteObjekt(Betriebsmodus modus);
    void zeigeFehler(const char* text);
    void zeigeZielErreicht();
    void zeigeModuswechselHinweis();
    void zeigeNachricht(const char* zeile1, const char* zeile2);
    void loeschen();

private:
    LiquidCrystal_I2C _lcd;
    const char* _modusText(Betriebsmodus m);
};

#endif // DISPLAY_H
