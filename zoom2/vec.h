#ifndef VEC_H
#define VEC_H

/*
  vec2
  2 dimensional vector of floats
*/
typedef struct vec2 {
  float x, y;
} vec2;

/*
  vec3i
  3 dimensional vector of signed ints
*/
typedef struct vec3i {
  union {
    struct { int x, y, z; };
    struct { int v, t, n; };
  };
} vec2i;

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
  vec3_sub
  subtracts 2 vec3's returning the difference
*/
vec3 vec3_sub(vec3 a, vec3 b);

/*
  vec3_mul
  multiplies to vec3
*/
vec3 vec3_mul(vec3 a, vec3 b);

/*
  vec3_mulf
  multiplies a vec3 by a scalar float
*/
vec3 vec3_mulf(vec3 vec, float f);

/*
  vec3_cross
  returns the vector cross product
*/
vec3 vec3_cross(vec3 a, vec3 b);

/*
  vec3_dot
  returns dot product of two vec3
*/
float vec3_dot(vec3 a, vec3 b);

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
