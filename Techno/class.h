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
  int convertion(char c, int i) {
    szLetters[i] = 'O';

    if (c == '.') return 62;
    if (c == ':') return 63;
    if (c == '/') return 64;
    if (c == '-') return 65;
    if (c == '+') return 66;
    if (c == '(') return 67;
    if (c == ')') return 68;
    if (c == '*') return 69;
    if (c == '=') return 70;
    if (c == ',') return 71;
    if (c == '!') return 72;
    if (c == ' ') return 73;

    int ASCII = static_cast<int>(c);
    if (ASCII - 97 >= 0) {  // low
      szLetters[i] = 'L';
      return ASCII - 97;
    } else if (ASCII - 65 >= 0) {  // up
      szLetters[i] = 'U';
      return ASCII - 65 + 26;
    } else if (ASCII - 48 >= 0) {  // num
      szLetters[i] = 'N';
      return ASCII - 48 + 52;
    }

    return 0;
  }

  void ChangeColor(int color) {
    int i = 0;
    int j = 0;

    for (int k = 0; k < 77; k++) {
      letters[k] = new Sprite("Images/Text.bmp", 0xffffffff);
      letters[k]->cut(w * j, h * i, w, h);
      letters[k]->replaceColor(0xFF000000, color);

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
      lettersColor[k]->replaceColor(0xFF000000, color);

      j++;

      if (j == 26) {
        i++;
        j = 0;
      }
    }
  }
};

class Player { // Hero: Player
 public:
  int x, y;
  int w, h;

  int exitX;
  int exitY;

  Sprite *currentFrame; // Image: currentFrame

  int gameMap[30][40]; // MatMap: gameMap

  bool isMovingLeft, isMovingRight, isJumping; // L, R, J
  bool isClimbingUp, isClimbingDown, canFall; // U, D, G

  int jumpVelocity; // velocityJ: jumpVelocity

 private:
  int rightWalkFrame, leftWalkFrame, climbFrame; // CadrR, CadrL, CadrU
  
  Sprite *rightWalk[5]; // ImageR: rightWalk
  Sprite *leftWalk[5]; // ImageL: leftWalk

  Sprite *ladderClimb[4]; // ImageUD: ladderClimb
  Sprite *rightJump[4]; // ImageJR: rightJump
  Sprite *leftJump[4]; // ImageJL: leftJump

  // Gravity
  int velocity;
  const int acceleration = 3; // accel: acceleration
  const int step = 7;

  int startTime;
  int verticalStartTime;

 public:
  Player() : x(0), y(0), velocity(0), jumpVelocity(0) {
    isMovingLeft = false;
    isMovingRight = false;
    isJumping = false;
    isClimbingUp = false;
    isClimbingDown = false;
    canFall = true;

    rightWalkFrame = 0;
    leftWalkFrame = 0;
    climbFrame = 0;

    int alphaChannel = 0xffffffff;

    for (int i = 0; i < 5; i++) {
      rightWalk[i] = new Sprite(getAssetName(i + 1, 'R'), alphaChannel);
      leftWalk[i] = new Sprite(getAssetName(i + 1, 'L'), alphaChannel);
    }

    for (int i = 0; i < 3; i++) {
      rightJump[i] = new Sprite(getAssetName(i + 1, 'J'), alphaChannel);
      leftJump[i] = new Sprite(getAssetName(i + 1, 'J'), alphaChannel);
      leftJump[i]->flipHorizontally();

      ladderClimb[i] = new Sprite(getAssetName(i + 1, 'U'), alphaChannel);
    }
    currentFrame = rightWalk[0];

    w = currentFrame->width;
    h = currentFrame->height;
  }

  void moveRight() { // MoveR: moveRight
    if (!isMovingRight) return;
    isMovingRight = false;

    int followingColumn = (x + w + step) / blockSize;
    int playerRow1 = y / blockSize;
    int playerRow2 = playerRow1 + 1;
    int playerRow3 = playerRow1 + 2;

    if (isPassThroughBlock(playerRow1, followingColumn) && 
        isPassThroughBlock(playerRow2, followingColumn) &&
        isPassThroughBlock(playerRow3, followingColumn)) {
      updateFrame('R');
      x += step;
    }
  }

