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

Sprite* MainMenu;
Button* btnMain[4];
//Main menu---



//Create map+++
bool Create_map=false;
bool UseTools=false;
bool setka=true;
bool rmb=false;

int colorFirst=0xFFE5E5E5;
int colorLast=0xFF7F7F7F;

int CMap[30][40];
int CMapBack[30][40];

int TypeOFBlock=0;
int TypeOFAdd=0;
int first=0;
int Color1;
int Color2;
int MiniMap[60][80];

char* block_material;
char* blockBack_material;
char* foregroundList[]={"Images/square.bmp","Images/squareStone.bmp","Images/squareMetal.bmp","Images/squareWood.bmp"};
char* backList[]={"Images/squareBack.bmp","Images/squareB1.bmp","Images/squareB2.bmp"};

Sprite* CMAP;
Sprite* tools;
Sprite* square;
Sprite* redSQ;
Sprite* white;

NeoElement* colorSel[2];
NeoElement* colorSelector;
NeoElement* squares[30][40];
NeoElement* squaresBack[30][40];

Button* btnT[5];
Button* btn_tool[3];
Button* check;
Button* block[4];
Button* backBlock[3];
Button* BackTextures;
Button* btn3[1];

NeoSprite* Everythink;
NeoSprite* Layer[2];
NeoSprite* lay;

Tochka p;
//Global variables------------------------------------------------------

//Global variables+++
IDirect3DSurface9* backBuffer = NULL;
D3DLOCKED_RECT rectangle;
RECT rectSize;

char buffer[256];
//Global variables---

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
	 {
		DrawPlay();
	 }
	 //PLAY-----------------------------------------------------------


	 //CREATE MAP+++++++++++++++++++++++++++++++++++++++++++++++++++++
	 if (Create_map==true)
	 {
		 resRect(0,0,white->width,white->height,white);

			  //MAP++++
		  if (setka==true)
			  resRect(0,0,CMAP->width,CMAP->height,CMAP);

		      resRect(0,0,Layer[BACK_LAYER]->width,Layer[BACK_LAYER]->height,Layer[BACK_LAYER]);
		      resRect(0,0,lay->width,lay->height,lay);
			  resRect(0,0,Layer[FOREGROUND_LAYER]->width,Layer[FOREGROUND_LAYER]->height,Layer[FOREGROUND_LAYER]);
			  //MAP----

			  //RED SQUARE+++
			  if (first==1)
				  resRect(p.X*20,p.Y*20,redSQ->width,redSQ->height,redSQ);
			  //RED SQUARE---

			  //TOOLS++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			  if (UseTools==true)
			  {
				  resRect(0,h-tools->height,tools->width,tools->height,tools);

			  //FOCUS ON PANEL'S BUTTONS+++
			  for (int i=0;i<6;i++)
			  {
					  btnT[i]->Draw(rectangle,rectSize,backBuffer);

				  if (btnT[i]->showBorder==true)
					  resRect( btnT[i]->x,btnT[i]->y, btnT[i]->w, btnT[i]->h,btnT[i]->Border);
			  }

			    for (int i=0;i<3;i++)
				{
					 btn_tool[i]->Draw(rectangle,rectSize,backBuffer);

				    if (btn_tool[i]->showBorder==true)
					  resRect( btn_tool[i]->x,btn_tool[i]->y, btn_tool[i]->w, btn_tool[i]->h,btn_tool[i]->Border);
				}
			  //FOCUS ON PANEL'S BUTTONS--

			  //CHOISE COLOR+++
			  if (colorSelector->show==true)
				  resRect(colorSelector->Button::x,colorSelector->Button::y,colorSelector->width,colorSelector->height,colorSelector->Image);


			  for (int i=1;i>=0;i--)
				  if (colorSel[i]->show==true)
					  resRect(colorSel[i]->Button::x,colorSel[i]->Button::y,colorSel[i]->width,colorSel[i]->height,colorSel[i]->Image);
			  //CHOISE COLOR---


			  check->Draw(rectangle,rectSize,backBuffer);
			  
			  //Textured blocks+++
			  if (TypeOFBlock==1)
			  {
			  for (int i=0;i<4;i++)
					  block[i]->Draw(rectangle,rectSize,backBuffer);
			  }

			  if (TypeOFBlock==2)
			  {
			  BackTextures->Draw(rectangle,rectSize,backBuffer);

			  for (int i=0;i<3;i++)
					  backBlock[i]->Draw(rectangle,rectSize,backBuffer);
			  }
			  //Textured blocks---
			  //TOOLS--------------------------------------------------------------------------------------
	 }
}
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
missions=new Sprite("Images/missions.bmp");
Menu_pause=new Sprite("Images/Pause_menu.bmp");

