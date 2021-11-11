// Classes+++
class Button {
 public:
  int x, y;
  int w, h;

  bool show;
  bool SubShow;
  bool showBorder;

  Sprite *Image;
  Sprite *Border;

 public:
  Button(int _x, int _y, char *name) : x(_x), y(_y) {
    Image = new Sprite(name);
    w = Image->width;
    h = Image->height;
  }
  Button(char *name) {
    Image = new Sprite(name);
    w = Image->width;
    h = Image->height;
  }
  Button(char *name, int trColor) {
    Image = new Sprite(name, trColor);
    w = Image->width;
    h = Image->height;
  }

  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + w && y >= this->y && y <= this->y + h;
  }

  void draw(Param *p) {
    if (show == true) p->draw(x, y, w, h, Image);
  }
};

class NeoElement : public Sprite, public Button {
 public:
  NeoElement(char *name) : Sprite(name), Button(name) {}
  NeoElement(char *name, int trColor)
      : Sprite(name, trColor), Button(name, trColor) {}

  void ChangeColor(int from, int to) {
    for (int i = 0; i < height; ++i)
      for (int j = 0; j < width; ++j)
        if (Button::Image->img[j + i * width] == from)
          Button::Image->img[j + i * width] = to;
  }
  int FollowColor(int X, int Y) {
    int color = Button::Image->img[X + Y * width];

    return color;
  }
};
class NeoSprite : public Sprite {
 public:
  int w, h;

 public:
  NeoSprite(char *name) : Sprite(name) {
    w = width;
    h = height;
  }
  NeoSprite(char *name, int trColor) : Sprite(name, trColor) {
    w = width;
    h = height;
  }

  void AddImage(int x, int y, Sprite *sp) {
    for (int i = y; i < y + sp->height; i++) {
      for (int j = x; j < x + sp->width; j++) {
        if (j <= width && i <= height &&
            sp->img[(j - x) + (i - y) * sp->width] != TransparentColor)
          img[j + i * width] = sp->img[(j - x) + (i - y) * sp->width];
      }
    }
  }
  void AddImage(int x, int y, Sprite *sp, int i) {
    for (int i = y; i < y + sp->height; i++) {
      for (int j = x; j < x + sp->width; j++) {
        if (j <= width && i <= height)
          img[j + i * width] = sp->img[(j - x) + (i - y) * sp->width];
      }
    }
  }

  int Color(int X, int Y, char letter) {
    int color = img[X + Y * width];
    int r, g, b;
    int final = 0;

    r = (BYTE)color;
    g = (WORD)color >> 8;
    b = (DWORD)color >> 16;

    if (letter == 'R') final = r;

    if (letter == 'G') final = g;

    if (letter == 'B') final = b;

    return final;
  }

  void Save(char *name) {
    std::ofstream os(name, std::ios::binary);

    unsigned char signature[2] = {'B', 'M'};
    unsigned int fileSize = 14 + 40 + w * h * 4;
    unsigned int reserved = 0;
    unsigned int offset = 14 + 40;

    unsigned int headerSize = 40;
    unsigned int dimensions[2] = {w, h};
    unsigned short colorPlanes = 1;
    unsigned short bpp = 32;
    unsigned int compression = 0;
    unsigned int imgSize = dimensions[0] * dimensions[1] * 4;
    unsigned int resolution[2] = {2795, 2795};
    unsigned int pltColors = 0;
    unsigned int impColors = 0;

    os.write(reinterpret_cast<char *>(signature), sizeof(signature));
    os.write(reinterpret_cast<char *>(&fileSize), sizeof(fileSize));
    os.write(reinterpret_cast<char *>(&reserved), sizeof(reserved));
    os.write(reinterpret_cast<char *>(&offset), sizeof(offset));

    os.write(reinterpret_cast<char *>(&headerSize), sizeof(headerSize));
    os.write(reinterpret_cast<char *>(dimensions), sizeof(dimensions));
    os.write(reinterpret_cast<char *>(&colorPlanes), sizeof(colorPlanes));
    os.write(reinterpret_cast<char *>(&bpp), sizeof(bpp));
    os.write(reinterpret_cast<char *>(&compression), sizeof(compression));
    os.write(reinterpret_cast<char *>(&imgSize), sizeof(imgSize));
    os.write(reinterpret_cast<char *>(resolution), sizeof(resolution));
    os.write(reinterpret_cast<char *>(&pltColors), sizeof(pltColors));
    os.write(reinterpret_cast<char *>(&impColors), sizeof(impColors));

    unsigned char x, r, g, b;

    for (int i = dimensions[1]; i > 0; --i) {
      for (int j = 0; j < dimensions[0]; ++j) {
        x = 0;
        r = Color(j, i, 'R');
        g = Color(j, i, 'G');
        b = Color(j, i, 'B');
        os.write(reinterpret_cast<char *>(&b), sizeof(b));
        os.write(reinterpret_cast<char *>(&g), sizeof(g));
        os.write(reinterpret_cast<char *>(&r), sizeof(r));
        os.write(reinterpret_cast<char *>(&x), sizeof(x));
      }
    }

    os.close();
  }
};
struct Point { // Tochka: Point
  int X, Y;
};
// Classes---
class Text {
 public:
  char *string;
  int x, y;

 private:
  Sprite *txt;
  Sprite *letters[77];
  Sprite *lettersColor[77];

  int size;
  int w, h;
  int *encode;

  char *szLetters;
  int initColor;

