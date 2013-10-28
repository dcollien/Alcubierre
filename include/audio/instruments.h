#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include "audio/synth.h"

// Prefabricated Instruments and ADSR Envelopes

typedef enum {
   WAVE_SINE,
   WAVE_SQUARE,
   WAVE_TRIANGLE,
   WAVE_SAW
} waveformPreset_t;

/*
instrument_t instrument_generic(
   envelope_t envelope,
   waveformPreset_t waveform,
   double cutoffLFO,
   double cutoffIntensity,
   double ampLFO,
   double ampIntensity
);
*/
instrument_t instrument_sine(envelope_t envelope);
instrument_t instrument_square(envelope_t envelope);
instrument_t instrument_triangle(envelope_t envelope);
instrument_t instrument_saw(envelope_t envelope);
instrument_t instrument_organ(envelope_t envelope);
instrument_t instrument_leslieOrgan(envelope_t envelope);
instrument_t instrument_pluckedString(envelope_t envelope);
instrument_t instrument_sciFi(envelope_t envelope);
instrument_t instrument_vibrato(envelope_t envelope);
instrument_t instrument_noisySin(envelope_t envelope);
instrument_t instrument_wobble(envelope_t envelope);

envelope_t envelope_stuccato(void);
envelope_t envelope_hold(double sustain);
envelope_t envelope_fadeOut(void);
envelope_t envelope_fadeIn(void);
envelope_t envelope_soft(void);

#endif
