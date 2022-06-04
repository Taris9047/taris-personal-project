/* 
  Quick inverse square root - copied from Quake 3 Arena code 

*/

#include "Q_rsqrt.h"

float Q_rsqrt(float number)
{
  long i;
  float x2, y;
  const float threehalfs = 1.5f;

  x2 = number * 0.5f;
  y = number;

  i = *(long*)&y;
  i = 0x5f3759df - ( i >> 1 );
  y = *(float*)&i;
  y = y * ( threehalfs - ( x2*y*y ) );
  y = y * ( threehalfs - ( x2*y*y ) ); // 2nd iteration of Newton. Can be removed if precision is acceptable.

  return y;
}