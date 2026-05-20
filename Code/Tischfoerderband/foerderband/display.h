/**
 * @file display.h
 * @brief Deklaration der LCD-Anzeige-Klasse.
 *
 * Die Klasse kapselt die Steuerung eines 16x2 LCD-Displays
 * mit I2C-Schnittstelle.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include "config.h"

/**
 * @class Display
 * @brief Verwaltet die Ausgabe auf dem LCD-Display.
 */
class Display {
public:

    /**
     * @brief Konstruktor der Display-Klasse.
     */
    Display();

    /**
     * @brief Initialisiert das LCD-Display.
     */
    void begin();

    /**
     * @brief Zeigt den Startbildschirm an.
     */
    void zeigeStartbildschirm();

    /**
     * @brief Zeigt die Freigabeabfrage an.
     */
    void zeigeFreigabeAbfrage();

    /**
     * @brief Zeigt das Hauptmenue mit Sensor- und Geschwindigkeitsdaten.
     *
     * @param ls1 Status der Lichtschranke 1.
     * @param ls2 Status der Lichtschranke 2.
     * @param modus Aktueller Betriebsmodus.
     * @param istGeschw Aktuelle Geschwindigkeit.
     * @param sollGeschw Sollgeschwindigkeit.
     */
    void zeigeHauptmenue(bool ls1, bool ls2,
                         Betriebsmodus modus,
                         float istGeschw,
                         float sollGeschw);

    /**
     * @brief Zeigt den aktuell gewaehlten Betriebsmodus.
     *
     * @param modus Gewaehlter Betriebsmodus.
     */
    void zeigeModus(Betriebsmodus modus);

    /**
     * @brief Zeigt den Wartezustand auf ein Objekt.
     *
     * @param modus Aktueller Betriebsmodus.
     */
    void zeigeWarteObjekt(Betriebsmodus modus);

    /**
     * @brief Zeigt eine Fehlermeldung an.
     *
     * @param text Fehlertext.
     */
    void zeigeFehler(const char* text);

    /**
     * @brief Zeigt die Meldung „Ziel erreicht“ an.
     */
    void zeigeZielErreicht();

    /**
     * @brief Zeigt einen Hinweis zum Moduswechsel.
     */
    void zeigeModuswechselHinweis();

    /**
     * @brief Zeigt eine beliebige zweizeilige Nachricht an.
     *
     * @param zeile1 Inhalt der ersten Zeile.
     * @param zeile2 Inhalt der zweiten Zeile.
     */
    void zeigeNachricht(const char* zeile1, const char* zeile2);

    /**
     * @brief Loescht den aktuellen Displayinhalt.
     */
    void loeschen();

private:

    /** @brief LCD-Objekt für die I2C-Ansteuerung. */
    LiquidCrystal_I2C _lcd;

    /**
     * @brief Liefert den Kurztext eines Betriebsmodus.
     *
     * @param m Betriebsmodus.
     * @return Zeichenkette des Modus.
     */
    const char* _modusText(Betriebsmodus m);
};

#endif // DISPLAY_H