  void moveLeft() { // MoveL: moveLeft
    if (!isMovingLeft) return;
    isMovingLeft = false;

    int previousColumn = (x - step) / blockSize;
    int playerRow1 = y / blockSize;
    int playerRow2 = playerRow1 + 1;
    int playerRow3 = playerRow1 + 2;

    if (isPassThroughBlock(playerRow1, previousColumn) && 
        isPassThroughBlock(playerRow2, previousColumn) &&
        isPassThroughBlock(playerRow3, previousColumn)) {
      updateFrame('L');
      x -= step;
    }
  }
  void gravity() { // Gravitaton: gravity
    if (isJumping) return;
    if (!canFall) return;

    if (velocity <= 20) velocity += acceleration;

    int nx = x / blockSize;
    int nx1 = (x + w) / blockSize;
    int ny = (y + h + velocity) / blockSize;

    if (gameMap[ny][nx] == AIR_ID) {
      if (gameMap[ny][nx1] == AIR_ID) {
        y += velocity;
      }
    } else {
      y = ny * blockSize - h;
      velocity = 0;
    }
  }
  void jump() { // Jump: jump
    if (!isJumping) return;
    
    jumpVelocity -= acceleration;

    int nx = x / blockSize;
    int ny = (y - jumpVelocity) / blockSize;

    if (gameMap[ny][nx] == AIR_ID && jumpVelocity >= 0) {
      y -= jumpVelocity;
      return;
    }

    jumpVelocity = 0;
    isJumping = false;
  }
  void moveUpLadder() {  // UD: moveUpLadder()
    if (!isClimbingUp) return;
    isClimbingUp = false;

    int nx = x / blockSize;
    int playerTopRow = y / blockSize;
    int playerBottomRow = (y + h) / blockSize;

    if (gameMap[playerTopRow][nx] == LADDER_ID) {
      updateFrame('U');
      y -= 5;
    }

    if (gameMap[playerTopRow][nx] == AIR_ID && 
        gameMap[playerBottomRow][nx] == LADDER_ID &&
        gameMap[playerTopRow + 1][nx] == LADDER_ID)
      y = (y / blockSize + 1) * blockSize - h;
  }

  void moveDownLadder() {
    if (!isClimbingDown) return;
    isClimbingDown = false;

    int nx = x / blockSize;
    int ny = (y + h + 5) / blockSize;

    if (gameMap[ny][nx] == LADDER_ID) {
      updateFrame('U');
      y += 5;
    }
  }

  void draw(Param *p) { p->draw(x, y, currentFrame->width, currentFrame->height, currentFrame); }

  bool didReachExitDoor() {  // ChangeLevel: didReachExitDoor
    if (x >= exitX && x <= exitX + 5 && y >= exitY && y <= exitY + 5)
      return true;

    if (x + currentFrame->width >= exitX &&
        x + currentFrame->width <= exitX + 5 && 
        y >= exitY && y <= exitY + 5)
      return true;

    return false;
  }

  void duplicateMap(int gameMap[30][40]) {  // ChargeMatMap: duplicateMap
    for (int i = 0; i < 30; i++)
      for (int j = 0; j < 40; j++) {
        this->gameMap[i][j] = gameMap[i][j];
      }
  }

  bool shouldUpdatePlayerActions() {
    // https://docs.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timegettime
    if (startTime == NULL) startTime = timeGetTime();
    int timeSinceLastFrame = timeGetTime() - startTime;

    if (timeSinceLastFrame > 15) {
      startTime = timeGetTime();
    }
    return timeSinceLastFrame > 15;
  }

