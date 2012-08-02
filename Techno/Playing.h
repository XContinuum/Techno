class Hero
{
public:
	int x,y;
	int w,h;
	int countCadr;
	int UCadr;
	int JCadr;
    int CountJ;

	bool MoveL;
	bool MoveR;
	bool MoveD;
	bool MoveU;
	bool Jump;
	bool FallDown;

	char Direction;
	
	Sprite* Image;

	Sprite* ImageR[5];
	Sprite* ImageL[5];
	Sprite* ImageU[4];
	Sprite* ImageJR[4];
	Sprite* ImageJL[4];

	static int dt;
	static int timer;
	static int timer1;

	static int dtj;
	static int timerj;
	static int timerj1;

	int vel;
	int acel;

public:
	Hero():countCadr(0),UCadr(0),JCadr(0),MoveR(false),MoveL(false),MoveD(false),MoveU(false),FallDown(true),CountJ(0),acel(5),vel(0)
	{
	Image=NULL;
	ImageR[0]=new Sprite("Images/heroR1.bmp",0xffffffff);
	ImageR[1]=new Sprite("Images/heroR2.bmp",0xffffffff);
	ImageR[2]=new Sprite("Images/heroR3.bmp",0xffffffff);
	ImageR[3]=new Sprite("Images/heroR4.bmp",0xffffffff);
	ImageR[4]=new Sprite("Images/heroR5.bmp",0xffffffff);
	
	ImageL[0]=new Sprite("Images/heroL1.bmp",0xffffffff);
	ImageL[1]=new Sprite("Images/heroL2.bmp",0xffffffff);
	ImageL[2]=new Sprite("Images/heroL3.bmp",0xffffffff);
	ImageL[3]=new Sprite("Images/heroL4.bmp",0xffffffff);
	ImageL[4]=new Sprite("Images/heroL5.bmp",0xffffffff);

	ImageU[0]=new Sprite("Images/heroU1.bmp",0xffffffff);
	ImageU[1]=new Sprite("Images/heroU2.bmp",0xffffffff);
	ImageU[2]=new Sprite("Images/heroU3.bmp",0xffffffff);
	ImageU[3]=new Sprite("Images/heroU4.bmp",0xffffffff);

	ImageJR[0]=new Sprite("Images/heroJ1.bmp",0xffffffff);
	ImageJR[1]=new Sprite("Images/heroJ2.bmp",0xffffffff);
	ImageJR[2]=new Sprite("Images/heroJ3.bmp",0xffffffff);
	ImageJR[3]=new Sprite("Images/heroJ4.bmp",0xffffffff);

	ImageJL[0]=new Sprite("Images/heroJ1.bmp",0xffffffff);
	ImageJL[1]=new Sprite("Images/heroJ2.bmp",0xffffffff);
	ImageJL[2]=new Sprite("Images/heroJ3.bmp",0xffffffff);
	ImageJL[3]=new Sprite("Images/heroJ4.bmp",0xffffffff);

	for (int i=0;i<4;i++)
	ImageJL[i]->Rotate();


	w=ImageR[0]->width;
	h=ImageR[0]->height;
	Jump=false;

	Image=ImageR[0];
	}

	bool Left(int MatMap[30][40])
	{
		Direction='L';

		w=Image->width;
		h=Image->height;

	bool l=true;
	int BlockSize=20;

	int j1=x/20-2;
	int j2=(x+w)/20+2;
	int i1=y/20-2;
	int i2=(y+h)/20+2;


	for (int i=i1;i<i2;i++)
	{
	for (int j=j1;j<j2;j++)
	{
		if (MatMap[i][j]==1 || MatMap[i][j]==4 || MatMap[i][j]==6)
		{
			if (x>=BlockSize*(j+1) && x<=BlockSize*(j+1)+5 && ((y>=i*BlockSize && y<=BlockSize*(i+1)) || (y+h-2>=i*BlockSize && y+h-2<=BlockSize*(i+1))))
		{
		l=false;

		if (x-BlockSize*(j+1)>1)
		x-=(x-BlockSize*(j+1));
			}
		}
	}
	}

	return l;
	}
	bool Right(int MatMap[30][40])
	{
		Direction='R';

        w=Image->width;
		h=Image->height;

	bool r=true;
	int BlockSize=20;

	int j1=x/20-2;
	int j2=(x+w)/20+2;
	int i1=y/20-2;
	int i2=(y+h)/20+2;


	for (int i=i1;i<i2;i++)
	{
	for (int j=j1;j<j2;j++)
	{
		if (MatMap[i][j]==1 || MatMap[i][j]==4 || MatMap[i][j]==6)
		{
			if (x+w<=BlockSize*j && x+w>=BlockSize*j-5 && ((y>=i*BlockSize && y<=BlockSize*(i+1))||(y+h-2>=i*BlockSize && y+h-2<=BlockSize*(i+1))))
			{
		if (BlockSize*j-(x+w)>0)
		x+=BlockSize*j-(x+w);

		r=false;
			}
		}
	}
	}

	return r;
	}
	bool Gravitation(int MatMap[30][40])
	{
	bool g=true;
	
	if (FallDown==true)
	{
	int BlockSize=20;


	for (int i=0;i<30;i++)
	{
	for (int j=0;j<40;j++)
	{
		if (MatMap[i][j]==1 || MatMap[i][j]==3 || MatMap[i][j]==6)
		{
			if (((x>=BlockSize*j && x<=BlockSize*(j+1))||(x+w>=BlockSize*j && x+w<=BlockSize*(j+1))) && y+h<=i*BlockSize && y+h>=BlockSize*i-5)
			{
		g=false;

		if (MatMap[i][j]!=3)
		{
		if (i*BlockSize-(y+h)>1)
		y+=i*BlockSize-(y+h);
		}
			}
		}
	}
	}
	}
	else
		g=false;

	if (DOWN(MatMap)==1)
		g=false;

	return g;
	}
	bool JUMP(int MatMap[30][40])
	{
	bool j=true;

		if (DOWN(MatMap)==1 || DOWN(MatMap)==3 ||DOWN(MatMap)==6)
		j=false;
	
		return j;
	}

	void Move(char d,int MatMap[30][40])
	{
	switch(d)
	{
	case 'L':
		if (dt>3 && MoveL==true && Left(MatMap)==true)
		{
		if (countCadr<4)
		countCadr++;
		else
			countCadr=0;
		
		Image=ImageL[countCadr];
		x-=5;

    	timer1=0;
	    dt=0;
    	MoveL=false;
		}
		break;

	case 'R':
	if (dt>3 && MoveR==true && Right(MatMap)==true)
	{
			if (countCadr<4)
		countCadr++;
		else
			countCadr=0;
		
		Image=ImageR[countCadr];


	x+=5;


	timer1=0;
	dt=0;
	MoveR=false;
	}
	break;
	

	case 'D':
		if (dt>10 && MoveD==true)
		{
		int eX=x/20;
		int eX2=(x+Image->width)/20;
		int eY=(y+Image->height+5)/20;

		if (MatMap[eY][eX]==1 && MatMap[eY][eX2]==1)
			y=eY*20-Image->height;
		else
		y+=5;
		
		ChangeImageU();

	    timer1=0;
	    dt=0;
	   MoveD=false;
		}
		break;


	case 'U':
		if (dt>10 && MoveU==true)
	   {
		 y-=5;
		
		ChangeImageU();

	//++++
	int mapX=x/20;
	int mapY=y/20;

	int mapY1=(y+Image->height)/20;

	if (MatMap[mapY][mapX]==0 && MatMap[mapY1][mapX]==3 && MoveD==false)
	{
		Image=ImageR[0];
		countCadr=0;
		y=(mapY+1)*20-Image->height;
	}
	//++++
	
	timer1=0;
	dt=0;
	MoveU=false;

	if (UP(MatMap)!=3)
		FallDown=true;
	}
		break;
	}
	
	}
	void MoveUD(char d,int MatMap[30][40])
	{
   switch(d)
	{
	case 'G':
	if (dt>5 && Jump==false && Gravitation(MatMap)==true)
	{
		vel+=acel;
		int some=(y+vel+Image->height)/20;
		
		if (MatMap[(y+vel+Image->height)/20][x/20]==0)
		y+=vel;
		else
			y=(some)*20-Image->height;

			/*if (Image==ImageJR[3] || Image==ImageJL[3])
			{
				if (Direction=='R')
				Image=ImageR[0];

				if (Direction=='L')
				Image=ImageL[0];
			}*/

		timer1=0;
		dt=0;
	}
	break;

	case 'J':
	if (UP(MatMap)==1 || UP(MatMap)==3 || UP(MatMap)==6)
	{
		JCadr=0;
		CountJ=0;
		Jump=false;
	}

	if (dtj>5 && Jump==true)
	{
		if (JCadr!=3)
		{
		ChangeImageJ();
		vel=20;
		}

		if (JCadr==3)
		{
			vel-=acel;
			y-=vel;
			CountJ++;

			if (CountJ>4)
			{
			CountJ=0;
			JCadr=0;
			vel=0;
			Jump=false;
			}

		}
			timerj1=0;
			dtj=0;
	}
		break;
	}
	
	}

	int UP(int MatMap[30][40])
	{
	int block=0;

	int mapX=x/20;
	int mapY=y/20;

	block=MatMap[mapY][mapX];

	return block;
	}
    int DOWN(int MatMap[30][40])
	{
	int block=0;

	int mapX=x/20;
	int mapY=(y+Image->height)/20;

	block=MatMap[mapY][mapX];

	return block;
	}

	void ChangeImageU()
	{
		if (UCadr<3)
		UCadr++;
		else
			UCadr=0;

		Image=ImageU[UCadr];
	}
	void ChangeImageJ()
	{
		if (JCadr<3)
		JCadr++;
		else
			JCadr=0;


		if (Direction=='R')
		Image=ImageJR[JCadr];

		if (Direction=='L')
		Image=ImageJL[JCadr];
	}
	
	void Draw(D3DLOCKED_RECT rectangle,RECT rectSize,IDirect3DSurface9* backBuffer)
	{
	 rectSize.left =x;
     rectSize.top =y;
	 rectSize.right = rectSize.left+Image->width;
	 rectSize.bottom =rectSize.top+ Image->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 Image->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
	}
	static void Timer()
	{
	if (timer1 == 0)
				timer1 = timeGetTime();

			timer = timeGetTime();
			dt = timer - timer1;
	}
	static void TimerJ()
	{
	if (timerj1 == 0)
				timerj1 = timeGetTime();

			timerj = timeGetTime();
			dtj = timerj - timerj1;
	}

};
 int Hero::dt=0;