btnOfMissions[0]=new Button("Images/m1.bmp");
btnOfMissions[0]->x=22;
btnOfMissions[0]->y=63;
btnOfMissions[0]->show=false;

pause=new Sprite("Images/pause.bmp",0xffffffff);
blocked=new Sprite("Images/blocked.bmp",0xffffffff);
//Create map+++
Everythink=new NeoSprite("Images/white.bmp",0xffffffff);

for (int i=0;i<30;i++)
{
for (int j=0;j<40;j++)
{
CMap[i][j]=0;
CMapBack[i][j]=0;
}
}

CMAP=new Sprite("Images/map.bmp",0xffffffff);
tools=new Sprite("Images/map_tools.bmp",0xffffffff);
square=new Sprite("Images/square.bmp",0xffffffff);
redSQ=new Sprite("Images/redSQ.bmp",0xffffffff);
white=new Sprite("Images/white.bmp",0xffffffff);


btnT[SOLID_BLOCK]=new Button("Images/btnT1.bmp");
btnT[SOLID_BLOCK]->x=8;
btnT[SOLID_BLOCK]->y=8+(h-tools->height);
btnT[SOLID_BLOCK]->show=false;
btnT[SOLID_BLOCK]->Border=new Sprite("Images/border.bmp",0xffffffff);
btnT[SOLID_BLOCK]->showBorder=false;

btnT[DELETE_BLOCK]=new Button("Images/btnT2.bmp");
btnT[DELETE_BLOCK]->x=8;
btnT[DELETE_BLOCK]->y=38+(h-tools->height);
btnT[DELETE_BLOCK]->show=false;
btnT[DELETE_BLOCK]->Border=new Sprite("Images/border.bmp",0xffffffff);
btnT[DELETE_BLOCK]->showBorder=false;

btnT[BACKGROUND_BLOCK]=new Button("Images/btnT3.bmp");
btnT[BACKGROUND_BLOCK]->x=8;
btnT[BACKGROUND_BLOCK]->y=68+(h-tools->height);
btnT[BACKGROUND_BLOCK]->show=false;
btnT[BACKGROUND_BLOCK]->Border=new Sprite("Images/border.bmp",0xffffffff);
btnT[BACKGROUND_BLOCK]->showBorder=false;

btnT[SAVE]=new Button("Images/btnT4.bmp");
btnT[SAVE]->x=40;
btnT[SAVE]->y=8+(h-tools->height);
btnT[SAVE]->show=false;
btnT[SAVE]->Border=new Sprite("Images/border.bmp",0xffffffff);
btnT[SAVE]->showBorder=false;

btnT[SMALL_SQUARE]=new Button("Images/btnT5.bmp");
btnT[SMALL_SQUARE]->x=40;
btnT[SMALL_SQUARE]->y=38+(h-tools->height);
btnT[SMALL_SQUARE]->show=false;
btnT[SMALL_SQUARE]->Border=new Sprite("Images/border.bmp",0xffffffff);
btnT[SMALL_SQUARE]->showBorder=false;

btnT[STAIRS]=new Button("Images/btnT6.bmp");
btnT[STAIRS]->x=40;
btnT[STAIRS]->y=68+(h-tools->height);
btnT[STAIRS]->show=false;
btnT[STAIRS]->Border=new Sprite("Images/border.bmp",0xffffffff);
btnT[STAIRS]->showBorder=false;


btn_tool[POINT]=new Button("Images/btn_tool1.bmp");
btn_tool[POINT]->x=82;
btn_tool[POINT]->y=8+(h-tools->height);
btn_tool[POINT]->show=false;
btn_tool[POINT]->Border=new Sprite("Images/border.bmp",0xffffffff);
btn_tool[POINT]->showBorder=false;

