#include <windows.h>
#include <d3d9.h>
#include <iostream>
#include <fstream>
#include <dinput.h>
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>
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

//Global variables+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Sprite
{
public:
	int x,y;
	int width, height;
	int* img;
	int* im;
	int TransparentColor; //0xffffffff
	char* name;

public:
	Sprite(char* fname) : img(NULL),x(0),y(0),TransparentColor(0),name(fname)
	{
		std::ifstream is (fname,std::ios::binary);
		is.seekg(18);
		is.read(reinterpret_cast<char*>(&width),sizeof(width));
		is.read(reinterpret_cast<char*>(&height),sizeof(height));
		is.seekg(28,std::ios::cur);
		img = new int[width*height*32/8];

		for (int i = height - 1; i >= 0; --i)
			is.read(reinterpret_cast<char*>(img)+width*i*32/8,width*32/8);

		is.close();
	}
	Sprite(char* fname,int trColor) : img(NULL),x(0),y(0),TransparentColor(trColor),name(fname)
	{
		std::ifstream is (fname,std::ios::binary);
		is.seekg(18);
		is.read(reinterpret_cast<char*>(&width),sizeof(width));
		is.read(reinterpret_cast<char*>(&height),sizeof(height));
		is.seekg(28,std::ios::cur);
		img = new int[width*height*32/8];

		for (int i = height - 1; i >= 0; --i)
			is.read(reinterpret_cast<char*>(img)+width*i*32/8,width*32/8);

		is.close();
	}


	~Sprite()
	{
		if (img != NULL)
			delete [] img;
		img = NULL;
	}

	void DrawIntObject(D3DLOCKED_RECT& lockedRect)
	{
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				if (img[j+i*width] != TransparentColor)
					memcpy(reinterpret_cast<char*>(lockedRect.pBits)+x*4+j*4+i*lockedRect.Pitch+y*lockedRect.Pitch,reinterpret_cast<char*>(&img[j+i*width]),4);
	}

	void Rotate()
	{
		std::ifstream is (name,std::ios::binary);
		is.seekg(18);
		is.read(reinterpret_cast<char*>(&width),sizeof(width));
		is.read(reinterpret_cast<char*>(&height),sizeof(height));
		is.seekg(28,std::ios::cur);
		im = new int[width*height*32/8];

		for (int i = height-1; i >= 0; --i)
			is.read(reinterpret_cast<char*>(im)+width*i*32/8,width*32/8);

		is.close();
		//++++

	int j1=width;

	for (int i = 0; i < height; ++i)
	{
			for (int j = 0; j < width; ++j)
			{
			img[j+i*width]=im[j1+i*width];

			j1--;
			}

			j1=width;
	}
	}

	void cut(int xI,int yI,int wI,int hI)
	{
		int* im1 = new int[width*height*32/8];
		img=new int[wI*hI*32/8];

		//++++
		std::ifstream is (name,std::ios::binary);
		is.seekg(18);
		is.read(reinterpret_cast<char*>(&width),sizeof(width));
		is.read(reinterpret_cast<char*>(&height),sizeof(height));
		is.seekg(28,std::ios::cur);
		im1 = new int[width*height*32/8];

		for (int i = height-1; i >= 0; --i)
			is.read(reinterpret_cast<char*>(im1)+width*i*32/8,width*32/8);

		is.close();
		//++++

		for (int i = yI; i < yI+hI; ++i)
			for (int j = xI; j < xI+wI; ++j)
					img[(j-xI)+(i-yI)*wI]=im1[j+i*width];

		width=wI;
		height=hI;

	}

	void RemplaceColor(int from,int to)
	{
	for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				if (img[j+i*width] == from)
					img[j+i*width]=to;
	}
};
class Param
{
public:
	D3DLOCKED_RECT rectangle;
	RECT rectSize;
	IDirect3DSurface9* backBuffer;

public:
	Param()
	{
		backBuffer=NULL;
	}

	void Draw(int x,int y,int w,int h,Sprite* Image)
	{
	  rectSize.left =x;
      rectSize.top =y;
	  rectSize.right = rectSize.left+w;
	  rectSize.bottom =rectSize.top+ h;

	  backBuffer->LockRect(&rectangle,&rectSize,0);		
	  Image->DrawIntObject(rectangle);
	  backBuffer->UnlockRect();
	}
};