int Hero::timer=0;
int Hero::timer1=0;

 int Hero::dtj=0;
int Hero::timerj=0;
int Hero::timerj1=0;


class Fire
{
public:
	int x,y;
	int CountCadr;
	Sprite* ImageView;
	Sprite* Image[3];

	static int counter;
	static int dt;
	static int timer;
	static int timer1;

public:
	Fire(int _x,int _y):x(_x),y(_y),CountCadr(0)
	{
		ImageView=NULL;

		Image[0]=new Sprite("Images/fire1.bmp",0xffffffff);
		Image[1]=new Sprite("Images/fire2.bmp",0xffffffff);
		Image[2]=new Sprite("Images/fire3.bmp",0xffffffff);

		ImageView=Image[0];

		counter++;
	}
	void ChangeCadr()
	{
	 if (CountCadr<2)
		 CountCadr++;
	 else
		 CountCadr=0;

	 ImageView=Image[CountCadr];
	}
	static void Timer()
	{
	if (timer1 == 0)
				timer1 = timeGetTime();

			timer = timeGetTime();
			dt = timer - timer1;
	}
};
int Fire::counter=0;
int Fire::dt=0;
int Fire::timer=0;
int Fire::timer1=0;

class Door
{
public: 
	int x,y;
	int Cadr;
	int num;

