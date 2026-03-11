/* 
 * File:   slotmachine.h
 * Author: bub
 *
 * Created on 3. Dezember 2025, 13:22
 */

#ifndef SLOTMACHINE_H
#define	SLOTMACHINE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
extern uint8_t ledmatrix[3][3];
extern uint32_t global_1ms_counter;

void slotmachine(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SLOTMACHINE_H */

