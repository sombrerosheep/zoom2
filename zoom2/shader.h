#ifndef SHADER_H
#define SHADER_H

#include "mat.h"

typedef unsigned int shader;

shader shader_create(const char* vert_data, const char* frag_data);
void shader_use(shader s);
void shader_destroy(shader s);

void shader_set_mat4(shader s, const char* name, mat4 mat);

#endif