btn_tool[LINE]=new Button("Images/btn_tool2.bmp");
btn_tool[LINE]->x=82;
btn_tool[LINE]->y=38+(h-tools->height);
btn_tool[LINE]->show=false;
btn_tool[LINE]->Border=new Sprite("Images/border.bmp",0xffffffff);
btn_tool[LINE]->showBorder=false;

btn_tool[RECTANGLE]=new Button("Images/btn_tool3.bmp");
btn_tool[RECTANGLE]->x=82;
btn_tool[RECTANGLE]->y=68+(h-tools->height);
btn_tool[RECTANGLE]->show=false;
btn_tool[RECTANGLE]->SubShow=false;
btn_tool[RECTANGLE]->Border=new Sprite("Images/border.bmp",0xffffffff);
btn_tool[RECTANGLE]->showBorder=false;

check=new Button("Images/check.bmp");
check->x=123;
check->y=11+(h-tools->height);
check->show=true;
check->SubShow=true;

colorSel[0]=new NeoElement("Images/colorSQ.bmp");
colorSel[0]->Button::x=731;
colorSel[0]->Button::y=24+(h-tools->height);
colorSel[0]->show=false;
colorSel[0]->ChangeColor(colorSel[0]->FollowColor(2,2),colorFirst);

colorSel[1]=new NeoElement("Images/colorSQ.bmp");
colorSel[1]->Button::x=747;
colorSel[1]->Button::y=40+(h-tools->height);
colorSel[1]->show=false;
colorSel[1]->ChangeColor(colorSel[1]->FollowColor(2,2),colorLast);

colorSelector=new NeoElement("Images/color_tools.bmp");
colorSelector->Button::x=w-colorSelector->width;
colorSelector->Button::y=h-tools->height-colorSelector->height;
colorSelector->show=false;

//++++
for (int i=0;i<4;i++)
{
block[i]=new Button("Images/redSelect.bmp",0xffffffff);
block[i]->x=237+24*i;
block[i]->y=17+(h-tools->height);
block[i]->show=false;
}
//----

//+++
for (int i=0;i<3;i++)
{
backBlock[i]=new Button("Images/redSelect.bmp",0xffffffff);
backBlock[i]->x=237+24*i;
backBlock[i]->y=17+(h-tools->height);
backBlock[i]->show=false;
}
//---

block_material="Images/square.bmp";
blockBack_material="Images/squareBack.bmp";


BackTextures=new Button("Images/backTextures.bmp");
BackTextures->x=229;
BackTextures->y=10+(h-tools->height);
BackTextures->show=true;

Layer[BACK_LAYER]=new NeoSprite("Images/white.bmp",0xffffffff);
Layer[FOREGROUND_LAYER]=new NeoSprite("Images/white.bmp",0xffffffff);
lay=new NeoSprite("Images/white.bmp",0xffffffff);

btn3[TEST_THE_MAP]=new Button("Images/btn3_1.bmp");
btn3[TEST_THE_MAP]->x=425;
btn3[TEST_THE_MAP]->y=10+(h-tools->height);
btn3[TEST_THE_MAP]->show=false;
}


