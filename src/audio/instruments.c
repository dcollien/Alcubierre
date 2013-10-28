#include "core/mathBasics.h"
#include "audio/instruments.h"

typedef double (*waveform_t)(double x);

typedef struct {
   double cutoffLFO;
   double cutoffIntensity;
   double ampLFO;
   double ampIntensity;
} genericInstrumentData_t;

static double sine_wave(double x) {
   return sin(x);
}

// built-in instruments
static double square_wave(double x) {
   double sin_x = sin(x);

   if (sin_x > 0) {
      return 1.0;
   } else {
      return -1.0;
   }
}

static double triangle_wave(double x) {
   return (4.0/TAU) * asin(sin(x));
}

static double saw_wave(double x) {
   double t = x/TAU;
   return 2.0 * (t - floor(t)) - 1.0;
   //return (-4.0 / TAU) * atan(1.0/tan(x/2.0));
}

static double organ_wave(double x) {
   return (sin(x) + sin(2*x) + 3*sin(3*x) + sin(4*x) + sin(5*x) + sin(6*x))/8;
}

static double wideband_wave(double x) {
   return sin(x*16);
}

//static double noisy_sine (double x) {
   // Fordy: TODO
//   return 0;
//}


static void generateWaveform(double *wavetable, int length, double velocity, void *data) {
   int i;

   waveform_t waveform = (waveform_t)data;

   double frequency = (double)SAMPLE_RATE / (double)length;
   double x = TAU * frequency / (double)SAMPLE_RATE;
   
   for (i = 0; i != length; ++i) {
      wavetable[i] = velocity * waveform(i * x);
   }
}
/*
static void generateWhiteNoise(double *wavetable, int length, double velocity, void *data) {
   int i;
   
   for (i = 0; i != length; ++i) {
      wavetable[i] = velocity * vary(1.0);
   }
}
*/
static void generateExcitation(double *wavetable, int length, double velocity, void *data) {
   int i;
   
   double frequency = (double)SAMPLE_RATE / (double)length;
   double x = TAU * frequency / (double)SAMPLE_RATE;

   for (i = 0; i != length; ++i) {
      if (i % 2 == 0) {
         wavetable[i] = randDouble(1.5) - 0.5;  
      } else {
         wavetable[i] = -randDouble(1.5) + 0.5; 
      }

      wavetable[i] *= triangle_wave(i * x);
   }
}


static double vibrationFilter(double *wavetable, int length, int samplesPlayed, int totalSamples, void *data) {
   int i;
   double smoothing = 0.5;

   if (samplesPlayed % length == 0) {
      // each vibration, low-pass and flip
      for (i = 1; i != length; ++i) {
         wavetable[i] = smoothing * wavetable[length-i] + (1.0 - smoothing) * wavetable[length-i-1];
      }
   }

   return wavetable[samplesPlayed % length];
}

static double sciFiFilter(double *wavetable, int length, int samplesPlayed, int totalSamples, void *data) {
   int i;
   double smoothing = 0.8;

   if (samplesPlayed % length == 0) {
      // each vibration, low-pass and flip
      for (i = 1; i != length; ++i) {
         wavetable[i] = smoothing * wavetable[length-i] + (1.0 - smoothing) * wavetable[length-i-1];
         wavetable[i] = wavetable[i] * 0.99 + randDouble(0.01);
      }

      for (i = 0; i != length; ++i) {
         wavetable[i] = wavetable[i] * 0.99 + randDouble(0.01);
      }
   }

   return wavetable[samplesPlayed % length];
}

static double flangeFilter(double *wavetable, int length, int samplesPlayed, int totalSamples, void *data) {

   int flange = 5;
 
   int i;
   if (samplesPlayed % length == 0 || length - flange < 1) {
      for (i = 0; i < length - flange; ++i) {
         wavetable[i] = (wavetable[i] + wavetable[i + flange]) / 1.7;
      }
   }

   return wavetable[samplesPlayed % length];
}

static double leslieFilter(int length, int samplesPlayed, int totalSamples, void *data) {
   double frequency = 40;

   return 0.8 + 0.2 * sin(frequency * samplesPlayed/(double)SAMPLE_RATE);
}

static double wobbleFilter(double *wavetable, int length, int samplesPlayed, int totalSamples, void *data) {
   double frequency = 2;
   double cutoff = 0.5 + 0.5*sin(frequency * TAU * samplesPlayed/(double)SAMPLE_RATE);

   double wob = wavetable[0];
   int i;

   for (i = 1; i < (samplesPlayed % length); ++i) {
      wob = cutoff * wob + (1.0 - cutoff) * wavetable[i];
   }

   return wob;
}

