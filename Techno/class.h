class Text
{
public:
	char* string;
	int x,y;

private:
	Sprite* txt;
	Sprite* letters[77];

	int size;
	int w,h;
	int* encode;

	char* szLetters;

public:
	Text(char* str,int color,int _x,int _y):string(str)
	{
		w=11;
		h=11;

		txt=new Sprite("Images/Text.bmp",0xffffffff);

		int i=0;
		int j=0;

		for (int k=0;k<77;k++)
		{
		letters[k]=new Sprite("Images/Text.bmp",0xffffffff);
		letters[k]->cut(w*j,h*i,w,h);
		letters[k]->RemplaceColor(0xFF000000,color);
		
		j++;
		
		if (j==26)
		{
			i++;
			j=0;
		}
		}

		x=_x;
		y=_y;
		//Conv+++
		size=0;

		while(string[size]!='\0')
        size++;

		encode=new int[size];
		
		szLetters=new char[size];
	    szLetters[size]='\0';


	for (int i=0;i<size;i++)
		encode[i]=convertion(static_cast<int>(string[i]),i);
	}
	void Draw(D3DLOCKED_RECT rectangle,RECT rectSize,IDirect3DSurface9* backBuffer)
	{
	int n=0;
	int distance=0;

	for (int i=0;i<size;i++)
	{
	  n=encode[i];

	  if (szLetters[i]=='U')
		  distance=1;

	  if (szLetters[i]=='L')
		  distance=5;

	  if (szLetters[i]=='N')
		  distance=5;


		  rectSize.left =x+i*letters[i]->width-distance*i;
     rectSize.top =y;
	 rectSize.right = rectSize.left+letters[n]->width;
	 rectSize.bottom =rectSize.top+ letters[n]->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 letters[n]->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
	}
	};
	void changeText(char* str)
	{
		string=str;
		size=0;

		while(string[size]!='\0')
        size++;

		encode=new int[size];
		
		szLetters=new char[size];
	    szLetters[size]='\0';

	for (int i=0;i<size;i++)
		encode[i]=convertion(static_cast<int>(string[i]),i);
	}

private:
	int convertion(char ASC,int i)
	{
		int ASCII=static_cast<int>(ASC);
		int sym=0;
		szLetters[i]='O';

		//Symb+++
		switch(ASC)
		{
		case '.':
			sym=62;
			break;
		
		case ':':
			sym=63;
			break;

			case '/':
			sym=64;
			break;

			case '-':
			sym=65;
			break;

			case '+':
			sym=66;
			break;

			case '(':
			sym=67;
			break;

			case ')':
			sym=68;
			break;
			
			case '*':
			sym=69;
			break;

			case '=':
			sym=70;
			break;

			case ',':
			sym=71;
			break;

			case '!':
			sym=72;
			break;

			case ' ':
			sym=73;
			break;

			default:
					if (ASCII-97>=0)  //low
		{
			sym=ASCII-97;
			szLetters[i]='L';
		}
	    else 
		if (ASCII-65>=0) //up
		{
			sym=ASCII-65+26;
			szLetters[i]='U';
		}
		else 
			if (ASCII-48>=0) //num
			{
			sym=ASCII-48+52;
			szLetters[i]='N';
			}
			break;
		}
		//Symb---


		return sym;
	}
};

class Hero
{
public:
	int x,y;
	int w,h;

	int exitX;
	int exitY;

	int CadrR,CadrL,CadrU;

	Sprite* ImageR[5];
	Sprite* ImageL[5];

	Sprite* ImageUD[4];
	Sprite* ImageJR[4];
	Sprite* ImageJL[4];

	Sprite* Image;

	int MatMap[30][40];

	static int dt;
	static int timer;
	static int timer1;

	static int dtG;
	static int timerG;
	static int timerG1;


	bool move;
	bool L,R,J,U,D,G;

	int velocity,velocityJ;
	int accel,accelJ;


public:

	Hero():x(0),y(0),CadrR(0),CadrL(0),L(false),R(false),J(false),velocity(0),accel(3),velocityJ(0),accelJ(3),U(false),D(false),CadrU(0),G(true)
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

	ImageJR[0]=new Sprite("Images/heroJ1.bmp",0xffffffff);
	ImageJR[1]=new Sprite("Images/heroJ2.bmp",0xffffffff);
	ImageJR[2]=new Sprite("Images/heroJ3.bmp",0xffffffff);
	ImageJR[3]=new Sprite("Images/heroJ4.bmp",0xffffffff);

