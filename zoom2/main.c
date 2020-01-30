#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "ogl.h"

#include "vec.h"
#include "mat.h"

static const char *app_name = "vroom 2 :: the sequel";
static const int HEIGHT = 600;
static const int WIDTH = 800;

unsigned int load_shader(char* vert, char* frag);
LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

char *vert = "#version 330\n" \
"layout (location = 0) in vec3 aPos;\n" \
"layout (location = 1) in vec3 aColor;\n" \
"out vec3 fColor;\n" \
"uniform mat4 model;" \
"void main() {\n" \
"fColor = aColor;\n" \
"gl_Position = model * vec4(aPos, 1.0);\n" \
"}";
char *frag = "#version 330\n" \
"in vec3 fColor;\n" \
"out vec4 FragColor;\n" \
"void main() {\n" \
"FragColor = vec4(fColor, 1.0);\n" \
"}";

typedef struct RenderContext {
  HDC device;
  HGLRC glContext;
} RenderContext;

HWND make_window(RenderContext *context) {
  PIXELFORMATDESCRIPTOR fakePfd = {
    .nSize = sizeof(PIXELFORMATDESCRIPTOR),
    .nVersion = 1,
    .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    .iPixelType = PFD_TYPE_RGBA,
    .cColorBits = 32,
    .cDepthBits = 24,
    .cStencilBits = 8,
  };

  WNDCLASSEX wndClass = {
    .cbSize = sizeof(WNDCLASSEX),
    .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
    .lpfnWndProc = WindowProc,
    .cbClsExtra = 0,
    .cbWndExtra = 0,
    .hInstance = 0,
    .hIcon = LoadIcon(0, IDI_APPLICATION),
    .hCursor = LoadCursor(NULL, IDC_ARROW),
    .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
    .lpszMenuName = NULL,
    .lpszClassName = app_name,
    .hIconSm = LoadIcon(0, IDI_APPLICATION)
  };

  if (!RegisterClassEx(&wndClass)) {
    MessageBox(NULL, "Call to RegisterClassEx failed!", app_name, MB_OK);
    return NULL;
  }

  HWND fakeWnd = CreateWindow(
    app_name, "fake window",
    WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
    0, 0, 1, 1,
    NULL, NULL, 0, NULL
  );
  if (fakeWnd == NULL) {
    MessageBox(NULL, "Error init fake window", app_name, MB_OK);
    return NULL;
  }

  HDC fake_context = GetDC(fakeWnd);
  int fakePFDID = ChoosePixelFormat(fake_context, &fakePfd);
  if (fakePFDID == 0) {
    MessageBox(NULL, "Failure calling ChoosePixelFormat", app_name, MB_OK);
    return NULL;
  }

  if (!SetPixelFormat(fake_context, fakePFDID, &fakePfd)) {
    MessageBox(NULL, "Failure calling SetPixelFormat", app_name, MB_OK);
    return NULL;
  }

  HGLRC fake_glc = wglCreateContext(fake_context);
  if (fake_glc == 0) {
    MessageBox(NULL, "Failure creating fgl context", app_name, MB_OK);
    return NULL;
  }

  if (!wglMakeCurrent(fake_context, fake_glc)) {
    MessageBox(NULL, "Failure making fctx current", app_name, MB_OK);
    return NULL;
  }

  OutputDebugString(glGetString(GL_VERSION));

  if (init_gl_func() != 0) {
    MessageBox(NULL, "failed to init opengl", app_name, MB_OK);
    return NULL;
  }

  HWND window = CreateWindowEx(
    WS_EX_APPWINDOW,
    app_name, app_name,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    WIDTH, HEIGHT,
    NULL, NULL, 0, NULL
  );
  context->device = GetDC(window);
  if (context->device == NULL) {
    MessageBox(NULL, "failed to get device context", app_name, MB_OK);
  }

  const int attribs[] = {
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_ALPHA_BITS_ARB, 8,
    WGL_DEPTH_BITS_ARB, 24,
    WGL_STENCIL_BITS_ARB, 8,
    WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
    WGL_SAMPLES_ARB, 4,
    0
  };

  int formatId;
  UINT numFormats;
  BOOL status = wglChoosePixelFormatARB(context->device, attribs, NULL, 1, &formatId, &numFormats);
  if (status == FALSE || numFormats == 0) {
    MessageBox(NULL, "Could not get a pixel format", app_name, MB_OK);
    return NULL;
  }

  PIXELFORMATDESCRIPTOR pfd;
  if (context->device != NULL) {
    DescribePixelFormat(context->device, formatId, sizeof(pfd), &pfd);
    SetPixelFormat(context->device, formatId, &pfd);
  }

  int contextAttribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };
  context->glContext = wglCreateContextAttribsARB(context->device, 0, contextAttribs);
  if (context->glContext == NULL) {
    MessageBox(NULL, "could not create gl context", app_name, MB_OK);
    return NULL;
  }

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(fake_glc);
  ReleaseDC(fakeWnd, fake_context);
  DestroyWindow(fakeWnd);
  if (!wglMakeCurrent(context->device, context->glContext)) {
    MessageBox(NULL, "failed to set final context", app_name, MB_OK);
    return NULL;
  }

  return window;
}