  bool shouldUpdateVerticalPosition() {
    // https://docs.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timegettime
    if (verticalStartTime == NULL) verticalStartTime = timeGetTime();
    int timeSinceLastFrame = timeGetTime() - verticalStartTime;

    if (timeSinceLastFrame > 20) {
      verticalStartTime = timeGetTime();
    }
    return timeSinceLastFrame > 20;
  }

  private:
   void updateFrame(char d) { // ChangeImage: updateFrame
     switch (d) {
       case 'L':
         leftWalkFrame = (leftWalkFrame + 1) % 4;
         currentFrame = leftWalk[leftWalkFrame];
         break;

       case 'R':
         rightWalkFrame = (rightWalkFrame + 1) % 4;
         currentFrame = rightWalk[rightWalkFrame];
         break;

       case 'U':
         climbFrame = (climbFrame + 1) % 3;
         currentFrame = ladderClimb[climbFrame];
         break;
     }

     w = currentFrame->width;
     h = currentFrame->height;
   }

   bool isPassThroughBlock(int row, int column) {
     return gameMap[row][column] == AIR_ID || gameMap[row][column] == 5 ||
            gameMap[row][column] == LADDER_ID || gameMap[row][column] == 9;
   }

   char *getAssetName(int i, char mode) {
     char *path = new char[30];
     sprintf(path, "Images/player/player%c%d.bmp", mode, i);

     return path;
   }
};

class Fire {
 public:
  int x, y;
  int frame = 0;
  Sprite *currentSprite;

  static int counter;

 private:
  Sprite *assets[3]; // Image: assets
  int startTime;

 public:
  Fire(int x, int y) {
    this->x = x;
    this->y = y;

    assets[0] = new Sprite("Images/fire1.bmp", 0xffffffff);
    assets[1] = new Sprite("Images/fire2.bmp", 0xffffffff);
    assets[2] = new Sprite("Images/fire3.bmp", 0xffffffff);

    currentSprite = assets[0];
    counter++;
  }
  void updateFrame() {  // ChangeCadr: updateFrame
    if (!shouldUpdate()) return;

    frame = (frame + 1) % 3;
    currentSprite = assets[frame];
  }

 private:
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

  Sprite *currentFrame; // ImageView: currentFrame
  static int counter;

 private:
  int num = 5;
  Sprite *assets[2]; // Images: assets
  int frame = 0; // Cadr: frame

 public:
  Door(int x, int y) {
    this->x = x;
    this->y = y;
    
    assets[0] = new Sprite("Images/Door1.bmp", 0xffffffff);
    assets[1] = new Sprite("Images/Door2.bmp", 0xffffffff);

    currentFrame = assets[0];
    counter++;
  }

  bool contains(int x, int y) {  // Touch: contains
    return x >= this->x && x <= this->x + currentFrame->width && y >= this->y && y <= this->y + currentFrame->height;
  }

 private:
  void updateFrame(int gameMap[30][40]) {  // ChangeCadr: updateFrame
    frame = (frame + 1) % 2;
    currentFrame = assets[frame];
    
    int row = y / blockSize;
    int column = x / blockSize;

    gameMap[row][column] = num;
    gameMap[row + 1][column] = num;
    gameMap[row + 2][column] = num;
    gameMap[row + 3][column] = num;

    num = num == 5 ? 4 : 5;
  }
};
int Door::counter = 0;

class Book {
 public:
  int x, y;
  bool show = true;
  bool isOpen = false; // state: isOpen

  Sprite *bookAsset; // Image: bookAsset
  static int counter;

 private:
    Sprite *assets[2];

 public:
  Book(int x, int y) {
    this->x = x;
    this->y = y;
    assets[0] = new Sprite("Images/closed_book.bmp");
    assets[1] = new Sprite("Images/open_book.bmp", 0xffffffff);
    
    bookAsset = assets[0];
    counter++;
  }

