// Direct3D 9 graphics
// https://docs.microsoft.com/en-us/windows/win32/api/_direct3d9/
#include <d3d9.h>

// dinput.h is a human interface device
// https://docs.microsoft.com/en-us/windows-hardware/drivers/hid/
#include <dinput.h>

// windows API
// https://en.wikipedia.org/wiki/Windows.h
#include <windows.h>

// Microsoft Graphics Device interface (GDI)
// https://docs.microsoft.com/en-us/windows/win32/gdi/windows-gdi
#include <mmsystem.h>

#include <fstream>
#include <iostream>
#pragma comment(lib, "winmm.lib")
#include <math.h>
#include <stdio.h>

#include "Create_map.h"
#include "Playing.h"
#include "class.h"
#include "resource.h"

#define PLAY 0
#define CREATE_MAP 1
#define SETTINGS 2
#define EXIT 3

#define PM_CONTINUE 0
#define PM_SAVE 1
#define PM_SETTINGS 2
#define PM_EXIT 3

// Main menu
#define MAIN_MENU_IMG "Images/main_menu/main_menu.bmp"
#define PLAY_BUTTON_IMG "Images/main_menu/btn_Play.bmp"
#define CREATE_MAP_BUTTON_IMG "Images/main_menu/btn_Map.bmp"
#define SETTINGS_BUTTON_IMG "Images/main_menu/btn_Settings.bmp"
#define EXIT_BUTTON_IMG "Images/main_menu/btn_Exit.bmp"

// Missions
#define MISSIONS_GRID_IMG "Images/missions/missions.bmp"
#define MISSION_CELL_IMG "Images/missions/m1.bmp"
#define LOCKED_MISSION_IMG "Images/missions/blocked.bmp"
#define BACK_BUTTON_IMG "Images/missions/back.bmp"

// Pause
#define PAUSE_MENU_IMG "Images/pause/menu.bmp"
#define BACKDROP_IMG "Images/pause/backdrop.bmp"

#define PAUSE_CONTINUE_IMG "Images/pause/continue.bmp"
#define PAUSE_SAVE_IMG "Images/pause/save.bmp"
#define PAUSE_SETTINGS_IMG "Images/pause/settings.bmp"
#define PAUSE_EXIT_IMG "Images/pause/exit.bmp"

const int screenPixelWidth = 800;   // w: screenPixelWidth
const int screenPixelHeight = 600;  // h: screenPixelHeight

const int blocksInWidth = 40;   // mW: blocksInWidth
const int blocksInHeight = 30;  // mH: blocksInHeight

const int left = (1280 - screenPixelWidth) / 2;
const int top = (800 - screenPixelHeight) / 2;

int clickedX, clickedY;            // X, Y: clickedX, clickedY
int cursorX, cursorY;              // mX, mY: cursorX, cursorY
bool didClickLeftButton = false;   // lmb: didClickLeftButton
bool didClickRightButton = false;  // rmb: didClickRightButton

// Main menu
bool mainMenuMode = true;  // Menu: isInitialState: mainMenuMode

Sprite *mainMenu;
Button *btnMain[4];

// Library vars
HWND hWnd;

IDirect3D9 *d3d = NULL;
IDirect3DDevice9 *videocard = NULL;
D3DPRESENT_PARAMETERS pp;

IDirectInputDevice8 *keyboard;
IDirectInput8 *di;

char buffer[256];

Param *paramDraw;
// Global variables

class Sprite {
 public:
  int x, y;
  int width, height;
  int *img;
  int *im;
  int TransparentColor;  // 0xffffffff
  char *name;

 public:
  Sprite(char *fname)
      : img(NULL), x(0), y(0), TransparentColor(0), name(fname) {
    std::ifstream is(fname, std::ios::binary);
    is.seekg(18);
    is.read(reinterpret_cast<char *>(&width), sizeof(width));
    is.read(reinterpret_cast<char *>(&height), sizeof(height));
    is.seekg(28, std::ios::cur);
    img = new int[width * height * 32 / 8];

    for (int i = height - 1; i >= 0; --i)
      is.read(reinterpret_cast<char *>(img) + width * i * 32 / 8,
              width * 32 / 8);

    is.close();
  }
  Sprite(char *fname, int trColor)
      : img(NULL), x(0), y(0), TransparentColor(trColor), name(fname) {
    std::ifstream is(fname, std::ios::binary);
    is.seekg(18);
    is.read(reinterpret_cast<char *>(&width), sizeof(width));
    is.read(reinterpret_cast<char *>(&height), sizeof(height));
    is.seekg(28, std::ios::cur);
    img = new int[width * height * 32 / 8];

    for (int i = height - 1; i >= 0; --i)
      is.read(reinterpret_cast<char *>(img) + width * i * 32 / 8,
              width * 32 / 8);

    is.close();
  }

