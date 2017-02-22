// filename ******** Music.h ************** 
// Lab 5 Spring 2017
// Matt Owens & Rebecca Ho
// 2/21/17

#include <stdint.h>

struct Note {
	uint16_t freq;
	uint32_t length; // in # of eigth notes
};
typedef struct Note Note;


// ***************** PlaySong ****************
// Begins the song from current index
// Inputs: none
// Outputs: none
void PlaySong(void);

// ***************** GetNextNote ****************
// Gets the next note in the song
// Inputs: none
// Outputs: next note in the song
Note GetNextNote(void);
