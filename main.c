/*
 * MAIN Generated Driver File
 * * @file main.c
 * * @defgroup main MAIN
 * * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS "AS IS." 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/timer/delay.h"

#include "main.h"
#include "wuerfel.h"
#include "slotmachine.h"

uint8_t Mode = 0;

/********************************************/
void setup(void)
{
    if(PORTA_get_pin_level(0) == true){
        Mode = 0; // Taster nicht gedrueckt: Modus 0 (Würfel)
    } else {
        Mode = 1; // Taster gedrueckt beim Start: Modus 1 (Slotmachine)
    }
}

void loop(void)
{
    if(Mode == 0){
        wuerfel();
    }
    else{
        slotmachine();
    }
}

/****************************************/
/*
    Main application
*/
int main(void)
{
    SYSTEM_Initialize();
    
    TCB0_CaptureCallbackRegister(myISR_TCB0);
    
    setup();

    while(1)
    {
        loop();
    }   
}

void myISR_TCB0(void)
{
    static uint8_t ActualRaw = 0; // Aktuelle Zeile (0, 1, 2)
    
    //clear flag
    TCB0.INTFLAGS = TCB_CAPT_bm;
    
    //test
    //PORTD_toggle_pin_level(5);
    
    global_1ms_counter++;
    
    // --- 1. Deaktiviere die ZUVOR aktive Reihe (Ghosting-Schutz) ---
    // Setze alle Zeilen-Pins auf den inaktiven Zustand (HIGH)
    VPORTD.OUT |= (REIHE_1_PIN_MASK | REIHE_2_PIN_MASK);
    VPORTC.OUT |= REIHE_3_PIN_MASK;

    // --- 2. Spalten-Steuerung (Daten-Ausgabe) ---
    // Setze alle Spalten auf den inaktiven Zustand (LOW)
    VPORTD.OUT &= ~(SPALTE_1_PIN_MASK | SPALTE_2_PIN_MASK); 
    VPORTC.OUT &= ~SPALTE_3_PIN_MASK;

    // Lese die Daten der aktuellen Reihe und schalte die entsprechenden Spalten ein
    for (uint8_t Spalte = 0; Spalte < 3; Spalte++) {
        
        if (ledmatrix[ActualRaw][Spalte] == ON) {
            
            // Schalte die Spalte EIN (HIGH-aktiv)
            if (Spalte == 0) {
                VPORTD.OUT |= SPALTE_1_PIN_MASK; // Port D Pin 6
            } else if (Spalte == 1) {
                VPORTD.OUT |= SPALTE_2_PIN_MASK; // Port D Pin 7
            } else { // Spalte == 2
                VPORTC.OUT |= SPALTE_3_PIN_MASK; // Port C Pin 1
            }
        }
    }

    // --- 3. Aktiviere die NEUE Reihe (Einschalten der aktuellen Zeile) ---
    // Schalte nur die aktuelle Reihe auf den aktiven Zustand (LOW)
    if (ActualRaw == 0) {
        VPORTD.OUT &= ~REIHE_1_PIN_MASK;
    } else if (ActualRaw == 1) {
        VPORTD.OUT &= ~REIHE_2_PIN_MASK;
    } else { // ActualRaw == 2
        VPORTC.OUT &= ~REIHE_3_PIN_MASK;
    }
    
    // --- 4. Aktualisierung des Reihen-Zaehlers ---
    ActualRaw++;
    if(ActualRaw > 2){
        ActualRaw = 0;
    }
}
