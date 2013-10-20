#include <stdlib.h>
#include <assert.h>

#include <SDL/SDL.h>

#include "core/mathBasics.h"
#include "audio/synth.h"


#define AMPLITUDE_MAX 32767
#define AMPLITUDE AMPLITUDE_MAX

#define TRACK_BUFFER_SIZE 1024

typedef struct {
   trackBuffer_t *tracks;
   int numTracks;
} audioPlayback_t;

static void audioCallback(void *data, Uint8 *stream, int length);
static void processTrackSamples(trackBuffer_t *track, Sint16 *trackSamples, int length);
static Sint16 getSample(tone_t *tone);

static audioPlayback_t *playback;

void synth_init(int numTracks) {
   int i;
   SDL_AudioSpec desiredSpec;
   SDL_AudioSpec spec;

   playback = malloc(sizeof(audioPlayback_t));

   // allocate tracks
   playback->numTracks = numTracks;
   playback->tracks = malloc(sizeof(trackBuffer_t) * numTracks);
   assert(playback->tracks != NULL);

   for (i = 0; i != numTracks; ++i) {
      // init tone queue
      playback->tracks[i].first = 0;
      playback->tracks[i].numTones = 0;

      // init track settings
      playback->tracks[i].volume = SDL_MIX_MAXVOLUME;
   }

   // set up SDL audio
   desiredSpec.freq = SAMPLE_RATE;
   desiredSpec.format = AUDIO_S16SYS;
   desiredSpec.channels = 1;
   desiredSpec.samples = 8192;
   desiredSpec.callback = audioCallback;
   desiredSpec.userdata = &playback;

   SDL_OpenAudio(&desiredSpec, &spec);

   SDL_Init(SDL_INIT_AUDIO);

   SDL_PauseAudio(0);
}

void synth_dispose(void) {
   int i;
   trackBuffer_t *track;

   SDL_LockAudio();
   for (i = 0; i != playback->numTracks; ++i) {
      track = &(playback->tracks[i]);

      while (track->numTones != 0) {
         if (track->tones[track->first].length != 0) {
            free(track->tones[track->first].wavetable);  
         }

         track->first = (track->first + 1) % TRACK_BUFFER_SIZE;
         --(track->numTones);
      }
   }

   free(playback->tracks);

   free(playback);

   SDL_UnlockAudio();
   SDL_CloseAudio();
}

void synth_setTrackVolume(int track, double volume) {
   playback->tracks[track].volume = volume * SDL_MIX_MAXVOLUME;
}

void synth_setTrackInstrument(int track, instrument_t instrument) {
   playback->tracks[track].instrument = instrument;
}

int synth_getNumTracks(void) {
   return playback->numTracks;
}

tone_t *synth_play(int trackIdx, double frequency, int duration, double velocity) {
   size_t last;
   tone_t *tone;

   trackBuffer_t *track = &(playback->tracks[trackIdx]);
   int bufLength;

   if (frequency == 0.0) {
      bufLength = 0;
      velocity = 0;
   } else {
      bufLength = SAMPLE_RATE / frequency;  
   }

   SDL_LockAudio();
   if (track->numTones < TRACK_BUFFER_SIZE) {
      // find the index where to add a new note to the queue
      last = (track->first + track->numTones) % TRACK_BUFFER_SIZE;

      // we're adding the tone here
      tone = &(track->tones[last]);

      // assign the track
      tone->track = trackIdx;

      // allocate the waveform
      if (bufLength != 0) {
         tone->wavetable = malloc(sizeof(double) * bufLength);
         assert(tone->wavetable != NULL);

         // use the instrument to generate the initial waveform
         track->instrument.signalGenerator(tone->wavetable, bufLength, velocity, track->instrument.data);
      }
      tone->length = bufLength;

      // calculate the number of samples
      tone->samplesLeft = duration * SAMPLE_RATE / 1000;
      tone->samplesPlayed = 0;

      // increment the queue
      ++(track->numTones);
   }
   SDL_UnlockAudio();

   return tone;
}

static void audioCallback(void *data, Uint8 *stream, int length) {
   int track;
   size_t trackLength = length/2;

   // convert stream and length to 2 byte signed
   Sint16 trackSamples[playback->numTracks][trackLength];

   for (track = 0; track != playback->numTracks; ++track) {
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
         trackSamples[trackIdx + toneIdx] = getSample(tone);
         tone->samplesPlayed++; 
         tone->samplesLeft--;
      }

      trackIdx += numSamples;

      if (tone->samplesLeft == 0) {
         // free the tone's wavetable
         if (tone->length != 0) {
            free(tone->wavetable);  
         }

         // move on to the next tone
         track->first = (track->first + 1) % TRACK_BUFFER_SIZE;
         --(track->numTones);
      }
   }

   // fill the rest of the buffer with silence
   while (trackIdx != length) {
      trackSamples[trackIdx] = 0;
      trackIdx++;
   }
}


static Sint16 getSample(tone_t *tone) {
   instrument_t *instrument = &(playback->tracks[tone->track].instrument);

   // silence
   if (tone->length == 0) {
      return 0;
   }

   int i = tone->samplesPlayed;
   int waveformIndex = i % tone->length;
   int totalSamples = i + tone->samplesLeft;

   double signalValue;
   double envelopeAmp;

   int attack, decay, release;
   double sustain;

   // filter the signal if need be
   if (instrument->signalFilter != NULL) {
      signalValue = instrument->signalFilter(tone->wavetable, tone->length, i, totalSamples, instrument->data);
   } else {
      signalValue = tone->wavetable[waveformIndex];
   }

   // adjust the volume
   if (instrument->volumeFilter != NULL) {
      signalValue *= instrument->volumeFilter(tone->length, i, totalSamples, instrument->data);
   }

   // calculate ADSR envelope
   envelopeAmp = 1.0;

   sustain = instrument->envelope.sustain;

   // convert AD & R to number of samples
   if (instrument->envelope.units == ENV_MILLIS) {
      attack  = (instrument->envelope.attack/1000.0f) * SAMPLE_RATE;
      decay   = (instrument->envelope.decay/1000.0f) * SAMPLE_RATE;
      release = (instrument->envelope.release/1000.0f) * SAMPLE_RATE;  
   } else if (instrument->envelope.units == ENV_PERCENT) {
      attack  = (instrument->envelope.attack/100.0f) * totalSamples;
      decay   = (instrument->envelope.decay/100.0f) * totalSamples;
      release = (instrument->envelope.release/100.0f) * totalSamples; 
   } else {
      attack  = instrument->envelope.attack;
      decay   = instrument->envelope.decay;
      release = instrument->envelope.release;
   }

   if (i < attack) {
      // attack
      envelopeAmp = (double)i/(double)attack;
   } else if (i < (attack + decay)) {
      // decay
      envelopeAmp = (double)(i - attack)/(double)decay;
      envelopeAmp = lerp(envelopeAmp, 1.0, sustain);
   } else if (i > (totalSamples - release)) {
      // release
      envelopeAmp = (double)(i - (totalSamples - release))/(double)release;
      envelopeAmp = lerp(envelopeAmp, sustain, 0.0);
   } else {
      // sustain
      envelopeAmp = sustain;
   }

   return envelopeAmp * signalValue * AMPLITUDE;
}



