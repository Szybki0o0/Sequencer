#include "MKL05Z4.h"
#define SIN 0
#define TRI 1
#define SAW 2
#define SOUND_NUM 3
#define STEPS 8
#define PLAY_TIME 400
extern volatile int32_t Sinus[32];
extern const int32_t Triangle[];
extern const int32_t Sawtooth[];
extern volatile uint8_t sequencer[SOUND_NUM][STEPS];
extern volatile uint8_t current_step;
extern volatile uint8_t play, write, clear, bpm;
extern volatile uint8_t play_sine;
extern volatile uint8_t play_tri;
extern volatile uint8_t play_saw;
extern volatile uint8_t wave;
void Timer_Init(uint32_t bpm);
void Sequencer_Update(char display[]);
void Key_Event(char display[]);