 public:
  Text(char *str, int color, int _x, int _y) : string(str) {
    w = 11;
    h = 11;

    txt = new Sprite("Images/Text.bmp", 0xffffffff);

    initColor = color;
    ChangeColor(color);

    x = _x;
    y = _y;
    // Conv+++
    size = 0;

    while (string[size] != '\0') size++;

    encode = new int[size];

    szLetters = new char[size];
    szLetters[size] = '\0';

    for (int i = 0; i < size; i++)
      encode[i] = convertion(static_cast<int>(string[i]), i);
  }
  Text(char *str, int color, int _x, int _y, int addC) : string(str) {
    w = 11;
    h = 11;

    txt = new Sprite("Images/Text.bmp", 0xffffffff);

    initColor = color;
    ChangeColor(color);

    x = _x;
    y = _y;
    // Conv+++
    size = 0;

    while (string[size] != '\0') size++;

    encode = new int[size];

    szLetters = new char[size];
    szLetters[size] = '\0';

    for (int i = 0; i < size; i++)
      encode[i] = convertion(static_cast<int>(string[i]), i);

    addColor(addC);
  }

  void draw(Param *p) {
    int n = 0;
    int distance = 0;

    for (int i = 0; i < size; i++) {
      n = encode[i];

      if (szLetters[i] == 'U') distance = 1;

      if (szLetters[i] == 'L') distance = 5;

      if (szLetters[i] == 'N') distance = 5;

      p->draw(x + i * letters[i]->width - distance * i, y, letters[n]->width,
              letters[n]->height, letters[n]);
    }
  }

  void draw(Param *p, int *pos, int s) {
    int n = 0;
    int distance = 0;
    Sprite *temp;

    for (int i = 0; i < size; i++) {
      n = encode[i];
      temp = letters[n];

      //++++
      for (int j = 0; j < s; j++)
        if (i == pos[j]) temp = lettersColor[n];
      //----

      if (szLetters[i] == 'U') distance = 1;

      if (szLetters[i] == 'L') distance = 5;

      if (szLetters[i] == 'N') distance = 5;

      p->draw(x + i * temp->width - distance * i, y, temp->width, temp->height,
              temp);
    }
  }

  void changeText(char *str) {
    string = str;
    size = 0;

    while (string[size] != '\0') size++;

    encode = new int[size];

    szLetters = new char[size];
    szLetters[size] = '\0';

    for (int i = 0; i < size; i++)
      encode[i] = convertion(static_cast<int>(string[i]), i);
  }

 private:
  int convertion(char ASC, int i) {
    int ASCII = static_cast<int>(ASC);
    int sym = 0;
    szLetters[i] = 'O';

    // Symb+++
    switch (ASC) {
      case '.':
        sym = 62;
        break;

      case ':':
        sym = 63;
        break;

      case '/':
        sym = 64;
        break;

      case '-':
        sym = 65;
        break;

      case '+':
        sym = 66;
        break;

      case '(':
        sym = 67;
        break;

      case ')':
        sym = 68;
        break;

      case '*':
        sym = 69;
        break;

      case '=':
        sym = 70;
        break;

      case ',':
        sym = 71;
        break;

      case '!':
        sym = 72;
        break;

      case ' ':
        sym = 73;
        break;

      default:
        if (ASCII - 97 >= 0) {  // low
          sym = ASCII - 97;
          szLetters[i] = 'L';
        } else if (ASCII - 65 >= 0) {  // up
          sym = ASCII - 65 + 26;
          szLetters[i] = 'U';
        } else if (ASCII - 48 >= 0) {  // num
          sym = ASCII - 48 + 52;
          szLetters[i] = 'N';
        }
        break;
    }
    // Symb---

    return sym;
  }

  void ChangeColor(int color) {
    int i = 0;
    int j = 0;

    for (int k = 0; k < 77; k++) {
      letters[k] = new Sprite("Images/Text.bmp", 0xffffffff);
      letters[k]->cut(w * j, h * i, w, h);
      letters[k]->RemplaceColor(0xFF000000, color);

      j++;

      if (j == 26) {
        i++;
        j = 0;
      }
    }
  }

  void addColor(int color) {
    int i = 0;
    int j = 0;

    for (int k = 0; k < 77; k++) {
      lettersColor[k] = new Sprite("Images/Text.bmp", 0xffffffff);
      lettersColor[k]->cut(w * j, h * i, w, h);
      lettersColor[k]->RemplaceColor(0xFF000000, color);

      j++;

      if (j == 26) {
        i++;
        j = 0;
      }
    }
  }
};

class Hero {
 public:
  int x, y;
  int w, h;

  int exitX;
  int exitY;

  int CadrR, CadrL, CadrU;

  Sprite *ImageR[5];
  Sprite *ImageL[5];

  Sprite *ImageUD[4];
  Sprite *ImageJR[4];
  Sprite *ImageJL[4];

  Sprite *Image;

  int MatMap[30][40];

  static int dt;
  static int timer;
  static int timer1;

  static int dtG;
  static int timerG;
  static int timerG1;

  bool move;
  bool L, R, J, U, D, G;

  int velocity, velocityJ;
  int accel, accelJ;

