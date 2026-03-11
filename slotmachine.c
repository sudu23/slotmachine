/*
 * File: slotmachine.c
 * Author: bub
 *
 * Implementierung des Slotmachine-Spiels (einarmiger Bandit)
 */

#include "mcc_generated_files/system/system.h"
#include "slotmachine.h" // Muss die extern-Deklarationen enthalten

#define OFF 0
#define ON 1

#define OVERALLDELAY 1

// Flanken-Konstanten
#define KEINE_FLANKE 0x00
#define POSITIVE_FLANKE 0x02
#define NEGATIVE_FLANKE 0x01

#define FLANKEN_ERKENNUNG_DELAY 10 // Debounce-Zeit (in ms)

// NEUE KONSTANTEN FUER DIE SLOTMACHINE (in ms)
#define SLOT_SPIN_DURATION 1500      // 1.5 Sekunden schnelles Drehen
#define SLOT_STOP_DELAY 500          // 0.5 Sekunden Verzoegerung zwischen dem Stoppen der Walzen
#define SLOT_RESULT_DURATION 1000    // 1 Sekunde Anzeige des Ergebnisses
#define SLOT_LOCKOUT_TIME 500        // 0.5 Sekunden Taster-Sperre nach dem Druecken

// Die 4 Symbole, die eine einzelne Walze (Spalte) darstellen
// Index 0=Oben (1 LED), 1=Mitte (1 LED), 2=Unten (1 LED), 3=Blank (0 LEDs)
static const uint8_t SYMBOLS[4][3] = {
    {ON, OFF, OFF}, // Symbol 0: Obere LED (Reihe 0)
    {OFF, ON, OFF}, // Symbol 1: Mittlere LED (Reihe 1)
    {OFF, OFF, ON}, // Symbol 2: Untere LED (Reihe 2)
    {OFF, OFF, OFF} // Symbol 3: Blank (Alle AUS)
};

// NEUE ZUSTAENDE FUER DIE SLOTMACHINE
enum slot_state
{
    SLOT_IDLE,
    SLOT_SPINNING,
    SLOT_STOP_R1, // Stoppt Walze 1 (Spalte 0)
    SLOT_STOP_R2, // Stoppt Walze 2 (Spalte 1)
    SLOT_STOP_R3, // Stoppt Walze 3 (Spalte 2)
    SLOT_RESULT_DISPLAY
};

// ledmatrix und global_1ms_counter sind jetzt ueber slotmachine.h als extern deklariert.

// Prototypen (interne Funktionen)
unsigned int Tastererkennung_PA0(void);
void Zeige_Slot_Muster(const unsigned int results[3]);

/**
 * @brief Liest den Modus-Taster (PA0) und erkennt Flanken mit Entprellung.
 * * @return unsigned int KEINE_FLANKE, POSITIVE_FLANKE oder NEGATIVE_FLANKE.
 */
unsigned int Tastererkennung_PA0(void)
{
    // PA0 ist typischerweise mit Pull-Up auf HIGH und wird bei Druck auf LOW gezogen.
    static bool old_level = true; 
    static unsigned int debounce_zaehler = 0; 
    
    // Die Tastererkennung laeuft auf Pin PA0
    bool new_level = PORTA_get_pin_level(0);
    unsigned int return_value = KEINE_FLANKE;

    // Entprellung: Pruefen, ob die Entprellzeit abgelaufen ist
    if (debounce_zaehler < FLANKEN_ERKENNUNG_DELAY) {
        debounce_zaehler++;
        return KEINE_FLANKE; 
    }
    
    // Flanke erkennen
    if (new_level != old_level)
    {
        // Flanke erkannt: Starte die Entprellzeit neu
        debounce_zaehler = 0; 
        old_level = new_level;
        
        // Wir loesen nur auf die NEGATIVE Flanke (Taste gedrueckt) aus, da PA0 HIGH-aktiv ist
        if (new_level == false) {
             return_value = NEGATIVE_FLANKE; 
        } else {
             return_value = POSITIVE_FLANKE; // Taste losgelassen
        }
    }
    
    return return_value;
}

/**
 * @brief Setzt das Muster der drei Walzen (Spalten) in die LED-Matrix.
 * * @param results Ein Array von 3 Symbol-Indizes (0-3) fuer jede Walze.
 */
void Zeige_Slot_Muster(const unsigned int results[3])
{
    // Die Indices 0, 1 und 2 fuer Reihen (i) und Spalten (j) sind bekannt (3x3)
    for (uint8_t i = 0; i < 3; i++) { // Reihen (0, 1, 2)
        for (uint8_t j = 0; j < 3; j++) { // Spalten/Walzen (0, 1, 2)
            
            unsigned int symbol_id = results[j]; // Hole die Symbol-ID fuer die aktuelle Spalte/Walze
            
            // Begrenze die Symbol-ID, um Array-Ueberlauf zu verhindern (max 3, da 4 Symbole 0-3)
            if (symbol_id > 3) symbol_id = 3; 
            
            // Setze das LED-Element in der globalen Matrix
            // ledmatrix[Reihe][Spalte] = SYMBOLS[Symbol][Reihe]
            ledmatrix[i][j] = SYMBOLS[symbol_id][i];
        }
    }
}