	Sprite* ImageView;
	Sprite* Image[2];

	static int counter;
public:
	Door(int _x,int _y):x(_x),y(_y),Cadr(0),num(5)
	{
		ImageView=NULL;
	Image[0]=new Sprite("Images/Door1.bmp",0xffffffff);
	Image[1]=new Sprite("Images/Door2.bmp",0xffffffff);

	ImageView=Image[0];

	counter++;
	}

	void ChangeCadr(int MatMap[30][40])
	{
	if (Cadr<1)
    Cadr++;
	else
		Cadr=0;
	
	ImageView=Image[Cadr];

	MatMap[y/20][x/20]=num;
	MatMap[y/20+1][x/20]=num;
	MatMap[y/20+2][x/20]=num;
	MatMap[y/20+3][x/20]=num;

	if (num==5)
	num=4;
	else
		num=5;
	}

	void Touch(int X,int Y,int MatMap[30][40],bool &rmb)
	{
		if (X>=x && X<=x+ImageView->width && Y>=y && Y<=y+ImageView->height && rmb==true)
	    {
		ChangeCadr(MatMap);
		rmb=false;
	    }
	}
};
int Door::counter=0;

class Book
{
public:
	int x,y;
	bool show;

	Sprite* Image;

	Sprite* Image1;
	Sprite* ImageBack;

