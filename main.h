/* 
 * File:   main.h
 * Author: bub
 *
 * Created on 4. Dezember 2025, 14:30
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#define OFF 0
#define ON 1

// --- PIN-MASKEN ---
// Reihen (Rows) sind LOW-aktiv (Setzen auf 0 schaltet die Zeile ein)
#define REIHE_1_PIN_MASK (1U << 5) // Port D Pin 5
#define REIHE_2_PIN_MASK (1U << 4) // Port D Pin 4
#define REIHE_3_PIN_MASK (1U << 3) // Port C Pin 3

// Spalten (Columns) sind HIGH-aktiv (Setzen auf 1 schaltet die Spalte ein)
#define SPALTE_1_PIN_MASK (1U << 6) // Port D Pin 6
#define SPALTE_2_PIN_MASK (1U << 7) // Port D Pin 7
#define SPALTE_3_PIN_MASK (1U << 1) // Port C Pin 1

uint8_t ledmatrix[3][3] = {
    {OFF, OFF, OFF},
    {ON, OFF, ON},
    {OFF, OFF, OFF}
};

uint32_t global_1ms_counter = 0;

// Function Prototypes (Erweiterung)
void myISR_TCB0(void);
void setup(void);
void loop(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

