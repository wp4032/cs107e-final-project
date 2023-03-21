#ifndef MATH_H
#define MATH_H

/*
 * Math Library
 *
 * Citation: from Sun Microsystems
 * https://elixir.bootlin.com/musl/v0.9.9/source/src/math
 */

void math_init(void);

float _cos[360];
float cos(int x);

float pwr(float a, signed int b);



#endif