  ~Sprite() {
    if (img != NULL) delete[] img;
    img = NULL;
  }

  void DrawIntObject(D3DLOCKED_RECT &lockedRect) {
    for (int i = 0; i < height; ++i)
      for (int j = 0; j < width; ++j)
        if (img[j + i * width] != TransparentColor)
          memcpy(reinterpret_cast<char *>(lockedRect.pBits) + x * 4 + j * 4 +
                     i * lockedRect.Pitch + y * lockedRect.Pitch,
                 reinterpret_cast<char *>(&img[j + i * width]), 4);
  }

  // Flips around Y axis
  void flipHorizontally() {  // Rotate: flipHorizontally
    std::ifstream is(name, std::ios::binary);
    is.seekg(18);
    is.read(reinterpret_cast<char *>(&width), sizeof(width));
    is.read(reinterpret_cast<char *>(&height), sizeof(height));
    is.seekg(28, std::ios::cur);
    im = new int[width * height * 32 / 8];

    for (int i = height - 1; i >= 0; --i)
      is.read(reinterpret_cast<char *>(im) + width * i * 32 / 8,
              width * 32 / 8);

    is.close();
    //++++

    int j1 = width;

    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        img[j + i * width] = im[j1 + i * width];

        j1--;
      }

      j1 = width;
    }
  }

  void cut(int xI, int yI, int wI, int hI) {
    int *im1 = new int[width * height * 32 / 8];
    img = new int[wI * hI * 32 / 8];

    //++++
    std::ifstream is(name, std::ios::binary);
    is.seekg(18);
    is.read(reinterpret_cast<char *>(&width), sizeof(width));
    is.read(reinterpret_cast<char *>(&height), sizeof(height));
    is.seekg(28, std::ios::cur);
    im1 = new int[width * height * 32 / 8];

    for (int i = height - 1; i >= 0; --i)
      is.read(reinterpret_cast<char *>(im1) + width * i * 32 / 8,
              width * 32 / 8);

    is.close();
    //++++

    for (int i = yI; i < yI + hI; ++i)
      for (int j = xI; j < xI + wI; ++j)
        img[(j - xI) + (i - yI) * wI] = im1[j + i * width];

    width = wI;
    height = hI;
  }

  void replaceColor(int from, int to) {  // RemplaceColor: replaceColor
    for (int i = 0; i < height; ++i)
      for (int j = 0; j < width; ++j)
        if (img[j + i * width] == from) img[j + i * width] = to;
  }
};
class Param {
 public:
  D3DLOCKED_RECT rectangle;
  RECT rectSize;
  IDirect3DSurface9 *backBuffer;

 public:
  Param() { backBuffer = NULL; }

  void draw(int x, int y, int w, int h, Sprite *Image) {  // Draw: draw
    rectSize.left = x;
    rectSize.top = y;
    rectSize.right = rectSize.left + w;
    rectSize.bottom = rectSize.top + h;

    backBuffer->LockRect(&rectangle, &rectSize, 0);
    Image->DrawIntObject(rectangle);
    backBuffer->UnlockRect();
  }
};

void draw() {  // Draw: draw
  if (mainMenuMode) drawMainMenu();
  if (playMode) drawScene(cursorX, cursorY);
  if (missionMode) drawMission();
  if (createMapMode) drawCreateMapScreen();
}

void drawMainMenu() {
  paramDraw->draw(0, 0, screenPixelWidth, screenPixelHeight, mainMenu);

  for (int i = 0; i < 4; i++) btnMain[i]->draw(paramDraw);
}

void initialization() {
  initMainMenu();
  initPlay();
  initCreateMap();
  initPause();
}
void initMainMenu() {
  mainMenu = new Sprite(MAIN_MENU_IMG);

  btnMain[PLAY] = new Button(271, 182, PLAY_BUTTON_IMG);
  btnMain[CREATE_MAP] = new Button(271, 280, CREATE_MAP_BUTTON_IMG);
  btnMain[SETTINGS] = new Button(271, 378, SETTINGS_BUTTON_IMG);
  btnMain[EXIT] = new Button(271, 476, EXIT_BUTTON_IMG);
}
void initPlay() {
  char *tt = "score:0";                                // delete
  scoreText = new Text(8, 43, tt, DARK_GRAYISH_BLUE);  // score

  missions = new Sprite(MISSIONS_GRID_IMG);
  pauseMenuSprite = new Sprite(PAUSE_MENU_IMG);

  missionButton[0] = new Button(22, 63, MISSION_CELL_IMG);

  pauseOverlay = new Sprite(BACKDROP_IMG, WHITE);
  missionLock = new Sprite(LOCKED_MISSION_IMG, WHITE);

  backButton = new Button(15, 554, BACK_BUTTON_IMG, WHITE);

  inventory = new Inventory();
}
void initPause() {
  int pauseX = (screenPixelWidth - pauseMenuSprite->width) / 2;
  int pauseY = (screenPixelHeight - pauseMenuSprite->height) / 2;
  int centeredX = pauseX + 79;

  PM[PM_CONTINUE] = new Button(centeredX, pauseY + 55, PAUSE_CONTINUE_IMG);
  PM[PM_SAVE] = new Button(centeredX,  pauseY + 101, PAUSE_SAVE_IMG);
  PM[PM_SETTINGS] = new Button(centeredX, pauseY + 149, PAUSE_SETTINGS_IMG);
  PM[PM_EXIT] = new Button(centeredX, pauseY + 200, PAUSE_EXIT_IMG);
}

