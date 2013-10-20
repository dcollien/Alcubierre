#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL/SDL.h>

#include "audio/synth.h"
#include "audio/instruments.h"
#include "audio/notation.h"

#define NUM_TRACKS 8

void playTune(int i) {
   int timing = 0;

   double chordVelocity = 0.3;

   playMajTriad(1,
      createNote("C", 4, 1000, chordVelocity), 0
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   timing += 1000;


   playMajTriad(1,
      createNote("G", 3, 1000, chordVelocity), 1
   );

   playNote(0,
      createNote("D", 4, 250, 0.8)
   );

   playNote(0,
      createNote("E", 4, 250, 0.8)
   );
   
   playNote(0,
      createNote("F", 4, 250, 0.8)
   );

   playNote(0,
      createNote("G", 4, 250, 0.8)
   );

   timing += 1000;

   playMajTriad(1,
      createNote("F", 3, 1000, chordVelocity), 0
   );

   playNote(0,
      createNote("D", 4, 250, 0.8)
   );

   playNote(0,
      createNote("E", 4, 250, 0.8)
   );
   
   playNote(0,
      createNote("F", 4, 250, 0.8)
   );

   playNote(0,
      createNote("G", 4, 250, 0.8)
   );

   timing += 1000;

   playMajTriad(1,
      createNote("G", 3, 250, chordVelocity), 1
   );

   playMajTriad(1,
      createNote("G", 3, 750, chordVelocity), 1
   );

   playNote(0,
      createNote("B", 4, 250, 0.8)
   );

   playNote(0,
      createNote("A", 4, 250, 0.8)
   );
   
   playNote(0,
      createNote("B", 4, 250, 0.8)
   );

   playNote(0,
      createNote("D", 5, 250, 0.8)
   );

   timing += 1000;

   playMinTriad(1,
      createNote("A", 4, 1000, chordVelocity), 0
   );

   if (i % 3 == 0 || i % 3 == 1) {
      playNote(0,
         createNote("E", 5, 250, 0.8)
      );

      playNote(0,
         createNote("D", 5, 250, 0.8)
      );
      
      playNote(0,
         createNote("C", 5, 250, 0.8)
      );

      playNote(0,
         createNote("B", 4, 250, 0.8)
      );
   } else {
      playNote(0,
         createNote("B", 4, 250, 0.8)
      );

      playNote(0,
         createNote("C", 5, 250, 0.8)
      );
      
      playNote(0,
         createNote("D", 5, 250, 0.8)
      );

      playNote(0,
         createNote("E", 5, 250, 0.8)
      );
   }

   timing += 1000;


   playMajTriad(1,
      createNote("G", 3, 1000, chordVelocity), 1
   );

   if (i % 2 == 0) {
      playNote(0,
         createNote("F", 5, 250, 0.8)
      );

      playNote(0,
         createNote("E", 5, 250, 0.8)
      );
      
      playNote(0,
         createNote("D", 5, 250, 0.8)
      );

      playNote(0,
         createNote("C", 5, 250, 0.8)
      );
   } else {
      
      playNote(0,
         createNote("C", 5, 250, 0.8)
      );

      playNote(0,
         createNote("D", 5, 250, 0.8)
      );
      
      playNote(0,
         createNote("E", 5, 250, 0.8)
      );

      playNote(0,
         createNote("C", 5, 250, 0.8)
      );
   }

   timing += 1000;

   playMajTriad(1,
      createNote("F", 3, 1000, chordVelocity), 0
   );

   playNote(0,
      createNote("A", 4, 250, 0.8)
   );

   playNote(0,
      createNote("G", 5, 250, 0.8)
   );
   
   playNote(0,
      createNote("A", 5, 250, 0.8)
   );

   playNote(0,
      createNote("C", 5, 250, 0.8)
   );

   timing += 1000;

   playMajTriad(1,
      createNote("G", 3, 1000, chordVelocity), 2
   );

   if (i % 2 == 0) {
      playNote(0,
         createNote("E", 5, 500, 0.8)
      );

      playNote(0,
         createNote("D", 5, 500, 0.8)
      );
   } else {
      playNote(0,
         createNote("D", 5, 500, 0.8)
      );

      playNote(0,
         createNote("E", 5, 500, 0.8)
      );
   }

   timing += 1000;

   SDL_Delay(timing);
}

int main(int argc, char *argv[]) {
   int timing = 0;
   int i;

   synth_init(NUM_TRACKS);

   envelope_t hold = envelope_hold(0.8);
   envelope_t soft = envelope_soft();
   envelope_t stuc = envelope_stuccato();

   instrument_t guitar  = instrument_pluckedString(hold);
   instrument_t organ   = instrument_leslieOrgan(soft);
   instrument_t strings = instrument_vibrato(soft);
   instrument_t sine    = instrument_sine(stuc);
   instrument_t square  = instrument_square(soft);
   instrument_t fuzzy   = instrument_sciFi(hold);

   synth_setTrackVolume(0, 1.0);
   synth_setTrackVolume(1, 0.8);
   synth_setTrackVolume(2, 0.8);
   synth_setTrackVolume(3, 0.8);
   synth_setTrackVolume(4, 0.8);
   synth_setTrackVolume(5, 0.8);


   for (i = 0; i != 5; ++i) {
      if (i == 0) {
         synth_setTrackInstrument(0, guitar);
         synth_setTrackInstrument(1, organ);
         synth_setTrackInstrument(2, organ);
         synth_setTrackInstrument(3, organ);
         synth_setTrackInstrument(4, fuzzy);
      } else if (i == 1) {
         synth_setTrackInstrument(0, fuzzy);
         synth_setTrackInstrument(1, strings);
         synth_setTrackInstrument(2, strings);
         synth_setTrackInstrument(3, strings);
      } else if (i == 2) {
         synth_setTrackInstrument(0, sine);
         synth_setTrackInstrument(1, square);
         synth_setTrackInstrument(2, square);
         synth_setTrackInstrument(3, square);   
      } else if (i == 3) {
         synth_setTrackInstrument(0, sine);
         synth_setTrackInstrument(1, guitar);
         synth_setTrackInstrument(2, guitar);
         synth_setTrackInstrument(3, guitar);
      } else {
         synth_setTrackInstrument(0, sine);
         synth_setTrackInstrument(1, fuzzy);
         synth_setTrackInstrument(2, fuzzy);
         synth_setTrackInstrument(3, fuzzy);   
      }
      
      playTune(i);
   }
   
   playMajTriad(1,
      createNote("C", 4, 2000, 0.6), 0
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   timing += 1000;

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   playNote(0,
      createNote("C", 4, 250, 0.8)
   );

   timing += 1000;

   playNote(0,
      createNote("C", 3, 1000, 0.8)
   );

   timing += 1000;

   SDL_Delay(timing);

   synth_dispose();

   return EXIT_SUCCESS;
}