	ImageJL[0]=new Sprite("Images/heroJ1.bmp",0xffffffff);
	ImageJL[1]=new Sprite("Images/heroJ2.bmp",0xffffffff);
	ImageJL[2]=new Sprite("Images/heroJ3.bmp",0xffffffff);
	ImageJL[3]=new Sprite("Images/heroJ4.bmp",0xffffffff);

	ImageUD[0]=new Sprite("Images/heroU1.bmp",0xffffffff);
	ImageUD[1]=new Sprite("Images/heroU2.bmp",0xffffffff);
	ImageUD[2]=new Sprite("Images/heroU3.bmp",0xffffffff);
	ImageUD[3]=new Sprite("Images/heroU4.bmp",0xffffffff);


	for (int i=0;i<5;i++)
	{
	if (i!=4)
	ImageJL[i]->Rotate();
	}
	Image=ImageR[0];

	w=Image->width;
	h=Image->height;

	}

	void MoveR()
	{
		int step=7;

		if (R==true)
		{
		int nx=(x+w+step)/20;
		int ny=y/20;
		int ny1=y/20+1;
		int ny2=y/20+2;

	if (MatMap[ny][nx]==0 || MatMap[ny][nx]==5 || MatMap[ny][nx]==3 || MatMap[ny][nx]==9)
	{
		if (MatMap[ny1][nx]==0 || MatMap[ny1][nx]==5 || MatMap[ny1][nx]==3 || MatMap[ny][nx]==9)
		{
			if (MatMap[ny2][nx]==0 || MatMap[ny2][nx]==5 || MatMap[ny2][nx]==3 || MatMap[ny][nx]==9)
		{
	ChangeImage('R');
	x+=step;
	timer1=0;
	}
	}
	}
	R=false;
		}
	}
	void MoveL()
	{
		int step=7;

		if (L==true)
		{
		int nx=(x-step)/20;
		int ny=y/20;
		int ny1=y/20+1;
		int ny2=y/20+2;

	if (MatMap[ny][nx]==0 || MatMap[ny][nx]==5  || MatMap[ny][nx]==3 || MatMap[ny][nx]==9)
	{
		if (MatMap[ny1][nx]==0 || MatMap[ny1][nx]==5  || MatMap[ny1][nx]==3 || MatMap[ny][nx]==9)
		{
			if (MatMap[ny2][nx]==0 || MatMap[ny2][nx]==5 || MatMap[ny2][nx]==3 || MatMap[ny][nx]==9)
			{
	ChangeImage('L');
	x-=step;
	timer1=0;
		}
		}
	}
	L=false;
		}
	}
	void Gravitaton()
	{
		if (J==false && G==true)
		{
			if (velocity<=20)
		velocity+=accel;

		int nx=x/20;
		int nx1=(x+w)/20;
		int ny=(y+h+velocity)/20;

	if (MatMap[ny][nx]==0 && MatMap[ny][nx1]==0)
	{
	y+=velocity;
	timerG1=0;
	}
	else
		if (MatMap[ny][nx]!=0)
		{
        y=ny*20-h;

		velocity=0;
		}

		}
	}
	void Jump()
	{
		if (J==true && dtG>20)
		{
	velocityJ-=accel;

	int nx=x/20;
	int ny=(y-velocityJ)/20;

	if (MatMap[ny][nx]==0 && velocityJ>=0)
	{
	y-=velocityJ;

	timerG1=0;
	}
	else
	{
		velocityJ=0;
	    timerG1=0;
		J=false;
	}
	}
	}
	void UD(char d)
	{
	switch(d)
	{
	case 'U':
		if (U==true)
		{
		int nx=x/20;
		int ny=y/20;

		if (MatMap[ny][nx]==3)
		{
		ChangeImage('U');
		y-=5;
		}

		if (MatMap[ny][nx]==0 && MatMap[(y+h)/20][nx]==3 &&  MatMap[ny+1][nx]==3)
		y=(y/20+1)*20-h;

		U=false;
		timer1=0;
		}
		break;

		case 'D':
		if (D==true)
		{
		int nx=x/20;
		int ny=(y+h+5)/20;


		if (MatMap[ny][nx]==3)
		{
		ChangeImage('U');
		y+=5;
		}

		D=false;
		timer1=0;
		}
		break;
	}
	}

	void ChangeImage(char d)
	{
		switch(d)
		{
		case 'L':
			if (CadrL<4)
			CadrL++;
			else
				CadrL=0;

			Image=ImageL[CadrL];
			break;

			case 'R':
			if (CadrR<4)
			CadrR++;
			else
				CadrR=0;

			Image=ImageR[CadrR];
			break;

			case 'U':
			if (CadrU<3)
			CadrU++;
			else
				CadrU=0;

			Image=ImageUD[CadrU];
			break;
		}

		w=Image->width;
		h=Image->height;
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
	bool ChangeLevel()
	{
		bool c=false;

	if (x>=exitX && x<=exitX+5 && y>=exitY && y<=exitY+5)
		c=true;

	if (x+Image->width>=exitX && x+Image->width<=exitX+5 && y>=exitY && y<=exitY+5)
		c=true;

		return c;
	}

	static void Timer()
	{
	if (timer1 == 0)
				timer1 = timeGetTime();

			timer = timeGetTime();
			dt = timer - timer1;
	}
	static void TimerG()
	{
	if (timerG1 == 0)
				timerG1 = timeGetTime();

			timerG = timeGetTime();
			dtG = timerG - timerG1;
	}

	void ChargeMatMap(int MatMap1[30][40])
	{
	for (int i=0;i<30;i++)
	for (int j=0;j<40;j++)
		MatMap[i][j]=MatMap1[i][j];
	}
};
int Hero::dt=0;
int Hero::timer=0;
int Hero::timer1=0;

