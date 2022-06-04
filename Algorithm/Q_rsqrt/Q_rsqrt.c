/* 
  Quick inverse square root - copied from Quake 3 Arena code 

*/

#include <stdint.h>

#include "Q_rsqrt.h"

float Q_rsqrt(float number)
{
  union {
    float f;
    uint32_t i;
  } conv = { .f = number };

  conv.i = 0x5f3759df - (conv.i >> 1);
  conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);

  return conv.f;
}

float Q_rsqrt2(float number)
{
  union {
    float f;
    uint32_t i;
  } conv = { .f = number };

  conv.i = 0x5f3759df - (conv.i >> 1);
  conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
  conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);

  return conv.f;
}