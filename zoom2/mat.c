#include "mat.h"

#include <math.h>

extern const mat4 mat4_zero = {
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};

mat4 mat4_identity() {
  mat4 identity = mat4_zero;
  identity.values[0][0] = 1.0f;
  identity.values[1][1] = 1.0f;
  identity.values[2][2] = 1.0f;
  identity.values[3][3] = 1.0f;

  return identity;
}

mat4 mat4_translate_vec3(mat4 mat, vec3 vec) {
  mat4 result = mat;

  result.values[0][3] *= vec.x;
  result.values[1][3] *= vec.y;
  result.values[2][3] *= vec.z;
  result.values[3][3] *= 1.0f;

  return result;
}

mat4 mat4_scale_vec3(mat4 mat, vec3 vec) {
  mat4 result = mat;

  result.values[0][0] *= vec.x;
  result.values[1][1] *= vec.y;
  result.values[2][2] *= vec.x;

  return result;
}

mat4 mat4_rotate_vec3(mat4 mat, float angle, vec3 vec) {
  float c = cosf(angle);
  float s = sinf(angle);

  vec3 n_axis = vec3_normalize(vec);
  vec3 temp = vec3_mulf(n_axis, (1.0f - c));

  vec3 x = {
    c + temp.x * n_axis.x,
    temp.x * n_axis.y + s * n_axis.z,
    temp.x * n_axis.z - s * n_axis.y
  };
  vec3 y = {
    temp.y * n_axis.x - s * n_axis.z,
    c + temp.y * n_axis.y,
    temp.y * n_axis.z + s * n_axis.x
  };
  vec3 z = {
    temp.z * n_axis.x + s * n_axis.y,
    temp.z * n_axis.y - s * n_axis.x,
    c + temp.z * n_axis.z
  };

  vec3 xa = vec3_mulf((vec3) { mat.values[0][0], mat.values[0][1], mat.values[0][2] }, x.x);
  vec3 xb = vec3_mulf((vec3) { mat.values[1][0], mat.values[1][1], mat.values[1][2] }, x.y);
  vec3 xc = vec3_mulf((vec3) { mat.values[2][0], mat.values[2][1], mat.values[2][2] }, x.z);
  x.x = xa.x + xb.x + xc.x;
  x.y = xa.y + xb.y + xc.y;
  x.z = xa.z + xb.z + xc.z;
  vec3 ya = vec3_mulf((vec3) { mat.values[0][0], mat.values[0][1], mat.values[0][2] }, y.x);
  vec3 yb = vec3_mulf((vec3) { mat.values[1][0], mat.values[1][1], mat.values[1][2] }, y.y);
  vec3 yc = vec3_mulf((vec3) { mat.values[2][0], mat.values[2][1], mat.values[2][2] }, y.z);
  y.x = ya.x + yb.x + yc.x;
  y.y = ya.y + yb.y + yc.y;
  y.z = ya.z + yb.z + yc.z;
  vec3 za = vec3_mulf((vec3) { mat.values[0][0], mat.values[0][1], mat.values[0][2] }, z.x);
  vec3 zb = vec3_mulf((vec3) { mat.values[1][0], mat.values[1][1], mat.values[1][2] }, z.y);
  vec3 zc = vec3_mulf((vec3) { mat.values[2][0], mat.values[2][1], mat.values[2][2] }, z.z);
  z.x = za.x + zb.x + zc.x;
  z.y = za.y + zb.y + zc.y;
  z.z = za.z + zb.z + zc.z;

  mat4 result = {
    x.x, x.y, x.z, 0.0f,
    y.x, y.y, y.z, 0.0f,
    z.x, z.y, z.z, 0.0f,
    mat.values[3][0], mat.values[3][1], mat.values[3][2], mat.values[3][3]
  };

  return result;
}
