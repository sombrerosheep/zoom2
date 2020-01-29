#ifndef MAT_H
#define MAT_H

#include "vec.h"

/*
  mat4
  4x4 matrix of floats
  in row major order
*/
typedef struct mat4 {
  float values[4][4];
} mat4;

extern const mat4 mat4_zero;

/*
  mat4_identity
  returns 4x4 identity matrix
*/
mat4 mat4_identity();

/*
  mat4_translate_vec3
  translates a mat4 by a vec3
  1.0f is inserted for the
  missing 'w' from the vec3
*/
mat4 mat4_translate_vec3(mat4 mat, vec3 vec);

/*
  mat4_scale_vec3
  scales a mat4 by a vec3
*/
mat4 mat4_scale_vec3(mat4 mat, vec3 vec);

/*
  mat4_rotate_vec3
  rotates a mat4 by a vec3
*/
mat4 mat4_rotate_vec3(mat4 mat, float angle, vec3 vec);

#endif