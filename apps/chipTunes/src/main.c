#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "core/mathBasics.h"

#include <SDL/SDL.h>

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

#define AMPLITUDE_MAX 32767

#define AMPLITUDE 28000

#define SAMPLE_RATE 44100

#define TONE_BUF_SIZE 1024
#define NUM_TRACKS 8

#define VOICE_SILENT (-1)
#define VOICE_SINE 0
#define VOICE_SQUARE 1
#define VOICE_TRIANGLE 2
#define VOICE_SAW 3
#define VOICE_PLUCK 4
#define VOICE_ORGAN 5

#define NOTE_STR_LEN 3


typedef int voice_t;

typedef struct {
   int attack;
   int decay;
   double sustain;
   int release;
} envelope_t;

typedef struct {
   char note[NOTE_STR_LEN];
   int octave;
   int duration;
   double velocity;
} note_t;

typedef struct {
   double frequency;
   voice_t voice;
   double velocity;
   int track;

   envelope_t envelope;

   double *wavetable;

   double wobbleFreq;
   double tremoloFreq;
   double tremoloAmp;

   int samplesPlayed;
   int samplesLeft;
} tone_t;

typedef struct {
   int numTones;
   int sizeTones;
   size_t first;
   tone_t *tones;
   int volume;
} trackBuffer_t;

typedef struct {
   trackBuffer_t tracks[NUM_TRACKS];
} audioPlayback_t;


note_t createNote(char noteStr[NOTE_STR_LEN], int octave, int duration, double velocity);


tone_t *playNote(audioPlayback_t *playback, int trackIdx, voice_t voice, note_t note);

void playMajTriad(audioPlayback_t *playback, int trackOffset, voice_t voice, note_t rootNote, int inversion);
void playMinTriad(audioPlayback_t *playback, int trackOffset, voice_t voice, note_t rootNote, int inversion);

tone_t *play(audioPlayback_t *playback, int trackIdx, double frequency, int duration, voice_t voice, double velocity);
void initPlayback(audioPlayback_t *playback);
void audio_callback(void *, Uint8 *, int);

void syncTracks(audioPlayback_t *playback);

static void processTrackSamples(trackBuffer_t *track, Sint16 *trackSamples, int length);


static int getSemitoneIndex(char noteStr[NOTE_STR_LEN]);
static double getSemitoneFrequency(int semitoneIndex);

static double square_wave(double x);
static double triangle_wave(double x);
static double saw_wave(double x);
static double pluck_wave(tone_t *tone);

static Sint16 getSignal(tone_t *tone);



