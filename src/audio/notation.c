#include <string.h>

#include "core/mathBasics.h"

#include "audio/synth.h"
#include "audio/notation.h"

#define C_FREQ      16.35
#define CSHARP_FREQ 17.32
#define D_FREQ      18.35
#define DSHARP_FREQ 19.45
#define E_FREQ      20.60
#define F_FREQ      21.83
#define FSHARP_FREQ 23.12
#define G_FREQ      24.50
#define GSHARP_FREQ 25.96
#define A_FREQ      27.50
#define ASHARP_FREQ 29.14
#define B_FREQ      30.87

#define OCTAVE_SEMITONES 12

static int stringToSemitone(char noteStr[NOTE_STR_LEN]);
static double getFrequency(int semitoneIndex, int octave);

note_t createNote(char noteStr[NOTE_STR_LEN], int octave, int duration, double velocity) {
   note_t note;

   strncpy(note.note, noteStr, NOTE_STR_LEN);
   note.octave = octave;
   note.duration = duration;
   note.velocity = velocity;

   return note;
}

tone_t *playNote(int track, note_t note) {
   double frequency = getFrequency(stringToSemitone(note.note), note.octave);
   return synth_play(track, frequency, note.duration, note.velocity);
}

tone_t *playSilence(int track, int duration) {
   return synth_play(track, 0, duration, 0);
}

void playTriad(int trackOffset, note_t rootNote, int semitoneA, int semitoneB, int inversion) {
   int numTracks  = synth_getNumTracks();
   int rootOctave = rootNote.octave;
   int aOctave    = rootNote.octave;
   int bOctave    = rootNote.octave;

   if (inversion >= 1) {
      rootOctave += 1;
   }
   if (inversion == 2) {
      aOctave += 1;
   }

   int rootIndex = stringToSemitone(rootNote.note);
   int aIndex = (stringToSemitone(rootNote.note) + semitoneA) % OCTAVE_SEMITONES;
   int bIndex = (stringToSemitone(rootNote.note) + semitoneB) % OCTAVE_SEMITONES;

   double rootFreq = getFrequency(rootIndex, rootOctave);

   double aFreq = getFrequency(aIndex, aOctave);
   double bFreq = getFrequency(bIndex, bOctave);

   synth_play(trackOffset % numTracks,       rootFreq, rootNote.duration, rootNote.velocity);
   synth_play((trackOffset + 1) % numTracks, aFreq, rootNote.duration, rootNote.velocity);
   synth_play((trackOffset + 2) % numTracks, bFreq, rootNote.duration, rootNote.velocity);
}

void playMajTriad(int trackOffset, note_t rootNote, int inversion) {
   playTriad(trackOffset, rootNote, 4, 7, inversion);
}

void playMinTriad(int trackOffset, note_t rootNote, int inversion) {
   playTriad(trackOffset, rootNote, 3, 7, inversion);
}


static int stringToSemitone(char noteStr[NOTE_STR_LEN]) {
   if (strncmp(noteStr, "C", NOTE_STR_LEN) == 0) {
      return 0;
   } else if (strncmp(noteStr, "C#", NOTE_STR_LEN) == 0 ||
      strncmp(noteStr, "Db", NOTE_STR_LEN) == 0) {
      return 1;
   } else if (strncmp(noteStr, "D", NOTE_STR_LEN) == 0) {
      return 2;
   } else if (strncmp(noteStr, "D#", NOTE_STR_LEN) == 0 ||
      strncmp(noteStr, "Eb", NOTE_STR_LEN) == 0) {
      return 3;
   } else if (strncmp(noteStr, "E", NOTE_STR_LEN) == 0) {
      return 4;
   } else if (strncmp(noteStr, "F", NOTE_STR_LEN) == 0) {
      return 5;
   } else if (strncmp(noteStr, "F#", NOTE_STR_LEN) == 0 ||
      strncmp(noteStr, "Gb", NOTE_STR_LEN) == 0) {
      return 6;
   } else if (strncmp(noteStr, "G", NOTE_STR_LEN) == 0) {
      return 7;
   } else if (strncmp(noteStr, "G#", NOTE_STR_LEN) == 0 ||
      strncmp(noteStr, "Ab", NOTE_STR_LEN) == 0) {
      return 8;
   } else if (strncmp(noteStr, "A", NOTE_STR_LEN) == 0) {
      return 9;
   } else if (strncmp(noteStr, "A#", NOTE_STR_LEN) == 0 ||
      strncmp(noteStr, "Bb", NOTE_STR_LEN) == 0) {
      return 10;
   } else if (strncmp(noteStr, "B", NOTE_STR_LEN) == 0) {
      return 11;
   } else {
      return 12;
   }

   return 0;
}

static double getFrequency(int semitoneIndex, int octave) {
   double frequencies[] = {
      C_FREQ     ,
      CSHARP_FREQ,
      D_FREQ     ,
      DSHARP_FREQ,
      E_FREQ     ,
      F_FREQ     ,
      FSHARP_FREQ,
      G_FREQ     ,
      GSHARP_FREQ,
      A_FREQ     ,
      ASHARP_FREQ,
      B_FREQ     ,
   };

   return frequencies[semitoneIndex] * (1 << octave);
}

