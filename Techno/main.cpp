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

#include "resource.h"

#define PLAY 0
#define CREATE_MAP 1
#define SETTINGS 2
#define EXIT 3

#define SOLID_BLOCK 0
#define DELETE_BLOCK 1
#define BACKGROUND_BLOCK 2
#define SAVE 3
#define SMALL_SQUARE 4
#define STAIRS 5

#define POINT 0
#define LINE 1
#define RECTANGLE 2

#define BACK_LAYER 0
#define FOREGROUND_LAYER 1

#define TEST_THE_MAP 0

#define PM_CONTINUE 0
#define PM_SAVE 1
#define PM_SETTINGS 2
#define PM_EXIT 3

// Global variables+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Sprite {
   public:
    int x, y;
    int width, height;
    int *img;
    int *im;
    int TransparentColor;  // 0xffffffff
    char *name;

   public:
    Sprite(char *fname): img(NULL), x(0), y(0), TransparentColor(0), name(fname) {
        std::ifstream is(fname, std::ios::binary);
        is.seekg(18);
        is.read(reinterpret_cast<char *>(&width), sizeof(width));
        is.read(reinterpret_cast<char *>(&height), sizeof(height));
        is.seekg(28, std::ios::cur);
        img = new int[width * height * 32 / 8];

        for (int i = height - 1; i >= 0; --i)
            is.read(reinterpret_cast<char *>(img) + width * i * 32 / 8, width * 32 / 8);

        is.close();
    }
    Sprite(char *fname, int trColor): img(NULL), x(0), y(0), TransparentColor(trColor), name(fname) {
        std::ifstream is(fname, std::ios::binary);
        is.seekg(18);
        is.read(reinterpret_cast<char *>(&width), sizeof(width));
        is.read(reinterpret_cast<char *>(&height), sizeof(height));
        is.seekg(28, std::ios::cur);
        img = new int[width * height * 32 / 8];

        for (int i = height - 1; i >= 0; --i)
            is.read(reinterpret_cast<char *>(img) + width * i * 32 / 8, width * 32 / 8);

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
                    memcpy(reinterpret_cast<char *>(lockedRect.pBits) + x * 4 +
                               j * 4 + i * lockedRect.Pitch +
                               y * lockedRect.Pitch,
                           reinterpret_cast<char *>(&img[j + i * width]), 4);
    }

    // Flips around Y axis
    void flipHorizontally() { // Rotate: flipHorizontally
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

    void replaceColor(int from, int to) { // RemplaceColor: replaceColor
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

    void draw(int x, int y, int w, int h, Sprite *Image) { // Draw: draw
        rectSize.left = x;
        rectSize.top = y;
        rectSize.right = rectSize.left + w;
        rectSize.bottom = rectSize.top + h;

        backBuffer->LockRect(&rectangle, &rectSize, 0);
        Image->DrawIntObject(rectangle);
        backBuffer->UnlockRect();
    }
};

const int screenPixelWidth = 800; // w: screenPixelWidth
const int screenPixelHeight = 600; // h: screenPixelHeight

#include "class.h"

const int blocksInWidth = 40; // mW: blocksInWidth
const int blocksInHeight = 30; // mH: blocksInHeight

int left = (1280 - screenPixelWidth) / 2;
int top = (800 - screenPixelHeight) / 2;

int clickedX, clickedY; // X, Y: clickedX, clickedY
int cursorX, cursorY; // mX, mY: cursorX, cursorY
bool didClickLeftButton = false; // lmb: didClickLeftButton
bool didClickRightButton = false; // rmb: didClickRightButton

// Main menu+++
bool isInitialState = true; // Menu: isInitialState
bool end = true;

Sprite *mainMenu;
Button *btnMain[4];

// Main menu----------------------
HWND hWnd;

IDirect3D9 *d3d = NULL;
IDirect3DDevice9 *videocard = NULL;
D3DPRESENT_PARAMETERS pp;
HRESULT hr;

IDirectInputDevice8 *keyboard;
IDirectInput8 *di;

char buffer[256];

Param *paramDraw;
// Global variables---------------

#include "Create_map.h"
#include "Playing.h"

int timer;
int timer1;
int dt;
Button* lock;

void Draw() {
    // MAIN MENU+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (isInitialState == true) {
        paramDraw->draw(0, 0, screenPixelWidth, screenPixelHeight, mainMenu);

        for (int i = 0; i < 4; i++) btnMain[i]->draw(paramDraw);
    }
    // MAIN MENU-------------------------------------------------------

    // PLAY++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (playMode == true) drawScene(cursorX, cursorY);
    // PLAY-----------------------------------------------------------

    // CREATE MAP+++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (Create_map == true) Draw_Create();
    // CREATE MAP-----------------------------------------------------------
}

