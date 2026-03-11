/* 
 * File:   wuerfel.h
 * Author: bub
 *
 * Created on 3. Dezember 2025, 13:22
 */

#ifndef WUERFEL_H
#define	WUERFEL_H

#ifdef	__cplusplus
extern "C" {
#endif
    
extern uint8_t ledmatrix[3][3];
extern uint32_t global_1ms_counter;

void wuerfel(void);

#ifdef	__cplusplus
}
#endif

#endif	/* WUERFEL_H */

