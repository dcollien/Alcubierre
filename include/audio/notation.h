#ifndef NOTATION_H
#define NOTATION_H

#define NOTE_STR_LEN 3

typedef struct {
   char note[NOTE_STR_LEN];
   int octave;
   int duration;
   double velocity;
} note_t;

note_t createNote(char noteStr[NOTE_STR_LEN], int octave, int duration, double velocity);

tone_t *playNote(int track, note_t note);
tone_t *playSilence(int track, int duration);

void playMajTriad(int trackOffset, note_t rootNote, int inversion);
void playMinTriad(int trackOffset, note_t rootNote, int inversion);

void playTriad(int trackOffset, note_t rootNote, int semitoneA, int semitoneB, int inversion);

#endif