 public:
  Hero()
      : x(0),
        y(0),
        CadrR(0),
        CadrL(0),
        L(false),
        R(false),
        J(false),
        velocity(0),
        accel(3),
        velocityJ(0),
        accelJ(3),
        U(false),
        D(false),
        CadrU(0),
        G(true) {
    Image = NULL;

    ImageR[0] = new Sprite("Images/heroR1.bmp", 0xffffffff);
    ImageR[1] = new Sprite("Images/heroR2.bmp", 0xffffffff);
    ImageR[2] = new Sprite("Images/heroR3.bmp", 0xffffffff);
    ImageR[3] = new Sprite("Images/heroR4.bmp", 0xffffffff);
    ImageR[4] = new Sprite("Images/heroR5.bmp", 0xffffffff);

    ImageL[0] = new Sprite("Images/heroL1.bmp", 0xffffffff);
    ImageL[1] = new Sprite("Images/heroL2.bmp", 0xffffffff);
    ImageL[2] = new Sprite("Images/heroL3.bmp", 0xffffffff);
    ImageL[3] = new Sprite("Images/heroL4.bmp", 0xffffffff);
    ImageL[4] = new Sprite("Images/heroL5.bmp", 0xffffffff);

    ImageJR[0] = new Sprite("Images/heroJ1.bmp", 0xffffffff);
    ImageJR[1] = new Sprite("Images/heroJ2.bmp", 0xffffffff);
    ImageJR[2] = new Sprite("Images/heroJ3.bmp", 0xffffffff);
    ImageJR[3] = new Sprite("Images/heroJ4.bmp", 0xffffffff);

    ImageJL[0] = new Sprite("Images/heroJ1.bmp", 0xffffffff);
    ImageJL[1] = new Sprite("Images/heroJ2.bmp", 0xffffffff);
    ImageJL[2] = new Sprite("Images/heroJ3.bmp", 0xffffffff);
    ImageJL[3] = new Sprite("Images/heroJ4.bmp", 0xffffffff);

    ImageUD[0] = new Sprite("Images/heroU1.bmp", 0xffffffff);
    ImageUD[1] = new Sprite("Images/heroU2.bmp", 0xffffffff);
    ImageUD[2] = new Sprite("Images/heroU3.bmp", 0xffffffff);
    ImageUD[3] = new Sprite("Images/heroU4.bmp", 0xffffffff);

    for (int i = 0; i < 5; i++) {
      if (i != 4) ImageJL[i]->Rotate();
    }
    Image = ImageR[0];

    w = Image->width;
    h = Image->height;
  }

  void MoveR() {
    int step = 7;

    if (R == true) {
      int nx = (x + w + step) / 20;
      int ny = y / 20;
      int ny1 = y / 20 + 1;
      int ny2 = y / 20 + 2;

      if (MatMap[ny][nx] == 0 || MatMap[ny][nx] == 5 || MatMap[ny][nx] == 3 ||
          MatMap[ny][nx] == 9) {
        if (MatMap[ny1][nx] == 0 || MatMap[ny1][nx] == 5 ||
            MatMap[ny1][nx] == 3 || MatMap[ny][nx] == 9) {
          if (MatMap[ny2][nx] == 0 || MatMap[ny2][nx] == 5 ||
              MatMap[ny2][nx] == 3 || MatMap[ny][nx] == 9) {
            ChangeImage('R');
            x += step;
            timer1 = 0;
          }
        }
      }
      R = false;
    }
  }

  void MoveL() {
    int step = 7;

    if (L == true) {
      int nx = (x - step) / 20;
      int ny = y / 20;
      int ny1 = y / 20 + 1;
      int ny2 = y / 20 + 2;

      if (MatMap[ny][nx] == 0 || MatMap[ny][nx] == 5 || MatMap[ny][nx] == 3 ||
          MatMap[ny][nx] == 9) {
        if (MatMap[ny1][nx] == 0 || MatMap[ny1][nx] == 5 ||
            MatMap[ny1][nx] == 3 || MatMap[ny][nx] == 9) {
          if (MatMap[ny2][nx] == 0 || MatMap[ny2][nx] == 5 ||
              MatMap[ny2][nx] == 3 || MatMap[ny][nx] == 9) {
            ChangeImage('L');
            x -= step;
            timer1 = 0;
          }
        }
      }
      L = false;
    }
  }
  void Gravitaton() {
    if (J == false && G == true) {
      if (velocity <= 20) velocity += accel;

      int nx = x / 20;
      int nx1 = (x + w) / 20;
      int ny = (y + h + velocity) / 20;

      if (MatMap[ny][nx] == 0 && MatMap[ny][nx1] == 0) {
        y += velocity;
        timerG1 = 0;
      } else if (MatMap[ny][nx] != 0) {
        y = ny * 20 - h;

        velocity = 0;
      }
    }
  }
  void Jump() {
    if (J == true && dtG > 20) {
      velocityJ -= accel;

      int nx = x / 20;
      int ny = (y - velocityJ) / 20;

      if (MatMap[ny][nx] == 0 && velocityJ >= 0) {
        y -= velocityJ;

        timerG1 = 0;
      } else {
        velocityJ = 0;
        timerG1 = 0;
        J = false;
      }
    }
  }
  void UD(char d) {
    switch (d) {
      case 'U':
        if (U == true) {
          int nx = x / 20;
          int ny = y / 20;

          if (MatMap[ny][nx] == 3) {
            ChangeImage('U');
            y -= 5;
          }

          if (MatMap[ny][nx] == 0 && MatMap[(y + h) / 20][nx] == 3 &&
              MatMap[ny + 1][nx] == 3)
            y = (y / 20 + 1) * 20 - h;

          U = false;
          timer1 = 0;
        }
        break;

      case 'D':
        if (D == true) {
          int nx = x / 20;
          int ny = (y + h + 5) / 20;

          if (MatMap[ny][nx] == 3) {
            ChangeImage('U');
            y += 5;
          }

          D = false;
          timer1 = 0;
        }
        break;
    }
  }

  void ChangeImage(char d) {
    switch (d) {
      case 'L':
        if (CadrL < 4)
          CadrL++;
        else
          CadrL = 0;

        Image = ImageL[CadrL];
        break;

      case 'R':
        if (CadrR < 4)
          CadrR++;
        else
          CadrR = 0;

        Image = ImageR[CadrR];
        break;

      case 'U':
        if (CadrU < 3)
          CadrU++;
        else
          CadrU = 0;

        Image = ImageUD[CadrU];
        break;
    }

    w = Image->width;
    h = Image->height;
  }
  void draw(Param *p) { p->draw(x, y, Image->width, Image->height, Image); }
  bool ChangeLevel() {
    bool c = false;

    if (x >= exitX && x <= exitX + 5 && y >= exitY && y <= exitY + 5) c = true;

    if (x + Image->width >= exitX && x + Image->width <= exitX + 5 &&
        y >= exitY && y <= exitY + 5)
      c = true;

    return c;
  }