	static int counter;
	char state;

public:
	Book(int _x,int _y):x(_x),y(_y),show(true),state('C')
	{
		ImageBack=NULL;
		Image=NULL;
		Image1=new Sprite("Images/book.bmp");

		Image=Image1;
		counter++;
	}

	bool Touch(int X,int Y)
	{
		bool t=false;

		if (X>=x && X<=x+Image->width && Y>=y && y<=Y+Image->height)
			t=true;

		return t;
	}
};
int Book::counter=0;

class Bonus
{
public:
	int x,y;
	int Cadr;
	bool show;

	Sprite* ImageView;
	Sprite* Image[10];

	static int counter;
	static int dt;
	static int timer;
	static int timer1;

	int c;

public:
	Bonus(int _x, int _y):x(_x),y(_y),Cadr(0),show(true),c(0)
	{
	ImageView=NULL;

	Image[0]=new Sprite("Images/bonus1.bmp",0xffffffff);
	Image[1]=new Sprite("Images/bonus2.bmp",0xffffffff);
	Image[2]=new Sprite("Images/bonus3.bmp",0xffffffff);
	Image[3]=new Sprite("Images/bonus4.bmp",0xffffffff);
	Image[4]=new Sprite("Images/bonus5.bmp",0xffffffff);
	Image[5]=new Sprite("Images/bonus6.bmp",0xffffffff);
	Image[6]=new Sprite("Images/bonus7.bmp",0xffffffff);
	Image[7]=new Sprite("Images/bonus8.bmp",0xffffffff);
	Image[8]=new Sprite("Images/bonus9.bmp",0xffffffff);
	Image[9]=new Sprite("Images/bonus10.bmp",0xffffffff);


	ImageView=Image[0];

	counter++;
	}

	void ChangeCadr()
	{
	if (Cadr<9)
		Cadr++;
	else
		Cadr=0;

	ImageView=Image[Cadr];
	}

	static void Timer()
	{
	if (timer1 == 0)
				timer1 = timeGetTime();

			timer = timeGetTime();
			dt = timer - timer1;
	}
	
};
int Bonus::counter=0;
int Bonus::dt=0;
int Bonus::timer=0;
int Bonus::timer1=0;

class Chest
{
public:
	int x,y;
	int Cadr;

	Sprite* ImageView;
	Sprite* Image[2];

	static int counter;

public:
	Chest(int _x,int _y):x(_x),y(_y),Cadr(0)
	{
	ImageView=NULL;
	Image[0]=new Sprite("Images/chest1.bmp");
	Image[1]=new Sprite("Images/chest2.bmp");
	ImageView=Image[0];

	counter++;
	}

