#include "Sequencer.h"
#include "MKL05Z4.h"
#include "Display.h"
#include "klaw.h"
#include <string.h>

volatile int32_t Sinus[32];
const int32_t Triangle[] = {
    0, 409, 818, 1227, 1636, 2045, 1636, 1227,
    818, 409, 0, -409, -818, -1227, -1636, -2045,
    -1636, -1227, -818, -409, 0, 409, 818, 1227,
    1636, 2045, 1636, 1227, 818, 409, 0, -409
};
const int32_t Sawtooth[] = {
    0, 205, 409, 614, 819, 1024, 1228, 1433,
    1638, 1842, 2047, -2047, -1820, -1592, -1365, -1137,
    -910, -682, -455, -227, 0, 205, 409, 614,
    819, 1024, 1228, 1433, 1638, 1842, 2047, -2047
};
volatile uint8_t play = 0, write = 0, clear = 0, bpm = 40;
volatile uint8_t current_step = 0;
volatile uint8_t play_sine = 0;
volatile uint8_t play_tri = 0;
volatile uint8_t play_saw = 0;
volatile uint8_t wave = 0;
volatile uint8_t sequencer[SOUND_NUM][STEPS] = {{0,0,0,0,0,0,0,0},
																								{0,0,0,0,0,0,0,0},
																								{0,0,0,0,0,0,0,0}};
void Timer_Init(uint32_t bpm) {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; 
    PIT->MCR = 0x00; 

    uint32_t step_time_ms = 60000 / (bpm * STEPS); // Czas kroku w ms
    uint32_t load_value = (SystemCoreClock / 1000) * step_time_ms - 1;

    PIT->CHANNEL[0].LDVAL = load_value;
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
    NVIC_EnableIRQ(PIT_IRQn);
}

void Key_Event(char display[]) {
	if(!write) {
		// Zmiana BPM
			if (Pin_press[ROW2][COL3]) {
				bpm+=10;
        Pin_press[ROW2][COL3] = 0;
			}
			if (Pin_press[ROW2][COL4]) {
				bpm-=10;
        Pin_press[ROW2][COL4] = 0;
			}
				Display(7,0,bpm,3,"",display);
		}
		// W³¹cz/wy³¹cz
		if (Pin_press[ROW1][COL1]) {
				play ^= 1;
        Pin_press[ROW1][COL1] = 0;
    }
		// W³¹cz/wy³¹cz zapis
		if (Pin_press[ROW1][COL2]) {
				write ^= 1;
				Display(2,0,write,1,"",display);
        Pin_press[ROW1][COL2] = 0;
    }
		// Wyczyœæ ca³¹ sekwencjê
		if (Pin_press[ROW1][COL3]) {
				clear = 1;
				memset((void*)sequencer, 0, sizeof(sequencer));
				for(int i = 0;i<8;i++) {	
					Display(i,1,sequencer[0][i],1,"",display);
				}
        Pin_press[ROW1][COL3] = 0;
    }
		// Wybór dŸwiêku
		if (Pin_press[ROW4][COL1]) {
        play_sine = 1;
        Pin_press[ROW4][COL1] = 0;
				wave = 0;
				Display(15,0,wave,1,"",display);
				for(int i = 0;i<8;i++) {	
					Display(i,1,sequencer[wave][i],1,"",display);
				}		
    }
		if (Pin_press[ROW4][COL3]) {
        play_saw = 1;
        Pin_press[ROW4][COL3] = 0;
				wave = 2;
				Display(15,0,wave,1,"",display);
				for(int i = 0;i<8;i++) {	
					Display(i,1,sequencer[wave][i],1,"",display);
				}
    }
		if (Pin_press[ROW4][COL4]) {
        play_tri = 1;
        Pin_press[ROW4][COL4] = 0;
				wave = 1;
				Display(15,0,wave,1,"",display);
				for(int i = 0;i<8;i++) {	
					Display(i,1,sequencer[wave][i],1,"",display);
				}
    }
}

void Sequencer_Update(char display[]) {
	if(write) {
			for(int i = 1;i<3;i++) {
				for(int j = 0;j<4;j++) {
					if(Pin_press[i][j] && i == 1) {
						sequencer[wave][j] ^= 1;
						for(int i = 0;i<8;i++) {	
							Display(i,1,sequencer[wave][i],1,"",display);
						}
						Pin_press[i][j] = 0;
					}
					else if(Pin_press[i][j] && i == 2){
						sequencer[wave][j+4] ^= 1;
						for(int i = 0;i<8;i++) {	
							Display(j+4,1,sequencer[wave][j+4],1,"",display);
						}
						Pin_press[i][j] = 0;
					}
				}		
			}
		}
}