int Hero::dtG=0;
int Hero::timerG=0;
int Hero::timerG1=0;

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

	bool Touch(int X,int Y)
	{
	bool t=false;

	if (X>=x && X<=x+ImageView->width && Y>=y && Y<=y+ImageView->height)
	t=true;

	return t;
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
	int mX1,mY1;
	int Answer;
	int check;
	int objects[6*8];

	bool expO;
	bool show;
	bool showC;
	bool move;
	bool BOk;

	static int counter;
	static bool iLmb;
	static int Xi;
	static int Yi;
	
	Sprite* ImObjects[6*8];
	Sprite* Image[2];
	Sprite* ImageView;
	Sprite* SubImage;
	Sprite* OpenExp;
	Sprite* Content;
	Sprite* OkOpen;
    Button* lock;

	Text* txt;
	char* question;

	static int dt;
    static int timer;
    static int timer1;

public:
	Chest(int _x,int _y,int Level,int ans1,int ans2):x(_x),y(_y),Cadr(0),show(false),expO(false),showC(false),move(false),check(0)
	{
	ImageView=NULL;
	Image[0]=new Sprite("Images/chest1.bmp");
	Image[1]=new Sprite("Images/chest2.bmp");
	ImageView=Image[0];

	counter++;

	SubImage=new Sprite("Images/code.bmp");

	Answer=ans1*1000+ans2;

	OpenExp=new Sprite("Images/OpenChest.bmp",0xffffffff);
	Content=new Sprite("Images/InvChest.bmp",0xffffffff);
	OkOpen=new Sprite("Images/OpenTrue.bmp",0xffffffff);
	BOk=false;
	//----
	for (int i=0;i<6*8;i++)
	 objects[i]=0;

	 objects[0]=2;

	ChangeImages();

	//+++
	lock=new Button("Images/code.bmp",0xffffffff);
lock->x=(w-lock->w)/2;
lock->y=(h-lock->h)/2;
lock->show=true;
	//---
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

	void Draw(D3DLOCKED_RECT rectangle,RECT rectSize,IDirect3DSurface9* backBuffer)
	{
	 rectSize.left =x;
     rectSize.top =y;
	 rectSize.right = rectSize.left+ImageView->width;
	 rectSize.bottom =rectSize.top+ ImageView->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 ImageView->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();


	  if (expO==true)
	 {
     rectSize.left =mX1;
     rectSize.top =mY1;
	 rectSize.right = rectSize.left+OpenExp->width;
	 rectSize.bottom =rectSize.top+ OpenExp->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 OpenExp->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
	 }


	}
	void DrawC(D3DLOCKED_RECT rectangle,RECT rectSize,IDirect3DSurface9* backBuffer)
	{
	  if (showC==true)
	 {
	 rectSize.left =(w-SubImage->width)/2;
	 rectSize.top =(h-SubImage->height)/2;
	 rectSize.right = rectSize.left+Content->width;
	 rectSize.bottom =rectSize.top+ Content->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 Content->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();


	 int ix=0;
	 int iy=0;

	  for (int i=0;i<6*8;i++)
	 {
		 if (objects[i]!=0)
		 {
	rectSize.left =(w-SubImage->width)/2+71+(ix)*5+(ix)*25;
     rectSize.top =(h-SubImage->height)/2+109+(iy)*5+(iy)*25;
	 rectSize.right = rectSize.left+ImObjects[i]->width;
	 rectSize.bottom =rectSize.top+ ImObjects[i]->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 ImObjects[i]->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
		 }
	 if (ix<7)
	 ix++;
	 else
	 {
		 iy++;
		 ix=0;
	 }
	 }
	 }

	  	if (BOk==true)
	 {
	 rectSize.left =(w-lock->w)/2+99+2;
     rectSize.top=(h-lock->h)/2+107;
	 rectSize.right = rectSize.left+OkOpen->width;
	 rectSize.bottom =rectSize.top+ OkOpen->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 OkOpen->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
	}
	 }
	//++++++
	int TouchObject(int X,int Y)
	{
		int x1=71+(w-SubImage->width)/2,x2=305+(w-SubImage->width)/2;
		int y1=109+(h-SubImage->height)/2,y2=283+(h-SubImage->height)/2;

		int zn=-1;
		int blockX=(X-x1)/(25+5);
		int blockY=(Y-y1)/(25+5);

		if (X>=x1 && X<=x2 && Y>=y1 && Y<=y2)
		{
			if (X-x1-blockX*(25+5)<=25)
			if (Y-y1-blockY*(25+5)<=25)
			zn=blockY*8+blockX;
		}

		return zn;
	}
	void ChangeImages()
	{
		char* path=new char[30];
	 int num;

		for (int i=0;i<6*8;i++)
		{
		 if (objects[i]!=0)
		 {
     num = sprintf(path, "Images/o%d.bmp",objects[i]);

	 ImObjects[i]=new Sprite(path,0xffffffff);
		 }
		}
	}

    static void SetPosition(int X,int Y)
   {
   Xi=X;
   Yi=Y;
   iLmb=true;
   }
	bool TouchInvChest(int X,int Y)
	{
	bool t=false;

	int x1=71+(w-SubImage->width)/2,x2=305+(w-SubImage->width)/2;
	int y1=109+(h-SubImage->height)/2,y2=283+(h-SubImage->height)/2;


	if (X>=x1 && X<=x2 && Y>=y1 && Y<=y2)
	t=true;

	return t;
	}

	static void Timer()
	{
	if (timer1 == 0)
				timer1 = timeGetTime();

			timer = timeGetTime();
			dt = timer - timer1;
	}
	//++++
	void LoadQuestion(int Level)
	{
	char* c=new char[300];
	std::ifstream ifs("Images/Data/questions.txt");

	for (int i=0;i<300;i++)
	ifs>>c[i];

    ifs.close();
	//+++++++++++++
	int j=0;
	int cont=0;
	int pos=0;

	while(c[j]!='\0')
	{
		if (c[j]=='%')
		{
			cont++;

			if (cont==Level)
			pos=j+1;
		}


		if (cont==Level+1)
		{
		char* str=new char[j-pos];

		for (int z=0;z<j-pos;z++)
			str[z]=c[z+pos];

		str[j-pos]='\0';

		int k=0;
		
		while(str[k]!='$')
			k++;

		//QUESTION+++
		question=new char[k];

		for (int z1=0;z1<k;z1++)
			question[z1]=str[z1];

		question[k]='\0';
		//ANSWER+++
		char* temp=new char[j-(k+1)];

		for (int z2=0;z2<j-(k+1);z2++)
			temp[z2]=str[z2+k+1];

		temp[j-(k+1)]='\0';

		Answer=atoi(temp);
		//ANSWER---
		}

    j++;
	}

	txt=new Text(question,0xFF3D3D3D,(w-SubImage->width)/2+15,(h-SubImage->height)/2+68);
	}
};
int Chest::dt=0;
int Chest::timer=0;
int Chest::timer1=0;

int Chest::counter=0;
bool Chest::iLmb=false;
int Chest::Xi=0;
int Chest::Yi=0;

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

class NumberDraw
{
public:
	int num;
	int x,y;

	Sprite* Image;
	Sprite* Images[11];

public:
	NumberDraw()
	{
	num=0;

	Image=NULL;
	Images[0]=new Sprite("Images/0.bmp",0xffffffff);
	Images[1]=new Sprite("Images/1.bmp",0xffffffff);
	Images[2]=new Sprite("Images/2.bmp",0xffffffff);
	Images[3]=new Sprite("Images/3.bmp",0xffffffff);
	Images[4]=new Sprite("Images/4.bmp",0xffffffff);
	Images[5]=new Sprite("Images/5.bmp",0xffffffff);
	Images[6]=new Sprite("Images/6.bmp",0xffffffff);
	Images[7]=new Sprite("Images/7.bmp",0xffffffff);
	Images[8]=new Sprite("Images/8.bmp",0xffffffff);
	Images[9]=new Sprite("Images/9.bmp",0xffffffff);
	Images[10]=new Sprite("Images/10.bmp",0xffffffff);
	Image=Images[num];

	x=0;
	y=0;
	}

	void Draw(D3DLOCKED_RECT rectangle,RECT rectSize,IDirect3DSurface9* backBuffer)
	{
	Image=Images[num];

	 rectSize.left =x;
     rectSize.top =y;
	 rectSize.right = rectSize.left+Image->width;
	 rectSize.bottom =rectSize.top+ Image->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 Image->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
	}
};

class Arrow
{
public:
	int x,y;
	int num;

	Sprite* Image;
	Sprite* Images[40];

public:
	Arrow()
	{
		x=0;
		y=0;
		num=0;

		Images[0]=new Sprite("Images/c0.bmp",0xffffffff);
		Images[1]=new Sprite("Images/c1.bmp",0xffffffff);
		Images[2]=new Sprite("Images/c2.bmp",0xffffffff);
		Images[3]=new Sprite("Images/c3.bmp",0xffffffff);
		Images[4]=new Sprite("Images/c4.bmp",0xffffffff);
		Images[5]=new Sprite("Images/c5.bmp",0xffffffff);
		Images[6]=new Sprite("Images/c6.bmp",0xffffffff);
		Images[7]=new Sprite("Images/c7.bmp",0xffffffff);
		Images[8]=new Sprite("Images/c8.bmp",0xffffffff);
		Images[9]=new Sprite("Images/c9.bmp",0xffffffff);
		Images[10]=new Sprite("Images/c10.bmp",0xffffffff);
		Images[11]=new Sprite("Images/c11.bmp",0xffffffff);
		Images[12]=new Sprite("Images/c12.bmp",0xffffffff);
		Images[13]=new Sprite("Images/c13.bmp",0xffffffff);
		Images[14]=new Sprite("Images/c14.bmp",0xffffffff);
		Images[15]=new Sprite("Images/c15.bmp",0xffffffff);
		Images[16]=new Sprite("Images/c16.bmp",0xffffffff);
		Images[17]=new Sprite("Images/c17.bmp",0xffffffff);
		Images[18]=new Sprite("Images/c18.bmp",0xffffffff);
		Images[19]=new Sprite("Images/c19.bmp",0xffffffff);
		Images[20]=new Sprite("Images/c20.bmp",0xffffffff);
		Images[21]=new Sprite("Images/c21.bmp",0xffffffff);
		Images[22]=new Sprite("Images/c22.bmp",0xffffffff);
		Images[23]=new Sprite("Images/c23.bmp",0xffffffff);
		Images[24]=new Sprite("Images/c24.bmp",0xffffffff);
		Images[25]=new Sprite("Images/c25.bmp",0xffffffff);
		Images[26]=new Sprite("Images/c26.bmp",0xffffffff);
		Images[27]=new Sprite("Images/c27.bmp",0xffffffff);
		Images[28]=new Sprite("Images/c28.bmp",0xffffffff);
		Images[29]=new Sprite("Images/c29.bmp",0xffffffff);
		Images[30]=new Sprite("Images/c30.bmp",0xffffffff);
		Images[31]=new Sprite("Images/c31.bmp",0xffffffff);
		Images[32]=new Sprite("Images/c32.bmp",0xffffffff);
		Images[33]=new Sprite("Images/c33.bmp",0xffffffff);
		Images[34]=new Sprite("Images/c34.bmp",0xffffffff);
		Images[35]=new Sprite("Images/c35.bmp",0xffffffff);
		Images[36]=new Sprite("Images/c36.bmp",0xffffffff);
		Images[37]=new Sprite("Images/c37.bmp",0xffffffff);
		Images[38]=new Sprite("Images/c38.bmp",0xffffffff);
		Images[39]=new Sprite("Images/c39.bmp",0xffffffff);
	}

	void Draw(D3DLOCKED_RECT rectangle,RECT rectSize,IDirect3DSurface9* backBuffer)
	{
	Image=Images[num];

	 rectSize.left =x;
     rectSize.top =y;
	 rectSize.right = rectSize.left+Image->width;
	 rectSize.bottom =rectSize.top+ Image->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 Image->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
	}
};

class Inventar
{
public:
	int x,y;
	int mX,mY;
	int w,h;
	int objects[9];
	int move;
	int check;

	bool show;
	bool exp;
	
	static bool iLmb;
	static int Xi;
	static int Yi;

	Sprite* Image;
	Sprite* Open;
	Sprite* InventarExp;
	Sprite* ImObjects[9];

public:
	Inventar():exp(false),move(false)
	{
	x=0;
	y=0;
	mX=0;
	mY=0;

	Image=new Sprite("Images/Inventar.bmp",0xffffffff);
	Open=new Sprite("Images/InventarOpen.bmp",0xffffffff);
	InventarExp=new Sprite("Images/InventarExp.bmp",0xffffffff);

	for (int i=0;i<9;i++)
	objects[i]=0;

	ChangeImages();
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


	 //0---
		 if (objects[0]!=0)
		 {
	  rectSize.left =17+0*7+0*25-7;

     rectSize.top =7;
	 rectSize.right = rectSize.left+ImObjects[0]->width;
	 rectSize.bottom =rectSize.top+ ImObjects[0]->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 ImObjects[0]->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
		 }
	 //0+++

	 if (show==true)
	 {
	 rectSize.left =x;
     rectSize.top =y;
	 rectSize.right = rectSize.left+Open->width;
	 rectSize.bottom =rectSize.top+ Open->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 Open->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();


	 for (int i=0;i<9;i++)
	 {
		 if (objects[i]!=0)
		 {
	 rectSize.left =17+i*7+i*25;

	 if (i==0)
	 rectSize.left =17+i*7+i*25-7;

     rectSize.top =7;
	 rectSize.right = rectSize.left+ImObjects[i]->width;
	 rectSize.bottom =rectSize.top+ ImObjects[i]->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 ImObjects[i]->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
		 }
	 }
	 }


	 if (exp==true)
	 {
     rectSize.left =mX;
     rectSize.top =mY;
	 rectSize.right = rectSize.left+InventarExp->width;
	 rectSize.bottom =rectSize.top+ InventarExp->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 InventarExp->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
	 }

	}
	void ChangeImages()
	{
		char* path=new char[30];
	 int num;

		for (int i=0;i<9;i++)
		{
		 if (objects[i]!=0)
		 {
     num = sprintf(path, "Images/o%d.bmp",objects[i]);

	 ImObjects[i]=new Sprite(path,0xffffffff);
		 }
		}
	}
	bool Touch(int X,int Y)
	{
		bool t=false;

		if (X>=x && X<=x+Image->width && Y>=y && Y<=y+Image->height)
		t=true;

		return t;
	}
	int TouchObject(int X,int Y)
	{
		int x1=17,x2=297;
		int y1=7,y2=31;

		int zn=-1;
		int block=(X-x1)/(25+7);

		if (X>=x1 && X<=x2 && Y>=y1 && Y<=y2)
			if (X-x1-block*(25+7)<=25)
			zn=block;

		return zn;
	}

   static void SetPosition(int X,int Y)
   {
   Xi=X;
   Yi=Y;
   iLmb=true;
   }

	int TouchInvShow(int X,int Y)
	{
		bool t=false;

		if (X>=x && X<=x+Open->width && Y>=y && Y<=y+Open->height)
		t=true;

		return t;
	}

	void AddObject(int num)
	{
	for (int i=0;i<9;i++)
	{
		if (num!=0 && objects[i]==0)
	   {
		objects[i]=num;
		ChangeImages();
		break;
	   }
	}
	}
};
bool Inventar::iLmb=false;
int Inventar::Xi=0;
int Inventar::Yi=0;


class FinalDoor
{
public: 
	int x,y;
	int Cadr;
	int num;
	bool nk;//Need key

	Sprite* ImageView;
	Sprite* Image[2];
	Sprite* NeedKey;

	static int counter;

	int type;
public:
	FinalDoor(int _x,int _y,int _type):x(_x),y(_y),Cadr(0),num(9),type(_type)
	{
	ImageView=NULL;
	Image[0]=new Sprite("Images/NextDoor1.bmp",0xffffffff);
	Image[1]=new Sprite("Images/NextDoor2.bmp",0xffffffff);

	ImageView=Image[0];

	NeedKey=new Sprite("Images/NeedKey.bmp",0xffffffff);
	nk=false;

	counter++;
	//
	if (type==1)
	{
	Image[0]->Rotate();
	Image[1]->Rotate();
	}
	}

	void ChangeCadr(int MatMap[30][40])
	{
	if (type==0)
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

	if (num==9)
	num=8;
	else
		num=9;
	}
	}
	void Touch(int X,int Y,int MatMap[30][40],bool &rmb,int inv)
	{
		if (X>=x && X<=x+ImageView->width && Y>=y && Y<=y+ImageView->height && rmb==true && inv==2)
	    {
		ChangeCadr(MatMap);
		rmb=false;
	    }

		if (X>=x && X<=x+ImageView->width && Y>=y && Y<=y+ImageView->height && rmb==true && inv!=2)
		nk=true;

		if ((X<x || X>x+ImageView->width) && (Y<y || Y>y+ImageView->height))
			nk=false;
	}

	void Draw(D3DLOCKED_RECT rectangle,RECT rectSize,IDirect3DSurface9* backBuffer)
	{
	 rectSize.left =(x+Image[0]->width)-ImageView->width;
     rectSize.top =y;
	 rectSize.right = rectSize.left+ImageView->width;
	 rectSize.bottom =rectSize.top+ ImageView->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 ImageView->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();

	 //NeedKey
	 if (nk==true)
	 {
	 rectSize.left =(w-NeedKey->width)/2;
	 rectSize.top =(h-NeedKey->height)/2;
	 rectSize.right = rectSize.left+NeedKey->width;
	 rectSize.bottom =rectSize.top+ NeedKey->height;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 NeedKey->DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
	 }
	}
};
int FinalDoor::counter=0;





