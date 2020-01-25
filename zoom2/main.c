#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

static TCHAR APP_NAME[] = _T("vroom 2 :: the sequel");
static const int HEIGHT = 600;
static const int WIDTH = 800;

LRESULT CALLBACK
WindowProc(
  HWND hWnd,
  UINT message,
  WPARAM wParam,
  LPARAM lParam
) {
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message) {
    case WM_DESTROY: {
      PostQuitMessage(0);
      break;
    }
    case WM_PAINT: {
      hdc = BeginPaint(hWnd, &ps);

      TCHAR hello[] = _T("hello world");
      TextOut(hdc, 5, 5, hello, _tcslen(hello));

      EndPaint(hWnd, &ps);
      break;
    }
    default: {
      return DefWindowProc(hWnd, message, wParam, lParam);
      break;
    }
  }

  return 0;
}

int CALLBACK
WinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPSTR     lpCmdLine,
  _In_ int       nCmdShow
) {
  printf("Hello world\n");

  WNDCLASSEX wndClass = {
    .cbSize = sizeof(WNDCLASSEX),
    .style = CS_HREDRAW | CS_VREDRAW,
    .lpfnWndProc = WindowProc,
    .cbClsExtra = 0,
    .cbWndExtra = 0,
    .hInstance = hInstance,
    .hIcon = LoadIcon(hInstance, IDI_APPLICATION),
    .hCursor = LoadCursor(NULL, IDC_ARROW),
    .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
    .lpszMenuName = NULL,
    .lpszClassName = APP_NAME,
    .hIconSm = LoadIcon(hInstance, IDI_APPLICATION)
  };

  if (!RegisterClassEx(&wndClass)) {
    MessageBox(NULL, "Call to RegisterClassEx failed!", APP_NAME, NULL);

    return -1;
  }

  HWND hWnd = CreateWindowEx(
    WS_EX_APPWINDOW,
    wndClass.lpszClassName,
    APP_NAME,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    WIDTH,
    HEIGHT,
    NULL,
    NULL,
    hInstance,
    NULL
  );

  if (!hWnd) {
    MessageBox(NULL, "Call to CreateWindow failed", APP_NAME, NULL);

    return -1;
  }
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int)msg.wParam;
}