  static void Timer() {
    if (timer1 == 0)
      timer1 =
          timeGetTime();  // https://docs.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timegettime

    timer = timeGetTime();
    dt = timer - timer1;
  }
  static void TimerG() {
    if (timerG1 == 0) timerG1 = timeGetTime();

    timerG = timeGetTime();
    dtG = timerG - timerG1;
  }

  void ChargeMatMap(int MatMap1[30][40]) {
    for (int i = 0; i < 30; i++)
      for (int j = 0; j < 40; j++) MatMap[i][j] = MatMap1[i][j];
  }
};
int Hero::dt = 0;
int Hero::timer = 0;
int Hero::timer1 = 0;

int Hero::dtG = 0;
int Hero::timerG = 0;
int Hero::timerG1 = 0;

class Fire {
 public:
  int x, y;
  int frame;
  Sprite *currentSprite;
  Sprite *Image[3];

  static int counter;

 public:
  Fire(int _x, int _y) : x(_x), y(_y), frame(0) {
    currentSprite = NULL;

    Image[0] = new Sprite("Images/fire1.bmp", 0xffffffff);
    Image[1] = new Sprite("Images/fire2.bmp", 0xffffffff);
    Image[2] = new Sprite("Images/fire3.bmp", 0xffffffff);

    currentSprite = Image[0];

    counter++;
  }
  void updateFrame() {  // ChangeCadr: updateFrame
    if (!shouldUpdate()) return;

    frame = (frame + 1) % 3;
    currentSprite = Image[frame];
  }

 private:
  int startTime;

  bool shouldUpdate() {
    if (startTime == NULL) startTime = timeGetTime();
    int timeSinceLastFrame = timeGetTime() - startTime;

    if (timeSinceLastFrame > 50) {
      startTime = timeGetTime();
    }
    return timeSinceLastFrame > 50;
  }
};
int Fire::counter = 0;

class Door {
 public:
  int x, y;
  int Cadr;
  int num;

  Sprite *ImageView;
  Sprite *Image[2];

  static int counter;

 public:
  Door(int _x, int _y) : x(_x), y(_y), Cadr(0), num(5) {
    ImageView = NULL;
    Image[0] = new Sprite("Images/Door1.bmp", 0xffffffff);
    Image[1] = new Sprite("Images/Door2.bmp", 0xffffffff);

    ImageView = Image[0];

    counter++;
  }

  bool contains(int x, int y) {  // Touch: contains
    return x >= this->x && x <= this->x + ImageView->width && y >= this->y && y <= this->y + ImageView->height;
  }

 private:
  void updateFrame(int MatMap[30][40]) {  // ChangeCadr
    if (Cadr < 1)
      Cadr++;
    else
      Cadr = 0;

    ImageView = Image[Cadr];

    MatMap[y / 20][x / 20] = num;
    MatMap[y / 20 + 1][x / 20] = num;
    MatMap[y / 20 + 2][x / 20] = num;
    MatMap[y / 20 + 3][x / 20] = num;

    if (num == 5)
      num = 4;
    else
      num = 5;
  }
};
int Door::counter = 0;

class Book {
 public:
  int x, y;
  bool show;

  Sprite *Image;

  Sprite *Image1;
  Sprite *ImageBack;

  static int counter;
  char state;

 public:
  Book(int _x, int _y) : x(_x), y(_y), show(true), state('C') {
    ImageBack = NULL;
    Image = NULL;
    Image1 = new Sprite("Images/book.bmp");

    Image = Image1;
    counter++;
  }

  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + Image->width && y >= this->y && this->y <= y + Image->height;
  }
};
int Book::counter = 0;

class Bonus {
 public:
  int x, y;
  int frame;  // Cadr: frame
  bool show;

  Sprite *currentFrame;  // ImageView: currentFrame
  Sprite *Image[10];

  static int counter;

  int c;  // TODO: delete this?

 public:
  Bonus(int _x, int _y) : x(_x), y(_y), frame(0), show(true), c(0) {
    currentFrame = NULL;

    Image[0] = new Sprite("Images/bonus1.bmp", 0xffffffff);
    Image[1] = new Sprite("Images/bonus2.bmp", 0xffffffff);
    Image[2] = new Sprite("Images/bonus3.bmp", 0xffffffff);
    Image[3] = new Sprite("Images/bonus4.bmp", 0xffffffff);
    Image[4] = new Sprite("Images/bonus5.bmp", 0xffffffff);
    Image[5] = new Sprite("Images/bonus6.bmp", 0xffffffff);
    Image[6] = new Sprite("Images/bonus7.bmp", 0xffffffff);
    Image[7] = new Sprite("Images/bonus8.bmp", 0xffffffff);
    Image[8] = new Sprite("Images/bonus9.bmp", 0xffffffff);
    Image[9] = new Sprite("Images/bonus10.bmp", 0xffffffff);

    currentFrame = Image[0];

    counter++;
  }

  void udpateFrame() {  // ChangeCadr: udpateFrame
    if (!shouldUpdate()) return;

    frame = (frame + 1) % 9;
    currentFrame = Image[frame];
  }

  bool contains(int x, int y) {  // Touch: contains
    return x >= this->x && x <= this->x + currentFrame->width && y >= this->y &&
           y <= this->y + currentFrame->height;
  }

 private:
  int startTime;

  bool shouldUpdate() {
    if (startTime == NULL) startTime = timeGetTime();
    int timeSinceLastFrame = timeGetTime() - startTime;

    if (timeSinceLastFrame > 50) {
      startTime = timeGetTime();
    }
    return timeSinceLastFrame > 50;
  }
};
int Bonus::counter = 0;

