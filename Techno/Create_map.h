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

//CREATING+++
void Draw_Create()
{
	paramDraw->Draw(0,0,white->width,white->height,white);

			  //MAP++++
		  if (setka==true)
			  paramDraw->Draw(0,0,CMAP->width,CMAP->height,CMAP);

		      paramDraw->Draw(0,0,Layer[BACK_LAYER]->width,Layer[BACK_LAYER]->height,Layer[BACK_LAYER]);
		      paramDraw->Draw(0,0,lay->width,lay->height,lay);
			  paramDraw->Draw(0,0,Layer[FOREGROUND_LAYER]->width,Layer[FOREGROUND_LAYER]->height,Layer[FOREGROUND_LAYER]);
			  //MAP----

			  //RED SQUARE+++
			  if (first==1)
				  paramDraw->Draw(p.X*20,p.Y*20,redSQ->width,redSQ->height,redSQ);
			  //RED SQUARE---

			  //TOOLS++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			  if (UseTools==true)
			  {
				  paramDraw->Draw(0,h-tools->height,tools->width,tools->height,tools);

			  //FOCUS ON PANEL'S BUTTONS+++
			  for (int i=0;i<6;i++)
			  {
				  btnT[i]->Draw(paramDraw);

				  if (btnT[i]->showBorder==true)
					  paramDraw->Draw( btnT[i]->x,btnT[i]->y, btnT[i]->w, btnT[i]->h,btnT[i]->Border);
			  }

			    for (int i=0;i<3;i++)
				{
					 btn_tool[i]->Draw(paramDraw);

				    if (btn_tool[i]->showBorder==true)
					  paramDraw->Draw( btn_tool[i]->x,btn_tool[i]->y, btn_tool[i]->w, btn_tool[i]->h,btn_tool[i]->Border);
				}
			  //FOCUS ON PANEL'S BUTTONS--

			  //CHOISE COLOR+++
			  if (colorSelector->show==true)
				  paramDraw->Draw(colorSelector->Button::x,colorSelector->Button::y,colorSelector->width,colorSelector->height,colorSelector->Image);


			  for (int i=1;i>=0;i--)
				  if (colorSel[i]->show==true)
					  paramDraw->Draw(colorSel[i]->Button::x,colorSel[i]->Button::y,colorSel[i]->width,colorSel[i]->height,colorSel[i]->Image);
			  //CHOISE COLOR---


			  check->Draw(paramDraw);
			  
			  //Textured blocks+++
			  if (TypeOFBlock==1)
			  {
			  for (int i=0;i<4;i++)
					  block[i]->Draw(paramDraw);
			  }

			  if (TypeOFBlock==2)
			  {
			  BackTextures->Draw(paramDraw);

			  for (int i=0;i<3;i++)
					  backBlock[i]->Draw(paramDraw);
			  }
			  //Textured blocks---
			  //TOOLS--------------------------------------------------------------------------------------
}
}

void Init_CreateMap()
{
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