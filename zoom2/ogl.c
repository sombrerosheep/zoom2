#include "ogl.h"

#define BIND_GL_FUNC(x, t) x = (t)wglGetProcAddress(#x); \
                           if (x == NULL) { MessageBox(NULL, #x, "error loading gl function", MB_OK); return -1; }

int init_gl_func() {
  BIND_GL_FUNC(wglCreateContextAttribsARB, PFNWGLCREATECONTEXTATTRIBSARBPROC);
  BIND_GL_FUNC(wglChoosePixelFormatARB, PFNWGLCHOOSEPIXELFORMATARBPROC);

  BIND_GL_FUNC(glCreateShader, PFNGLCREATESHADERPROC);
  BIND_GL_FUNC(glShaderSource, PFNGLSHADERSOURCEPROC);
  BIND_GL_FUNC(glCompileShader, PFNGLCOMPILESHADERPROC);
  BIND_GL_FUNC(glAttachShader, PFNGLATTACHSHADERPROC);
  BIND_GL_FUNC(glDeleteShader, PFNGLDELETESHADERPROC);
  BIND_GL_FUNC(glGetShaderiv, PFNGLGETSHADERIVPROC);
  BIND_GL_FUNC(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC);

  BIND_GL_FUNC(glCreateProgram, PFNGLCREATEPROGRAMPROC);
  BIND_GL_FUNC(glDeleteProgram, PFNGLDELETEPROGRAMPROC);
  BIND_GL_FUNC(glLinkProgram, PFNGLLINKPROGRAMPROC);
  BIND_GL_FUNC(glUseProgram, PFNGLUSEPROGRAMPROC);
  BIND_GL_FUNC(glGetProgramiv, PFNGLGETPROGRAMIVPROC);
  BIND_GL_FUNC(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC);

  BIND_GL_FUNC(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC);
  BIND_GL_FUNC(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC);
  BIND_GL_FUNC(glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC);

  BIND_GL_FUNC(glGenBuffers, PFNGLGENBUFFERSPROC);
  BIND_GL_FUNC(glBindBuffer, PFNGLBINDBUFFERPROC);
  BIND_GL_FUNC(glBufferData, PFNGLBUFFERDATAPROC);
  BIND_GL_FUNC(glDeleteBuffers, PFNGLDELETEBUFFERSPROC);

  BIND_GL_FUNC(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC);
  BIND_GL_FUNC(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC);

  BIND_GL_FUNC(glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC);
  BIND_GL_FUNC(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC);

  return 0;
}