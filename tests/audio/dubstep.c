// clang -I../../include  -o ../../bin/dubstep dubstep.c `sdl-config --cflags --libs`  ../../lib/libAlcubierre.a -lm

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL/SDL.h>

#include "audio/synth.h"
#include "audio/instruments.h"
#include "audio/notation.h"

#define NUM_TRACKS 8

int main(int argc, char *argv[]) {
   int timing = 0;
   int i;

   synth_init(NUM_TRACKS);

   envelope_t hold = envelope_hold(0.8);
   envelope_t soft = envelope_soft();
   envelope_t stuc = envelope_stuccato();

   instrument_t wob = instrument_wobble(hold);

   synth_setTrackVolume(0, 1.0);
   synth_setTrackVolume(1, 0.8);
   synth_setTrackVolume(2, 0.8);
   synth_setTrackVolume(3, 0.8);
   synth_setTrackVolume(4, 0.8);
   synth_setTrackVolume(5, 0.8);
   
   synth_setTrackInstrument(0, wob);

   timing += 1000;

   playNote(0,
      createNote("C", 2, 250, 0.8)
   );

   playNote(0,
      createNote("C", 2, 250, 0.8)
   );

   playNote(0,
      createNote("E", 2, 250, 0.8)
   );

   playNote(0,
      createNote("E", 2, 250, 0.8)
   );

   timing += 1000;

   playNote(0,
      createNote("C", 2, 250, 0.8)
   );

   playNote(0,
      createNote("D", 2, 250, 0.8)
   );

   playNote(0,
      createNote("E", 2, 250, 0.8)
   );

   playNote(0,
      createNote("G", 2, 250, 0.8)
   );

   timing += 1000;

   playNote(0,
      createNote("G", 1, 250, 0.8)
   );

   playNote(0,
      createNote("G", 1, 250, 0.8)
   );

   playNote(0,
      createNote("G", 1, 250, 0.8)
   );

   playNote(0,
      createNote("G", 1, 250, 0.8)
   );

   timing += 1000;

   playNote(0,
      createNote("C", 2, 1000, 0.8)
   );

   timing += 1000;

   playNote(0,
      createNote("C", 1, 1000, 0.8)
   );

   timing += 1000;

   playNote(0,
      createNote("C", 0, 1000, 0.8)
   );

   timing += 1000;

   SDL_Delay(timing);

   synth_dispose();

   return EXIT_SUCCESS;
}