	bool Touch(int X,int Y)
	{
	bool t=false;

	if (X>=x && X<=x+ImageView->width && Y>=y && Y<=y+ImageView->height)
	t=true;

	return t;
	}

	void ChangeCadr()
	{
	if (Cadr<1)
		Cadr++;
	else
		Cadr=0;
	
	ImageView=Image[Cadr];
	}
};
int Chest::counter=0;


class ButtonON
{
public:
	int x,y;
	Sprite* Image;

	static int counter;

public:
	ButtonON(int _x,int _y):x(_x),y(_y)
	{
	Image=new Sprite("Images/buttonON.bmp");

	counter++;
	}

	bool Touch(int X,int Y)
	{
		bool t=false;

		if (X>=x && X<=x+Image->width && Y>=y && Y<=y+Image->height)
		t=true;

	return t;
	}
};
int ButtonON::counter=0;

class BlockMoves
{
public:
	int x,y;
	int pX,pY;
	int pI;

	Sprite* Image;

	static bool UP;
	static int counter;
	static int dt;
	static int timer;
	static int timer1;

public:
	BlockMoves(int _x,int _y,int _pI):x(_x),y(_y-1),pX(_x),pY(_y-1),pI(_pI-1)
	{
		Image=new Sprite("Images/blockM.bmp");

		counter++;
	}

	void BlockMoveUp(int MatMap[30][40],bool &b)
	{

			if (y>pI)
			{
			MatMap[pY/20+1][pX/20]=0;

			if (y-pI<=5)
				y-=(y-pI);
			else
		    y-=5;
			}

			if (y==pI)
			{
			MatMap[y/20+1][x/20]=6;
		b=false;
			}
	}
	void BlockMoveDown(int MatMap[30][40])
	{
			if (y<pY)
			{
			MatMap[pI/20+1][x/20]=0;

			if (pY-y<=5)
				y+=(pY-y);
			else
		    y+=5;
			}

			if (y==pY)
			{
			MatMap[y/20+1][x/20]=6;
			}
	}

	static void Timer()
	{
	if (timer1 == 0)
				timer1 = timeGetTime();

			timer = timeGetTime();
			dt = timer - timer1;
	}
};
bool BlockMoves::UP=false;
int BlockMoves::counter=0;
int BlockMoves::dt=0;
int BlockMoves::timer=0;
int BlockMoves::timer1=0;

//Play++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool Play=false;

//Missions+++
bool Missions=false;

Sprite* missions;
Button* btnOfMissions[1];
Sprite* blocked;
//Missions---

//Game+++
int MatMap[30][40];

int timer;
int timer1;
int dt;
int Level=1;

bool stopB=false;
bool stop=false;

Sprite* map;
Sprite* pause;
Sprite* Menu_pause;

//Objects in the game+++
Hero* Personnage;
Fire* f[10];
Door* d[10];
Book* b[10];
Bonus* bons[10];
Chest* chest[10];
BlockMoves* bm[5];
ButtonON* bt[10];
//Objects in the game---
//Play------------------------------------------------------

