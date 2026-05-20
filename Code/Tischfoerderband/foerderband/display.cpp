
/**
 * @file display.cpp
 * @brief Implementierung der Display-Klasse.
 */

#include "display.h"

/**
 * @brief Konstruktor der Display-Klasse.
 *
 * Initialisiert das LCD mit Adresse und Abmessungen.
 */
Display::Display()
    : _lcd(LCD_ADRESSE, LCD_SPALTEN, LCD_ZEILEN)
{}

/**
 * @brief Initialisiert das LCD-Display.
 */
void Display::begin() {
    _lcd.init();
    _lcd.backlight();
    _lcd.clear();
}

/**
 * @brief Zeigt den Startbildschirm an.
 */
void Display::zeigeStartbildschirm() {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("  Tischfoerder  ");
    _lcd.setCursor(0, 1);
    _lcd.print("  band  v1.0    ");
}

/**
 * @brief Zeigt die Freigabeabfrage an.
 */
void Display::zeigeFreigabeAbfrage() {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("LS frei?        ");
    _lcd.setCursor(0, 1);
    _lcd.print("Quitt.=Bestaet. ");
}

/**
 * @brief Zeigt das Hauptmenü mit Statusinformationen.
 *
 * @param ls1 Status von Lichtschranke 1.
 * @param ls2 Status von Lichtschranke 2.
 * @param modus Aktiver Betriebsmodus.
 * @param istGeschw Aktuelle Geschwindigkeit.
 * @param sollGeschw Sollgeschwindigkeit.
 */
void Display::zeigeHauptmenue(bool ls1, bool ls2,
                               Betriebsmodus modus,
                               float istGeschw,
                               float sollGeschw) {

    // Zeile 1: Statusinformationen der Lichtschranken
    _lcd.setCursor(0, 0);
    _lcd.print("LS1:");
    _lcd.print(ls1 ? "X" : "O");
    _lcd.print(" LS2:");
    _lcd.print(ls2 ? "X" : "O");
    _lcd.print(" [");
    _lcd.print(_modusText(modus));
    _lcd.print("]  ");

    // Zeile 2: Anzeige der Geschwindigkeiten
    _lcd.setCursor(0, 1);
    _lcd.print("Ist:");

    char buf[6];

    // Istgeschwindigkeit formatieren
    dtostrf(istGeschw, 4, 1, buf);
    _lcd.print(buf);

    _lcd.print(" Sol:");

    // Sollgeschwindigkeit formatieren
    dtostrf(sollGeschw, 3, 1, buf);
    _lcd.print(buf);
}

/**
 * @brief Zeigt den gewählten Betriebsmodus an.
 *
 * @param modus Aktueller Betriebsmodus.
 */
void Display::zeigeModus(Betriebsmodus modus) {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Modus gewaehlt:");
    _lcd.setCursor(0, 1);

    switch (modus) {
        case MODUS_LINKS:
           








