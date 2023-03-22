#ifndef MATH_H
#define MATH_H

/*
 * Math Library
 *
 * Citation: from Sun Microsystems
 * https://elixir.bootlin.com/musl/v0.9.9/source/src/math
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

// static float _cos[360];
// static float _sin[360];
// static float _tan[360];
// static float _acos[201];
// static float _asin[201];
// static float _atan[2001];

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

// TEST FUNCTIONS:
// printf("%d\n", (int) (acos(-1.0) * 1000));
// printf("%d\n", (int) (asin(0.0) * 1000));
// printf("%d\n", (int) (acos(1.0) * 1000));
// printf("%d\n", (int) (asin(-1.00) * 1000));
// printf("%d\n", (int) (asin(0.0) * 1000));
// printf("%d\n", (int) (asin(1.00) * 1000));
// printf("%d\n", (int) (atan(-10.200) * 1000));
// printf("%d\n", (int) (atan(-10.00) * 1000));
// printf("%d\n", (int) (atan(0.0) * 1000));
// printf("%d\n", (int) (atan(10.00) * 1000));
// printf("%d\n", (int) (atan(10.200) * 1000));
// printf("%d\n", (int) (sqrt(2.14) * tan(270.2)) * 1000);

#endif
