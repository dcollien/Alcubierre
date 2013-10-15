#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <stdbool.h>

typedef double scalar_t;

typedef struct {
   scalar_t x;
   scalar_t y;
} vector2d_t;


vector2d_t v_(scalar_t x, scalar_t y);

scalar_t v_len(vector2d_t v);
scalar_t v_len2(vector2d_t v);
double v_angle(vector2d_t v);

bool v_eq(vector2d_t v1, vector2d_t v2);

vector2d_t v_rotate(vector2d_t v1, vector2d_t v2);

vector2d_t v_add(vector2d_t v1, vector2d_t v2);
vector2d_t v_sub(vector2d_t v1, vector2d_t v2);

vector2d_t v_mul(vector2d_t v1, scalar_t s);
vector2d_t v_div(vector2d_t v1, scalar_t s);

vector2d_t v_lerp(scalar_t t, vector2d_t v1, vector2d_t v2);
vector2d_t v_unit(vector2d_t v1);
vector2d_t v_scale(vector2d_t v1, scalar_t s);
vector2d_t v_rotateAngle(vector2d_t v1, double a);

vector2d_t v_forAngle(double a);
vector2d_t v_forAngleLength(double a, scalar_t s);

scalar_t v_dot(vector2d_t v1, vector2d_t v2);
scalar_t v_dist(vector2d_t v1, vector2d_t v2);
scalar_t v_dist2(vector2d_t v1, vector2d_t v2);

#endif
