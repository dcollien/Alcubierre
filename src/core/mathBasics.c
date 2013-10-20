#include "core/mathBasics.h"

double randDouble(double amt) {
   return amt * (rand()/(double)RAND_MAX);
}

double vary(double amt) {
   return 2.0 * amt * (randDouble(1.0) - 0.5);
}

int randInt(int amt) {
   return rand() % amt;
}

double lerp(double t, double from, double to) {
   return t * to + (1.0 - t) * from;
}

double sq(double x) {
   return x * x;
}

double cube(double x) {
   return x * x * x;
}