class Arrow {
 public:
  int x, y;
  int num;

  Sprite *Image;
  Sprite *Images[40];

 public:
  Arrow() {
    x = 0;
    y = 0;
    num = 0;

    Images[0] = new Sprite("Images/c0.bmp", 0xffffffff);
    Images[1] = new Sprite("Images/c1.bmp", 0xffffffff);
    Images[2] = new Sprite("Images/c2.bmp", 0xffffffff);
    Images[3] = new Sprite("Images/c3.bmp", 0xffffffff);
    Images[4] = new Sprite("Images/c4.bmp", 0xffffffff);
    Images[5] = new Sprite("Images/c5.bmp", 0xffffffff);
    Images[6] = new Sprite("Images/c6.bmp", 0xffffffff);
    Images[7] = new Sprite("Images/c7.bmp", 0xffffffff);
    Images[8] = new Sprite("Images/c8.bmp", 0xffffffff);
    Images[9] = new Sprite("Images/c9.bmp", 0xffffffff);
    Images[10] = new Sprite("Images/c10.bmp", 0xffffffff);
    Images[11] = new Sprite("Images/c11.bmp", 0xffffffff);
    Images[12] = new Sprite("Images/c12.bmp", 0xffffffff);
    Images[13] = new Sprite("Images/c13.bmp", 0xffffffff);
    Images[14] = new Sprite("Images/c14.bmp", 0xffffffff);
    Images[15] = new Sprite("Images/c15.bmp", 0xffffffff);
    Images[16] = new Sprite("Images/c16.bmp", 0xffffffff);
    Images[17] = new Sprite("Images/c17.bmp", 0xffffffff);
    Images[18] = new Sprite("Images/c18.bmp", 0xffffffff);
    Images[19] = new Sprite("Images/c19.bmp", 0xffffffff);
    Images[20] = new Sprite("Images/c20.bmp", 0xffffffff);
    Images[21] = new Sprite("Images/c21.bmp", 0xffffffff);
    Images[22] = new Sprite("Images/c22.bmp", 0xffffffff);
    Images[23] = new Sprite("Images/c23.bmp", 0xffffffff);
    Images[24] = new Sprite("Images/c24.bmp", 0xffffffff);
    Images[25] = new Sprite("Images/c25.bmp", 0xffffffff);
    Images[26] = new Sprite("Images/c26.bmp", 0xffffffff);
    Images[27] = new Sprite("Images/c27.bmp", 0xffffffff);
    Images[28] = new Sprite("Images/c28.bmp", 0xffffffff);
    Images[29] = new Sprite("Images/c29.bmp", 0xffffffff);
    Images[30] = new Sprite("Images/c30.bmp", 0xffffffff);
    Images[31] = new Sprite("Images/c31.bmp", 0xffffffff);
    Images[32] = new Sprite("Images/c32.bmp", 0xffffffff);
    Images[33] = new Sprite("Images/c33.bmp", 0xffffffff);
    Images[34] = new Sprite("Images/c34.bmp", 0xffffffff);
    Images[35] = new Sprite("Images/c35.bmp", 0xffffffff);
    Images[36] = new Sprite("Images/c36.bmp", 0xffffffff);
    Images[37] = new Sprite("Images/c37.bmp", 0xffffffff);
    Images[38] = new Sprite("Images/c38.bmp", 0xffffffff);
    Images[39] = new Sprite("Images/c39.bmp", 0xffffffff);
  }

  void draw(Param *p) {
    Image = Images[num];

    p->draw(x, y, Image->width, Image->height, Image);
  }
};

class Inventory { // Inventar: Inventory
 public:
  int x, y;
  int mX, mY;
  int w, h;
  int objects[9];
  int move;
  int check;

  bool show;
  bool exp;

  static bool iLmb;
  static int Xi;
  static int Yi;

  Sprite *Image;
  Sprite *Open;
  Sprite *toolTip; // InventarExp: toolTip
  Sprite *ImObjects[9];

 public:
  Inventory() : exp(false), move(false) {
    x = 0;
    y = 0;
    mX = 0;
    mY = 0;

    Image = new Sprite("Images/inventory/Inventar.bmp", 0xffffffff);
    Open = new Sprite("Images/inventory/InventarOpen.bmp", 0xffffffff);
    toolTip = new Sprite("Images/inventory/InventarExp.bmp", 0xffffffff);

    for (int i = 0; i < 9; i++) objects[i] = 0;

    ChangeImages();
  }

