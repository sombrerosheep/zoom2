#include "shader.h"
#include "ogl.h"

static void shader_compilation_status(unsigned int shader) {
  int success;
  char infoLog[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    MessageBox(NULL, infoLog, "ERROR::SHADER::COMPILATION_FAILED", MB_OK);
  }
}

static void shader_program_link_status(unsigned int program) {
  int success;
  char infoLog[512];

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    MessageBox(NULL, infoLog, "ERROR::SHADER::PROGRAM::LINK_FAILED", MB_OK);
  }
}

shader shader_create(const char* vert_data, const char* frag_data) {
  shader shader_program = glCreateProgram();
  unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
  unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vert, 1, &vert_data, NULL);
  glCompileShader(vert);
  shader_compilation_status(vert);

  glShaderSource(frag, 1, &frag_data, NULL);
  glCompileShader(frag);
  shader_compilation_status(frag);

  glAttachShader(shader_program, vert);
  glAttachShader(shader_program, frag);
  glLinkProgram(shader_program);
  shader_program_link_status(shader_program);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return shader_program;
 }

void shader_use(shader s) {
  glUseProgram(s);
}

void shader_destroy(shader s) {
  glDeleteProgram(s);
}

void shader_set_mat4(shader s, const char* name, mat4 mat) {
  glUniformMatrix4fv(glGetUniformLocation(s, name), 1, GL_FALSE, &mat.values);
}