//CREATING+++
void Save()
{
std::ofstream oText("Save/map1.txt");
			char a='0';

			for (int i=0;i<mH;i++)
			{
				for (int j=0;j<mW;j++)
				{
					if (CMap[i][j]==0)
					a='0';

					if (CMap[i][j]==1)
					a='1';

					if (CMap[i][j]==2)
					a='2';

					if (CMap[i][j]==3)
					a='3';

          oText.write(&a,sizeof(a));
				}
			}

			a='|';
			oText.write(&a,sizeof(a));

			for (int i=0;i<mH*2;i++)
			{
				for (int j=0;j<mW*2;j++)
				{
					if (MiniMap[i][j]==0)
					a='0';

					if (MiniMap[i][j]==1)
					a='1';

					if (MiniMap[i][j]==2)
					a='2';

					if (MiniMap[i][j]==3)
					a='3';

          oText.write(&a,sizeof(a));
				}
			}

			oText.close();

			Everythink->AddImage(0,0,Layer[BACK_LAYER]);
			Everythink->AddImage(0,0,lay);
			Everythink->AddImage(0,0,Layer[FOREGROUND_LAYER]);
			Everythink->Save("Save/map1.bmp");

			btnT[SAVE]->showBorder=false;
}
void Focus()
{
		for (int i=0;i<6;i++)
		{
			if (btnT[i]->Touch(mX,mY)==true)
				btnT[i]->show=true;
			else
				btnT[i]->show=false;
		}

		for (int i=0;i<3;i++)
		{
			if (btn_tool[i]->Touch(mX,mY)==true)
				btn_tool[i]->show=true;
			else
				btn_tool[i]->show=false;
		}

			for (int i=0;i<1;i++)
		{
			if (btn3[i]->Touch(mX,mY)==true)
				btn3[i]->show=true;
			else
				btn3[i]->show=false;
		}


}
void UseToolsInPanel()
{
if (UseTools==true)
		{
		Focus();

		//CLICK+++
		if (btnT[SOLID_BLOCK]->Touch(X,Y)==true)
		{
			for (int i=0;i<6;i++)
			btnT[i]->showBorder=false;

			btnT[SOLID_BLOCK]->showBorder=true;

			TypeOFBlock=1;
		}

		if (btnT[DELETE_BLOCK]->Touch(X,Y)==true)
		{
			for (int i=0;i<6;i++)
			btnT[i]->showBorder=false;

			btnT[DELETE_BLOCK]->showBorder=true;

			TypeOFBlock=0;
		}

		if (btnT[BACKGROUND_BLOCK]->Touch(X,Y)==true)
		{
			for (int i=0;i<6;i++)
			btnT[i]->showBorder=false;

			btnT[BACKGROUND_BLOCK]->showBorder=true;

			TypeOFBlock=2;
		}

		if (btnT[SAVE]->Touch(X,Y)==true)
		{
			for (int i=0;i<6;i++)
			btnT[i]->showBorder=false;

			btnT[SAVE]->showBorder=true;

			//Saving+++
			Save();
			//Saving---
		}

		if (btnT[SMALL_SQUARE]->Touch(X,Y)==true)
		{
			for (int i=0;i<6;i++)
			btnT[i]->showBorder=false;

			btnT[SMALL_SQUARE]->showBorder=true;

			TypeOFBlock=4;
		}
		
		if (btnT[STAIRS]->Touch(X,Y)==true)
		{
			for (int i=0;i<6;i++)
			btnT[i]->showBorder=false;

			btnT[STAIRS]->showBorder=true;

			TypeOFBlock=3;
		}

		//+++++++++
		if (btn_tool[POINT]->Touch(X,Y)==true)
		{
			for (int i=0;i<3;i++)
			btn_tool[i]->showBorder=false;

			btn_tool[POINT]->showBorder=true;

			TypeOFAdd=0;
		}

		if (btn_tool[LINE]->Touch(X,Y)==true)
		{
			for (int i=0;i<3;i++)
			btn_tool[i]->showBorder=false;

			btn_tool[LINE]->showBorder=true;

			TypeOFAdd=1;
			first=0;
		}

		if (btn_tool[RECTANGLE]->Touch(X,Y)==true)
		{
			for (int i=0;i<3;i++)
			btn_tool[i]->showBorder=false;

			btn_tool[RECTANGLE]->showBorder=true;

			TypeOFAdd=2;
			first=0;
		}

		//
		
		if (btn3[TEST_THE_MAP]->Touch(X,Y)==true)
		{
			for (int i=0;i<3;i++)
			btn3[i]->showBorder=false;

			btn3[TEST_THE_MAP]->showBorder=true;

		}


		//Кнопка скрытия сетки++++
		if (check->Touch(X,Y)==true && lmb==true)
		{
			if (check->SubShow==true)
			{
				check->SubShow=false;
				check->show=false;
				setka=false;
			}
			else
			if (check->SubShow==false)
			{
				check->SubShow=true;
				check->show=true;
				setka=true;
			}
		}
		//Кнопка скрытия сетки++++


		//Выбор материала блока+++
		for (int i=0;i<4;i++)
			{
		if (block[i]->Touch(X,Y)==true)
		{
			for (int j=0;j<4;j++)
				block[j]->show=false;

				block[i]->show=true;

				block_material=foregroundList[i];
		}
		}
			//++++++++++++++++
			for (int i=0;i<3;i++)
			{
			if (backBlock[i]->Touch(X,Y)==true)
			{
			for (int j=0;j<3;j++)
				backBlock[j]->show=false;

				backBlock[i]->show=true;

				blockBack_material=backList[i];
			}
			}
		//Выбор материала блока---
		//CLICK---
		}
}