//PLAYING+++
void DrawPlay()
{
 if (Missions==true)
		 {
		 resRect(0,0,w,h,missions);

		 if(btnOfMissions[0]->show==true)
			 btnOfMissions[0]->Draw(rectangle,rectSize,backBuffer);


		 for (int i=0;i<4;i++)
		 for (int j=0;j<11;j++)
			 if (i!=0 || j!=0)
		 resRect(22+j*13+j*blocked->width,63+i*36+i*blocked->height,blocked->width,blocked->height,blocked);
		 
		 }
		 else
		 {
	 resRect(0,0,w,h,map);


	 //Fire++++
	 for (int i=0;i<Fire::counter;i++)
	 resRect(f[i]->x,f[i]->y,f[i]->ImageView->width,f[i]->ImageView->height,f[i]->ImageView);
	 //Fire---

	 //Door++++
	 for (int i=0;i<Door::counter;i++)
	 resRect(d[i]->x,d[i]->y,d[i]->ImageView->width,d[i]->ImageView->height,d[i]->ImageView);
	 //Door---

	 //Bonus++++
	 for (int i=0;i<Bonus::counter;i++)
	 {
		 if (bons[i]->show==true)
			 resRect(bons[i]->x,bons[i]->y,bons[i]->ImageView->width,bons[i]->ImageView->height,bons[i]->ImageView);
	 }
	 //Bonus---

	  //Chest++++
	 for (int i=0;i<Chest::counter;i++)
	 resRect(chest[i]->x,chest[i]->y,chest[i]->ImageView->width,chest[i]->ImageView->height,chest[i]->ImageView);
	 //Chest---

	  //BlockMoves++++
	 for (int i=0;i<BlockMoves::counter;i++)
	 resRect(bm[i]->x,bm[i]->y,bm[i]->Image->width,bm[i]->Image->height,bm[i]->Image);
	 //BlockMoves---
	 
	 Personnage->Draw(rectangle,rectSize,backBuffer);

	   //Book++++
	 for (int i=0;i<Book::counter;i++)
	 {
		 if (b[i]->show==true)
		 {
			 if (b[i]->state=='O')
	 resRect(0,0,pause->width,pause->height,pause);

	 resRect(b[i]->x,b[i]->y,b[i]->Image->width,b[i]->Image->height,b[i]->Image);
		 }
	 }
	 //Book---


	 //Menu Pause+++
	 if (stop==true)
	 {
	 resRect(0,0,pause->width,pause->height,pause);
	 resRect((w-Menu_pause->width)/2,(h-Menu_pause->height)/2,Menu_pause->width,Menu_pause->height,Menu_pause);
	 }
	 //Menu Pause---
		 }
}

