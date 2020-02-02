#ifndef MESH_H
#define MESH_H

#include "vec.h"
#include "shader.h"

typedef struct vertex {
  vec3 position;
  vec3 color;
} vertex;

typedef struct mesh {
  vertex *vertices;
  unsigned int num_vertices;
  unsigned int *indices;
  unsigned int num_indices;
  unsigned int   vao;
  unsigned int   vbo;
  unsigned int   ebo;
} mesh;

int mesh_init(mesh *mesh, vertex *vertices, unsigned int num_vertices, unsigned int *indices, unsigned int num_indices);
void mesh_destroy(mesh *mesh);

void mesh_draw(mesh *mesh, shader shader);

#endif
