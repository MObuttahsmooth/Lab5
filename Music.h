// filename ******** Music.h ************** 
// Lab 5 Spring 2017
// Matt Owens & Rebecca Ho
// 2/21/17

#include <stdint.h>

struct Note {
	uint16_t length;
	uint16_t freq;
};
typedef struct Note Note;