static double tremoloFilter(int length, int samplesPlayed, int totalSamples, void *data) {
   double progress = (samplesPlayed/(double)totalSamples);
   double frequency = 24 + 10 * progress;

   return progress * sin(frequency * samplesPlayed/(double)SAMPLE_RATE) + (1.0 - progress);
}

/*
instrument_t instrument_generic(
   envelope_t envelope,
   waveformPreset_t waveform,
   double cutoffLFO,
   double cutoffIntensity,
   double ampLFO,
   double ampIntensity
) {
   static genericInstrumentData_t instrumentData;

   instrumentData.
}
*/

instrument_t instrument_sine(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = NULL;
   instrument.volumeFilter = NULL;
   instrument.envelope = envelope;
   instrument.data = (void *)sine_wave;

   return instrument;
}

instrument_t instrument_square(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = NULL;
   instrument.volumeFilter = NULL;
   instrument.envelope = envelope;
   instrument.data = (void *)square_wave;

   return instrument;
}

instrument_t instrument_triangle(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = NULL;
   instrument.volumeFilter = NULL;
   instrument.envelope = envelope;
   instrument.data = (void *)triangle_wave;

   return instrument;
}

instrument_t instrument_saw(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = NULL;
   instrument.volumeFilter = NULL;
   instrument.envelope = envelope;
   instrument.data = (void *)saw_wave;

   return instrument;
}

instrument_t instrument_organ(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = NULL;
   instrument.volumeFilter = NULL;
   instrument.envelope = envelope;
   instrument.data = (void *)organ_wave;

   return instrument;
}

instrument_t instrument_leslieOrgan(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = NULL;
   instrument.volumeFilter = leslieFilter;
   instrument.envelope = envelope;
   instrument.data = (void *)organ_wave;

   return instrument;
}

instrument_t instrument_pluckedString(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateExcitation;
   instrument.signalFilter = vibrationFilter;
   instrument.volumeFilter = NULL;
   instrument.envelope = envelope;
   instrument.data = NULL;

   return instrument;
}

instrument_t instrument_sciFi(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = sciFiFilter;
   instrument.volumeFilter = NULL;
   instrument.envelope = envelope;
   instrument.data = (void *)wideband_wave;

   return instrument;
}

instrument_t instrument_vibrato(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = NULL;
   instrument.volumeFilter = tremoloFilter;
   instrument.envelope = envelope;
   instrument.data = (void *)triangle_wave;

   return instrument; 
}

instrument_t instrument_noisySin(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = flangeFilter;
   instrument.volumeFilter = NULL;
   instrument.envelope = envelope;
   instrument.data = (void *)triangle_wave;

   return instrument;
}

instrument_t instrument_wobble(envelope_t envelope) {
   instrument_t instrument;

   instrument.signalGenerator = generateWaveform;
   instrument.signalFilter = wobbleFilter;
   instrument.volumeFilter = NULL;
   instrument.envelope = envelope;
   instrument.data = (void *)square_wave;

   return instrument; 
}



envelope_t envelope_stuccato(void) {
   envelope_t env;

   env.units = ENV_MILLIS;

   env.attack  = 0;
   env.decay   = 180;
   env.sustain = 0.1;
   env.release = 200;

   return env;
}

envelope_t envelope_hold(double sustain) {
   envelope_t env;

   env.units = ENV_MILLIS;

   env.attack  = 0;
   env.decay   = 150;
   env.sustain = sustain;
   env.release = 150;

   return env;
}

envelope_t envelope_fadeOut(void) {
   envelope_t env;

   env.units = ENV_PERCENT;

   env.attack  = 0;
   env.decay   = 50;
   env.sustain = 0.5;
   env.release = 50;

   return env;
}

envelope_t envelope_fadeIn(void) {
   envelope_t env;

   env.units = ENV_PERCENT;

   env.attack  = 150;
   env.decay   = 0;
   env.sustain = 0.5;
   env.release = 0;

   return env;
}

envelope_t envelope_soft(void) {
   envelope_t env;

   env.units = ENV_MILLIS;

   env.attack  = 100;
   env.decay   = 150;
   env.sustain = 0.5;
   env.release = 500;

   return env;
}
