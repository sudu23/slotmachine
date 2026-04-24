/*
 * File: slotmachine.c
 * Author: bub
 *
 * Implementierung des Slotmachine-Spiels (einarmiger Bandit)
 * mit Gewinn-Logik und Sound-Support.
 */

#include "mcc_generated_files/system/system.h"
#include "slotmachine.h"
#include "audio.h" // Wichtig für die Melodie-Funktionen

#define OFF 0
#define ON 1

#define OVERALLDELAY 1

// Flanken-Konstanten
#define KEINE_FLANKE 0x00
#define POSITIVE_FLANKE 0x02
#define NEGATIVE_FLANKE 0x01

#define FLANKEN_ERKENNUNG_DELAY 10 // Debounce-Zeit (in ms)

// KONSTANTEN FUER DIE SLOTMACHINE (in ms)
#define SLOT_SPIN_DURATION 1500      
#define SLOT_STOP_DELAY 500          
#define SLOT_RESULT_DURATION 1000    // Etwas länger für den Sieges-Moment
#define SLOT_LOCKOUT_TIME 500        

// GEWINNCHANCE: 1 zu 5 (entspricht 20%)
#define WIN_CHANCE_DIVISOR 5

// Die 4 Symbole
static const uint8_t SYMBOLS[4][3] = {
    {ON, OFF, OFF}, // Symbol 0: Obere LED
    {OFF, ON, OFF}, // Symbol 1: Mittlere LED
    {OFF, OFF, ON}, // Symbol 2: Untere LED
    {OFF, OFF, OFF} // Symbol 3: Blank
};

static uint16_t slotmachine_melody[][2] = {
    {12, 0},      // 12 Töne folgen
    {C4, 80},     // Schneller Aufstieg (C-Dur)
    {E4, 80},   
    {G4, 80},   
    {C5, 80},   
    {G4, 80},   
    {C5, 80},   
    {E5, 150},    // Kurzes Verweilen auf dem hohen E
    {G5, 100},    // Der höchste Punkt
    {E5, 100},
    {C5, 100},
    {G4, 100},
    {C5, 400}     // Krönender Abschluss-Ton
};

enum slot_state {
    SLOT_IDLE,
    SLOT_SPINNING,
    SLOT_STOP_R1,
    SLOT_STOP_R2,
    SLOT_STOP_R3,
    SLOT_RESULT_DISPLAY
};

// Prototypen
unsigned int Tastererkennung_PA0(void);
void Zeige_Slot_Muster(const unsigned int results[3]);

unsigned int Tastererkennung_PA0(void) {
    static bool old_level = true; 
    static unsigned int debounce_zaehler = 0; 
    
    bool new_level = PORTA_get_pin_level(0);
    unsigned int return_value = KEINE_FLANKE;

    if (debounce_zaehler < FLANKEN_ERKENNUNG_DELAY) {
        debounce_zaehler++;
        return KEINE_FLANKE; 
    }
    
    if (new_level != old_level) {
        debounce_zaehler = 0; 
        old_level = new_level;
        if (new_level == false) {
             return_value = NEGATIVE_FLANKE; 
        } else {
             return_value = POSITIVE_FLANKE;
        }
    }
    return return_value;
}

void Zeige_Slot_Muster(const unsigned int results[3]) {
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            unsigned int symbol_id = results[j];
            if (symbol_id > 3) symbol_id = 3; 
            ledmatrix[i][j] = SYMBOLS[symbol_id][i];
        }
    }
}

