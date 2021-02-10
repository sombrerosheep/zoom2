#ifndef MESH_H
#define MESH_H

#include "vec.h"
#include "shader.h"

typedef struct vertex_pc {
  vec3 position;
  vec3 color;
} vertex_pc;

typedef struct vertex_pnt {
  vec3 position;
  vec3 normal;
  vec2 tex_coord;
} vertex_pnt;

typedef struct mesh_vpc {
  vertex_pc *vertices;
  unsigned int num_vertices;
  unsigned int *indices;
  unsigned int num_indices;
  unsigned int   vao;
  unsigned int   vbo;
  unsigned int   ebo;
} mesh_vpc;

int mesh_vpc_init(mesh_vpc *mesh, vertex_pc *vertices, unsigned int num_vertices, unsigned int *indices, unsigned int num_indices);
void mesh_vpc_destroy(mesh_vpc *mesh);

void mesh_vpc_draw(mesh_vpc *mesh, shader shader);

#endif
