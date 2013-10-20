#ifndef SYNTH_H
#define SYNTH_H

#include <stdbool.h>

#define SAMPLE_RATE 44100
#define AMPLITUDE_MAX 32767

#define TRACK_BUFFER_SIZE 1024

// Generates a signal and saves it to the wavetable. Frequency = SAMPLE_RATE/length
typedef void (*signalGenerator_t)(double *wavetable, int length, double velocity, void *data);

// Reads a signal, returns the current value (may also modify the signal)
typedef double (*signalFilter_t)(double *wavetable, int length, int samplesPlayed, int totalSamples, void *data);

// Modifies the volume of a tone over its lifetime
typedef double (*volumeFilter_t)(int length, int samplesPlayed, int totalSamples, void *data);

typedef enum {
   ENV_MILLIS,
   ENV_PERCENT,
   ENV_SAMPLES
} envelopeUnits_t;

typedef struct {
   envelopeUnits_t units;
   int attack;
   int decay;
   int release;

   // volume
   double sustain;
} envelope_t;

typedef struct {
   signalGenerator_t signalGenerator;
   signalFilter_t signalFilter;
   volumeFilter_t volumeFilter;

   envelope_t envelope;
   void *data;
} instrument_t;

typedef struct {
   int track;

   double *wavetable;
   int length;

   int samplesPlayed;
   int samplesLeft;
} tone_t;

typedef struct {
   // queue of tones to play on this track
   int numTones;
   size_t first;
   tone_t tones[TRACK_BUFFER_SIZE];

   // instrument and volume to use
   instrument_t instrument;
   int volume;
} trackBuffer_t;


// init and de-init
void synth_init(int numTracks);
void synth_dispose(void);

// play a tone
tone_t *synth_play(int track, double frequency, int duration, double velocity);

void synth_setTrackVolume(int track, double volume);
void synth_setTrackInstrument(int track, instrument_t instrument);

int synth_getNumTracks(void);

#endif
