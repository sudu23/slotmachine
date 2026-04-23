/* 
 * File:   audio.h
 * Author: bub
 *
 * Created on 23. April 2026, 10:01
 */

#ifndef AUDIO_H
#define	AUDIO_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//Audio
#define SOUNDBUFFERSIZE 84
//Frequenzen auf 1Hz gerundet
#define B5      988
#define Ab5     932
#define A5      880
#define Gb5     831
#define G5      784
#define Fb5     740
#define F5      698
#define E5      659
#define Db5     622
#define D5      587
#define Cb5     554
#define C5      523
#define B4      494
#define Ab4     466
#define A4      440
#define Gb4     415
#define G4      392
#define Fb4     370
#define F4      349
#define E4      330
#define Db4     311
#define D4      294
#define Cb4     277
#define C4      262
#define B3      247
#define Bb3     233
#define A3      220
#define Ab3     208
#define G3      196
#define Gb3     185
#define F3      175
#define E3      165
#define Eb3     156
#define D3      147
#define Db3     139
#define C3      131
#define B2      123
#define Bb2     117
#define A2      110
#define Ab2     104
#define G2      98
#define Gb2     92
#define F2      87
#define E2      82
#define Eb2     78
#define D2      73
#define Db2     69
#define C2      65
    
#define BREAK   0
    
#define TCA0_CLK    4000000        //4Mhz: Peripheral/Main Clock frequency
#define PRESCALER   1              //Clock prescaler
    
extern uint8_t ledmatrix[3][3];

void AddToneToSoundBuffer(unsigned int,unsigned int);
void AddMelodyToSoundBuffer(unsigned int soundbufferpnt[][2]);
			
void soundEnable(bool Enable);

void myISR_TCB1(void);

#ifdef	__cplusplus
}
#endif

#endif	/* AUDIO_H */