  void draw(Param *p) {
    p->draw(x, y, Image->width, Image->height, Image);

    // 0---
    if (objects[0] != 0)
      p->draw(17 + 0 * 7 + 0 * 25 - 7, 7, ImObjects[0]->width,
              ImObjects[0]->height, ImObjects[0]);
    // 0+++

    if (show == true) {
      p->draw(x, y, Open->width, Open->height, Open);

      for (int i = 0; i < 9; i++) {
        if (objects[i] != 0) {
          int l = 17 + i * 7 + i * 25;

          if (i == 0) l = 17 + i * 7 + i * 25 - 7;

          p->draw(l, 7, ImObjects[i]->width, ImObjects[i]->height,
                  ImObjects[i]);
        }
      }
    }

    if (exp == true)
      p->draw(mX, mY, toolTip->width, toolTip->height, toolTip);
  }
  void ChangeImages() {
    char *path = new char[30];
    int num;

    for (int i = 0; i < 9; i++) {
      if (objects[i] != 0) {
        num = sprintf(path, "Images/o%d.bmp", objects[i]);

        ImObjects[i] = new Sprite(path, 0xffffffff);
      }
    }
  }
  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + Image->width && y >= this->y && y <= this->y + Image->height;
  }
  int TouchObject(int X, int Y) {
    // "Images/inventory/InventarOpen.bmp"
    int x1 = 17;
    int x2 = 297;
    int y1 = 7;
    int y2 = 31;
    int cellSize = 25;
    int cellSpace = 7;

    int block = (X - x1) / (cellSize + cellSpace);
    bool isWithinInventory = X >= x1 && X <= x2 && Y >= y1 && Y <= y2;
    bool isWithinCell = X - x1 - block * (cellSize + cellSpace) <= cellSize;

    if (isWithinInventory && isWithinCell) return block;

    return -1;
  }

  static void SetPosition(int X, int Y) {
    Xi = X;
    Yi = Y;
    iLmb = true;
  }

  int TouchInvShow(int X, int Y) {
    bool t = false;

    if (X >= x && X <= x + Open->width && Y >= y && Y <= y + Open->height)
      t = true;

    return t;
  }

  void AddObject(int num) {
    for (int i = 0; i < 9; i++) {
      if (num != 0 && objects[i] == 0) {
        objects[i] = num;
        ChangeImages();
        break;
      }
    }
  }
};
bool Inventory::iLmb = false;
int Inventory::Xi = 0;
int Inventory::Yi = 0;

class Chest {
 public:
  int x, y;
  int Cadr;
  int mX1, mY1;
  int Answer;
  int check;
  int objects[6 * 8];
  int sd;
  int cd[3];
  int combinaison;

  bool expO;
  bool show;
  bool showC;
  bool move;
  bool BOk;

  static bool iLmb;
  static int counter;
  static int Xi;
  static int Yi;
  static int dt;
  static int timer;
  static int timer1;

  char *question;

  Sprite *ImageView;
  Sprite *ImObjects[6 * 8];
  Sprite *Image[2];
  Sprite *code;
  Sprite *OpenExp;
  Sprite *Content;
  Sprite *OkOpen;
  Text *txt;
  Text *codeView;
  Button *lock;
  Button *locker;
  Button *LockerLight;
  Button *lOK;

  Arrow *ar;

 public:
  Chest(int _x, int _y, int Level, int ans1, int ans2)
      : x(_x),
        y(_y),
        Cadr(0),
        show(false),
        expO(false),
        showC(false),
        move(false),
        check(0) {
    Image[0] = new Sprite("Images/chest/chest1.bmp");
    Image[1] = new Sprite("Images/chest/chest2.bmp");
    code = new Sprite("Images/chest/code.bmp");
    OpenExp = new Sprite("Images/chest/OpenChest.bmp", 0xffffffff);
    Content = new Sprite("Images/chest/InvChest.bmp", 0xffffffff);
    OkOpen = new Sprite("Images/chest/OpenTrue.bmp", 0xffffffff);
    ImageView = Image[0];

    counter++;
    Answer = ans1 * 1000 + ans2;

    BOk = false;
    //----
    for (int i = 0; i < 6 * 8; i++) objects[i] = 0;

    objects[0] = 2;

    ChangeImages();
    //+++
    sd = 0;
    for (int i = 0; i < 3; i++) cd[i] = 0;

    combinaison = 0;

    InitButtons();
  }
  void InitButtons() {
    lock = new Button("Images/chest/code.bmp", 0xffffffff);
    lock->x = (w - lock->w) / 2;
    lock->y = (h - lock->h) / 2;
    lock->show = true;

    ar = new Arrow();
    ar->x = lock->x + 145;
    ar->y = lock->y + 149;

    locker = new Button("Images/chest/locker.bmp", 0xffffffff);
    locker->x = (w - lock->w) / 2 + 99;
    locker->y = (h - lock->h) / 2 + 107;
    locker->show = true;

    LockerLight = new Button("Images/chest/lockerL.bmp", 0xffffffff);
    LockerLight->x = locker->x;
    LockerLight->y = locker->y;
    LockerLight->show = false;

    lOK = new Button("Images/chest/lOK.bmp", 0xffffffff);
    lOK->x = lock->x + 156;
    lOK->y = lock->y + 161;
    lOK->show = false;

    codeView = new Text("00 00 00", 0xFF808E9B, locker->x + 100,
                        locker->y + 100, 0xFFED1C24);  // code
  }

