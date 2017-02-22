// filename ******** Music.c ************** 
// Lab 5 Spring 2017
// Matt Owens & Rebecca Ho
// 2/21/17

#include <stdint.h>
#include "Music.h"

#define F2HZ 50000000/2 	// tempo of 60 bpm

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

Note Song[] = {{C_low,2},{F_low,3},{F_low,1},{F_low,2},{A_mid,2},{G_mid,3},{F_low,1},{G_mid,2},{A_mid,2},
		{F_low,3},{F_low,1},{A_mid,2},{C_mid,2},{D_mid,6},{D_mid,2},{C_mid,3},{A_mid,1},{A_mid,2},{F_low,2},
		{G_mid,3},{F_low,1},{G_mid,2},{A_mid,2},{F_low,3},{D_low,1},{D_low,2},{C_low,2},{F_low,6},{D_mid,2},
		{C_mid,3},{A_mid,1},{A_mid,2},{F_low,2},{G_mid,3},{F_low,1},{G_mid,2},{D_mid,2},{C_mid,3},{A_mid,1},
		{A_mid,2},{C_mid,2},{D_mid,6},{F_mid,2},{C_mid,3},{A_mid,1},{A_mid,2},{F_low,2},{G_mid,3},{F_low,1},
		{G_mid,2},{A_mid,1},{G_mid,1},{F_low,3},{D_low,1},{Bflat_low,4},{F_low,6}};
