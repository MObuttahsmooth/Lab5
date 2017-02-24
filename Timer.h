// filename ******** Timer.h ************** 
// Lab 5 Spring 2017
// Matt Owens & Rebecca Ho
// 2/21/17

// Runs on LM4F120/TM4C123
// Use Timer0A and Timer 1 in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 11, 2013

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

#ifndef __TIMER_H__ // do not include more than once
#define __TIMER_H__

// ***************** Timer0A_Init ****************
// Activate Timer0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer0A_Init(uint32_t period);

// ***************** Timer0A_Disable ****************
// Disable Timer0A
// Inputs:  none
// Outputs: none
void Timers_Disable(void);

// ***************** Timer0A_Enable ****************
// Enable Timer0A
// Inputs:  none
// Outputs: none
void Timers_Enable(void);

// ***************** Timer0A_SetReload ****************
// Change the reload value of Timer0A.
// Inputs: period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer0A_SetReload(uint32_t period, uint32_t count, uint32_t wait);

// ***************** Timer1A_Init ****************
// Activate Timer0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer1A_Init(uint32_t period);

// ***************** Timer1A_Disable ****************
// Disable Timer1A
// Inputs:  none
// Outputs: none
void Timer1A_Disable(void);

// ***************** Timer1A_Enable ****************
// Enable Timer1A
// Inputs:  none
// Outputs: none
void Timer1A_Enable(void);

// ***************** Timer1A_SetReload ****************
// Change the reload value of Timer1A.
// Inputs: period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer1A_SetReload(uint32_t period, uint32_t count, uint32_t wait);

#endif // __TIMER_H__
