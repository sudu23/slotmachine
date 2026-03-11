/*
 * File: wuerfel.c
 * Author: bub
 *
 * Implementierung des Wuerfel-Spiels (Dice)
 */

#include "mcc_generated_files/system/system.h"
#include "wuerfel.h" // Muss die extern-Deklarationen enthalten

#define OFF 0
#define ON 1

#define OVERALLDELAY 1

// Flanken-Konstanten
#define KEINE_FLANKE 0x00
#define POSITIVE_FLANKE 0x02
#define NEGATIVE_FLANKE 0x01

// Timing-Konstanten (in ms)
#define WUERFEL_ZAHL_BLINK_PERIODE 500
#define WUERFEL_ZAHL_BLINKZEIT_AN 333
#define WUERFEL_ZAHL_ANZEIGEN 2000
#define ROLL_COOLDOWN_TIME 1000

// Debounce-Zeit (in ms)
#define FLANKEN_ERKENNUNG_DELAY 10

// ledmatrix und global_1ms_counter sind jetzt ueber wuerfel.h als extern deklariert.
// Ihre Definition befindet sich in main.c

// Globale Zustands- und Zaehlvariablen
enum wuerfel_state
{
    idle,           // Wartet auf Tilt-Sensor-Ereignis (Nur am Programmstart)
    durchwuerfeln,  // Zeigt die schnelle Blink-Animation
    zahl_anzeigen,  // Zeigt das Ergebnis (blinkend)
    result_stay_on  // NEU: Zeigt das Ergebnis statisch an, bis zum naechsten Wurf
};

// Prototypen (interne Funktionen)
void SetMatrix(const uint8_t neue_matrix[3][3]);
void Zeige_Zahl(unsigned int Number);
void Wuerfel_Durchwuerfeln_Animation(unsigned int* zaehler, unsigned int* zufallszahl_seed, enum wuerfel_state* naechster_state);
void Zeige_Zufalls_Zahl(unsigned int* zaehler, unsigned int zufallszahl_seed, enum wuerfel_state* naechster_state);
unsigned int Sensorerkennung(void);
void LoescheLEDs(void);
void Wuerfel_Idle_Animation(void);

/**
 * @brief Hauptlogik fuer den Wuerfel-Modus.
 * * Wird in der main-loop aufgerufen. Implementiert einen Zustandsautomaten
 * fuer das Wuerfel-Spiel.
 */
void wuerfel(void)
{
    static enum wuerfel_state wuerfelstate = idle;
    static unsigned int wuerfel_durchwuerfel_zaehler = 0;
    static unsigned int zahl_anzeigen_zaehler = 0;
    static unsigned int random_wuerfel_zahl = 0; // Seed fuer die Zufallszahl
    static unsigned int cooldown_timer = 0; // Zaehler fuer die Sperrzeit
    unsigned int augenzahl;
    static uint32_t lastruntime = 0;
    unsigned int sensor_ereignis;
    
    if((lastruntime + OVERALLDELAY) <= global_1ms_counter)
    {
        lastruntime = global_1ms_counter;
        
        // Tastsensor (z.B. Tilt-Sensor an PA1) abfragen
        sensor_ereignis = Sensorerkennung();

        // Logik basierend auf dem aktuellen Zustand
        switch (wuerfelstate) 
        {
            case idle:
                // Nur am Start wird die Idle-Animation gezeigt
                Wuerfel_Idle_Animation(); 

                // 1. Cooldown-Timer verwalten
                if (cooldown_timer > 0) {
                    cooldown_timer--; // Zaehlt jede 1ms um 1 herunter
                }

                // 2. UEBERGANG: Nur starten, wenn der Timer abgelaufen ist UND eine Flanke da ist.
                if ((cooldown_timer == 0) && (sensor_ereignis != KEINE_FLANKE))
                {
                    // Initialisiere die Zaehler und generiere den "Seed" fuer die Zahl
                    wuerfel_durchwuerfel_zaehler = 0;
                    // global_1ms_counter dient als gute Quelle fuer einen Zufalls-Seed
                    random_wuerfel_zahl = global_1ms_counter; 

                    cooldown_timer = ROLL_COOLDOWN_TIME;
                    wuerfelstate = durchwuerfeln;
                }
                break;

            case durchwuerfeln:
                // Wuerfel-Animation
                Wuerfel_Durchwuerfeln_Animation(&wuerfel_durchwuerfel_zaehler, &random_wuerfel_zahl, &wuerfelstate);
                break;

            case zahl_anzeigen:
                // Ergebnis blinkt
                Zeige_Zufalls_Zahl(&zahl_anzeigen_zaehler, random_wuerfel_zahl, &wuerfelstate);
                break;

            case result_stay_on: 
                // NEUER ZUSTAND: Ergebnis bleibt statisch stehen

                // Berechne die Augenzahl (1 bis 6) einmalig
                augenzahl = (random_wuerfel_zahl % 6) + 1;
                Zeige_Zahl(augenzahl); // Ergebnis statisch anzeigen

                // 1. Cooldown-Timer verwalten
                if (cooldown_timer > 0) {
                    cooldown_timer--; // Zaehlt jede 1ms um 1 herunter
                }

                // 2. UEBERGANG: Wenn Sensor gedrueckt wird, zurueck zum Wuerfeln
                if ((cooldown_timer == 0) && (sensor_ereignis != KEINE_FLANKE))
                {
                    // Initialisiere die Zaehler und generiere den "Seed" fuer die Zahl
                    wuerfel_durchwuerfel_zaehler = 0;
                    random_wuerfel_zahl = global_1ms_counter; 

                    cooldown_timer = ROLL_COOLDOWN_TIME; // Setze die Sperrzeit
                    wuerfelstate = durchwuerfeln;
                }
                break;
        }
    }
}