int w=800;
int h=600;

#include "class.h"

int mW=40;
int mH=30;

int left=(1280-w)/2;
int top=(800-h)/2;

int X,Y;
int mX,mY;
bool lmb=false;

//Main menu+++
bool Menu=true;
bool end=true;

Sprite* MainMenu;
Button* btnMain[4];

//Main menu----------------------
HWND hWnd;

IDirect3D9* d3d = NULL;
IDirect3DDevice9* videocard = NULL;
D3DPRESENT_PARAMETERS pp;
HRESULT hr;

IDirectInputDevice8* keyboard;
IDirectInput8* di;

char buffer[256];

Param* paramDraw;
//Global variables---------------



#include "Playing.h"
#include "Create_map.h"

void Draw()
{
	 //MAIN MENU+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 if (Menu==true)
	 {
		 paramDraw->Draw(0,0,w,h,MainMenu);

	 for (int i=0;i<4;i++)
		btnMain[i]->Draw(paramDraw);
	 }
	 //MAIN MENU-------------------------------------------------------

	 //PLAY++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 if (Play==true)
		DrawPlay();
	 //PLAY-----------------------------------------------------------

	 //CREATE MAP+++++++++++++++++++++++++++++++++++++++++++++++++++++
	 if (Create_map==true)
		Draw_Create();
	 //CREATE MAP-----------------------------------------------------------
}

void Initialisation()
{
//Main menu+++
MainMenu=new Sprite("Images/main_menu.bmp");

btnMain[PLAY]=new Button("Images/btn_Play.bmp");
btnMain[PLAY]->x=271;
btnMain[PLAY]->y=182;
btnMain[PLAY]->show=false;

btnMain[CREATE_MAP]=new Button("Images/btn_Map.bmp");
btnMain[CREATE_MAP]->x=271;
btnMain[CREATE_MAP]->y=280;
btnMain[CREATE_MAP]->show=false;

btnMain[SETTINGS]=new Button("Images/btn_Settings.bmp");
btnMain[SETTINGS]->x=271;
btnMain[SETTINGS]->y=378;
btnMain[SETTINGS]->show=false;

btnMain[EXIT]=new Button("Images/btn_Exit.bmp");
btnMain[EXIT]->x=271;
btnMain[EXIT]->y=476;
btnMain[EXIT]->show=false;

//Play+++
char* tt="score:0";//delete
txt=new Text(tt,0xFF808E9B,8,43);//score

//Rectangle+++
dr=new DrawRectangle(5,5,40,50,0xff000000,0xffffffff);
//Rectangle---

missions=new Sprite("Images/missions.bmp");
Menu_pause=new Sprite("Images/Pause_menu.bmp");

btnOfMissions[0]=new Button("Images/m1.bmp");
btnOfMissions[0]->x=22;
btnOfMissions[0]->y=63;
btnOfMissions[0]->show=false;

pause=new Sprite("Images/pause.bmp",0xffffffff);
blocked=new Sprite("Images/blocked.bmp",0xffffffff);


//Locker+++
lock=new Button("Images/code.bmp",0xffffffff);
lock->x=(w-lock->w)/2;
lock->y=(h-lock->h)/2;
lock->show=true;


//Locker---

Back=new Button("Images/back.bmp",0xffffffff);
Back->x=15;
Back->y=554;
Back->show=false;

Inv=new Inventar();
Inv->x=0;
Inv->y=0;
Inv->show=false;

//CREATE MAP+++
Init_CreateMap();

PM[PM_CONTINUE]=new Button("Images/pm_continue.bmp");
PM[PM_CONTINUE]->x=(w-Menu_pause->width)/2+79;
PM[PM_CONTINUE]->y=(h-Menu_pause->height)/2+55;
PM[PM_CONTINUE]->show=false;

PM[PM_SAVE]=new Button("Images/pm_save.bmp");
PM[PM_SAVE]->x=(w-Menu_pause->width)/2+79;
PM[PM_SAVE]->y=(h-Menu_pause->height)/2+101;
PM[PM_SAVE]->show=false;

PM[PM_SETTINGS]=new Button("Images/pm_settings.bmp");
PM[PM_SETTINGS]->x=(w-Menu_pause->width)/2+79;
PM[PM_SETTINGS]->y=(h-Menu_pause->height)/2+149;
PM[PM_SETTINGS]->show=false;

PM[PM_EXIT]=new Button("Images/pm_exit.bmp");
PM[PM_EXIT]->x=(w-Menu_pause->width)/2+79;
PM[PM_EXIT]->y=(h-Menu_pause->height)/2+200;
PM[PM_EXIT]->show=false;
}

