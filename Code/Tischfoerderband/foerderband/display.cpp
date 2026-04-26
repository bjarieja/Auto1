/*
 * display.cpp
 */

#include "display.h"

Display::Display()
    : _lcd(LCD_ADRESSE, LCD_SPALTEN, LCD_ZEILEN)
{}

void Display::begin() {
    _lcd.init();
    _lcd.backlight();
    _lcd.clear();
}

void Display::zeigeStartbildschirm() {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("  Tischfoerder  ");
    _lcd.setCursor(0, 1);
    _lcd.print("  band  v1.0    ");
}

void Display::zeigeFreigabeAbfrage() {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("LS frei?        ");
    _lcd.setCursor(0, 1);
    _lcd.print("Quitt.=Bestaet. ");
}

void Display::zeigeHauptmenue(bool ls1, bool ls2,
                               Betriebsmodus modus,
                               float istGeschw,
                               float sollGeschw) {
    // Zeile 1: LS-Status + Modus
    _lcd.setCursor(0, 0);
    _lcd.print("LS1:");
    _lcd.print(ls1 ? "X" : "O");
    _lcd.print(" LS2:");
    _lcd.print(ls2 ? "X" : "O");
    _lcd.print(" [");
    _lcd.print(_modusText(modus));
    _lcd.print("]  ");

    // Zeile 2: Geschwindigkeit
    _lcd.setCursor(0, 1);
    _lcd.print("Ist:");
    // Auf eine Nachkommastelle
    char buf[6];
    dtostrf(istGeschw,  4, 1, buf);
    _lcd.print(buf);
    _lcd.print(" Sol:");
    dtostrf(sollGeschw, 3, 1, buf);
    _lcd.print(buf);
}

void Display::zeigeModus(Betriebsmodus modus) {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Modus gewaehlt:");
    _lcd.setCursor(0, 1);
    switch (modus) {
        case MODUS_LINKS:    _lcd.print("  Linkslauf (L) "); break;
        case MODUS_RECHTS:   _lcd.print(" Rechtslauf (R) "); break;
        case MODUS_AUTOMATIK:_lcd.print(" Automatik  (A) "); break;
    }
}

void Display::zeigeWarteObjekt(Betriebsmodus modus) {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Warten auf T-Box");
    _lcd.setCursor(0, 1);
    switch (modus) {
        case MODUS_LINKS:    _lcd.print(" -> in LS2      "); break;
        case MODUS_RECHTS:   _lcd.print(" -> in LS1      "); break;
        case MODUS_AUTOMATIK:_lcd.print(" -> LS1 oder LS2"); break;
    }
}

void Display::zeigeFehler(const char* text) {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print(text);
    _lcd.setCursor(0, 1);
    _lcd.print("Quitt.=Bestaet. ");
}

void Display::zeigeZielErreicht() {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("T-Box entfernen ");
    _lcd.setCursor(0, 1);
    _lcd.print("Reset in 5s...  ");
}

void Display::zeigeModuswechselHinweis() {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Band leeren!    ");
    _lcd.setCursor(0, 1);
    _lcd.print("Quitt.=weiter   ");
}

void Display::zeigeNachricht(const char* zeile1, const char* zeile2) {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print(zeile1);
    _lcd.setCursor(0, 1);
    _lcd.print(zeile2);
}

void Display::loeschen() {
    _lcd.clear();
}

const char* Display::_modusText(Betriebsmodus m) {
    switch (m) {
        case MODUS_LINKS:     return "L";
        case MODUS_RECHTS:    return "R";
        case MODUS_AUTOMATIK: return "A";
        default:              return "?";
    }
}
