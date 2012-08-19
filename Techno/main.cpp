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


//Classes+++
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
class Button
{
public:
	int x,y;
	int w,h;

	bool show;
	bool SubShow;
	bool showBorder;

	Sprite* Image;
	Sprite* Border;

public:
	Button(int _x,int _y,char* name):x(_x),y(_y)
	{
    Image=new Sprite(name);
	w=Image->width;
	h=Image->height;
	}
	Button(char* name)
	{
    Image=new Sprite(name);
	w=Image->width;
	h=Image->height;
	}
	Button(char* name,int trColor)
	{
	Image=new Sprite(name,trColor);
	w=Image->width;
	h=Image->height;
	}

	bool Touch(int X,int Y)
	{
	bool t=false;

	if (X>=x && X<=x+w && Y>=y && Y<=y+h)
	t=true;

	return t;
	}

	void Draw(D3DLOCKED_RECT rectangle,RECT rectSize,IDirect3DSurface9* backBuffer)
	{
		if (show==true)
		{
	 rectSize.left =x;
     rectSize.top =y;
	 rectSize.right = rectSize.left+w;
	 rectSize.bottom =rectSize.top+ h;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 Image->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
		}
	}
};

class NeoElement:public Sprite,public Button
{
public:
	NeoElement(char* name):Sprite(name),Button(name)
	{}
	NeoElement(char* name,int trColor):Sprite(name,trColor),Button(name,trColor)
	{}

	void ChangeColor(int from,int to)
	{
	for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				if (Button::Image->img[j+i*width] == from)
					Button::Image->img[j+i*width]=to;
	}
	int FollowColor(int X,int Y)
	{
	int color=Button::Image->img[X+Y*width];

	return color;
	}
};
class NeoSprite:public Sprite
{
public: 
	int w,h;

public:
	NeoSprite(char* name):Sprite(name)
	{
		w=width;
		h=height;
	}
	NeoSprite(char* name,int trColor):Sprite(name,trColor)
	{
		w=width;
		h=height;
	}


	void AddImage(int x,int y,Sprite* sp)
	{
		for (int i = y; i < y+sp->height; i++)
		{
			for (int j =x; j <x+sp->width; j++)
			{
				if (j<=width && i<=height && sp->img[(j-x)+(i-y)*sp->width]!=TransparentColor)
				img[j+i*width]=sp->img[(j-x)+(i-y)*sp->width];
			}
		}
	}
	void AddImage(int x,int y,Sprite* sp,int i)
	{
		for (int i = y; i < y+sp->height; i++)
		{
			for (int j =x; j <x+sp->width; j++)
			{
				if (j<=width && i<=height)
				img[j+i*width]=sp->img[(j-x)+(i-y)*sp->width];
			}
		}
	}

	int Color(int X,int Y,char letter)
	{
	int color=img[X+Y*width];
    int r,g,b;
	int final=0;

	r = (BYTE)color;
	g = (WORD)color >>8;
	b = (DWORD)color >>16;

	if (letter=='R')
	final=r;
	
	if (letter=='G')
	final=g;
	
	if (letter=='B')
	final=b;

	return final;
	}

	void Save(char* name)
	{
	std::ofstream os(name, std::ios::binary);

unsigned char signature[2] = { 'B', 'M' };
unsigned int fileSize = 14 + 40 + w*h*4;
unsigned int reserved = 0;
unsigned int offset = 14 + 40;

unsigned int headerSize = 40;
unsigned int dimensions[2] = {w,h};
unsigned short colorPlanes = 1;
unsigned short bpp = 32;
unsigned int compression = 0;
unsigned int imgSize = dimensions[0]*dimensions[1]*4;
unsigned int resolution[2] = { 2795, 2795 };
unsigned int pltColors = 0;
unsigned int impColors = 0;

os.write(reinterpret_cast<char*>(signature), sizeof(signature));
os.write(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
os.write(reinterpret_cast<char*>(&reserved), sizeof(reserved));
os.write(reinterpret_cast<char*>(&offset),   sizeof(offset));

os.write(reinterpret_cast<char*>(&headerSize),  sizeof(headerSize));
os.write(reinterpret_cast<char*>(dimensions),   sizeof(dimensions));
os.write(reinterpret_cast<char*>(&colorPlanes), sizeof(colorPlanes));
os.write(reinterpret_cast<char*>(&bpp),         sizeof(bpp));
os.write(reinterpret_cast<char*>(&compression), sizeof(compression));
os.write(reinterpret_cast<char*>(&imgSize),     sizeof(imgSize));
os.write(reinterpret_cast<char*>(resolution),   sizeof(resolution));
os.write(reinterpret_cast<char*>(&pltColors),   sizeof(pltColors));
os.write(reinterpret_cast<char*>(&impColors),   sizeof(impColors));

unsigned char x,r,g,b;

for (int i=dimensions[1]; i >0; --i)
{
  for (int j=0; j < dimensions[0]; ++j)
  {
    x = 0;
	r = Color(j,i,'R');
    g =Color(j,i,'G');
    b =Color(j,i,'B');
    os.write(reinterpret_cast<char*>(&b),sizeof(b));
    os.write(reinterpret_cast<char*>(&g),sizeof(g));
    os.write(reinterpret_cast<char*>(&r),sizeof(r));
    os.write(reinterpret_cast<char*>(&x),sizeof(x));
  }
}

os.close();
	}
};
struct Tochka
{
	int X,Y;
};
//Classes---

//Global variables+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int w=800;
int h=600;

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
IDirect3DSurface9* backBuffer = NULL;
D3DLOCKED_RECT rectangle;
RECT rectSize;
D3DPRESENT_PARAMETERS pp;
HRESULT hr;

IDirectInputDevice8* keyboard;
IDirectInput8* di;

char buffer[256];
//Global variables---------------


void resRect(int l,int t,int r,int b,Sprite* sp)
{
	 rectSize.left =l;
     rectSize.top =t;
	 rectSize.right = rectSize.left+r;
	 rectSize.bottom =rectSize.top+ b;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 sp->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
}

#include "Playing.h"
#include "Create_map.h"

void Draw()
{
	 //MAIN MENU+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 if (Menu==true)
	 {
	 resRect(0,0,w,h,MainMenu);

	 for (int i=0;i<4;i++)
		btnMain[i]->Draw(rectangle,rectSize,backBuffer);
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
txt=new Text(tt,0xFF808E9B,8,43);//delete

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

locker=new Button("Images/locker.bmp",0xffffffff);
locker->x=(w-lock->w)/2+99;
locker->y=(h-lock->h)/2+107;
locker->show=true;

for (int i=0;i<8;i++)
{
nmb[i]=new NumberDraw();

nmb[i]->x=lock->x+i*2+nmb[i]->Image->width*i+162;
nmb[i]->y=lock->y+190;

if (i%3==0 && i!=0)
	nmb[i-1]->num=10;
}

LockerLight=new Button("Images/lockerL.bmp",0xffffffff);
LockerLight->x=locker->x;
LockerLight->y=locker->y;
LockerLight->show=false;

ar=new Arrow();
ar->x=lock->x+145;
ar->y=lock->y+149;

lOK=new Button("Images/lOK.bmp",0xffffffff);
lOK->x=lock->x+156;
lOK->y=lock->y+161;
lOK->show=false;
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
		

	videocard->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backBuffer);
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