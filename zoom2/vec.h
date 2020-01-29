#ifndef VEC_H
#define VEC_H

/*
  vec3
  3 dimensional vector of floats
*/
typedef struct vec3 {
  float x, y, z;
} vec3;

/*
  vec3_magnitude
  returns magnitude of a vec3
*/
float vec3_magnitude(vec3 vec);

/*
  vec3_normalize
  returns a normalized vector from vec
*/
vec3 vec3_normalize(vec3 vec);

/*
  vec3_mulf
  multiplies a vec3 by a scalar float
*/
vec3 vec3_mulf(vec3 vec, float f);

/*
  vec4
  4 dimensional vector of floats
*/
typedef struct vec4 {
  float x, y, z, w;
} vec4;

/*
  vec4_magnitude
  returns magnitude of a vec4
*/
float vec4_magnitude(vec4 vec);

/*
  vec4_normalize
  returns a normalized vector from vec
*/
vec4 vec4_normalize(vec4 vec);

#endif