void InitialSys(HINSTANCE hInstance)
{
	paramDraw=new Param();

d3d = Direct3DCreate9(D3D_SDK_VERSION);

	ZeroMemory(&pp,sizeof(pp));

	pp.BackBufferWidth = w;
	pp.BackBufferHeight = h;
	pp.BackBufferFormat = D3DFMT_X8R8G8B8;
	pp.BackBufferCount = 1;
	pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.hDeviceWindow = hWnd;
	pp.Windowed = true;
	pp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&pp, &videocard);

	
	DirectInput8Create(hInstance,DIRECTINPUT_VERSION, IID_IDirectInput8,(void**)&di, NULL);
	di->CreateDevice(GUID_SysKeyboard,&keyboard,NULL);
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	keyboard->Acquire();

	ZeroMemory(buffer,sizeof(buffer));
		

	videocard->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&paramDraw->backBuffer);
}
void MAINMENU()
{
	if (Menu==true)
	{
		//Mouse move+++
		for (int i=0;i<4;i++)
		{
		if (btnMain[i]->Touch(mX,mY)==true)
			btnMain[i]->show=true;
		else
			btnMain[i]->show=false;
		}
		//Mouse move---

		//Click+++
		if (btnMain[PLAY]->Touch(X,Y)==true && lmb==true)
		{
			Play=true;
			Missions=true;
			Menu=false;
		}

		if (btnMain[CREATE_MAP]->Touch(X,Y)==true && lmb==true)
		{
			Create_map=true;
			Menu=false;
		}

		if (btnMain[SETTINGS]->Touch(X,Y)==true  && lmb==true)
		{
			Menu=false;
		}

		//Click---
	}

		//Procedures+++++++++
		PLAYING();
		CREATING();
}

LRESULT _stdcall WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

int _stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lCmdLine,int nCmdShow)
{
	Initialisation();

	WNDCLASS wc;
	HICON hMyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH)(6);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hIcon=hMyIcon;
	wc.hInstance=hInstance;
	wc.lpfnWndProc=WndProc;
	wc.lpszClassName="class";
	wc.lpszMenuName=NULL;
	wc.style=CS_OWNDC;

	RegisterClass(&wc);

	hWnd=CreateWindow("class","Game",WS_POPUP,left,top,w,h,NULL,NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	InitialSys(hInstance);

	while(end)
	{
	keyboard->Acquire();

		lmb = false;
		rmb = false;

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
		
			if (msg.message==WM_DESTROY)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_LBUTTONUP)
			{
				X = msg.pt.x-left;
				Y = msg.pt.y-top;

				lmb = true;

				Inventar::SetPosition(X,Y);
				Chest::SetPosition(X,Y);
			}

			if (msg.message == WM_RBUTTONUP)
			{
				X = msg.pt.x-left;
				Y = msg.pt.y-top;

				rmb = true;
			}

			mX = msg.pt.x-left;
			mY = msg.pt.y-top;
		}

		hr = keyboard->GetDeviceState(sizeof(buffer),buffer);

		 //Timer+++
			if (timer1 == 0)
				timer1 = timeGetTime();

			timer = timeGetTime();
			dt = timer - timer1;
		//Timer---		


		MAINMENU();

		if (btnMain[EXIT]->Touch(X,Y)==true && Menu==true)
			return 0;

		videocard->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(255,255,255),1.0f,0);
		Draw();
	    videocard->Present(NULL,NULL,NULL,NULL);
		Sleep(20);
	}

	
	if (keyboard != NULL)
		keyboard->Release();
	if (di != NULL)
		di->Release();
	if (d3d != NULL)
		d3d->Release();
	if (videocard != NULL)
		videocard->Release();
	UnregisterClass("class",hInstance);

	return 0;
}

LRESULT _stdcall WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
switch(message)
{
case WM_DESTROY:
	PostQuitMessage(0);
	break;
}

return DefWindowProc(hWnd,message,wParam,lParam);
}