/**
 * @brief Schreibt ein 3x3 Muster in die globale ledmatrix.
 * * @param neue_matrix Das zu setzende 3x3 Muster.
 */
void SetMatrix(const uint8_t neue_matrix[3][3])
{
    // Die Indices 0, 1 und 2 sind bekannt (Groesse 3x3)
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            // Elementweise Zuweisung
            ledmatrix[i][j] = neue_matrix[i][j];
        }
    }
}

/**
 * @brief Zeigt die Augenzahl (1 bis 6) auf der LED-Matrix an.
 * * @param Number Die anzuzeigende Augenzahl (1 bis 6).
 */
void Zeige_Zahl(unsigned int Number)
{
    // Muster fuer die Wuerfel-Augenzahlen
    // Index-Muster: [Reihe][Spalte]
    static const uint8_t Leer[3][3] = {{OFF, OFF, OFF}, {OFF, OFF, OFF}, {OFF, OFF, OFF}};
    
    // ** Zahl 1: Nur Mitte [1][1] **
    static const uint8_t Zahl_1[3][3] = {
        {OFF, OFF, OFF},
        {OFF, ON, OFF},
        {OFF, OFF, OFF}
    };
    
    // ** Zahl 2: Oben rechts [0][2] und unten links [2][0] **
    static const uint8_t Zahl_2[3][3] = {
        {OFF, OFF, ON},
        {OFF, OFF, OFF},
        {ON, OFF, OFF}
    };
    
    // ** Zahl 3: Muster von 2 + Mitte [1][1] **
    static const uint8_t Zahl_3[3][3] = {
        {OFF, OFF, ON},
        {OFF, ON, OFF},
        {ON, OFF, OFF}
    };
    
    // ** Zahl 4: Alle Ecken **
    static const uint8_t Zahl_4[3][3] = {
        {ON, OFF, ON},
        {OFF, OFF, OFF},
        {ON, OFF, ON}
    };
    
    // ** Zahl 5: Muster von 4 + Mitte [1][1] **
    static const uint8_t Zahl_5[3][3] = {
        {ON, OFF, ON},
        {OFF, ON, OFF},
        {ON, OFF, ON}
    };
    
    // ** Zahl 6: Alle Ecken + Mittig links/rechts **
    static const uint8_t Zahl_6[3][3] = {
        {ON, OFF, ON},
        {ON, OFF, ON},
        {ON, OFF, ON}
    };
    
    switch (Number)
    {
        case 1: SetMatrix(Zahl_1); break;
        case 2: SetMatrix(Zahl_2); break;
        case 3: SetMatrix(Zahl_3); break;
        case 4: SetMatrix(Zahl_4); break;
        case 5: SetMatrix(Zahl_5); break;
        case 6: SetMatrix(Zahl_6); break;
        default: SetMatrix(Leer); break;
    }
}

/**
 * @brief Simuliert die schnelle Animation waehrend des Wuerfelns.
 * * @param zaehler Zaehler, der in jedem Aufruf inkrementiert wird.
 * @param zufallszahl_seed Der Zufalls-Seed.
 * @param naechster_state Zeiger auf den Zustand, der nach Abschluss gesetzt wird.
 */
