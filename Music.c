// filename ******** Music.c ************** 
// Lab 5 Spring 2017
// Matt Owens & Rebecca Ho
// 2/21/17

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "Music.h"
#include "Timer.h"
#include "DAC.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

#define BUS 50000000
#define F2HZ BUS/2 	// tempo of 60 bpm


// define tempos
const uint16_t SIXTY_BPM = 32/2; // tempo of 60 bpm
const uint16_t ONETWENTY_BPM = 32/4; // tempo of 120 bpm

//tempo divider (speed up)
float tempo = 1;

// define frequencies for different notes
// range of song is high b flat to low c
const uint16_t Bflat_hi = 932;
const uint16_t A_hi = 880;
const uint16_t G_hi = 784;
const uint16_t F_mid = 698;
const uint16_t E_mid = 659;
const uint16_t D_mid = 587;
const uint16_t C_mid = 523;
const uint16_t Bflat_mid = 466;
const uint16_t A_mid = 440;
const uint16_t G_mid = 392;
const uint16_t F_low = 349;
const uint16_t E_low = 330;
const uint16_t D_low = 277;
const uint16_t C_low = 262;
const uint16_t Bflat_low = 233;
const uint16_t A_low = 220;

Note Song[56] = {{C_low,2},{F_low,3},{F_low,1},{F_low,2},{A_mid,2},{G_mid,3},{F_low,1},{G_mid,2},{A_mid,2},
		{F_low,3},{F_low,1},{A_mid,2},{C_mid,2},{D_mid,6},{D_mid,2},{C_mid,3},{A_mid,1},{A_mid,2},{F_low,2},
		{G_mid,3},{F_low,1},{G_mid,2},{A_mid,2},{F_low,3},{D_low,1},{D_low,2},{C_low,2},{F_low,6},{D_mid,2},
		{C_mid,3},{A_mid,1},{A_mid,2},{F_low,2},{G_mid,3},{F_low,1},{G_mid,2},{D_mid,2},{C_mid,3},{A_mid,1},
		{A_mid,2},{C_mid,2},{D_mid,6},{F_mid,2},{C_mid,3},{A_mid,1},{A_mid,2},{F_low,2},{G_mid,3},{F_low,1},
		{G_mid,2},{A_mid,1},{G_mid,1},{F_low,3},{F_low,1},{E_low,4},{F_low,6}};

int song_index = 0;		
int song_size = 56;
		
Note Harmony[52] = {{C_low,2},{C_low,3},{C_low,1},{C_low,2},{F_low,2},{E_low,3},{D_low,1},{E_low,2},{E_low,2},
		{F_low,3},{F_low,1},{F_low,2},{F_low,2},{F_low,6},{F_low,2},{F_low,3},{F_low,1},{F_low,2},{F_low,2},
		{E_low,3},{D_low,1},{E_low,2},{E_low,2},{D_low,3},{D_low,1},{E_low,2},{E_low,2},{F_low,6},{F_low,2},
		{F_low,4},{F_low,4},{F_low,2},{E_low,3},{D_low,1},{E_low,2},{E_low,2},{F_low,4},{F_low,4},{F_low,6},
		{F_low,2},{F_low,3},{F_low,1},{F_low,2},{C_low,2},{E_low,3},{D_low,1},{E_low,2},{E_low,2},{D_low,3},
		{F_low,1},{E_low,4},{F_low,6}};

int harmony_index = 0;
int harmony_size = 52;

uint16_t Envelope[100] = {0,29,39,45,49,52,55,58,60,62,63,65,66,67,69,70,71,72,73,74,74,75,76,77,77,78,79,79,
		80,80,81,81,82,82,83,83,84,85,85,85,86,86,87,87,87,88,88,88,89,89,89,90,90,91,91,91,91,92,92,92,92,92,92,
		92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,
		92,92};


uint16_t env_index = 0;
uint16_t env_size = 100;
	
// 12-bit 32-element sine wave
// multiply each value by 2 to shift into bits 12:1 of SSI packet
// three control bits in 15:13 are all zero for immediate DAC update
// book figure shows MAX5353 in unipolar rail-to-rail configuration
// that means when wave[n] = 0x0000 (LSB = 0), output = 0
//                 wave[n] = 0x1000 (LSB = 0), output = Vref
//                 wave[n] = 0x1FFE (LSB = 0), output = 2*Vref

// harmony wave
const uint16_t wave0[32] = {
  2048*2,2448*2,2832*2,3186*2,3496*2,3751*2,3940*2,4057*2,4095*2,4057*2,3940*2,
  3751*2,3496*2,3186*2,2832*2,2448*2,2048*2,1648*2,1264*2,910*2,600*2,345*2,
  156*2,39*2,0*2,39*2,156*2,345*2,600*2,910*2,1264*2,1648*2};