void slotmachine(void) {
    static enum slot_state slot_machine_state = SLOT_IDLE;
    static unsigned int reel_results[3] = {1, 1, 1}; 
    static unsigned int slot_state_timer = 0;
    static unsigned int trigger_lockout_timer = 0;
    static uint32_t lastruntime = 0;
    static bool win_this_round = false; // Speichert die Entscheidung "Gewinn oder nicht"
    
    unsigned int taster_ereignis;
    
    if((lastruntime + OVERALLDELAY) <= global_1ms_counter) {
        taster_ereignis = Tastererkennung_PA0();
        lastruntime = global_1ms_counter;

        if (trigger_lockout_timer > 0) trigger_lockout_timer--;
        slot_state_timer++; 

        switch (slot_machine_state) {
            case SLOT_IDLE:
                Zeige_Slot_Muster(reel_results);

                if (taster_ereignis == NEGATIVE_FLANKE && trigger_lockout_timer == 0) {
                    slot_state_timer = 0;
                    trigger_lockout_timer = SLOT_LOCKOUT_TIME; 

                    // Würfeln: Gewinnt der Spieler diesmal?
                    // Nutzt den Millisekunden-Counter als einfachen Zufall
                    win_this_round = ((global_1ms_counter % WIN_CHANCE_DIVISOR) == 0);

                    slot_machine_state = SLOT_SPINNING;
                }
                break;

            case SLOT_SPINNING:
                // Alle Walzen drehen wild durcheinander
                reel_results[0] = (global_1ms_counter) % 4;
                reel_results[1] = (global_1ms_counter + 1) % 4;
                reel_results[2] = (global_1ms_counter + 2) % 4;
                Zeige_Slot_Muster(reel_results);

                if (slot_state_timer >= SLOT_SPIN_DURATION) {
                    slot_state_timer = 0;
                    // Walze 1 stoppt immer zufällig auf Symbol 0, 1 oder 2
                    reel_results[0] = (global_1ms_counter % 3); 
                    slot_machine_state = SLOT_STOP_R1;
                }
                break;

            case SLOT_STOP_R1:
                // Walze 2 und 3 drehen weiter
                reel_results[1] = (global_1ms_counter) % 4;     
                reel_results[2] = (global_1ms_counter / 2) % 4; 
                Zeige_Slot_Muster(reel_results);

                if (slot_state_timer >= SLOT_STOP_DELAY) {
                    slot_state_timer = 0;
                    if (win_this_round) {
                        reel_results[1] = reel_results[0]; // Treffer erzwingen
                    } else {
                        reel_results[1] = (global_1ms_counter + 123) % 3;
                        // Falls zufällig derselbe Wert wie R0 kommt, kurz korrigieren für "Knapp vorbei"
                        if (reel_results[1] == reel_results[0]) {
                            reel_results[1] = (reel_results[1] + 1) % 3;
                        }
                    }
                    slot_machine_state = SLOT_STOP_R2;
                }
                break;

            case SLOT_STOP_R2:
                // Nur Walze 3 dreht noch
                reel_results[2] = (global_1ms_counter) % 4; 
                Zeige_Slot_Muster(reel_results);

                if (slot_state_timer >= SLOT_STOP_DELAY) {
                    slot_state_timer = 0;
                    if (win_this_round) {
                        reel_results[2] = reel_results[1]; // Jackpot erzwingen
                        AddMelodyToSoundBuffer(slotmachine_melody); // Sound abspielen
                    } else {
                        reel_results[2] = (global_1ms_counter + 456) % 3;
                        // Sicherstellen, dass bei "kein Gewinn" nicht doch zufällig 3 gleiche da stehen
                        if (reel_results[2] == reel_results[1]) {
                             reel_results[2] = (reel_results[2] + 1) % 3;
                        }
                    }
                    slot_machine_state = SLOT_STOP_R3;
                }
                break;

            case SLOT_STOP_R3:
                Zeige_Slot_Muster(reel_results);
                if (slot_state_timer >= SLOT_STOP_DELAY) {
                    slot_state_timer = 0;
                    slot_machine_state = SLOT_RESULT_DISPLAY;
                }
                break;

            case SLOT_RESULT_DISPLAY:
                // Falls gewonnen: Blinken lassen
                if (win_this_round && (global_1ms_counter % 200 < 100)) {
                    unsigned int blank[3] = {3, 3, 3};
                    Zeige_Slot_Muster(blank);
                } else {
                    Zeige_Slot_Muster(reel_results);
                }

                if (slot_state_timer >= SLOT_RESULT_DURATION) {
                    slot_state_timer = 0;
                    slot_machine_state = SLOT_IDLE;
                }
                break;
        }
    }
}