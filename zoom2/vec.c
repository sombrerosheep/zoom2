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

vec3 vec3_sub(vec3 a, vec3 b) {
  vec3 result;

  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;

  return result;
}

vec3 vec3_mul(vec3 a, vec3 b) {
  vec3 vec;

  vec.x = a.x * b.x;
  vec.y = a.y * b.y;
  vec.z = a.z * b.z;

  return vec;
}

vec3 vec3_mulf(vec3 vec, float f) {
  vec3 result = vec;

  result.x *= f;
  result.y *= f;
  result.z *= f;

  return result;
}

vec3 vec3_cross(vec3 a, vec3 b) {
  vec3 result;

  result.x = a.y * b.z - a.z * b.y;
  result.y = a.z * b.x - a.x * b.z;
  result.z = a.x * b.y - a.y * b.x;

  return result;
}

float vec3_dot(vec3 a, vec3 b) {
  vec3 product = vec3_mul(a, b);
  float psum = product.x + product.y + product.z;

  return psum;
}

float vec4_magnitude(vec4 vec) {
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
