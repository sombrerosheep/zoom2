#include "vec.h"

#include <math.h>

float vec3_magnitude(vec3 vec) {
  float squares = vec.x * vec.x +
    vec.y * vec.y +
    vec.z * vec.z;
  float result = sqrtf(squares);

  return result;
}

vec3 vec3_normalize(vec3 vec) {
  vec3 result = vec;
  float mag = vec3_magnitude(vec);

  result.x /= mag;
  result.y /= mag;
  result.z /= mag;

  return result;
}

vec3 vec3_mulf(vec3 vec, float f) {
  vec3 result = vec;

  result.x *= f;
  result.y *= f;
  result.z *= f;

  return result;
}

float vec4_magnitude(vec4 vec) {
  // sqrt of sum of squares
  float squares = vec.x * vec.x +
    vec.y * vec.y +
    vec.z * vec.z +
    vec.w * vec.w;
  float result = sqrtf(squares);

  return result;
}

vec4 vec4_normalize(vec4 vec) {
  vec4 result = vec;
  float mag = vec4_magnitude(vec);

  result.x /= mag;
  result.y /= mag;
  result.z /= mag;
  result.w /= mag;

  return result;
}