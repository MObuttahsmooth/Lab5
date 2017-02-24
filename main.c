// filename ******** main.c ************** 
// Lab 5 Spring 2017
// Matt Owens & Rebecca Ho
// 2/21/17

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
  Program 7.5, example 7.6

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// oscilloscope or LED connected to PF3-1 for period measurement
// When using the color wheel, the blue LED on PF2 is on for four
// consecutive interrupts then off for four consecutive interrupts.
// Blue is off for: dark, red, yellow, green
// Blue is on for: light blue, blue, purple, white
// Therefore, the frequency of the pulse measured on PF2 is 1/8 of
// the frequency of the Timer0A interrupts.

#include "..\ValvanoWareTM4C123\ValvanoWareTM4C123\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"
#include "Timer.h"
#include "DAC.h"
#include "Switch.h"
#include "SysTick.h"
#include "Music.h"


#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define LEDS      (*((volatile uint32_t *)0x40025038))

// 12-bit 32-element sine wave
// multiply each value by 2 to shift into bits 12:1 of SSI packet
// three control bits in 15:13 are all zero for immediate DAC update
// book figure shows MAX5353 in unipolar rail-to-rail configuration
// that means when wave[n] = 0x0000 (LSB = 0), output = 0
//                 wave[n] = 0x1000 (LSB = 0), output = Vref
//                 wave[n] = 0x1FFE (LSB = 0), output = 2*Vref
const uint16_t wave[32] = {
  2048*2,2448*2,2832*2,3186*2,3496*2,3751*2,3940*2,4057*2,4095*2,4057*2,3940*2,
  3751*2,3496*2,3186*2,2832*2,2448*2,2048*2,1648*2,1264*2,910*2,600*2,345*2,
  156*2,39*2,0*2,39*2,156*2,345*2,600*2,910*2,1264*2,1648*2};


uint16_t wave_index = 0;
	
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void UserTask0(void){
	PF1 ^= 0x02;
}

void UserTask1(void){
	PF2 ^= 0x04;
	/*
	if(wave_index == 1) { 
		DAC_Out(4000); 
		wave_index = 0;
	}
	else { 
		DAC_Out(0); 
		wave_index++;
	}
	*/
	wave_index = wave_index%32;
	uint16_t val = wave[wave_index%32];
	DAC_Out(val);
	wave_index++;
	
}
// if desired interrupt frequency is f, Timer0A_Init parameter is busfrequency/f
#define F16HZ (50000000/16)
#define F20KHZ (50000000/20000)
#define F10KHZ (50000000/10000)
#define F1HZ 50000000
//debug code
int main(void){ 
  PLL_Init(Bus50MHz);              // bus clock at 50 MHz
  SYSCTL_RCGCGPIO_R |= 0x20;       // activate port F
  while((SYSCTL_PRGPIO_R&0x0020) == 0){};// ready?
  GPIO_PORTF_DIR_R |= 0x0E;        // make PF3-1 output (PF3-1 built-in LEDs)
  GPIO_PORTF_AFSEL_R &= ~0x0E;     // disable alt funct on PF3-1
  GPIO_PORTF_DEN_R |= 0x0E;        // enable digital I/O on PF3-1
                                   // configure PF3-1 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
  LEDS = 0;                        // turn all LEDs off
//  Timer0A_Init(&UserTask, F20KHZ);     // initialize timer0A (20,000 Hz)
  Timer0A_Init(&UserTask0, F1HZ);  // initialize timer0A (16 Hz)
	Timer1A_Init(&UserTask1, F10KHZ);  // initialize timer0A (16 Hz)
	SysTick_Init();
	//DAC_Init(0x1000);                  // initialize with command: Vout = Vref
	DAC_Init(0x1FFE);
  EnableInterrupts();

  while(1){
    WaitForInterrupt();
  }
}
