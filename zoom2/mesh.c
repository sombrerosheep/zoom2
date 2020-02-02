#include "mesh.h"

#include "ogl.h"

int mesh_init(mesh *mesh, vertex *vertices, unsigned int num_vertices, unsigned int *indices, unsigned int num_indices) {
  if ((mesh->vertices = malloc(sizeof(vertex) * num_vertices)) == 0) {
    return -1;
  }

  if ((mesh->indices = malloc(sizeof(unsigned int) * num_indices)) == 0) {
    free(mesh->vertices);
    return -1;
  }
  
  mesh->num_vertices = num_vertices;
  memcpy(mesh->vertices, vertices, sizeof(vertex) * num_vertices);
  
  mesh->num_indices = num_indices;
  memcpy(mesh->indices, indices, sizeof(unsigned int) * num_indices);

  glGenVertexArrays(1, &mesh->vao);
  glBindVertexArray(mesh->vao);

  glGenBuffers(1, &mesh->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * num_vertices, mesh->vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &mesh->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, mesh->indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(sizeof(GL_FLOAT) * 3));
  glEnableVertexAttribArray(1);

  return 0;
}

void mesh_destroy(mesh *mesh) {
  glDeleteBuffers(1, &mesh->vbo);
  glDeleteBuffers(1, &mesh->ebo);
  glDeleteVertexArrays(1, &mesh->vao);

  mesh->vbo = 0;
  mesh->ebo = 0;
  mesh->vao = 0;

  free(mesh->vertices);
  mesh->num_vertices = 0;
  free(mesh->indices);
  mesh->num_indices = 0;
}

void mesh_draw(const mesh *mesh, shader shader) {
  shader_use(shader);
  glBindVertexArray(mesh->vao);
  glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}