void AddBlock(int i,int j)
{
	 Sprite* Empty;
	 Sprite* mini;

	switch(TypeOFBlock)
	{
	case 0: //Delete block
	   CMap[i][j]=0;
	   CMapBack[i][j]=0;

	   Empty=new Sprite("Images/empty.bmp");

	   Layer[FOREGROUND_LAYER]->AddImage(j*20,i*20,Empty,0);
	   Layer[BACK_LAYER]->AddImage(j*20,i*20,Empty,0);
	break;

	case 1: //Add block
            CMap[i][j]=1;

			squares[i][j]=new NeoElement(block_material);

			if (block_material=="Images/square.bmp")
			{
			squares[i][j]->ChangeColor(squares[i][j]->FollowColor(1,1),colorFirst);
			squares[i][j]->ChangeColor(squares[i][j]->FollowColor(0,0),colorLast);
			}
			Layer[FOREGROUND_LAYER]->AddImage(j*20,i*20,squares[i][j]);
    break;


	case 2: //Add back block
            CMapBack[i][j]=2;
			squaresBack[i][j]=new NeoElement(blockBack_material);

			if (blockBack_material=="Images/squareBack.bmp")
			squaresBack[i][j]->ChangeColor(squaresBack[i][j]->FollowColor(0,0),colorFirst);

			Layer[BACK_LAYER]->AddImage(j*20,i*20,squaresBack[i][j]);
	break;

	
	case 3: //Add stairs
            CMap[i][j]=3;
             squares[i][j]=new NeoElement("Images/squareUP.bmp");

			squares[i][j]->ChangeColor(squares[i][j]->FollowColor(1,1),colorFirst);
			squares[i][j]->ChangeColor(squares[i][j]->FollowColor(0,0),colorLast);

			Layer[FOREGROUND_LAYER]->AddImage(j*20,i*20,squares[i][j]);
	break;

	case 4:
	   int Xj=X/10;
	   int Yi=Y/10;

       MiniMap[Yi][Xj]=1;


	   mini=new Sprite("Images/squareSmall.bmp");


	   lay->AddImage(Xj*10,Yi*10,mini);
		break;
	}
}
void ModifyTheMap()
{
if (lmb==true && UseTools==false)
		{
			int mapX=X/20;
			int mapY=Y/20;


			//POINT+++
			if (TypeOFAdd==0)
		    AddBlock(mapY,mapX);
			//POINT---

			//LINE+++
			if (TypeOFAdd==1)
			{
		    int mapX=X/20;
			int mapY=Y/20;


			if (first==0)
			{
			p.X=mapX;
			p.Y=mapY;

			first=1;
			}
			else
			   if (first==1)
			   {
			      int X1[2];
			      int Y1[2];

				  X1[0]=mapX;
				  Y1[0]=mapY;
				  
				  X1[1]=p.X;
				  Y1[1]=p.Y;

				  if (mapX>p.X)
				  {
				  X1[0]=p.X;
				  X1[1]=mapX;
				  }

				   if (mapY>p.Y)
				  {
				  Y1[0]=p.Y;
				  Y1[1]=mapY;
				  }


				   if (X1[0]==X1[1] || Y1[0]==Y1[1])
				   for (int i=Y1[0];i<Y1[1]+1;i++)
				   for (int j=X1[0];j<X1[1]+1;j++)
					    AddBlock(i,j);

				   first=0;
		     	}


			}
			//LINE---

			//RECTANGLE+++
			if (TypeOFAdd==2)
			{
		    int mapX=X/20;
			int mapY=Y/20;

			

			if (first==0)
			{
			p.X=mapX;
			p.Y=mapY;

			first=1;
			}
			else
			   if (first==1)
			   {
			      int X1[2];
			      int Y1[2];

				  X1[0]=mapX;
				  Y1[0]=mapY;
				  
				  X1[1]=p.X;
				  Y1[1]=p.Y;

				  if (mapX>p.X)
				  {
				  X1[0]=p.X;
				  X1[1]=mapX;
				  }

				   if (mapY>p.Y)
				  {
				  Y1[0]=p.Y;
				  Y1[1]=mapY;
				  }


				   for (int i=Y1[0];i<Y1[1]+1;i++)
				   for (int j=X1[0];j<X1[1]+1;j++)
					    AddBlock(i,j);


				   first=0;
		     	}


			}
			//RECTANGLE---
		}
}

