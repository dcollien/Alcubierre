#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "core/mathBasics.h"

#include <SDL/SDL.h>

#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494
#define C5 523

#define AMPLITUDE_MAX 32767

#define AMPLITUDE 28000

#define FREQUENCY 44100

#define MAX_TONES 1024
#define MAX_POLYPHONY 8

#define VOICE_SINE 0
#define VOICE_SQUARE 1

typedef int voice_t;

typedef struct {
   double freqs[MAX_POLYPHONY];
   int numFreqs;
   voice_t voice;
   int samplesLeft;
} tone_t;


typedef struct {
   tone_t tones[MAX_TONES];
   size_t first;
   int numTones;
   double t[MAX_POLYPHONY];
   int volume;
} audioPlayback_t;

void audio_callback(void *, Uint8 *, int);


void audio_callback(void *data, Uint8 *stream, int length) {
   int i;
   int f;
   int numSamples;
   tone_t *tone;
   size_t channelLength = length/2;

   // convert stream and length to 2 byte signed
   Sint16 channels[MAX_POLYPHONY][channelLength];


   audioPlayback_t *playback = (audioPlayback_t *)data;

   if (playback->numTones == 0) {
      // silence
      for (i = 0; i != channelLength; ++i) {
         for (f = 0; f != MAX_POLYPHONY; ++f) {
            channels[f][i] = 0;
         }
      }
   } else {
      i = 0;

      while (i < channelLength) {
         // grab a tone off the queue
         tone = &(playback->tones[playback->first]);

         numSamples = i + tone->samplesLeft;
         if (numSamples > channelLength) {
            numSamples = channelLength;
         }

         tone->samplesLeft -= numSamples - i;

         while (i < numSamples) {
            for (f = 0; f != MAX_POLYPHONY; ++f) {
               if (f < tone->numFreqs) {
                  if (tone->voice == VOICE_SINE) {
                     channels[f][i] = AMPLITUDE * sin(playback->t[f] * TAU / FREQUENCY);  
                  } else {
                     channels[f][i] = AMPLITUDE * (sin(playback->t[f] * TAU / FREQUENCY) > 0);
                  }
                  playback->t[f] += tone->freqs[f];  
               } else {
                  channels[f][i] = 0;
               }
            }
            ++i;
         }

         if (tone->samplesLeft == 0) {
            playback->first = (playback->first + 1) % MAX_TONES;
            --(playback->numTones);
         }
      }  
   }

   for (f = 0; f != MAX_POLYPHONY; ++f) {
      SDL_MixAudio(stream, (Uint8 *)channels[f], length, playback->volume);
   }
}


void playChord(audioPlayback_t *playback, double freqs[MAX_POLYPHONY], int numFreqs, int duration, voice_t voice) {
   size_t last;
   int f;
   tone_t *tone;

   SDL_LockAudio();
   if (playback->numTones < MAX_TONES) {
      last = (playback->first + playback->numTones) % MAX_TONES;
      tone = &(playback->tones[last]);

      for (f = 0; f != numFreqs; ++f) {
         tone->freqs[f] = freqs[f];
      }

      tone->numFreqs = numFreqs;
      tone->samplesLeft = duration * FREQUENCY / 1000;
      tone->voice = voice;

      ++(playback->numTones);
   }
   SDL_UnlockAudio();
}


int main(int argc, char *argv[]) {
   SDL_AudioSpec desiredSpec;
   SDL_AudioSpec spec;
   int f;
   double chord[MAX_POLYPHONY];

   audioPlayback_t playback;

   playback.first = 0;
   playback.numTones = 0;
   playback.volume = 3 * SDL_MIX_MAXVOLUME / 4;

   for (f = 0; f != MAX_POLYPHONY; ++f) {
      playback.t[f] = 0.0;
   }
   

   desiredSpec.freq = FREQUENCY;
   desiredSpec.format = AUDIO_S16SYS;
   desiredSpec.channels = 1;
   desiredSpec.samples = 8192;
   desiredSpec.callback = audio_callback;
   desiredSpec.userdata = &playback;

   SDL_OpenAudio(&desiredSpec, &spec);

   // start play audio
   SDL_PauseAudio(0);

   SDL_Init(SDL_INIT_AUDIO);

   chord[0] = C4;
   chord[1] = E4;
   chord[2] = G4;
   chord[3] = C5;
   playChord(&playback, chord, 4, 500, VOICE_SINE);

   chord[0] = G4;
   playChord(&playback, chord, 1, 500, VOICE_SINE);

   chord[0] = E4;
   playChord(&playback, chord, 1, 500, VOICE_SQUARE);

   chord[0] = G4;
   playChord(&playback, chord, 1, 500, VOICE_SINE);

   chord[0] = C4;
   chord[1] = E4;
   chord[2] = G4;
   chord[3] = C5;
   playChord(&playback, chord, 4, 500, VOICE_SQUARE);

   SDL_Delay(5000);

   SDL_CloseAudio();

   return EXIT_SUCCESS;
}