void initialization() {
    // Main menu+++
    mainMenu = new Sprite("Images/main_menu.bmp");

    btnMain[PLAY] = new Button("Images/btn_Play.bmp");
    btnMain[PLAY]->x = 271;
    btnMain[PLAY]->y = 182;
    btnMain[PLAY]->show = false;

    btnMain[CREATE_MAP] = new Button("Images/btn_Map.bmp");
    btnMain[CREATE_MAP]->x = 271;
    btnMain[CREATE_MAP]->y = 280;
    btnMain[CREATE_MAP]->show = false;

    btnMain[SETTINGS] = new Button("Images/btn_Settings.bmp");
    btnMain[SETTINGS]->x = 271;
    btnMain[SETTINGS]->y = 378;
    btnMain[SETTINGS]->show = false;

    btnMain[EXIT] = new Button("Images/btn_Exit.bmp");
    btnMain[EXIT]->x = 271;
    btnMain[EXIT]->y = 476;
    btnMain[EXIT]->show = false;

    // Play+++
    char *tt = "score:0";                   // delete
    scoreText = new Text(8, 43, tt, 0xFF808E9B);  // score

    missions = new Sprite("Images/missions.bmp");
    pauseMenuSprite = new Sprite("Images/Pause_menu.bmp");

    missionButtons[0] = new Button("Images/m1.bmp");
    missionButtons[0]->x = 22;
    missionButtons[0]->y = 63;
    missionButtons[0]->show = false;

    pauseOverlay = new Sprite("Images/pause.bmp", 0xffffffff);
    missionLock = new Sprite("Images/blocked.bmp", 0xffffffff);

    // Locker+++
    lock = new Button("Images/chest/code.bmp", 0xffffffff);
    lock->x = (screenPixelWidth - lock->w) / 2;
    lock->y = (screenPixelHeight - lock->h) / 2;
    lock->show = true;

    // Locker---

    backButton = new Button("Images/back.bmp", 0xffffffff);
    backButton->x = 15;
    backButton->y = 554;
    backButton->show = false;

    inventory = new Inventory();
    inventory->x = 0;
    inventory->y = 0;
    inventory->show = false;

    // CREATE MAP+++
    Init_CreateMap();

    PM[PM_CONTINUE] = new Button("Images/pm_continue.bmp");
    PM[PM_CONTINUE]->x = (screenPixelWidth - pauseMenuSprite->width) / 2 + 79;
    PM[PM_CONTINUE]->y = (screenPixelHeight - pauseMenuSprite->height) / 2 + 55;
    PM[PM_CONTINUE]->show = false;

    PM[PM_SAVE] = new Button("Images/pm_save.bmp");
    PM[PM_SAVE]->x = (screenPixelWidth - pauseMenuSprite->width) / 2 + 79;
    PM[PM_SAVE]->y = (screenPixelHeight - pauseMenuSprite->height) / 2 + 101;
    PM[PM_SAVE]->show = false;

    PM[PM_SETTINGS] = new Button("Images/pm_settings.bmp");
    PM[PM_SETTINGS]->x = (screenPixelWidth - pauseMenuSprite->width) / 2 + 79;
    PM[PM_SETTINGS]->y = (screenPixelHeight - pauseMenuSprite->height) / 2 + 149;
    PM[PM_SETTINGS]->show = false;

    PM[PM_EXIT] = new Button("Images/pm_exit.bmp");
    PM[PM_EXIT]->x = (screenPixelWidth - pauseMenuSprite->width) / 2 + 79;
    PM[PM_EXIT]->y = (screenPixelHeight - pauseMenuSprite->height) / 2 + 200;
    PM[PM_EXIT]->show = false;
}

void InitialSys(HINSTANCE hInstance) {
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

void mainMenuInteractions() {
    if (isInitialState == true) {
        // Mouse move+++
        for (int i = 0; i < 4; i++) {
            if (btnMain[i]->contains(cursorX, cursorY) == true)
                btnMain[i]->show = true;
            else
                btnMain[i]->show = false;
        }
        // Mouse move---

        // Click+++
        if (btnMain[PLAY]->contains(clickedX, clickedY) == true && didClickLeftButton == true) {
            playMode = true;
            missionMode = true;
            isInitialState = false;
        }

        if (btnMain[CREATE_MAP]->contains(clickedX, clickedY) == true && didClickLeftButton == true) {
            Create_map = true;
            isInitialState = false;
        }

        if (btnMain[SETTINGS]->contains(clickedX, clickedY) == true && didClickLeftButton == true) {
            isInitialState = false;
        }

        // Click---
    }

    // Procedures+++++++++
    if (playMode) {
      playLoop(cursorX, cursorY, clickedX, clickedY);
    }
    createMap();
}

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lCmdLine, int nCmdShow) {
    initialization();

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

    hWnd = CreateWindow("class", "Game", WS_POPUP, left, top, screenPixelWidth, screenPixelHeight, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

    InitialSys(hInstance);

    while (end) {
        keyboard->Acquire();

        didClickLeftButton = false;
        didClickRightButton = false;

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_DESTROY) break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_LBUTTONUP) {
                clickedX = msg.pt.x - left;
                clickedY = msg.pt.y - top;

                didClickLeftButton = true;
            }

            if (msg.message == WM_RBUTTONUP) {
                clickedX = msg.pt.x - left;
                clickedY = msg.pt.y - top;

                didClickRightButton = true;
            }

            cursorX = msg.pt.x - left;
            cursorY = msg.pt.y - top;
        }

        hr = keyboard->GetDeviceState(sizeof(buffer), buffer);

        // Timer+++
        if (timer1 == 0) timer1 = timeGetTime();

        timer = timeGetTime();
        dt = timer - timer1;
        // Timer---

        mainMenuInteractions();

        if (btnMain[EXIT]->Touch(clickedX, clickedY) == true && isInitialState == true) return 0;

        videocard->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255),
                         1.0f, 0);
        Draw();
        videocard->Present(NULL, NULL, NULL, NULL);
        Sleep(20);
    }

    if (keyboard != NULL) keyboard->Release();
    if (di != NULL) di->Release();
    if (d3d != NULL) d3d->Release();
    if (videocard != NULL) videocard->Release();
    UnregisterClass("class", hInstance);

    return 0;
}

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}