LRESULT CALLBACK
WindowProc(
  HWND window,
  UINT message,
  WPARAM wParam,
  LPARAM lParam
) {
  switch (message) {
    case WM_CLOSE: {
      PostQuitMessage(0);
      break;
    }
    default: {
      return DefWindowProc(window, message, wParam, lParam);
    }
  }

  return 0;
}

int CALLBACK
WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nCmdShow
) {
  OutputDebugString("INFO :: Hello world\n");
 
  // create window
  RenderContext ctx = {
    .device = NULL,
    .glContext = NULL
  };
  HWND window = make_window(&ctx);

  if (!window) {
    MessageBox(NULL, "Call to CreateWindow failed", app_name, MB_OK);
    return -1;
  }
  
  ShowWindow(window, nCmdShow);

  glEnable(GL_DEPTH_TEST);

  unsigned int shader_prog = load_shader(vert, frag);

  float cubeVertexData[] = {
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 0
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 1
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // 2
     0.5f, -0.5f,  0.5f, 0.9f, 0.0f, 0.9f, // 3
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 4
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 5
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 6
     0.5f, -0.5f, -0.5f, 0.9f, 0.0f, 0.9f  // 7
  };
  vec3 cubePositions[] = {
    (vec3) { .x = -1.0f, .y = 0.0f, .z = -1.0f },
    (vec3) { .x =  2.0f, .y = 0.0f, .z =  0.0f }
  };
  unsigned int cubeIndices[] = {
    0, 1, 2, 2, 3, 0, // front
    4, 0, 3, 3, 7, 4, // right
    4, 7, 6, 6, 5, 4, // back
    1, 5, 6, 6, 2, 1, // left
    4, 5, 1, 1, 0, 4, // top
    3, 2, 6, 6, 7, 3  // bottom
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)(sizeof(GL_FLOAT) * 3));
  glEnableVertexAttribArray(1);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  BOOL running = TRUE;
  MSG msg;
  while (running == TRUE) {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        running = FALSE;
      }

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float frot = sinf((float)clock() / CLOCKS_PER_SEC);
    mat4 model = mat4_identity();
    model = mat4_rotate_vec3(model, frot, (vec3) { 0.0f, 1.0f, 0.0f });

    glUseProgram(shader_prog);
    unsigned int mloc = glGetUniformLocation(shader_prog, "model");
    glUniformMatrix4fv(mloc, 1, GL_FALSE, &model.values);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(cubeIndices), GL_UNSIGNED_INT, 0);
    
    SwapBuffers(ctx.device);
  }

  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(ctx.glContext);
  if (ctx.device != NULL) {
    ReleaseDC(window, ctx.device);
  }
  DestroyWindow(window);

  return (int)msg.wParam;
}

void shader_compilation_status(unsigned int shader) {
  int success;
  char infoLog[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    MessageBox(NULL, infoLog, "ERROR::SHADER::COMPILATION_FAILED", MB_OK);
  }
}

void shader_program_link_status(unsigned int program) {
  int success;
  char infoLog[512];

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    MessageBox(NULL, infoLog, "ERROR::SHADER::PROGRAM::LINK_FAILED", MB_OK);
  }
}

unsigned int load_shader(char* vert, char* frag) {
  unsigned int vertex_shader, fragment_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vert, NULL);
  glCompileShader(vertex_shader);
  shader_compilation_status(vertex_shader);

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &frag, NULL);
  glCompileShader(fragment_shader);
  shader_compilation_status(fragment_shader);

  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  shader_program_link_status(shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return shader_program;
}