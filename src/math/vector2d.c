#include <math.h>

#include "math/vector2d.h"
#include "math/mathBasics.h"


vector2d_t v_(scalar_t x, scalar_t y) {
	vector2d_t v;
	v.x = x;
	v.y = y;

	return v;
}

scalar_t v_len2(vector2d_t v) {
	return sq(v.x) + sq(v.y);
}

scalar_t v_len(vector2d_t v) {
	return sqrt(v_len2(v));
}

double v_angle(vector2d_t v) {
	return atan2(v.y, v.x);
}



bool v_eq(vector2d_t v1, vector2d_t v2) {
	return (v1.x == v2.x && v1.y == v2.y);
}


vector2d_t v_rotate(vector2d_t v1, vector2d_t v2) {
	return v_(v1.x * v2.x - v1.y * v2.y, v1.x * v2.y + v1.y * v2.x);
}

vector2d_t v_add(vector2d_t v1, vector2d_t v2) {
	return v_(v1.x + v2.x, v1.y + v2.y);
}

vector2d_t v_sub(vector2d_t v1, vector2d_t v2) {
	return v_(v1.x - v2.x, v1.y - v2.y);
}


vector2d_t v_mul(vector2d_t v1, scalar_t s) {
	return v_(v1.x * s, v1.y * s);
}

vector2d_t v_div(vector2d_t v1, scalar_t s) {
	return v_(v1.x / s, v1.y / s);
}


vector2d_t v_lerp(scalar_t t, vector2d_t v1, vector2d_t v2) {
	return v_(lerp(t, v1.x, v2.x), lerp(t, v1.y, v2.y));
}

vector2d_t v_unit(vector2d_t v1) {
  int len = v_len(v1);
  return v_(v1.x/len, v1.y/len);
}

vector2d_t v_scale(vector2d_t v1, scalar_t s) {
  return v_mul(v_unit(v1), s);
}

vector2d_t v_rotateAngle(vector2d_t v1, double a) {
	return v_rotate(v1, v_forAngle(a));
}


vector2d_t v_forAngle(double a) {
	return v_(cos(a), sin(a));
}

vector2d_t v_forAngleLength(double a, scalar_t s) {
	return v_(cos(a) * s, sin(a) * s);
}


scalar_t v_dot(vector2d_t v1, vector2d_t v2) {
	return (v1.x * v2.x + v1.y * v2.y);
}

scalar_t v_dist(vector2d_t v1, vector2d_t v2) {
	return v_len(v_sub(v1, v2));
}

scalar_t v_dist2(vector2d_t v1, vector2d_t v2) {
	return v_len2(v_sub(v1, v2));
}