void Wuerfel_Durchwuerfeln_Animation(unsigned int* zaehler, unsigned int* zufallszahl_seed, enum wuerfel_state* naechster_state)
{
    // Simuliert eine schnelle Abfolge der Augenzahlen (wie in Ihrem Originalcode)
    unsigned int z = *zaehler;
    
    if (z <= 166) { Zeige_Zahl(5); }
    else if (z <= 333) { Zeige_Zahl(1); }
    else if (z <= 500) { Zeige_Zahl(2); }
    else if (z <= 666) { Zeige_Zahl(6); }
    else if (z <= 833) { Zeige_Zahl(3); }
    else if (z <= 999) { Zeige_Zahl(4); }
    
    (*zaehler)++;
    
    if (*zaehler >= 1000) 
    {
        *zaehler = 0;
        // Wechsel zum naechsten Zustand
        *naechster_state = zahl_anzeigen; 
    }
}

/**
 * @brief Liest den Tilt-Sensor (PA1) und erkennt Flanken mit Entprellung.
 * * @return unsigned int KEINE_FLANKE, POSITIVE_FLANKE oder NEGATIVE_FLANKE.
 */
unsigned int Sensorerkennung(void)
{
    static bool old_level = false;
    // NEU: Zaehler fuer die Entprellzeit (zaehlt in ms-Schritten)
    static unsigned int debounce_zaehler = 0; 
    
    bool new_level = PORTA_get_pin_level(1); // Sensor an PA1
    unsigned int return_value = KEINE_FLANKE;
    
    // Zaehler hochzaehlen, wenn wir uns im Entprell-Modus befinden
    if (debounce_zaehler < FLANKEN_ERKENNUNG_DELAY) {
        debounce_zaehler++;
        // Wir ignorieren Flanken, solange die Entprellzeit laeuft
        return KEINE_FLANKE; 
    }
    
    // Entprellzeit ist abgelaufen, jetzt auf Flanke pruefen
    if (new_level != old_level)
    {
        // Flanke erkannt: Starte die Entprellzeit neu
        debounce_zaehler = 0; 
        old_level = new_level;
        
        if (new_level == true) {
            return_value = POSITIVE_FLANKE;
        } else {
            return_value = NEGATIVE_FLANKE;
        }
    }
    
    return return_value;
}

/**
 * @brief Zeigt ein einfaches Idle-Muster an.
 */
void Wuerfel_Idle_Animation(void)
{
    // Die neue Matrix-Konfiguration definieren (z.B. mittlere Reihe EIN)
    const uint8_t idle_pattern[3][3] = {
        {OFF, OFF, OFF},
        {ON, ON, ON},
        {OFF, OFF, OFF}
    };

    // Die gesamte Matrix mit einem Aufruf setzen
    SetMatrix(idle_pattern);
}

/**
 * @brief Loescht alle LEDs in der globalen Matrix.
 */
void LoescheLEDs(void)
{
    // Setzt jedes Element der globalen Matrix auf OFF (0)
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            ledmatrix[i][j] = OFF;
        }
    }
}

/**
 * @brief Zeigt die zufaellige Augenzahl blinkend an.
 * * @param zaehler Zaehler fuer die Blink-Logik.
 * @param zufallszahl_seed Der Seed, um die Augenzahl zu bestimmen.
 * @param naechster_state Zeiger auf den Zustand, der nach Abschluss gesetzt wird.
 */
void Zeige_Zufalls_Zahl(unsigned int* zaehler, unsigned int zufallszahl_seed, enum wuerfel_state* naechster_state)
{
    // Berechne die Augenzahl (1 bis 6) einmalig
    unsigned int augenzahl = (zufallszahl_seed % 6) + 1;
    
    // Blinken der Augenzahl
    if (*zaehler % WUERFEL_ZAHL_BLINK_PERIODE < WUERFEL_ZAHL_BLINKZEIT_AN) 
    {
        Zeige_Zahl(augenzahl);
    }
    else
    {
        LoescheLEDs();
    }

    (*zaehler)++;
    
    // Nach Ablauf der Anzeigezeit Uebergang zu NEU: statisch anzeigen
    if (*zaehler >= WUERFEL_ZAHL_ANZEIGEN) 
    {
        *zaehler = 0;
        // GEAENDERT: Wechsel zum neuen Zustand result_stay_on
        *naechster_state = result_stay_on; 
    }
}