/**
 * @brief Hauptlogik fuer den Slotmachine-Modus.
 * * Wird in der main-loop aufgerufen. Implementiert einen Zustandsautomaten
 * fuer das Slotmachine-Spiel.
 */
void slotmachine(void)
{
    static enum slot_state slot_machine_state = SLOT_IDLE;
    // Ergebnisse der 3 Walzen (0=Spalte 0, 1=Spalte 1, 2=Spalte 2). Starte mit Mitte-LED.
    static unsigned int reel_results[3] = {1, 1, 1}; 
    static unsigned int slot_state_timer = 0;
    static unsigned int trigger_lockout_timer = 0; // Sperre nach dem Start des Spins
    static uint32_t lastruntime = 0;
    unsigned int taster_ereignis;
    
    if((lastruntime + OVERALLDELAY) <= global_1ms_counter)
    {
        taster_ereignis = Tastererkennung_PA0();
        
        lastruntime = global_1ms_counter;

        // Zustandstimer in jedem Durchlauf inkrementieren
        slot_state_timer++; 

        // Sperr-Timer verwalten
        if (trigger_lockout_timer > 0) {
            trigger_lockout_timer--;
        }

        switch (slot_machine_state) 
        {
            case SLOT_IDLE:
                // Anzeige eines Warte-Musters (Mitte-LEDs)
                Zeige_Slot_Muster(reel_results);

                // UEBERGANG: Starte das Spiel bei gedruecktem Taster und abgelaufener Sperrzeit
                if (taster_ereignis == NEGATIVE_FLANKE && trigger_lockout_timer == 0) 
                {
                    slot_state_timer = 0;
                    trigger_lockout_timer = SLOT_LOCKOUT_TIME; 

                    // Walzen mit Start-Symbolen initialisieren
                    reel_results[0] = 1;
                    reel_results[1] = 1;
                    reel_results[2] = 1;

                    slot_machine_state = SLOT_SPINNING;
                }
                break;

            case SLOT_SPINNING:
                // Walzen 1, 2 und 3 drehen sich schnell (Index 0-3)
                reel_results[0] = (global_1ms_counter) % 4;
                reel_results[1] = (global_1ms_counter + 1) % 4; // 1ms Versatz
                reel_results[2] = (global_1ms_counter + 2) % 4; // 2ms Versatz

                Zeige_Slot_Muster(reel_results);

                // UEBERGANG zu: Stoppe Walze 1
                if (slot_state_timer >= SLOT_SPIN_DURATION) 
                {
                    slot_state_timer = 0;
                    // Walze 1 stoppt auf einem zufaelligen EINZEL-LED-Ergebnis (Index 0, 1 oder 2)
                    reel_results[0] = (global_1ms_counter % 3); 
                    slot_machine_state = SLOT_STOP_R1;
                }
                break;

            case SLOT_STOP_R1:
                // Walze 1 fixiert, Walzen 2 und 3 drehen weiter (Index 0-3)
                reel_results[1] = (global_1ms_counter) % 4;     
                reel_results[2] = (global_1ms_counter / 2) % 4; // Langsamerer Spin fuer Walze 3

                Zeige_Slot_Muster(reel_results);

                // UEBERGANG zu: Stoppe Walze 2
                if (slot_state_timer >= SLOT_STOP_DELAY) 
                {
                    slot_state_timer = 0;
                    // Walze 2 stoppt auf einem zufaellligen EINZEL-LED-Ergebnis (Index 0, 1 oder 2)
                    reel_results[1] = (global_1ms_counter + 123) % 3; 
                    slot_machine_state = SLOT_STOP_R2;
                }
                break;

            case SLOT_STOP_R2:
                // Walzen 1 und 2 fixiert, Walze 3 dreht weiter (Index 0-3)
                reel_results[2] = (global_1ms_counter) % 4; 

                Zeige_Slot_Muster(reel_results);

                // UEBERGANG zu: Stoppe Walze 3
                if (slot_state_timer >= SLOT_STOP_DELAY) 
                {
                    slot_state_timer = 0;
                    // Walze 3 stoppt auf einem zufaellligen EINZEL-LED-Ergebnis (Index 0, 1 oder 2)
                    reel_results[2] = (global_1ms_counter + 456) % 3;
                    
                    slot_machine_state = SLOT_STOP_R3;
                }
                break;

            case SLOT_STOP_R3:
                // Alle Walzen sind fixiert. Kurze Pause vor der Ergebnisanzeige.
                Zeige_Slot_Muster(reel_results);

                // UEBERGANG zu: Ergebnis anzeigen
                if (slot_state_timer >= SLOT_STOP_DELAY) 
                {
                    slot_state_timer = 0;
                    slot_machine_state = SLOT_RESULT_DISPLAY;
                }
                break;

            case SLOT_RESULT_DISPLAY:
                // Zeigt das Endergebnis fuer eine laengere Zeit an
                Zeige_Slot_Muster(reel_results);

                // UEBERGANG zurueck zu Idle
                if (slot_state_timer >= SLOT_RESULT_DURATION) 
                {
                    slot_state_timer = 0;
                    slot_machine_state = SLOT_IDLE;
                }
                break;
        }
    }
}