  int TouchObject(int X, int Y) {
    // This data is pulled from "Images/chest/InvChest.bmp"
    int inventoryChestLeft = 71;
    int inventoryChestTop = 109;
    int inventoryChestRight = 305;
    int inventoryChestBottom = 283;
    int inventorySquareDim = 25;  // size of a single cell
    int spaceBetweenCells = 5;

    int x1 = inventoryChestLeft + (w - code->width) / 2;
    int x2 = inventoryChestRight + (w - code->width) / 2;
    int y1 = inventoryChestTop + (h - code->height) / 2;
    int y2 = inventoryChestBottom + (h - code->height) / 2;

    int blockX = (X - x1) / (inventorySquareDim + spaceBetweenCells);
    int blockY = (Y - y1) / (inventorySquareDim + spaceBetweenCells);
    bool isWithinInventory = X >= x1 && X <= x2 && Y >= y1 && Y <= y2;

    if (isWithinInventory) {
      if (X - x1 - blockX * (inventorySquareDim + spaceBetweenCells) <=
          inventorySquareDim)
        if (Y - y1 - blockY * (inventorySquareDim + spaceBetweenCells) <=
            inventorySquareDim)
          return blockY * 8 + blockX;
    }

    return -1;
  }
  bool TouchInvChest(int X, int Y) {
    bool t = false;

    int x1 = 71 + (w - code->width) / 2, x2 = 305 + (w - code->width) / 2;
    int y1 = 109 + (h - code->height) / 2, y2 = 283 + (h - code->height) / 2;

    if (X >= x1 && X <= x2 && Y >= y1 && Y <= y2) t = true;

    return t;
  }
  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + ImageView->width && y >= this->y && y <= this->y + ImageView->height;
  }

  static void SetPosition(int X, int Y) {
    Xi = X;
    Yi = Y;
    iLmb = true;
  }
  static void Timer() {
    if (timer1 == 0) timer1 = timeGetTime();

    timer = timeGetTime();
    dt = timer - timer1;
  }

  void ChangeImages() {
    char *path = new char[30];
    int num;

    for (int i = 0; i < 6 * 8; i++) {
      if (objects[i] != 0) {
        num = sprintf(path, "Images/o%d.bmp", objects[i]);

        ImObjects[i] = new Sprite(path, 0xffffffff);
      }
    }
  }
  void LoadQuestion(int Level) {
    char *c = new char[300];
    std::ifstream ifs("Images/Data/questions.txt");

    for (int i = 0; i < 300; i++) ifs >> c[i];

    ifs.close();
    //+++++++++++++
    int j = 0;
    int cont = 0;
    int pos = 0;

    while (c[j] != '\0') {
      if (c[j] == '%') {
        cont++;

        if (cont == Level) pos = j + 1;
      }

      if (cont == Level + 1) {
        char *str = new char[j - pos];

        for (int z = 0; z < j - pos; z++) str[z] = c[z + pos];

        str[j - pos] = '\0';

        int k = 0;

        while (str[k] != '$') k++;

        // QUESTION+++
        question = new char[k];

        for (int z1 = 0; z1 < k; z1++) question[z1] = str[z1];

        question[k] = '\0';
        // ANSWER+++
        char *temp = new char[j - (k + 1)];

        for (int z2 = 0; z2 < j - (k + 1); z2++) temp[z2] = str[z2 + k + 1];

        temp[j - (k + 1)] = '\0';

        Answer = atoi(temp);
        // ANSWER---
      }

      j++;
    }

    txt = new Text(question, 0xFF3D3D3D, (w - code->width) / 2 + 15,
                   (h - code->height) / 2 + 68);
  }
  //+++
  void OpenLock(Inventory *Inv, bool &lmb, int X, int Y, int mX, int mY) {
    if (show == true) {
      if (lock->contains(X, Y) == false && lmb == true &&
          Inv->TouchInvShow(X, Y) == false)
        show = false;

      int mmx = mX - locker->x;
      int mmy = mY - locker->y;
      int dist = sqrt((mmx - locker->w / 2) * (mmx - locker->w / 2) +
                      (locker->h / 2 - mmy) * (locker->h / 2 - mmy));

      if (lOK->contains(X, Y) == true && lmb == true) {
        lOK->show = true;

        if (sd < 2)
          sd++;
        else
          sd = 0;

        lmb = false;
      } else
        lOK->show = false;

      if (LockerLight->contains(mX, mY) == true && dist > 55 && dist < 88) {
        LockerLight->show = true;

        if (locker->contains(X, Y) == true && lmb == true) {
          // Calculate angle+++
          double xn = locker->w / 2;
          double yn = locker->h / 2;

          double x1 = 0;
          double y1 = locker->h / 2;

          double xm = (X - locker->x) - xn;
          double ym = yn - (Y - locker->y);

          double a2 = sqrt(xm * xm + ym * ym);

          double angle1 = acos(ym / a2);
          double angle2 = (angle1 * 180) / 3.14;

          if (X - locker->x < xn) angle2 = 360 - angle2;

          int angle = angle2 / 9;

          cd[sd] = angle;
          combinaison = cd[0] * 10000 + cd[1] * 100 + cd[2];
          // Calculate angle---

          // Show angle+++
          ar->num = angle;

          //Показ комбинации на экран+++
          char *showText = new char[100];
          int ran = 0;

          if (cd[0] < 10)
            ran = sprintf(showText, "0%d 0%d 0%d", cd[0], cd[1], cd[2]);
          else
            // ran=sprintf(showText,"%d %d %d",cd[0],cd[1],cd[2]);

            codeView->changeText(showText);
          //Показ комбинации на экран---
          // Show angle---

          lmb = false;
        }
      } else {
        LockerLight->show = false;
      }

      // Open++++
      if (combinaison == Answer && sd == 0) BOk = true;

      if (BOk == true) {
        Timer();

        if (dt > 2000) {
          showC = true;
          BOk = false;
          combinaison = 0;

          dt = 0;
          timer = 0;
          timer1 = 0;
        }
      }
      // Open----
    }
  }
  //++++
  void draw(Param *p) {
    p->draw(x, y, ImageView->width, ImageView->height, ImageView);

    if (expO == true)
      p->draw(mX1, mY1, OpenExp->width, OpenExp->height, OpenExp);
  }
  void drawC(Param *p) {
    lock->draw(p);

    if (showC == false) locker->draw(p);

    ar->draw(p);
    //---
    int *pos = new int[2];

    pos[0] = sd;
    pos[1] = sd + 1;

    codeView->draw(p, pos, 2);
    //+++
    LockerLight->draw(p);
    lOK->draw(p);
    txt->draw(p);

    if (showC == true) {
      p->draw((w - code->width) / 2, (h - code->height) / 2, Content->width,
              Content->height, Content);

      int ix = 0;
      int iy = 0;

      for (int i = 0; i < 6 * 8; i++) {
        if (objects[i] != 0)
          p->draw((w - code->width) / 2 + 71 + (ix)*5 + (ix)*25,
                  (h - code->height) / 2 + 109 + (iy)*5 + (iy)*25,
                  ImObjects[i]->width, ImObjects[i]->height, ImObjects[i]);

        if (ix < 7)
          ix++;
        else {
          iy++;
          ix = 0;
        }
      }
    }

    if (BOk == true)
      p->draw((w - lock->w) / 2 + 99 + 2, (h - lock->h) / 2 + 107,
              OkOpen->width, OkOpen->height, OkOpen);
  }
};
int Chest::dt = 0;
int Chest::timer = 0;
int Chest::timer1 = 0;