  void closeBook() {
    bookAsset = assets[0];
    isOpen = false;
  }
  void openBook() {
    bookAsset = assets[1];
    isOpen = true;

    // center book
    x = (screenPixelWidth - bookAsset->width) / 2;
    y = (screenPixelHeight - bookAsset->height) / 2;
  }
  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + bookAsset->width && y >= this->y && this->y <= y + bookAsset->height;
  }
};
int Book::counter = 0;

class Bonus {
 public:
  int x, y;
  bool show = true;

  Sprite *currentFrame;  // ImageView: currentFrame
  static int counter;

 private:
  int frame = 0;  // Cadr: frame
  int startTime;
  Sprite *bonusAssets[10];  // Image: bonusAssets

 public:
  Bonus(int x, int y) {
    this->x = x;
    this->y = y;

    for (int i = 0; i < 10; i++) {
      bonusAssets[i] = new Sprite(getAssetName(i + 1), 0xffffffff);
    }

    currentFrame = bonusAssets[0];
    counter++;
  }

  void udpateFrame() {  // ChangeCadr: udpateFrame
    if (!shouldUpdate()) return;

    frame = (frame + 1) % 9;
    currentFrame = bonusAssets[frame];
  }

  bool contains(int x, int y) {  // Touch: contains
    return x >= this->x && x <= this->x + currentFrame->width && y >= this->y &&
           y <= this->y + currentFrame->height;
  }

 private:
  bool shouldUpdate() {
    if (startTime == NULL) startTime = timeGetTime();
    int timeSinceLastFrame = timeGetTime() - startTime;

    if (timeSinceLastFrame > 50) {
      startTime = timeGetTime();
    }
    return timeSinceLastFrame > 50;
  }

  char *getAssetName(int i) {
    char *path = new char[30];
    sprintf(path, "Images/bonus%d.bmp", i);

    return path;
  }
};
int Bonus::counter = 0;

class Arrow { // LockerPin
 public:
  int x, y;

 private:
  int frame = 0;
  Sprite *arrowAssets[40];

 public:
  Arrow() {
    x = 0;
    y = 0;

    char *path = new char[30];
    for (int i = 0; i < 40; i++) {
      sprintf(path, "Images/c%d.bmp", i + 1);

      arrowAssets[i] = new Sprite(path, 0xffffffff);
    }
  }

  void draw(Param *p) {
    Sprite *currentFrame = arrowAssets[frame];

    p->draw(x, y, currentFrame->width, currentFrame->height, currentFrame);
  }
};

class Inventory { // Inventar: Inventory
 public:
  int cells[9]; // objects: cells

  int x, y;
  int move;
  int check;

  bool show;
  bool showTooltip; // exp: showTooltip

 private:
  Sprite *toolTip; // InventarExp: toolTip
  Sprite *inventorySprite; // Image: inventorySprite
  Sprite *openInventory; // Open: openInventory
  
  Sprite *cellSprites[9]; // ImObjects: cellSprites

 public:
  Inventory() : showTooltip(false), move(false) {
    x = 0;
    y = 0;

    inventorySprite = new Sprite("Images/inventory/inventory.bmp", 0xffffffff);
    openInventory = new Sprite("Images/inventory/inventory_open.bmp", 0xffffffff);
    toolTip = new Sprite("Images/inventory/tool_tip.bmp", 0xffffffff);

    for (int i = 0; i < 9; i++) cells[i] = INV_EMPTY_CELL;

    updateCellSprites();
  }