class DrawRectangle
{
public:
	int x,y;
	int w,h;

	int* img;

	int BorderC;
	int FillC;

public:
	DrawRectangle(int _x,int _y,int _w,int _h,int BorColor,int FillColor) 
	{
		 x=_x;
		 y=_y;
		 w=_w;
		 h=_h;

		 BorderC=BorColor;
		 FillC=FillColor;

		 img=new int[w*h*32/8];

		 for (int i = 0; i < h; ++i)
			 for (int j = 0; j < w; ++j)
			img[j+i*w]=FillColor;


		  for (int i = 0; i < h; ++i)
		  {
			  for (int j = 0; j < w; ++j)
			 {
			 if (i==0 || i==h-1)
			 img[j+i*w]=BorColor;

			 if (j==0 || j==w-1)
			 img[j+i*w]=BorColor;
			 }
		  }
	}

	void Draw(D3DLOCKED_RECT rectangle,RECT rectSize,IDirect3DSurface9* backBuffer)
	{
	 rectSize.left =x;
     rectSize.top =y;
	 rectSize.right = rectSize.left+w;
	 rectSize.bottom =rectSize.top+ h;

	 backBuffer->LockRect(&rectangle,&rectSize,0);		
	 DrawIntObject(rectangle);
	 backBuffer->UnlockRect();
	}
private:
	void DrawIntObject(D3DLOCKED_RECT& lockedRect)
	{
		for (int i = 0; i < h; ++i)
			for (int j = 0; j < w; ++j)
					memcpy(reinterpret_cast<char*>(lockedRect.pBits)+x*4+j*4+i*lockedRect.Pitch+y*lockedRect.Pitch,reinterpret_cast<char*>(&img[j+i*w]),4);
	}
};