int* Read(int p1,int p2,char* c)
{
	int* cor=NULL;
		char* line=new char[p2-p1];

	    for (int i=p1;i<p2;i++)
			line[i-p1]=c[i];

		if (line[0]!='N')
		{
	int quantiteXY=(p2-p1+2)/4; //количество координат для бонусов

	int quantite=quantiteXY/2; //количество бонусов

	cor=new int[quantiteXY];

	for (int i=0;i<quantiteXY;i++)
	{
		char a1,a2,a3;

			a1=line[i*4];
			a2=line[i*4+1];
			a3=line[i*4+2];

		cor[i]=atoi(&a1)*100+atoi(&a2)*10+atoi(&a3);
	}
	}

	return cor;
}
void ReadScript()
{
	//Чтение файла---
	char* c=new char[mH*mW+100];

	std::ifstream is("Images/Data/map1.txt");

	for (int i=0;i<mH*mW+100;i++)
	is>>c[i];

    is.close();
	//Чтение файла+++

bool LoadMap=false;
int z=0;
char l;
int pi=0;

int pos[5]={-1,-1,-1,-1,-1};

for (int k=0;k<mH*mW+100;k++)
{
	//Загрузка карты+++++++++++++++++++++
	if (LoadMap==true)
	{

	for (int i=0;i<mH;i++)
	for (int j=0;j<mW;j++)
	{
    l=c[k+z];

	if (l!='\0')
	MatMap[i][j]=atoi(&l);

	z++;
	}

	//Загрузка дверей+++
	for (int i=0;i<mH;i++)
	for (int j=0;j<mW;j++)
	{
		if (MatMap[i][j]==4 && MatMap[i-1][j]!=4)
			d[Door::counter-1]=new Door(j*20,i*20);


		//Block moves+++
		if (MatMap[i][j]==6)
		{
			if (BlockMoves::counter==0)
				pi=(i-1)*20;

			bm[BlockMoves::counter-1]=new BlockMoves(j*20,i*20,pi);
		}
		//Block moves---
	}
	//Загрузка дверей---

	LoadMap=false;
	}
	//Загрузка карты--------------------------

	if (c[k]=='|' && z==0)
	{
	LoadMap=true;
	}


	//Загрузка кнопок---
	if (c[k]==':' && pos[4]!=-1)
		{
	    int* cor=Read(pos[4],k,c); //Выделение координат X и Y

	   if (cor!=NULL)
		for (int i=0;i<(k-pos[4]+2)/8;i++)
				bt[i]=new ButtonON(cor[i*2],cor[i*2+1]);
		}
	//Загрузка кнопок---

	//Загрузка сундуков---
	if (c[k]==':' && pos[3]!=-1 && pos[4]==-1)
		{
		pos[4]=k+1;

	    int* cor=Read(pos[3],k,c); //Выделение координат X и Y

	   if (cor!=NULL)
		for (int i=0;i<(k-pos[3]+2)/8;i++)
				chest[i]=new Chest(cor[i*2],cor[i*2+1]);
		}
	//Загрузка сундуков---


	//Загрузка бонусов---
	if (c[k]==':' && pos[2]!=-1 && pos[3]==-1)
		{
		pos[3]=k+1;

	    int* cor=Read(pos[2],k,c); //Выделение координат X и Y

	   if (cor!=NULL)
		for (int i=0;i<(k-pos[2]+2)/8;i++)
				bons[i]=new Bonus(cor[i*2],cor[i*2+1]);
		}
	//Загрузка бонусов---


	//Загрузка книг+++
		if (c[k]==':' && pos[1]!=-1 && pos[2]==-1)
		{
		pos[2]=k+1;

	    int* cor=Read(pos[1],k,c); //Выделение координат X и Y

	   if (cor!=NULL)
		for (int i=0;i<(k-pos[1]+2)/8;i++)
				b[i]=new Book(cor[i*2],cor[i*2+1]);
		}
	//Загрузка книг---


	//Загрузка огня+++++++++++
	if (c[k]==':' && pos[0]!=-1 && pos[1]==-1)
	{
	pos[1]=k+1;
	
	int* cor=Read(pos[0],k,c); //Выделение координат X и Y

	   if (cor!=NULL)
		for (int i=0;i<(k-pos[0]+2)/8;i++)
				f[i]=new Fire(cor[i*2],cor[i*2+1]);
	}

	if (c[k]==':')
		pos[0]=k+1;
   //Загрузка огня------------
	

	//Загрузка координат игрока+++
	if (LoadMap==false)
	{
    Personnage->x=atoi(&c[0]);
	Personnage->y=atoi(&c[4]);
	}
	//Загрузка координат игрока+++
}
}

