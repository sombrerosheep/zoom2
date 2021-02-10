#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "ogl.h"
#include "shader.h"
#include "mesh.h"

#include "vec.h"
#include "mat.h"
#include "darray.h"

static const char *app_name = "vroom 2 :: the sequel";
static const int HEIGHT = 600;
static const int WIDTH = 800;

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void _mprintf(const char* format, va_list args) {
  char buf[512];
  vsprintf_s(buf, 512, format, args);
  OutputDebugString(buf);
}

void mprintf(const char* format, ...) {
  va_list argList;
  va_start(argList, format);
  _mprintf(format, argList);
  va_end(argList);
}

char* get_file_contents(const char* fileName) {
HANDLE hFile = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        mprintf("Error reading file (%s): %d\n", fileName, GetLastError());
        return NULL;
    }

    DWORD size = GetFileSize(hFile, NULL);
    if (size < 1) {
        mprintf("File size is zero (%s)\n", fileName);
        return NULL;
    }

    char *buffer = malloc(sizeof(char) * size + 1);
    if (buffer == NULL) {
        mprintf("Unable to allocate memory for file %s\n", fileName);
        return NULL;
    }
    DWORD bytesRead;
    BOOL read = ReadFile(hFile, buffer, size, &bytesRead, NULL);
    if (read) {
        buffer[bytesRead] = '\0';
        return buffer;
    }

    return NULL;
}

