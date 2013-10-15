#ifndef MATHBASICS_H
#define MATHBASICS_H

#include <math.h>
#include <stdlib.h>

#define TAU (PI * 2)

inline double randDouble(double amt) {
   return amt * (rand()/(double)RAND_MAX);
}

inline double vary(double amt) {
   return 2.0 * amt * (randDouble(1.0) - 0.5);
}

inline int randInt(int amt) {
   return rand() % amt;
}

inline double lerp(double t, double from, double to) {
   return t * to + (1.0 - t) * from;
}

inline double sq(double x) {
   return x * x;
}

inline double cube(double x) {
   return x * x * x;
}

#endif