void initDrawingSystem(HINSTANCE hInstance) { // InitialSys: initDrawingSystem
  paramDraw = new Param();

  d3d = Direct3DCreate9(D3D_SDK_VERSION);

  ZeroMemory(&pp, sizeof(pp));

  pp.BackBufferWidth = screenPixelWidth;
  pp.BackBufferHeight = screenPixelHeight;
  pp.BackBufferFormat = D3DFMT_X8R8G8B8;
  pp.BackBufferCount = 1;
  pp.MultiSampleType = D3DMULTISAMPLE_NONE;
  pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  pp.hDeviceWindow = hWnd;
  pp.Windowed = true;
  pp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

  d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                    D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &videocard);

  DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
                     (void **)&di, NULL);
  di->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
  keyboard->SetDataFormat(&c_dfDIKeyboard);
  keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
  keyboard->Acquire();

  ZeroMemory(buffer, sizeof(buffer));

  videocard->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO,
                           &paramDraw->backBuffer);
}

void interactions(int cursorX, int cursorY, int clickedX, int clickedY) {  // mainMenuInteractions: interactions
  if (mainMenuMode) mainMenuInteractions(cursorX, cursorY, clickedX, clickedY);
  if (missionMode) mission(cursorX, cursorY, clickedX, clickedY);
  if (playMode) playLoop(cursorX, cursorY, clickedX, clickedY);
  if (createMapMode) createMapInteractions(clickedX, clickedY);
}

void mainMenuInteractions(int cursorX, int cursorY, int clickedX,
                          int clickedY) {
  for (int i = 0; i < 4; i++) {
    btnMain[i]->show = btnMain[i]->contains(cursorX, cursorY);
  }

  if (!didClickLeftButton) return;

  if (btnMain[PLAY]->contains(clickedX, clickedY)) {
    playMode = true;
    missionMode = true;
    mainMenuMode = false;
  }

  if (btnMain[CREATE_MAP]->contains(clickedX, clickedY)) {
    createMapMode = true;
    mainMenuMode = false;
  }

  if (btnMain[SETTINGS]->contains(clickedX, clickedY)) {
    mainMenuMode = false;
  }
}

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lCmdLine, int nCmdShow) {
  initialization();
  intializeWindow(hInstance, nCmdShow);
  initDrawingSystem(hInstance);

  MSG msg;

  while (true) {
    keyboard->Acquire();

    didClickLeftButton = false;
    didClickRightButton = false;

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_DESTROY) break;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
      cursorX = msg.pt.x - left;
      cursorY = msg.pt.y - top;

      if (msg.message == WM_RBUTTONUP || msg.message == WM_LBUTTONUP) {
        clickedX = cursorX;
        clickedY = cursorY;
      }

      didClickLeftButton = msg.message == WM_LBUTTONUP;
      didClickRightButton = msg.message == WM_RBUTTONUP;
    }

    keyboard->GetDeviceState(sizeof(buffer), buffer);

    interactions(cursorX, cursorY, clickedX, clickedY);

    if (shouldExit()) break;

    videocard->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255),
                     1.0f, 0);
    draw();
    videocard->Present(NULL, NULL, NULL, NULL);
    Sleep(20);
  }

  cleanup();

  return 0;
}
void shouldExit() {
    if (mainMenuMode && btnMain[EXIT]->contains(clickedX, clickedY)) return true;

    return false;
}

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}

void initializeWindow(HINSTANCE hInstance, int nCmdShow) {
  WNDCLASS wc;
  HICON hMyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)(6);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = hMyIcon;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WndProc;
  wc.lpszClassName = "class";
  wc.lpszMenuName = NULL;
  wc.style = CS_OWNDC;

  RegisterClass(&wc);

  hWnd = CreateWindow("class", "Game", WS_POPUP, left, top, screenPixelWidth,
                      screenPixelHeight, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
}

void cleanup() {
  if (keyboard) keyboard->Release();
  if (di) di->Release();
  if (d3d) d3d->Release();
  if (videocard) videocard->Release();
  UnregisterClass("class", hInstance);
}