darray *load_obj(const char* fileName) {
  FILE* f;
  errno_t err = fopen_s(&f, fileName, "r");
  if (err != 0) {
    mprintf("Error reading file! %d\n", err);
    return;
  }
  if (f == NULL) {
    mprintf("file handler is null\n");
    return;
  }

  darray* vertices = darray_create(50, sizeof(vec3));
  darray* texcoords = darray_create(50, sizeof(vec2));
  darray* normals = darray_create(50, sizeof(vec3));
  darray* vertex_data = darray_create(50, sizeof(vertex_pc));

  #define LINE_BUFFER_SIZE 256
  char buffer[LINE_BUFFER_SIZE];

  while (fgets(buffer, LINE_BUFFER_SIZE, f)) {
    assert(strlen(buffer) < LINE_BUFFER_SIZE);
    if (buffer[0] == 'o') {
      mprintf("processing new model: %s\n", buffer);

    } else if (buffer[0] == 'v' && buffer[1] == ' ') {
      mprintf("\tprocessing vertex: %s", buffer);
      vec3 vec;
      float x, y, z;
      int n = sscanf_s(&buffer, "v %f %f %f", &vec.x, &vec.y, &vec.z);
      mprintf("\tprocessed vertex: %f %f %f\n", vec.x, vec.y, vec.z);
      darray_insert(vertices, &vec);

    } else if (buffer[0] == 'v' && buffer[1] == 't') {
      mprintf("\tprocessing texcoord: %s", buffer);
      vec2 vec;
      int n = sscanf_s(buffer, "vt %f %f", &vec.x, &vec.y);
      mprintf("\tprocessed texcoord: %f %f\n", vec.x, vec.y);
      darray_insert(texcoords, &vec);

    } else if (buffer[0] == 'v' && buffer[1] == 'n') {
      mprintf("\tprocessing normal: %s", buffer);
      vec3 vec;
      int n = sscanf_s(buffer, "vn %f %f %f", &vec.x, &vec.y, &vec.z);
      mprintf("\tprocessed normal: %f %f %f\n", vec.x, vec.y, vec.z);
      darray_insert(normals, &vec);

    } else if (buffer[0] == 'f') { 
      mprintf("\tprocessing face: %s", buffer);
      // assuming quads...
      // todo: not assume quads
      unsigned int faces[12];
      sscanf_s(
        buffer, "f %u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u",
        &faces[0], &faces[1], &faces[2],
        &faces[3], &faces[4], &faces[5],
        &faces[6], &faces[7], &faces[8],
        &faces[9], &faces[10], &faces[11]
      );

      vertex_pc vert;

      vert.position = *(vec3*)darray_get(vertices, faces[0] - 1);
      vert.color = *(vec3*)darray_get(normals, faces[2] - 1);
      darray_insert(vertex_data, &vert);

      vert.position = *(vec3*)darray_get(vertices, faces[3] - 1);
      vert.color = *(vec3*)darray_get(normals, faces[5] - 1);
      darray_insert(vertex_data, &vert);

      vert.position = *(vec3*)darray_get(vertices, faces[6] - 1);
      vert.color = *(vec3*)darray_get(normals, faces[8] - 1);
      darray_insert(vertex_data, &vert);

      vert.position = *(vec3*)darray_get(vertices, faces[9] - 1);
      vert.color = *(vec3*)darray_get(normals, faces[11] - 1);
      darray_insert(vertex_data, &vert);
    } else {
      mprintf("\tunhandled token: %s\n", buffer);
    }

    // TODO: load material
  }
    /*
      o      - object name    char*
      v      - vertex         vec3[] (space delimited)
      vt     - texcoord       vec2[] (space delimited)
      vn     - vert norm      vec3[] (space delimited)
      f      - face           int[3] (/ delimited) vertex/texCoord/normal
      s      - smooth group   bool (as on/off)
      usemtl - material name
      mtllib - material lib
    */

  fclose(f);
  darray_destroy(vertices);
  darray_destroy(texcoords);
  darray_destroy(normals);
  mprintf("Done reading file\n");
  return vertex_data;
}

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
    case WM_KEYDOWN: {
      if (wParam == VK_ESCAPE) {
        PostQuitMessage(0);
      }
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
  mprintf("INFO :: Hello world\n");
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

  char* vert = get_file_contents("resources\\shaders\\basic.vert");
  char* frag = get_file_contents("resources\\shaders\\basic.frag");
  shader shader_prog = shader_create(vert, frag);
  free(vert);
  free(frag);

  darray* cube_data = load_obj("c:\\Users\\Noah\\Documents\\GameDev\\Projects\\zoom2\\zoom2\\resources\\models\\cube\\cube.obj");

  vec3 cube_positions[] = {
    (vec3) { .x = -2.0f,.y = 1.0f,.z = -1.0f },
    (vec3) { .x = 2.0f,.y = -1.5f,.z = 0.0f }
  };
  vertex_pc cube_vertices[] = {
    (vertex_pc){ (vec3){ 0.5f,  0.5f,  0.5f }, (vec3){ 1.0f, 0.0f, 0.0f } }, // 0
    (vertex_pc){ (vec3){-0.5f,  0.5f,  0.5f }, (vec3){ 0.0f, 1.0f, 0.0f } }, // 1
    (vertex_pc){ (vec3){-0.5f, -0.5f,  0.5f }, (vec3){ 0.0f, 0.0f, 1.0f } }, // 2
    (vertex_pc){ (vec3){ 0.5f, -0.5f,  0.5f }, (vec3){ 0.9f, 0.0f, 0.9f } }, // 3
    (vertex_pc){ (vec3){ 0.5f,  0.5f, -0.5f }, (vec3){ 1.0f, 0.0f, 0.0f } }, // 4
    (vertex_pc){ (vec3){-0.5f,  0.5f, -0.5f }, (vec3){ 0.0f, 1.0f, 0.0f } }, // 5
    (vertex_pc){ (vec3){-0.5f, -0.5f, -0.5f }, (vec3){ 0.0f, 0.0f, 1.0f } }, // 6
    (vertex_pc){ (vec3){ 0.5f, -0.5f, -0.5f }, (vec3){ 0.9f, 0.0f, 0.9f } }  // 7
  };
  unsigned int cube_indices[] = {
    0, 1, 2, 2, 3, 0, // front
    4, 0, 3, 3, 7, 4, // right
    4, 7, 6, 6, 5, 4, // back
    1, 5, 6, 6, 2, 1, // left
    4, 5, 1, 1, 0, 4, // top
    3, 2, 6, 6, 7, 3  // bottom
  };
  mesh_vpc cube;
  mesh_vpc_init(&cube, cube_vertices, 8, cube_indices, 36);

  vec3 camera_pos = { 2.0f, 2.0f, 7.0f };
  vec3 camera_target = { 0.0f, 0.0f, 0.0f };
  float camera_pitch = -90.f;
  float camera_yaw = -30.f;
  vec3 up = { 0.0f, 1.0f, 0.0f };

  mat4 view = mat4_lookat(camera_pos, camera_target, up);
  mat4 projection = mat4_perspective(
    0.785398f, // fov: 45deg (in rad)
    (float)WIDTH / (float)HEIGHT,
    0.1f,
    100.0f
  );
  
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

    float yrot = sinf((float)clock() / CLOCKS_PER_SEC);
    float xrot = cosf((float)clock() / CLOCKS_PER_SEC);

    shader_use(shader_prog);
    shader_set_mat4(shader_prog, "view", view);
    shader_set_mat4(shader_prog, "projection", projection);

    mat4 model = mat4_identity;
    model = mat4_translate_vec3(model, cube_positions[0]);
    model = mat4_rotate_vec3(model, yrot, (vec3) { 0.0f, 1.0f, 0.0f });
    model = mat4_rotate_vec3(model, xrot, (vec3) { 1.0f, 0.0f, 0.0f });
    shader_set_mat4(shader_prog, "model", model);
    mesh_vpc_draw(&cube, shader_prog);

    model = mat4_identity;
    model = mat4_translate_vec3(model, cube_positions[1]);
    model = mat4_rotate_vec3(model, yrot, (vec3) { 0.0f, 1.0f, 0.0f });
    model = mat4_rotate_vec3(model, -xrot, (vec3) { 1.0f, 0.0f, 0.0f });
    shader_set_mat4(shader_prog, "model", model);
    mesh_vpc_draw(&cube, shader_prog);
    
    SwapBuffers(ctx.device);
  }

  shader_destroy(shader_prog);
  mesh_vpc_destroy(&cube);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(ctx.glContext);
  if (ctx.device != NULL) {
    ReleaseDC(window, ctx.device);
  }
  DestroyWindow(window);

  return (int)msg.wParam;
}