int main(int argc, char *argv[]) {
   SDL_AudioSpec desiredSpec;
   SDL_AudioSpec spec;

   audioPlayback_t playback;

   initPlayback(&playback);

   desiredSpec.freq = SAMPLE_RATE;
   desiredSpec.format = AUDIO_S16SYS;
   desiredSpec.channels = 1;
   desiredSpec.samples = 8192;
   desiredSpec.callback = audio_callback;
   desiredSpec.userdata = &playback;

   SDL_OpenAudio(&desiredSpec, &spec);

   // start play audio
   SDL_PauseAudio(0);

   SDL_Init(SDL_INIT_AUDIO);


   int timing = 0;
   double chordVelocity = 1.0;

   voice_t voice;

   int i;

   for (i = 0; i != 10; ++i) {
      if (i % 5 == 0) {
         voice = VOICE_PLUCK;
         chordVelocity = 1.0;
      } else if (i % 5 == 1) {
         voice = VOICE_TRIANGLE;
         chordVelocity = 0.6;
      } else if (i % 5 == 2) {
         voice = VOICE_SAW;
         chordVelocity = 0.4;
      } else if (i % 5 == 3) {
         voice = VOICE_SQUARE;
         chordVelocity = 0.4;
      } else {
         voice = VOICE_ORGAN;
         chordVelocity = 0.6;
      }

      playMajTriad(&playback, 1, voice, 
         createNote("C", 4, 1000, chordVelocity), 0
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("C", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("C", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("C", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("C", 4, 250, 0.8)
      );

      timing += 1000;


      playMajTriad(&playback, 1, voice, 
         createNote("G", 3, 1000, chordVelocity), 1
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("D", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("E", 4, 250, 0.8)
      );
      
      playNote(&playback, 0, VOICE_SINE,
         createNote("F", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("G", 4, 250, 0.8)
      );

      timing += 1000;

      playMajTriad(&playback, 1, voice, 
         createNote("F", 3, 1000, chordVelocity), 0
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("D", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("E", 4, 250, 0.8)
      );
      
      playNote(&playback, 0, VOICE_SINE,
         createNote("F", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("G", 4, 250, 0.8)
      );

      timing += 1000;

      playMajTriad(&playback, 1, voice, 
         createNote("G", 3, 250, chordVelocity), 1
      );

      playMajTriad(&playback, 1, voice, 
         createNote("G", 3, 750, chordVelocity), 1
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("B", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("A", 4, 250, 0.8)
      );
      
      playNote(&playback, 0, VOICE_SINE,
         createNote("B", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("D", 5, 250, 0.8)
      );

      timing += 1000;

      playMinTriad(&playback, 1, voice, 
         createNote("A", 4, 1000, chordVelocity), 0
      );

      if (i % 3 == 0 || i % 3 == 1) {
         playNote(&playback, 0, VOICE_SINE,
            createNote("E", 5, 250, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("D", 5, 250, 0.8)
         );
         
         playNote(&playback, 0, VOICE_SINE,
            createNote("C", 5, 250, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("B", 4, 250, 0.8)
         );
      } else {
         playNote(&playback, 0, VOICE_SINE,
            createNote("B", 4, 250, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("C", 5, 250, 0.8)
         );
         
         playNote(&playback, 0, VOICE_SINE,
            createNote("D", 5, 250, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("E", 5, 250, 0.8)
         );
      }

      timing += 1000;


      playMajTriad(&playback, 1, voice, 
         createNote("G", 3, 1000, chordVelocity), 1
      );

      if (i % 2 == 0) {
         playNote(&playback, 0, VOICE_SINE,
            createNote("F", 5, 250, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("E", 5, 250, 0.8)
         );
         
         playNote(&playback, 0, VOICE_SINE,
            createNote("D", 5, 250, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("C", 5, 250, 0.8)
         );
      } else {
         
         playNote(&playback, 0, VOICE_SINE,
            createNote("C", 5, 250, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("D", 5, 250, 0.8)
         );
         
         playNote(&playback, 0, VOICE_SINE,
            createNote("E", 5, 250, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("C", 5, 250, 0.8)
         );
      }

      timing += 1000;

      playMajTriad(&playback, 1, voice, 
         createNote("F", 3, 1000, chordVelocity), 0
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("A", 4, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("G", 5, 250, 0.8)
      );
      
      playNote(&playback, 0, VOICE_SINE,
         createNote("A", 5, 250, 0.8)
      );

      playNote(&playback, 0, VOICE_SINE,
         createNote("C", 5, 250, 0.8)
      );

      timing += 1000;

      playMajTriad(&playback, 1, voice, 
         createNote("G", 3, 1000, chordVelocity), 2
      );

      if (i % 2 == 0) {
         playNote(&playback, 0, VOICE_SINE,
            createNote("E", 5, 500, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("D", 5, 500, 0.8)
         );
      } else {
         playNote(&playback, 0, VOICE_SINE,
            createNote("D", 5, 500, 0.8)
         );

         playNote(&playback, 0, VOICE_SINE,
            createNote("E", 5, 500, 0.8)
         );
      }

      timing += 1000;
   }

  playMajTriad(&playback, 1, voice, 
      createNote("C", 4, 2000, chordVelocity), 0
   );

   playNote(&playback, 0, VOICE_SINE,
      createNote("C", 4, 250, 0.8)
   );

   playNote(&playback, 0, VOICE_SINE,
      createNote("C", 4, 250, 0.8)
   );

   playNote(&playback, 0, VOICE_SINE,
      createNote("C", 4, 250, 0.8)
   );

   playNote(&playback, 0, VOICE_SINE,
      createNote("C", 4, 250, 0.8)
   );

   timing += 1000;

   playNote(&playback, 0, VOICE_SINE,
      createNote("C", 4, 250, 0.8)
   );

   playNote(&playback, 0, VOICE_SINE,
      createNote("C", 4, 250, 0.8)
   );

   playNote(&playback, 0, VOICE_SINE,
      createNote("C", 4, 250, 0.8)
   );

   playNote(&playback, 0, VOICE_SINE,
      createNote("C", 4, 250, 0.8)
   );

   timing += 1000;

   playNote(&playback, 0, VOICE_SINE,
      createNote("C", 3, 1000, 0.8)
   );

   timing += 1000;

   SDL_Delay(timing);

   SDL_CloseAudio();

   return EXIT_SUCCESS;
}


note_t createNote(char noteStr[NOTE_STR_LEN], int octave, int duration, double velocity) {
   note_t note;

   strncpy(note.note, noteStr, NOTE_STR_LEN);
   note.octave = octave;
   note.duration = duration;
   note.velocity = velocity;

   return note;
}

static int getSemitoneIndex(char noteStr[NOTE_STR_LEN]) {
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

static double getSemitoneFrequency(int semitoneIndex) {
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

   return frequencies[semitoneIndex];
}

tone_t *playNote(audioPlayback_t *playback, int trackIdx, voice_t voice, note_t note) {
   double frequency;

   frequency = getSemitoneFrequency(getSemitoneIndex(note.note));

   frequency = frequency * pow(2, note.octave);

   return play(playback, trackIdx, frequency, note.duration, voice, note.velocity);
}

void playMajTriad(audioPlayback_t *playback, int trackOffset, voice_t voice, note_t rootNote, int inversion) {
   int rootOctave = rootNote.octave;
   int thirdOctave = rootNote.octave;
   int sixthOctave = rootNote.octave;

   if (inversion >= 1) {
      rootOctave += 1;
   }
   if (inversion == 2) {
      thirdOctave += 1;
   }

   int rootIndex = getSemitoneIndex(rootNote.note);
   int thirdIndex = (getSemitoneIndex(rootNote.note) + 4) % OCTAVE_SEMITONES;
   int sixthIndex = (getSemitoneIndex(rootNote.note) + 7) % OCTAVE_SEMITONES;

   double rootFreq = getSemitoneFrequency(rootIndex) * (1 << rootOctave);

   double thirdFreq = getSemitoneFrequency(thirdIndex) * (1 << thirdOctave);
   double sixthFreq = getSemitoneFrequency(sixthIndex) * (1 << sixthOctave);
   
   // TODO: fill each track with silence if track isn't empty

   play(playback, trackOffset % NUM_TRACKS, rootFreq, rootNote.duration, voice, rootNote.velocity * 0.8);
   play(playback, (trackOffset + 1) % NUM_TRACKS, thirdFreq, rootNote.duration, voice, rootNote.velocity * 0.8);
   play(playback, (trackOffset + 2) % NUM_TRACKS, sixthFreq, rootNote.duration, voice, rootNote.velocity * 0.8);
}

void playMinTriad(audioPlayback_t *playback, int trackOffset, voice_t voice, note_t rootNote, int inversion) {
   int rootOctave = rootNote.octave;
   int thirdOctave = rootNote.octave;
   int sixthOctave = rootNote.octave;

   if (inversion >= 1) {
      rootOctave += 1;
   }
   if (inversion == 2) {
      thirdOctave += 1;
   }

   int rootIndex = getSemitoneIndex(rootNote.note);
   int thirdIndex = (getSemitoneIndex(rootNote.note) + 3) % OCTAVE_SEMITONES;
   int sixthIndex = (getSemitoneIndex(rootNote.note) + 7) % OCTAVE_SEMITONES;

   double rootFreq = getSemitoneFrequency(rootIndex) * (1 << rootOctave);

   double thirdFreq = getSemitoneFrequency(thirdIndex) * (1 << thirdOctave);
   double sixthFreq = getSemitoneFrequency(sixthIndex) * (1 << sixthOctave);
   
   // TODO: fill each track with silence if track isn't empty

   play(playback, trackOffset % NUM_TRACKS, rootFreq, rootNote.duration, voice, rootNote.velocity * 0.8);
   play(playback, (trackOffset + 1) % NUM_TRACKS, thirdFreq, rootNote.duration, voice, rootNote.velocity * 0.8);
   play(playback, (trackOffset + 2) % NUM_TRACKS, sixthFreq, rootNote.duration, voice, rootNote.velocity * 0.8);
}

tone_t *play(audioPlayback_t *playback, int trackIdx, double frequency, int duration, voice_t voice, double velocity) {
   size_t last;
   int f;
   tone_t *tone;

   trackBuffer_t *track = &(playback->tracks[trackIdx]);

   SDL_LockAudio();
   if (track->numTones < TONE_BUF_SIZE) {
      last = (track->first + track->numTones) % TONE_BUF_SIZE;
      
      if (track->numTones >= track->sizeTones) {
         track->sizeTones *= 2;
         track->tones = realloc(track->tones, sizeof(tone_t) * track->sizeTones);
      }

      tone = &(track->tones[last]);

      assert(tone != NULL);

      tone->frequency = frequency;
      tone->voice = voice;
      tone->track = trackIdx;
      tone->velocity = velocity;

      tone->samplesLeft = duration * SAMPLE_RATE / 1000;
      tone->samplesPlayed = 0;

      tone->wobbleFreq = 0;
      tone->tremoloFreq = 0;
      tone->tremoloAmp = 0.25;

      // default ramp
      tone->envelope.attack  = 0;
      tone->envelope.decay   = 7*tone->samplesLeft/8;
      tone->envelope.sustain = 0.8;
      tone->envelope.release = tone->samplesLeft/8;

      ++(track->numTones);
   }
   SDL_UnlockAudio();

   return tone;
}


void initPlayback(audioPlayback_t *playback) {
   int i;

   for (i = 0; i != NUM_TRACKS; ++i) {
      playback->tracks[i].first = 0;
      playback->tracks[i].numTones = 0;
      playback->tracks[i].sizeTones = 8;
      playback->tracks[i].volume = SDL_MIX_MAXVOLUME;
      playback->tracks[i].tones = malloc(sizeof(tone_t) * playback->tracks[i].sizeTones);
   }
}

void audio_callback(void *data, Uint8 *stream, int length) {
   int i;
   int track;
   int numSamples;
   tone_t *tone;
   size_t trackLength = length/2;

   // convert stream and length to 2 byte signed
   Sint16 trackSamples[NUM_TRACKS][trackLength];


   audioPlayback_t *playback = (audioPlayback_t *)data;


   for (track = 0; track != NUM_TRACKS; ++track) {
      processTrackSamples(&(playback->tracks[track]), trackSamples[track], trackLength);

      SDL_MixAudio(stream, (Uint8 *)trackSamples[track], length, playback->tracks[track].volume);
   }
}

static void processTrackSamples(trackBuffer_t *track, Sint16 *trackSamples, int length) {
   tone_t *tone;
   int trackIdx, toneIdx;
   int numSamples;

   trackIdx = 0;

   // go through all the tones to play on this track 
   // until the track buffer is filled
   while (track->numTones != 0 && trackIdx != length) {
      // grab a tone off the queue
      tone = &(track->tones[track->first]);

      // find out how many samples are left
      // limited by how much is left in the track buffer
      numSamples = tone->samplesLeft;
      if (numSamples > (length - trackIdx)) {
         numSamples = length - trackIdx;
      }

      // add the samples from the tone to the track buffer
      for (toneIdx = 0; toneIdx != numSamples; ++toneIdx) {
         trackSamples[trackIdx + toneIdx] = getSignal(tone);
         tone->samplesPlayed++; 
         tone->samplesLeft--;
      }

      trackIdx += numSamples;

      if (tone->samplesLeft == 0) {
         // move on to the next tone
         track->first = (track->first + 1) % TONE_BUF_SIZE;
         --(track->numTones);
      }
   }

   // fill the rest of the buffer with silence
   while (trackIdx != length) {
      trackSamples[trackIdx] = 0;
      trackIdx++;
   }
}

static double square_wave(double x) {
   double sin_x = sin(x);

   if (sin_x > 0) {
      return 1.0;
   } else {
      return -1.0;
   }
}

static double triangle_wave(double x) {
   double t = x/TAU;
   return abs(((int)floor((4.0 * t) - 1) % 4) - 2) - 1;
}

static double saw_wave(double x) {
   double t = x/TAU;
   return 2.0 * (t - floor(t)) - 1.0;
   //return (-4.0 / TAU) * atan(1.0/tan(x/2.0));
}

static double organ_wave(double x) {
   return (sin(x) + sin(2*x) + 3*sin(3*x) + sin(4*x) + sin(5*x) + sin(6*x))/8;
}

static double pluck_wave(tone_t *tone) {
   // karplus-strong

   int i;
   double value;

   int bufLength = SAMPLE_RATE / tone->frequency;

   int totalSamples = tone->samplesLeft + tone->samplesPlayed;

   double smoothing = 0.8/(pow(totalSamples, (1.0/bufLength)));

   if (tone->samplesPlayed == 0) {
      // just plucked
      // fill with random noise
      tone->wavetable = malloc(sizeof(double) * bufLength);
      assert(tone->wavetable != NULL);

      for (i = 0; i != bufLength; ++i) {
         tone->wavetable[i] = vary(1.0);
      }
   } else {
      // ringing
      if (tone->samplesPlayed % bufLength == 0) {
         // process sample buffer
         for (i = 1; i != bufLength; ++i) {
            tone->wavetable[i] = lerp(smoothing, tone->wavetable[i-1], tone->wavetable[i]);
         }
      }
   }

   value = tone->wavetable[tone->samplesPlayed % bufLength];

   if (tone->samplesLeft == 0) {
      free(tone->wavetable);
   }

   return value;
}

static Sint16 getSignal(tone_t *tone) {
   int i;
   double amplitude = AMPLITUDE * tone->velocity;
   voice_t voice = tone->voice;

   int totalSamples = tone->samplesPlayed + tone->samplesLeft;
   double x = tone->samplesPlayed * tone->frequency * TAU / SAMPLE_RATE;

   double signalValue = 0;
   double envelopeAmp = 1.0;

   // Waveform
   if (tone->frequency == 0 || voice == VOICE_SILENT) {
      signalValue = 0;
   } else if (voice == VOICE_SINE) {
      signalValue = (Sint16)(amplitude * sin(x));
   } else if (voice == VOICE_SQUARE) {
      signalValue = amplitude * square_wave(x);
   } else if (voice == VOICE_TRIANGLE) {
      signalValue = amplitude * triangle_wave(x);
   } else if (voice == VOICE_SAW) {
      signalValue = amplitude * saw_wave(x);
   } else if (voice == VOICE_PLUCK) {
      signalValue = amplitude * pluck_wave(tone);
   } else if (voice == VOICE_ORGAN) {
      signalValue = amplitude * organ_wave(x);
   }

   // ADSR
   if (tone->samplesPlayed < tone->envelope.attack) {
      // attack
      envelopeAmp = (double)tone->samplesPlayed/(double)tone->envelope.attack;
   } else if (tone->samplesPlayed < (tone->envelope.attack + tone->envelope.decay)) {
      // decay
      envelopeAmp = (double)(tone->samplesPlayed-tone->envelope.attack)/(double)tone->envelope.decay;
      envelopeAmp = lerp(envelopeAmp, 1.0, tone->envelope.sustain);
   } else if (tone->samplesPlayed > (totalSamples - tone->envelope.release)) {
      // release
      envelopeAmp = (double)(tone->samplesPlayed - (totalSamples - tone->envelope.release))/(double)tone->envelope.release;
      envelopeAmp = lerp(envelopeAmp, tone->envelope.sustain, 0.0);
   } else {
      // sustain
      envelopeAmp = tone->envelope.sustain;
   }

   // LFO
   if (tone->tremoloFreq != 0) {
      x = tone->samplesPlayed * tone->tremoloFreq * TAU / SAMPLE_RATE;
      envelopeAmp *= (sin(x) * tone->tremoloAmp) + (1 - tone->tremoloAmp);
   }

   // TODO Low-Pass Filter with LFO (wobble) on cut-off
   

   return envelopeAmp * signalValue;
}