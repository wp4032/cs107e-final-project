#ifndef MATH_H
#define MATH_H

/*
 * A very simple math library for the
 * DREAM'S DON'T DIE Moonwalker Brain Machine Interface
 *
 * Author: William Pan <williampan@cs.stanford.edu>
 * Date: Mar 28, 2023
 */

void math_init(void);

const float _pi;
const float _pi_2;
const float _pi_4;

typedef struct {
  float x;
  float y;
  float z;
} vector_t;

float __cos(int x);
float __sin(int x);
float __tan(int x);
float __acos(int x);
float __asin(int x);
float __atan(int x);

float cos(float x);
float sin(float x);
float tan(float x);
float acos(float x);
float asin(float x);
float atan(float x);
float atan2(float y, float x);

float pwr(float a, signed int b);
float pwr10(signed int b);
float sqrt(float a);

int floor(float x);
int round(float x);
float decimals(float x);
float decimals_flipped(float x);
void swap(int *a, int *b);
unsigned int abs(signed int a);

float v_dot(vector_t a, vector_t b);
vector_t v_add(vector_t a, vector_t b);
vector_t v_sub(vector_t a, vector_t b);
vector_t v_multiply(vector_t a, float b);
vector_t v_divide(vector_t a, float b);

#endif
