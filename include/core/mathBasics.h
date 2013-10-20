#ifndef MATHBASICS_H
#define MATHBASICS_H

#include <math.h>
#include <stdlib.h>

#define TAU (M_PI * 2)

double randDouble(double amt);

double vary(double amt);

int randInt(int amt);

double lerp(double t, double from, double to);

double sq(double x);

double cube(double x);

#endif