  void draw(Param *p, int cursorX, int cursorY) {
    if (show) {
      p->draw(x, y, openInventory->width, openInventory->height, openInventory);

      for (int i = 0; i < 9; i++) {
        if (cells[i] != INV_EMPTY_CELL) {
          std::tie(x, y) = cellPosition(i);

          p->draw(x, y, cellSprites[i]->width, cellSprites[i]->height,
                  cellSprites[i]);
        }
      }
    } else {
      p->draw(x, y, inventorySprite->width, inventorySprite->height,
              inventorySprite);

      if (cells[0] != INV_EMPTY_CELL) {
        std::tie(x, y) = cellPosition(0);

        p->draw(x, y, cellSprites[0]->width, cellSprites[0]->height,
                cellSprites[0]);
      }
    }

    if (showTooltip)
      p->draw(cursorX + 10, cursorY, toolTip->width, toolTip->height, toolTip);
  }
  void updateCellSprites() { // ChangeImages: updateCellSprites
    char *path = new char[30];
    int num;

    for (int i = 0; i < 9; i++) {
      if (cells[i] != INV_EMPTY_CELL) {
        num = sprintf(path, "Images/o%d.bmp", cells[i]);

        cellSprites[i] = new Sprite(path, 0xffffffff);
      }
    }
  }
  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + inventorySprite->width && y >= this->y && y <= this->y + inventorySprite->height;
  }
  int touchedCellIndex(int X, int Y) { // TouchObject: touchedCellIndex
    // "Images/inventory/inventory_open.bmp"
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

  bool containsInOpenInventory(int x, int y) { // TouchInvShow: containsInOpenInventory
    return x >= this->x && x <= this->x + openInventory->width && y >= this->y && y <= this->y + openInventory->height;
  }

  void addItem(int num) { // AddObject: addItem
    if (num == INV_EMPTY_CELL) return;

    for (int i = 0; i < 9; i++) {
      if (cells[i] != INV_EMPTY_CELL) continue;
      
      cells[i] = num;
      break;
    }

    updateCellSprites();
  }

  private:
  std::tuple<int, int> cellPosition(int cellIndex) {
      int offset = cellIndex == 0 ? -7 : 0;

      int cellSize = 25;
      int cellSpace = 7;

      return std::tuple<int, int>{17 + cellIndex * (cellSpace + cellSize) + offset, 7};
  }
};

class Chest {
 public:
  int x, y;
  int mX1, mY1; // mX1, mY1
  int check = 0; // check:
  int items[6 * 8]; // objects: items

  bool showToolTip = false; // expO: showToolTip
  bool show = false; // show:
  bool showChestContents = false; // showC: showChestContents
  bool move = false; // move:

  static bool iLmb; // iLmb:
  static int counter; // counter:
  static int Xi; // Xi:
  static int Yi; // Yi:
  static int dt; // dt:
  static int timer; // timer:
  static int timer1; // timer1:

 private:
  Sprite *currentFrame; // ImageView: currentFrame
  Sprite *itemAssets[6 * 8]; // ImObjects: itemAssets

  Sprite *assets[2]; // Image: assets; TODO: remove
  Sprite *code; // code:
  Sprite *toolTip; // OpenExp: toolTip
  Sprite *chestGrid; // Content: chestGrid
  Sprite *successUnlockAsset; // OkOpen: successUnlockAsset
  
  Button *lock; // lock:
  Button *locker; // locker:
  Button *LockerLight; // LockerLight:
  Button *lOK; // lOK:

  Arrow *ar; // ar:

  Text *txt; // txt:
  Text *codeView; // codeView:
  char *question;
  int answer; // Answer: answer

  bool BOk = false; // BOk:

  int frame = 0; // Cadr: frame
  int combination = 0; // combinaison: combination
  int cd[3]; // cd:
  int sd; // sd:

 public:
  Chest(int x, int y, int ans1, int ans2) {
    this->x = x;
    this->y = y;
    assets[0] = new Sprite("Images/chest/chest1.bmp");
    assets[1] = new Sprite("Images/chest/chest2.bmp");

    code = new Sprite("Images/chest/code.bmp");
    toolTip = new Sprite("Images/chest/OpenChest.bmp", 0xffffffff);
    chestGrid = new Sprite("Images/chest/InvChest.bmp", 0xffffffff);
    successUnlockAsset = new Sprite("Images/chest/OpenTrue.bmp", 0xffffffff);
    currentFrame = assets[0];

    counter++;
    answer = ans1 * 1000 + ans2;
    //----
    for (int i = 0; i < 6 * 8; i++) items[i] = 0;

    items[0] = 2;

    ChangeImages();
    //+++
    sd = 0;
    for (int i = 0; i < 3; i++) cd[i] = 0;

    initializeButtons();
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
    return x >= this->x && x <= this->x + currentFrame->width && y >= this->y && y <= this->y + currentFrame->height;
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
      if (items[i] != 0) {
        num = sprintf(path, "Images/o%d.bmp", items[i]);

        itemAssets[i] = new Sprite(path, 0xffffffff);
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

        answer = atoi(temp);
        // ANSWER---
      }

      j++;
    }

