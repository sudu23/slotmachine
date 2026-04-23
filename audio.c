#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/timer/delay.h"
#include "audio.h"

int8_t soundbuffercnt = 0;
uint16_t soundbuffer[SOUNDBUFFERSIZE][2];

void AddToneToSoundBuffer(uint16_t Frequency, uint16_t Length)
{   //Gibt einen Ton mit einer bestimmten Dauer aus
	if(soundbuffercnt < SOUNDBUFFERSIZE)
	{
        soundbuffer[soundbuffercnt][0] = Frequency;
        soundbuffer[soundbuffercnt][1] = Length;
        soundbuffercnt++;
    }

    //start timer B1 for Sound Server
     TCB1.CTRLA |= 0x01;
}

void AddMelodyToSoundBuffer(uint16_t soundbufferpnt[][2])
{   //Ganze Melodie wird ausgegeben
	uint8_t tempcnt;
	uint8_t soundbuffersize = soundbufferpnt[0][0];
		
	for(tempcnt=1;tempcnt<=soundbuffersize;tempcnt++)
	{
		AddToneToSoundBuffer(soundbufferpnt[tempcnt][0],soundbufferpnt[tempcnt][1]);
	}
}

//Enables/Disables Sound
//True = Enabled
//False = Disabled
void soundEnable(bool Enable)
{
    if (Enable == false)
    {
        TCA0.SINGLE.CTRLA &= ~0x01;     //Disables frequency output
    }
    else
    {
        TCA0.SINGLE.CTRLA |= 0x01;      //Enables frequency output
    }
}

void myISR_TCB1(void)
{
    //10ms-Interruptfunktion mit Timer B1 --> SoundTimer
    uint16_t TempTCA0;
    uint8_t tempcnt;
    uint16_t frequency;
    static uint16_t playtime = 0;
     
    TCB1.INTFLAGS = TCB_CAPT_bm;   //Delete Interrupt Flag
     
    frequency = soundbuffer[0][0];

    if(soundbuffercnt > 0)
    {
        if(playtime == 0)
        {
            if(frequency > 0)                                         //wenn Melodie nicht fertig
            {
                TempTCA0 = (TCA0_CLK/(frequency * (uint16_t)2 * (uint16_t)PRESCALER)) -1; 
                TCA0.SINGLE.CMP0 = (uint16_t)TempTCA0; // Setze die Periode
                             
                soundEnable(true); //Enable Frequency Output
            }
            else
            {
                //wenn "break" im Lied
                soundEnable(false);
                PORTC_set_pin_level(2,false); //set pin low
            }
            soundbuffercnt--;
        }
    }
     
    playtime += 10;
     
    if(playtime >= soundbuffer[0][1])
    {
        playtime = 0;
         
        if(soundbuffercnt <= 0) //stop oder nächster ton?
        {
            soundEnable(false); //disable frequency output
            PORTC_set_pin_level(2,false); //set pin low
            TCB1.CTRLA &= ~0x01; //stop Timer und damit den Sound Timer
        }
        else
        {
            //soundbuffer nach links schieben
            for(tempcnt=0;tempcnt<soundbuffercnt;tempcnt++)
            {
                soundbuffer[tempcnt][0] = soundbuffer[tempcnt+1][0];   //Ton schieben
                soundbuffer[tempcnt][1] = soundbuffer[tempcnt+1][1];   //Dauer schieben
            }
        }
    }         
}