int Chest::counter = 0;
bool Chest::iLmb = false;
int Chest::Xi = 0;
int Chest::Yi = 0;

class ButtonON {
 public:
  int x, y;
  Sprite *Image;

  static int counter;

 public:
  ButtonON(int _x, int _y) : x(_x), y(_y) {
    Image = new Sprite("Images/buttonON.bmp");

    counter++;
  }

  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + Image->width && y >= this->y && y <= this->y + Image->height;
  }
};
int ButtonON::counter = 0;

class BlockMoves {
 public:
  int x, y;
  int pX, pY;
  int pI;

  Sprite *Image;

  static bool UP;
  static int counter;
  static int dt;
  static int timer;
  static int timer1;

 public:
  BlockMoves(int _x, int _y, int _pI)
      : x(_x), y(_y - 1), pX(_x), pY(_y - 1), pI(_pI - 1) {
    Image = new Sprite("Images/blockM.bmp");

    counter++;
  }

  bool BlockMoveUp(int MatMap[30][40]) {
    if (y > pI) {
      MatMap[pY / 20 + 1][pX / 20] = 0;

      if (y - pI <= 5)
        y -= (y - pI);
      else
        y -= 5;
    }

    if (y == pI) {
      MatMap[y / 20 + 1][x / 20] = 6;
      return false;
    }

    return true;
  }
  void BlockMoveDown(int MatMap[30][40]) {
    if (y < pY) {
      MatMap[pI / 20 + 1][x / 20] = 0;

      if (pY - y <= 5)
        y += (pY - y);
      else
        y += 5;
    }

    if (y == pY) {
      MatMap[y / 20 + 1][x / 20] = 6;
    }
  }

  static void Timer() {
    if (timer1 == 0) timer1 = timeGetTime();

    timer = timeGetTime();
    dt = timer - timer1;
  }
};
bool BlockMoves::UP = false;
int BlockMoves::counter = 0;
int BlockMoves::dt = 0;
int BlockMoves::timer = 0;
int BlockMoves::timer1 = 0;

class FinalDoor {
 public:
  int x, y;
  int Cadr;
  int num;
  bool needsKey;  // Need key

  Sprite *ImageView;
  Sprite *Image[2];
  Sprite *NeedKey;

  static int counter;

  int type;

 public:
  FinalDoor(int _x, int _y, int _type)
      : x(_x), y(_y), Cadr(0), num(9), type(_type) {
    ImageView = NULL;
    Image[0] = new Sprite("Images/NextDoor1.bmp", 0xffffffff);
    Image[1] = new Sprite("Images/NextDoor2.bmp", 0xffffffff);

    ImageView = Image[0];

    NeedKey = new Sprite("Images/NeedKey.bmp", 0xffffffff);
    nk = false;

    counter++;
    //
    if (type == 1) {
      Image[0]->Rotate();
      Image[1]->Rotate();
    }
  }

  bool contains(int x, int y) {  // Touch: contains
    return x >= this->x && x <= this->x + ImageView->width && y >= this->y &&
           y <= this->y + ImageView->height;
  }

  void draw(Param *p) {
    p->draw((x + Image[0]->width) - ImageView->width, y, ImageView->width,
            ImageView->height, ImageView);

    // NeedKey
    if (nk == true)
      p->draw((w - NeedKey->width) / 2, (h - NeedKey->height) / 2,
              NeedKey->width, NeedKey->height, NeedKey);
  }

  void updateFrame(int MatMap[30][40]) {  // ChangeCadr: updateFrame
    if (type == 0) {
      if (Cadr < 1)
        Cadr++;
      else
        Cadr = 0;

      ImageView = Image[Cadr];

      MatMap[y / 20][x / 20] = num;
      MatMap[y / 20 + 1][x / 20] = num;
      MatMap[y / 20 + 2][x / 20] = num;
      MatMap[y / 20 + 3][x / 20] = num;

      if (num == 9)
        num = 8;
      else
        num = 9;
    }
  }
};
int FinalDoor::counter = 0;

class DrawRectangle {
 public:
  int x, y;
  int w, h;

  int *img;

  int BorderC;
  int FillC;

 public:
  DrawRectangle(int _x, int _y, int _w, int _h, int BorColor, int FillColor) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;

    BorderC = BorColor;
    FillC = FillColor;

    img = new int[w * h * 32 / 8];

    for (int i = 0; i < h; ++i)
      for (int j = 0; j < w; ++j) img[j + i * w] = FillColor;

    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        if (i == 0 || i == h - 1) img[j + i * w] = BorColor;

        if (j == 0 || j == w - 1) img[j + i * w] = BorColor;
      }
    }
  }

  void draw(Param *p) {
    p->rectSize.left = x;
    p->rectSize.top = y;
    p->rectSize.right = p->rectSize.left + w;
    p->rectSize.bottom = p->rectSize.top + h;

    p->backBuffer->LockRect(&p->rectangle, &p->rectSize, 0);
    DrawIntObject(p->rectangle);
    p->backBuffer->UnlockRect();
  }

 private:
  void DrawIntObject(D3DLOCKED_RECT &lockedRect) {
    for (int i = 0; i < h; ++i)
      for (int j = 0; j < w; ++j)
        memcpy(reinterpret_cast<char *>(lockedRect.pBits) + x * 4 + j * 4 +
                   i * lockedRect.Pitch + y * lockedRect.Pitch,
               reinterpret_cast<char *>(&img[j + i * w]), 4);
  }
};