    txt = new Text(question, 0xFF3D3D3D, (w - code->width) / 2 + 15,
                   (h - code->height) / 2 + 68);
  }
  //+++
  void openLock(Inventory *inventory, bool &lmb, int X, int Y, int mX, int mY) { // OpenLock: openLock
    if (!show) return;

    if (lmb && !lock->contains(X, Y) && !inventory->containsInOpenInventory(X, Y))
      show = false;

    int dist = distanceFromLocker(mX, mY);

    if (lmb && lOK->contains(X, Y)) {
      lOK->show = true;
      sd = (sd + 1) % 3;

      lmb = false;
    } else {
      lOK->show = false;
    }

    if (LockerLight->contains(mX, mY) && dist > 55 && dist < 88) {
      LockerLight->show = true;

      if (lmb && locker->contains(X, Y)) {
        int angle = calculateAngle(X, Y);
        cd[sd] = angle;
        combination = cd[0] * 10000 + cd[1] * 100 + cd[2];

        // Show angle+++
        ar->num = angle;

        displayCombination(cd[0], cd[1], cd[2]);
        // Show angle---

        lmb = false;
      }
    } else {
      LockerLight->show = false;
    }

    // Open++++
    if (combination == answer && sd == 0) BOk = true;

    if (BOk == true) {
      Timer();

      if (dt > 2000) {
        showChestContents = true;
        BOk = false;
        combination = 0;

        dt = 0;
        timer = 0;
        timer1 = 0;
      }
    }
    // Open----
  }
  //++++
  void draw(Param *p) {
    p->draw(x, y, currentFrame->width, currentFrame->height, currentFrame);

    if (showToolTip)
      p->draw(mX1, mY1, toolTip->width, toolTip->height, toolTip);
  }
  void drawC(Param *p) {
    lock->draw(p);

    if (!showChestContents) locker->draw(p);

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

    if (showChestContents) {
      p->draw((w - code->width) / 2, (h - code->height) / 2, chestGrid->width,
              chestGrid->height, chestGrid);

      int ix = 0;
      int iy = 0;

      for (int i = 0; i < 6 * 8; i++) {
        if (items[i] != 0)
          p->draw((w - code->width) / 2 + 71 + (ix)*5 + (ix)*25,
                  (h - code->height) / 2 + 109 + (iy)*5 + (iy)*25,
                  itemAssets[i]->width, itemAssets[i]->height, itemAssets[i]);

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
              successUnlockAsset->width, successUnlockAsset->height, successUnlockAsset);
  }
  private:
   void initializeButtons() {
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

   int calculateAngle(int clickedX, int clickedY) {
     double xn = locker->w / 2;
     double yn = locker->h / 2;

     double x1 = 0;
     double y1 = locker->h / 2;

     double xm = (clickedX - locker->x) - xn;
     double ym = yn - (clickedY - locker->y);

     double a2 = sqrt(xm * xm + ym * ym);

     double angle1 = acos(ym / a2);
     double angle2 = (angle1 * 180) / 3.14;

     if (clickedX - locker->x < xn) angle2 = 360 - angle2;

     return angle2 / 9;
   }

   int distanceFromLocker(int cursorX, int cursorY) {
     int dx = cursorX - locker->x - locker->w / 2;
     int dy = locker->h / 2 - (cursorY - locker->y);
     int distance = sqrt(dx * dx + dy * dy);

     return distance;
   }

   void displayCombination(int num1, int num2, int num3) {
     char *showText = new char[100];

     if (num1 < 10)
       sprintf(showText, "0%d 0%d 0%d", num1, num2, num3);
     else
       sprintf(showText, "%d %d %d", num1, num2, num3);

     codeView->changeText(showText);
   }
};
int Chest::dt = 0;
int Chest::timer = 0;
int Chest::timer1 = 0;

int Chest::counter = 0;
bool Chest::iLmb = false;
int Chest::Xi = 0;
int Chest::Yi = 0;

class PressurePlate { // ButtonON: PressurePlate
 public:
  int x, y;
  static int counter;

 private:
  Sprite *asset; // Image: asset

 public:
  PressurePlate(int x, int y) {
    this->x = x;
    this->y = y;
    asset = new Sprite("Images/pressure_plate.bmp");

    counter++;
  }

  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + asset->width && y >= this->y && y <= this->y + asset->height;
  }
};
int PressurePlate::counter = 0;

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
  BlockMoves(int x, int y, int pi) {
    this->x = x;
    this->y = y - 1;
    this->pX = x;
    this->pY = y - 1;
    this->pI = pi - 1;
    Image = new Sprite("Images/blockM.bmp");

    counter++;
  }

  bool BlockMoveUp(int gameMap[30][40]) {
    if (y > pI) {
      gameMap[pY / 20 + 1][pX / 20] = 0;

      if (y - pI <= 5)
        y -= (y - pI);
      else
        y -= 5;
    }

    if (y == pI) {
      gameMap[y / 20 + 1][x / 20] = 6;
      return false;
    }

    return true;
  }
  void BlockMoveDown(int gameMap[30][40]) {
    if (y < pY) {
      gameMap[pI / 20 + 1][x / 20] = 0;

      if (pY - y <= 5)
        y += (pY - y);
      else
        y += 5;
    }

    if (y == pY) {
      gameMap[y / 20 + 1][x / 20] = 6;
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
  bool needsKey = false;  // nk: Need key

  static int counter;

 private:
  int frame = 0; // Cadr: frame
  int doorId = 9; // num: doorId

  int doorType; // type: doorType
  Sprite *assets[2]; // Image: assets
  Sprite *currentFrame; // ImageView: currentFrame
  Sprite *needKeyAsset; // NeedKey: needKeyAsset

 public:
  FinalDoor(int x, int y, int doorType) {
    this->x = x;
    this->y = y;
    this->doorType = doorType;

    assets[0] = new Sprite("Images/NextDoor1.bmp", 0xffffffff);
    assets[1] = new Sprite("Images/NextDoor2.bmp", 0xffffffff);

    currentFrame = assets[0];
    needKeyAsset = new Sprite("Images/NeedKey.bmp", 0xffffffff);

    counter++;

    if (doorType == 1) {
      assets[0]->flipHorizontally();
      assets[1]->flipHorizontally();
    }
  }

  bool contains(int x, int y) {  // Touch: contains
    return x >= this->x && x <= this->x + currentFrame->width && y >= this->y &&
           y <= this->y + currentFrame->height;
  }

  void draw(Param *p) {
    p->draw((x + assets[0]->width) - currentFrame->width, y, currentFrame->width,
            currentFrame->height, currentFrame);

    if (needsKey)
      p->draw((screenPixelWidth - needKeyAsset->width) / 2, (screenPixelHeight - needKeyAsset->height) / 2,
              needKeyAsset->width, needKeyAsset->height, needKeyAsset);
  }

  void updateFrame(int gameMap[30][40]) {  // ChangeCadr: updateFrame
    if (doorType != 0) return;
    frame = (frame + 1) % 2;

    currentFrame = assets[frame];

    int row = y / blockSize;
    int column = x / blockSize;
    gameMap[row][column] = doorId;
    gameMap[row + 1][column] = doorId;
    gameMap[row + 2][column] = doorId;
    gameMap[row + 3][column] = doorId;

    doorId = doorId == 9 ? 8 : 9;
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

