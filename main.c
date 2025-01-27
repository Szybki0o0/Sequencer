#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "leds.h"
#include "klaw.h"
#include "DAC.h"
#include "Sequencer.h"
#include "Display.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

volatile uint8_t n, trig=0;
volatile uint16_t dac;	
volatile uint16_t play_duration = 0;
uint16_t DIV_CORE=10000;

void PIT_IRQHandler(void) {
    if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
        PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK; // Wyczyœæ flagê przerwania
				if(play) {
					if (sequencer[SIN][current_step] == 1) {
							play_sine = 1;
							play_duration = PLAY_TIME;
					} else {
							play_sine = 0;
					}
					if (sequencer[TRI][current_step] == 1) {
							play_tri = 1; 
							play_duration = PLAY_TIME; 
					} else {
							play_tri = 0;
					}
					if (sequencer[SAW][current_step] == 1) {
							play_saw = 1;
							play_duration = PLAY_TIME;
					} else {
							play_saw = 0;
					} 
				}
    }

        current_step = (current_step + 1) % STEPS;
}

void SysTick_Handler(void) {
    trig ^= 0x1;

    if (trig) { // Odtwarzanie tylko, gdy flaga jest ustawiona
        int32_t output = 0;

        if (play_sine)
            output += Sinus[n];
        if (play_tri)
            output += Triangle[n];
        if (play_saw)
            output += Sawtooth[n];
				
        if (output > 2047) output = 2047;
        if (output < -2048) output = -2048;

        dac = output + 2048;
        DAC_Load_Trig(dac);
        n += 1;
        if (n > 31) {
            n = 0;
        }

        // Zmniejsz licznik czasu odtwarzania
        if (play_duration > 0) {
            play_duration--;
        } else {
            play_sine = 0;
						play_tri = 0;
						play_saw = 0;
						clear = 0;
        }
    }
}


void PORTA_IRQHandler(void) {
    const uint8_t rows[4] = {R1, R2, R3, R4};
    const int cols[4] = {C1_MASK, C2_MASK, C3_MASK, C4_MASK};
    uint32_t buf;

    buf = PORTA->ISFR & (C4_MASK | C3_MASK | C2_MASK | C1_MASK);

    switch (buf) {
        case C4_MASK:
            DELAY(100); // Minimalizacja drgañ zestyków
            if (!(PTA->PDIR & C4_MASK)) { 
                DELAY(100);
                if (!(PTA->PDIR & C4_MASK)) { 
                    PTB->PSOR = (1 << R1) | (1 << R2) | (1 << R3) | (1 << R4); 

                    for (int row = 0; row < 4; row++) { 
                        PTB->PCOR = (1 << rows[row]); 
                        DELAY(10);

                        if (!(PTA->PDIR & C4_MASK)) { // SprawdŸ zwarcie wiersz-kolumna
                            Pin_press[row][COL1] = 1;
														if(row == ROW4) {
															play_sine = 0;
															play_duration = PLAY_TIME; }
                            break;
                        }

                        PTB->PSOR = (1 << rows[row]); // Przywróæ stan wysoki na bie¿¹cym wierszu
                    }

                    PTB->PCOR = (1 << R1) | (1 << R2) | (1 << R3) | (1 << R4); // Przywróæ wszystkie wiersze do stanu niskiego
                }
            }
            break;
						
		case C3_MASK:	DELAY(100);
            if (!(PTA->PDIR & C3_MASK)) { 
                DELAY(100);
                if (!(PTA->PDIR & C3_MASK)) {
                    PTB->PSOR = (1 << R1) | (1 << R2) | (1 << R3) | (1 << R4);

                    for (int row = 0; row < 4; row++) {
                        PTB->PCOR = (1 << rows[row]);
                        DELAY(10);

                        if (!(PTA->PDIR & C3_MASK)) { 
                            Pin_press[row][COL2] = 1;
                            break;
                        }

                        PTB->PSOR = (1 << rows[row]);
                    }

                    PTB->PCOR = (1 << R1) | (1 << R2) | (1 << R3) | (1 << R4);
                }
            }
            break;
		case C2_MASK:	DELAY(100);
            if (!(PTA->PDIR & C2_MASK)) { 
                DELAY(100);
                if (!(PTA->PDIR & C2_MASK)) {
                    PTB->PSOR = (1 << R1) | (1 << R2) | (1 << R3) | (1 << R4);

                    for (int row = 0; row < 4; row++) {
                        PTB->PCOR = (1 << rows[row]);
                        DELAY(10);

                        if (!(PTA->PDIR & C2_MASK)) {
                            Pin_press[row][COL3] = 1;
														if(row == ROW4) {
															play_saw = 0;
															play_duration = PLAY_TIME; }
														if(row == ROW1) {
															clear = 0;
															play_duration = PLAY_TIME; }
                            break;
                        }

                        PTB->PSOR = (1 << rows[row]);
                    }

                    PTB->PCOR = (1 << R1) | (1 << R2) | (1 << R3) | (1 << R4);
                }
            }
            break;
		case C1_MASK:	DELAY(100);
            if (!(PTA->PDIR & C1_MASK)) {
                DELAY(100);
                if (!(PTA->PDIR & C1_MASK)) {
                    PTB->PSOR = (1 << R1) | (1 << R2) | (1 << R3) | (1 << R4);

                    for (int row = 0; row < 4; row++) {
                        PTB->PCOR = (1 << rows[row]);
                        DELAY(10);

                        if (!(PTA->PDIR & C1_MASK)) {
                            Pin_press[row][COL4] = 1;
														if(row == ROW4) {
															play_tri = 0;
															play_duration = PLAY_TIME; }
                            break;
                        }

                        PTB->PSOR = (1 << rows[row]);
                    }

                    PTB->PCOR = (1 << R1) | (1 << R2) | (1 << R3) | (1 << R4);
                }
            }
            break;
		default:			break;
	}	

	PORTA->ISFR |=  C4_MASK | C3_MASK | C2_MASK | C1_MASK;
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	
}


int main (void) 
{
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	
	Klaw_Init();							
	Klaw_Inter();						
	LED_Init();									
	LCD1602_Init();		 					
	LCD1602_Backlight(TRUE);  	
	LCD1602_ClearAll();	
	
	Display(0,0,write,1,"w:",display);
	Display(3,0,bpm,1,"BPM:",display);
	Display(10,0,wave,1,"WAVE:",display);

	for(int i = 0;i<8;i++) {	
		Display(i,1,sequencer[wave][i],1,"",display);
	}
	
	DAC_Init();
	for(n=0;n<32;n++)
		Sinus[n]=(sin((double)n*0.314159)*2047.0); // £adowanie dwudziestu, 12-bitowych próbek funkcji sisnus do tablicy
	n=0;
	SysTick_Config(SystemCoreClock/DIV_CORE);
	
  while(1)
	{
		Timer_Init(bpm);
		Key_Event(display);
		Sequencer_Update(display);		
	}
}