void CREATING()
{
	if (Create_map==true)
	{
		//SELECT COLOR+++
		if (UseTools==true)
		if (colorSel[0]->Touch(X,Y)==true)
		colorSelector->show=true;

		//FIRST+++
		if (colorSelector->Touch(X,Y)==true && colorSelector->show==true && lmb==true)
		{
	 	colorSel[0]->ChangeColor(colorSel[0]->FollowColor(2,2),colorSelector->FollowColor(X-colorSelector->Button::x,Y-colorSelector->Button::y));
		
		colorFirst=colorSelector->FollowColor(X-colorSelector->Button::x,Y-colorSelector->Button::y);
		}
		//FIRST---

		//LAST+++
		if (colorSelector->Touch(X,Y)==true && colorSelector->show==true && rmb==true)
		{
	 	colorSel[1]->ChangeColor(colorSel[1]->FollowColor(2,2),colorSelector->FollowColor(X-colorSelector->Button::x,Y-colorSelector->Button::y));
		
		colorLast=colorSelector->FollowColor(X-colorSelector->Button::x,Y-colorSelector->Button::y);
		}
		//LAST---
		//SELECT COLOR---

		//NET+++
		if (buffer[DIK_H] & 0x80)
		{
				check->SubShow=false;
				check->show=false;
				setka=false;
		}
		if (buffer[DIK_S] & 0x80)
		{
			check->SubShow=true;
				check->show=true;
				setka=true;
		}
		//NET---

		//Show panel+++
		if (buffer[DIK_SPACE] & 0x80)
		{
		UseTools=true;

		colorSel[0]->show=true;
		colorSel[1]->show=true;

		if (check->SubShow==true)
			check->show=true;
		}
		//Show panel+++

		//Hide panel+++
		if (X>=0 && X<=w && Y>=0 && Y<=h-tools->height && lmb==true && ((colorSelector->show==true && colorSelector->Touch(X,Y)==false) || (colorSelector->show==false)))
		{
		UseTools=false;
			
		colorSel[0]->show=false;
		colorSel[1]->show=false;
		colorSelector->show=false;
		}
		//Hide panel---


		//Use tools in panel+++
		UseToolsInPanel();
		//Use tools in panel---
		

		//MODIFY THE MAP+++
		ModifyTheMap();
		//MODIFY THE MAP---

		//EXIT++++
		if (buffer[DIK_ESCAPE] & 0x80)
		{
			Create_map=false;
		    Menu=true;
		}
		//EXIT----
	}
}
//CREATING---
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

	HWND hWnd=CreateWindow("class","Game",WS_POPUP,left,top,w,h,NULL,NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	//++++++++++++++
	IDirect3D9* d3d = NULL;
	IDirect3DDevice9* videocard = NULL;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS pp;
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

	IDirectInputDevice8* keyboard;
	IDirectInput8* di;
	DirectInput8Create(hInstance,DIRECTINPUT_VERSION, IID_IDirectInput8,(void**)&di, NULL);
	di->CreateDevice(GUID_SysKeyboard,&keyboard,NULL);
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	keyboard->Acquire();

	ZeroMemory(buffer,sizeof(buffer));
		
	HRESULT hr;

	videocard->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backBuffer);
	//++++++++++++++

	while(true)
	{
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