uint16_t wave0_index = 0;	

// melody wave	
const uint16_t wave1[32] = {
  2048*2,2448*2,2832*2,3186*2,3496*2,3751*2,3940*2,4057*2,4095*2,4057*2,3940*2,
  3751*2,3496*2,3186*2,2832*2,2448*2,2048*2,1648*2,1264*2,910*2,600*2,345*2,
  156*2,39*2,0*2,39*2,156*2,345*2,600*2,910*2,1264*2,1648*2};


uint16_t wave1_index = 0;
	
bool Play = false;
	
void OutputSine0(){ // harmony
	//if(Play) {
		wave0_index = wave0_index%32;
		uint16_t val = wave0[wave0_index];
		//DAC_Out((val/3) * 2);
		DAC_Out(val);
		wave0_index++;	
	}
//}
	
void OutputSine1(bool end){ // melody
	if(Play) {
		wave1_index = wave1_index%32;
		uint32_t val = wave1[wave1_index];
		
		uint32_t env;
		//DAC_Out((val/3) * 2);
		if(end == true) { // last 100 values in the note
			env_index = env_index - 1;
			env = Envelope[env_index];
		} else if(env_index == env_size) {
			env = Envelope[99]; // last value in the envelope
		} else {
			env = Envelope[env_index];
			env_index++;
		}
		val = (val*env)/100;
		DAC_Out(val);
		wave1_index++;	
	}
}

	
void PlaySong() {
	Timers_Enable();

	Play = true;
	long sr = StartCritical();
	song_index = song_index%song_size;
	harmony_index = harmony_index%harmony_size;
	Note s = Song[song_index];
	Note h = Harmony[harmony_index];
	song_index++;
	harmony_index++;
	
	uint32_t s_freq = BUS/s.freq; // frequency of DAC output
	s_freq = s_freq/32; // sine wave has 32 pieces
	
	uint32_t h_freq = BUS/h.freq;
	h_freq = h_freq/32;
	
	// will interrupt n.freq*32 times a second, length is based on half a second 
	// so halve 32 to 16
	uint32_t s_count = s.freq*ONETWENTY_BPM*s.length; 
	
	//TEMPO
	s_count = s_count/tempo;
	
	uint32_t h_count = h.freq*ONETWENTY_BPM*h.length;
	uint32_t wait = 0;
	
	//Timer0A_SetReload(h_freq, h_count, wait);
	Timer1A_SetReload(s_freq, s_count, wait); // note frequency interrupt	
	EndCritical(sr);
	
	// // note length interrupt
	
}

Note GetNextNote(NoteType t) {
	long sr = StartCritical();
	Note n;
	if(t == SongNote) {
		song_index = song_index%song_size;
		n = Song[song_index];
		song_index++;
	} else {
		harmony_index = harmony_index%harmony_size;
		n = Harmony[harmony_index];
		harmony_index++;
	}
	EndCritical(sr);
	return n;
}

void Timer1SetNextNote() {
	Note n = GetNextNote(SongNote);
	uint32_t freq = BUS/n.freq; // frequency of DAC output
	freq = freq/32; // sine wave has 32 pieces
	// will interrupt n.freq*32 times a second, length is based on half a second 
	// so halve 32 to 16
	//uint32_t wait = (n.freq*32)/100; // wait 10 ms
	uint32_t wait = 0;
	uint32_t count = n.freq*ONETWENTY_BPM*n.length + wait;
	
	//TEMPO
	count = count/tempo;
	
	Timer1A_SetReload(freq, count, wait); // note frequency interrupt	
	env_index = 0;
	
}

void Timer0SetNextNote() {
	Note n = GetNextNote(HarmonyNote);
	uint32_t freq = BUS/n.freq; // frequency of DAC output
	freq = freq/32; // sine wave has 32 pieces
	// will interrupt n.freq*32 times a second, length is based on half a second 
	// so halve 32 to 16
	uint32_t wait = (n.freq*32)/100; // wait 10 ms
	uint32_t count = n.freq*ONETWENTY_BPM*n.length + wait; 
	Timer1A_SetReload(freq, count, wait); // note frequency interrupt	
	
}

bool GetPlayState() {
	return Play;
}

void Pause() {
	Timers_Disable();
	Play = false;

	wave0_index = 0;
	wave1_index = 0;
}

void Rewind(bool playing) {
	if(playing){							//if song is playing, pause it
		Pause();
	}
	wave0_index = 0;
	wave1_index = 0;
	song_index = 0;
	harmony_index = 0;
	env_index = 0;
}

void ChangeTempo(void){
	if(tempo == 1)
		tempo = 2;
	else if(tempo == 2)
		tempo = 0.5;
	else
		tempo = 1;
}
