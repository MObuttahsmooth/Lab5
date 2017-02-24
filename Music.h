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

enum NoteType {SongNote, HarmonyNote}; 
typedef enum NoteType NoteType;

// ***************** PlaySong ****************
// Begins the song from current index
// Inputs: none
// Outputs: none
void PlaySong(void);

// ***************** GetNextNote ****************
// Gets the next note in the song
// Inputs: none
// Outputs: next note in the song
Note GetNextNote(NoteType);

// ***************** Timer1SetNextNote ****************
// Sets the new count and reload value for Timer1 depending on
// the next note
// Inputs: none
// Outputs: next note in the song
void Timer1SetNextNote(void);

void Timer0SetNextNote(void);

void OutputSine0(void);

void OutputSine1(void);