void InteractiveObjects()
{
        //FIRE---
		Fire::Timer();

		if (Fire::dt>50)
		{
		  for (int i=0;i<Fire::counter;i++)
		 f[i]->ChangeCadr();


		 Fire::timer1=0;
		 Fire::dt=0;
		}
		//FIRE+++

		//BONUS---
		Bonus::Timer();

		if (Bonus::dt>50)
		{
		  for (int i=0;i<Bonus::counter;i++)
		 bons[i]->ChangeCadr();


		 Bonus::timer1=0;
		 Bonus::dt=0;
		}
		//BONUS+++

		//DOOR+++
		for (int i=0;i<Door::counter;i++)
			d[i]->Touch(X,Y,MatMap,lmb);
		//DOOR---

		//ButtonON+++
		for (int i=0;i<ButtonON::counter;i++)
		{
			if (bt[i]->Touch(Personnage->x,Personnage->y+Personnage->Image->height)==true)
				BlockMoves::UP=true;
		}
		//ButtonON---

		//Block Moves+++
		BlockMoves::Timer();

		if (BlockMoves::dt>5 && BlockMoves::UP==true)
		{
			bool b=true;

		for (int i=0;i<BlockMoves::counter;i++)
			bm[i]->BlockMoveUp(MatMap,b);

		if (b==true)
		BlockMoves::timer1=0;
		}

		if (BlockMoves::dt>1000*10)
		BlockMoves::UP=false;

		if (BlockMoves::dt>5 && BlockMoves::UP==false)
		{
			for (int i=0;i<BlockMoves::counter;i++)
				bm[i]->BlockMoveDown(MatMap);
		}
		//Block Moves---

			//BOOKS+++
		for (int i=0;i<Book::counter;i++)
			if (b[i]->state='C' && b[i]->show==true && (b[i]->Touch(Personnage->x,Personnage->y+Personnage->Image->height)==true ||b[i]->Touch(Personnage->x+Personnage->Image->width,Personnage->y+Personnage->Image->height)==true))
			{
			b[i]->state='O';
			b[i]->ImageBack=new Sprite("Images/book1.bmp",0xffffffff);
			b[i]->Image=b[i]->ImageBack;
			b[i]->x=(w-b[i]->ImageBack->width)/2;
			b[i]->y=(h-b[i]->ImageBack->height)/2;
			stopB=true;
			}
			//BOOKS---

}
void HeroMoves()
{
	Hero::Timer();
	Hero::TimerJ();

	//GRAVITAION+++
	Personnage->MoveUD('G',MatMap);
	//GRAVITAION---

	//JUMP+++
	if (buffer[DIK_SPACE] & 0x80 && Personnage->JUMP(MatMap)==false &&  Personnage->DOWN(MatMap)!=3)
		Personnage->Jump=true;
	
	Personnage->MoveUD('J',MatMap);
	//JUMP---

	//UP+++
	if (buffer[DIK_UP] & 0x80 && Personnage->UP(MatMap)==3)
		{
		Personnage->MoveU=true;
		Personnage->MoveD=false;
		Personnage->FallDown=false;
	}

	Personnage->Move('U',MatMap);
	//UP---
	
	//DOWN+++
	if (buffer[DIK_DOWN] & 0x80 && Personnage->DOWN(MatMap)==3)
	{
		Personnage->MoveD=true;
		Personnage->MoveU=false;
	}
		
	Personnage->Move('D',MatMap);

	if (Personnage->dt>10 && Personnage->MoveD==true && Personnage->DOWN(MatMap)!=3)
	Personnage->MoveD=false;
	//DOWN---


	//LEFT+++
	if ((buffer[DIK_LEFT] & 0x80) || (buffer[DIK_A] & 0x80))
	{
		Personnage->MoveL=true;
		Personnage->MoveR=false;	
		Personnage->FallDown=true;
	}

	Personnage->Move('L',MatMap);
	//LEFT---

	//RIGHT+++
	if ((buffer[DIK_RIGHT] & 0x80) || (buffer[DIK_D] & 0x80))
	{
		Personnage->MoveR=true;	
		Personnage->MoveL=false;	
		Personnage->FallDown=true;
	}
		
	Personnage->Move('R',MatMap);
	//RIGHT---

}
void PLAYING()
{
if (Play==true)
{
	if (Missions==true)
	{
		if (btnOfMissions[0]->Touch(X,Y)==true)
		{
			Personnage=new Hero();
            Personnage->x=40;
            Personnage->y=40;

            ReadScript();

			map=new Sprite("Images/Data/map1.bmp",0xffffffff);

		Missions=false;
		}
		
			if (btnOfMissions[0]->Touch(mX,mY)==true)
				btnOfMissions[0]->show=true;
			else
				btnOfMissions[0]->show=false;
	}
	else
	{
		if (stopB==false && stop==false)
		{
		InteractiveObjects();
        HeroMoves();
		}

			//CLOSE THE BOOK++++
	   if (buffer[DIK_RETURN] & 0x80)
		{
			for (int i=0;i<Book::counter;i++)
			{
				if (b[i]->state=='O')
				{
				b[i]->show=false;
				b[i]->state='C';
				stopB=false;
				}
			}
		}
		//CLOSE THE BOOK---


	//EXIT++++
		if (buffer[DIK_ESCAPE] & 0x80)
		{
			stop=true;
		}
	//EXIT----
	}


}
}
//PLAYING---
