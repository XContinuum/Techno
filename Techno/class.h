#define WHITE 0xffffffff
#define BLACK 0xff000000
#define VERY_DARK_GRAY 0xff3d3d3d
#define VIVID_RED 0xffed1c24
#define DARK_GRAYISH_BLUE 0xff808e9b
#define VERY_LIGHT_GRAY 0xffe5e5e5
#define DARK_GRAY 0xff7f7f7f

#define CHARACTERS_ASSET "Images/Text.bmp"

#define CLOSED_BOOK_IMG "Images/closed_book.bmp"
#define OPEN_BOOK_IMG "Images/open_book.bmp"

// Inventory
#define INVENTORY_IMG "Images/inventory/inventory.bmp"
#define OPEN_INVENTORY_IMG "Images/inventory/inventory_open.bmp"
#define INVENTORY_TOOLTIP_IMG "Images/inventory/tool_tip.bmp"

// Chest
#define CHEST_CODE_IMG "Images/chest/code.bmp"
#define CHEST_TOOLTIP_IMG "Images/chest/tool_tip.bmp"
#define CHEST_GRID_IMG "Images/chest/chest_grid.bmp"
#define SUCCESS_UNLOCK_IMG "Images/chest/success_unlock_asset.bmp"
#define CLOSED_CHEST_IMG "Images/chest/chest1.bmp"

#define LOCKER_IMG "Images/chest/locker.bmp"
#define LIGHT_LOCKER_IMG "Images/chest/lockerL.bmp"
#define CIRCLE_IMG "Images/chest/lOK.bmp"

#define MOVING_BLOCK_IMG "Images/blockM.bmp"
#define PRESSURE_PLATE_IMG "Images/pressure_plate.bmp"

#define NEED_KEY_IMG "Images/NeedKey.bmp"

Sprite **loadAssets(char *ASSET_PATH_FORMAT, int total, int transparency) {
  Sprite **assets;

  for (int i = 0; i < total; i++) {
    char *path = new char[30];
    sprintf(path, ASSET_PATH_FORMAT, i + 1);

    assets[i] = new Sprite(path, transparency);
  }

  return assets;
}

auto item_assets = loadAssets("Images/o%d.bmp", 2, WHITE);

class Button { // Button: TappableArea
 public:
  int x, y;
  int w, h;
  Sprite *image; // Image: image
  bool show;

  bool SubShow; // SubShow
  bool showBorder;
  Sprite *Border; // Border

 public:
  Button(char *name, int transparency) {
    image = new Sprite(name, transparency);
    w = image->width;
    h = image->height;
  }
  Button(char *name): Button(name, 0) {}
  Button(int x, int y, char *name): Button(name, 0) {
    this->x = x;
    this->y = y;
  }
  Button(int x, int y, char *name, int transparency): Button(name, transparency) {
    this->x = x;
    this->y = y;
  }

  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + w && y >= this->y && y <= this->y + h;
  }

  void draw(Param *p) {
    if (!show) return;
    
    p->draw(x, y, w, h, image);
  }
};

class NeoElement : public Sprite, public Button {
 public:
  NeoElement(char *name) : Sprite(name), Button(name) {}
  NeoElement(char *name, int trColor)
      : Sprite(name, trColor), Button(name, trColor) {}

  void changeColor(int from, int to) { // ChangeColor: changeColor
    for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++)
        if (Button::image->img[j + i * width] == from)
          Button::image->img[j + i * width] = to;
  }
  int followColor(int x, int y) { // FollowColor: followColor
    return Button::image->img[x + y * width];
  }
};
class NeoSprite : public Sprite {
 public:
  NeoSprite(char *name) : Sprite(name) {}
  NeoSprite(char *name, int transparentColor) : Sprite(name, transparentColor) {}

  void addImage(int x, int y, Sprite *sprite) { // AddImage: addImage
    for (int i = 0; i < sprite->height; i++) {
      for (int j = 0; j < sprite->width; j++) {
        int index = j + i * sprite->width;
        int pixel = sprite->img[index];
        
        if (pixel == TransparentColor) continue;
        
        if (j + x <= width && i + y <= height) {
          int currentPos = (j + x) + (i + y) * width;
          img[currentPos] = sprite->img[index];
        }
      }
    }
  }
  void addImage(int x, int y, Sprite *sprite, int i) { // AddImage: addImage
    for (int i = 0; i < sprite->height; i++) {
      for (int j = 0; j < sprite->width; j++) {
        if (j + x > width) continue;
        if (i + y > height) continue; // TODO: should return?

        int pos1 = j + i * sprite->width;
        int pos2 = (j + x) + (i + y) * width;

        img[pos2] = sprite->img[pos1];
      }
    }
  }

  void save(char *name) { // Save: save
    std::ofstream os(name, std::ios::binary);

    unsigned char signature[2] = {'B', 'M'};
    unsigned int fileSize = 14 + 40 + width * height * 4;
    unsigned int reserved = 0;
    unsigned int offset = 14 + 40;

    unsigned int headerSize = 40;
    unsigned int dimensions[2] = {width, height};
    unsigned short colorPlanes = 1;
    unsigned short bpp = 32;
    unsigned int compression = 0;
    unsigned int imgSize = width * height * 4;
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

    unsigned char alpha, r, g, b;

    for (int i = height; i > 0; i--) {
      for (int j = 0; j < width; j++) {
        alpha = 0; // alpha channel?
        r = getColor(j, i, 'R');
        g = getColor(j, i, 'G');
        b = getColor(j, i, 'B');
        os.write(reinterpret_cast<char *>(&b), sizeof(b));
        os.write(reinterpret_cast<char *>(&g), sizeof(g));
        os.write(reinterpret_cast<char *>(&r), sizeof(r));
        os.write(reinterpret_cast<char *>(&alpha), sizeof(alpha));
      }
    }

    os.close();
  }

  private:
   int getColor(int x, int y, char channel) {  // Color: getColor
     int color = img[x + y * width];
     
     if (channel == 'R') return (BYTE)color;
     if (channel == 'G') return (WORD)color >> 8;
     if (channel == 'B') return (DWORD)color >> 16;

     return NULL;
   }
};
struct Point { // Tochka: Point
  int X, Y;
};
// Classes---
class Text {
 public:
  int x, y;

 private:
  char *str; // string: str
  Sprite *letters[77];
  int length; // size: length

 public:
  Text(int x, int y, char *str, int color) { // remove color
    this->x = x;
    this->y = y;

    cutLetters();
    changeColor(color);
    changeText(str);
  }

  void draw(Param *p) {
    for (int i = 0; i < length; i++) {
      int index = convert(str[i]);
      int offset = charOffset(str[i]);

      p->draw(x + i * (letters[i]->width - offset), y, letters[index]->width,
              letters[index]->height, letters[index]);
    }
  }

  void draw(Param *p, int *pos, int size, int highlightColor) {
    for (int i = 0; i < length; i++) {
      int index = convert(str[i]);
      int offset = charOffset(str[i]);

      Sprite *letter = letters[index];
      if (isValueInArray(i, pos, size)) {
        letter->replaceColor(color, highlightColor);
      }

      p->draw(x + i * (letter->width - offset), y, letter->width,
              letter->height, letter);
    }
  }
  void changeText(char *str) {
    this->str = str; // TODO: investigate if I should free memory for previous string
    length = calculateLength(str);
  }

 private:
  bool isValueInArray(int value, int *arr, int size) {
    for (int i = 0; i < size; i++) {
      if (arr[i] == value) return true;
    }
    return false;
  }

  int charOffset(char c) {
      char category = letterCategory(c);

      if (category == 'U') return 1;
      if (category == 'L') return 5;
      if (category == 'N') return 5;

      return 0;
  }

  int letterCategory(char c) {
    int ascii = static_cast<int>(c);

    if (getCode(c) != NULL) return 'O'; // other?
    if (ascii - 97 >= 0) return 'L';  // low
    if (ascii - 65 >= 0) return 'U';  // up
    if (ascii - 48 >= 0) return 'N';  // num

    return 'O';
  }
  int convert(char c) { // convertion: convert: getIndex
    int code = getCode(c);
    int ascii = static_cast<int>(c);

    if (code != NULL) return code; // other
    if (ascii - 97 >= 0) return ascii - 97;  // low
    if (ascii - 65 >= 0) return ascii - 65 + 26;  // up
    if (ascii - 48 >= 0) return ascii - 48 + 52;  // num

    return 0;
  }
  int getCode(char c) {
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
    
    return NULL;
  }
  int calculateLength(char* str) {
    int length = 0;
    while (str[length] != '\0') length++;
    
    return str;
  }

  void cutLetters() { // ChangeColor: changeColor: cutLetters
    const int w = 11, h = 11;

    for (int i = 0; i < 77; i++) {
      int column = i % 26;
      int row = (i - column) / 26;

      letters[i] = new Sprite(CHARACTERS_ASSET, WHITE); // TODO: make this more efficient
      letters[i]->cut(w * column, h * row, w, h);
    }
  }

  void changeColor(int color) {
    for (int i = 0; i < 77; i++) {
      letters[i]->replaceColor(BLACK, color);
    }
  }
};

class Player { // Hero: Player
 public:
  int x = 0, y = 0;
  int width, height; // w, h

  bool isMovingLeft = false, isMovingRight = false, isJumping = false; // L, R, J
  bool isClimbingUp = false, isClimbingDown = false; // U, D
  bool canFall = true; // G: canFall

  int jumpVelocity = 0; // velocityJ: jumpVelocity

 private:
  int rightWalkFrame = 0, leftWalkFrame = 0, climbFrame = 0; // CadrR, CadrL, CadrU
  
  Sprite *currentFrame; // Image: currentFrame

  Sprite *rightWalk[5]; // ImageR: rightWalk
  Sprite *leftWalk[5]; // ImageL: leftWalk

  Sprite *ladderClimb[4]; // ImageUD: ladderClimb
  Sprite *rightJump[4]; // ImageJR: rightJump
  Sprite *leftJump[4]; // ImageJL: leftJump

  int exitX, exitY;

  // Gravity
  int velocity = 0;
  const int acceleration = 3; // accel: acceleration
  const int step = 7;

  int startTime;
  int verticalStartTime;

 public:
  Player(int x, int y) {
    this->x = x;
    this->y = y;

    rightWalk = loadAssets("Images/player/playerR%d.bmp", 5, WHITE);
    leftWalk = loadAssets("Images/player/playerL%d.bmp", 5, WHITE);

    rightJump = loadAssets("Images/player/playerJ%d.bmp", 4, WHITE);
    leftJump = loadAssets("Images/player/playerJ%d.bmp", 4, WHITE);
    ladderClimb = loadAssets("Images/player/playerU%d.bmp", 4, WHITE);

    for (int i = 0; i < 4; i++) {
      leftJump[i]->flipHorizontally();
    }
    currentFrame = rightWalk[0];

    width = currentFrame->width;
    height = currentFrame->height;
  }

  void moveRight(const Map gameMap) { // MoveR: moveRight
    if (!isMovingRight) return;
    isMovingRight = false;

    if (canPlayerMoveRight(gameMap)) {
      updateFrame('R');
      x += step;
    }
  }

  void moveLeft(const Map gameMap) { // MoveL: moveLeft
    if (!isMovingLeft) return;
    isMovingLeft = false;

    if (canPlayerMoveLeft(gameMap)) {
      updateFrame('L');
      x -= step;
    }
  }

  void gravity(const Map gameMap) { // Gravitaton: gravity
    if (isJumping) return;
    if (!canFall) return;

    if (velocity <= 20) velocity += acceleration;

    if (canPlayerFallthrough(gameMap)) {
      y += velocity;
    } else {
      int blockBelow = (y + height + velocity) / blockSize;
      y = blockBelow * blockSize - height;
      velocity = 0;
    }
  }

  void jump(const Map gameMap) { // Jump: jump
    if (!isJumping) return;
    
    jumpVelocity -= acceleration;
 
    if (jumpVelocity >= 0 && canPlayerJumpUp(gameMap)) {
      y -= jumpVelocity;
    } else {
      jumpVelocity = 0;
      isJumping = false;
    }
  }

  void moveUpLadder(const Map gameMap) {  // UD: moveUpLadder()
    if (!isClimbingUp) return;
    isClimbingUp = false;

    int leftPlayerSide = x / blockSize;
    int playerTopRow = y / blockSize;

    if (gameMap[playerTopRow][leftPlayerSide] == LADDER_ID) {
      updateFrame('U');
      y -= 5;
    }

    if (didPlayerFinishClimbingUp(gameMap))
      y = (y / blockSize + 1) * blockSize - height;
  }

  void moveDownLadder(const Map gameMap) {
    if (!isClimbingDown) return;
    isClimbingDown = false;

    int leftPlayerSide = x / blockSize;
    int blockBelow = (y + height + 5) / blockSize;

    if (gameMap[blockBelow][leftPlayerSide] == LADDER_ID) {
      updateFrame('U');
      y += 5;
    }
  }

  void draw(Param *p) { p->draw(x, y, currentFrame->width, currentFrame->height, currentFrame); }

  bool didReachExitDoor() {  // ChangeLevel: didReachExitDoor
    return isWithinExit(x, y) || isWithinExit(x + currentFrame->width, y);
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

  void setExit(int x, int y) {
    exitX = x;
    exitY = y;
  }

  private:
   void updateFrame(char direction) { // ChangeImage: updateFrame
     switch (direction) {
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

     width = currentFrame->width;
     height = currentFrame->height;
   }

   bool isWithinExit(int x, int y) {
     return x >= exitX && x <= exitX + 5 && y >= exitY && y <= exitY + 5;
   }
   bool didPlayerFinishClimbingUp(const Map gameMap) {
     int leftPlayerSide = x / blockSize;
     int playerTopRow = y / blockSize;
     int playerBottomRow = (y + height) / blockSize;

     return gameMap[playerTopRow][leftPlayerSide] == AIR_ID &&
            gameMap[playerBottomRow][leftPlayerSide] == LADDER_ID &&
            gameMap[playerTopRow + 1][leftPlayerSide] == LADDER_ID;
   }
   bool canPlayerJumpUp(const Map gameMap) {
     int leftPlayerSide = x / blockSize;
     int blockAbove = (y - jumpVelocity) / blockSize;

     return gameMap[blockAbove][leftPlayerSide] == AIR_ID;
   }
   bool canPlayerFallthrough(const Map gameMap) {
     int leftPlayerSide = x / blockSize;
     int rightPlayerSide = (x + width) / blockSize;
     int blockBelow = (y + height + velocity) / blockSize;

     return gameMap[blockBelow][leftPlayerSide] == AIR_ID &&
            gameMap[blockBelow][rightPlayerSide] == AIR_ID;
   }
   bool canPlayerMoveLeft(const Map gameMap) {
     int previousColumn = (x - step) / blockSize;
     int playerRow = y / blockSize;

     return canPassThrough(playerRow, previousColumn, gameMap);
   }
   bool canPlayerMoveRight(const Map gameMap) {
     int followingColumn = (x + width + step) / blockSize;
     int playerRow = y / blockSize;

     return canPassThrough(playerRow, followingColumn, gameMap);
   }
   bool canPassThrough(int row, int column, const Map gameMap) {
     return isPassThroughBlock(row, column, gameMap) &&
            isPassThroughBlock(row + 1, column, gameMap) &&
            isPassThroughBlock(row + 2, column, gameMap);
   }

   bool isPassThroughBlock(int row, int column, const Map gameMap) {
     return gameMap[row][column] == AIR_ID || gameMap[row][column] == OPEN_DOOR_ID ||
            gameMap[row][column] == LADDER_ID || gameMap[row][column] == OPEN_FINAL_DOOR_ID;
   }
};

class Fire {
 public:
  static int counter;

 private:
  int x, y;
  int frame = 0;
  Sprite *currentFrame; // currentSprite: currentFrame
  Sprite *assets[3]; // Image: assets
  int startTime;

 public:
  Fire(int x, int y) {
    this->x = x;
    this->y = y;

    assets = loadAssets("Images/fire%d.bmp", 3, WHITE);

    currentFrame = assets[0];
    counter++;
  }
  void updateFrame() {  // ChangeCadr: updateFrame
    if (!shouldUpdate()) return;

    frame = (frame + 1) % 3;
    currentFrame = assets[frame];
  }

  void draw(Param *p) {
    p->draw(x, y, currentFrame->width, currentFrame->height, currentFrame);
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

  static int counter;

 private:
  bool isOpen = false;

  Sprite *currentFrame; // ImageView: currentFrame
  Sprite *assets[2]; // Images: assets
  int frame = 0; // Cadr: frame

 public:
  Door(int x, int y) {
    this->x = x;
    this->y = y;
    
    assets = loadAssets("Images/Door%d.bmp", 2, WHITE);

    currentFrame = assets[0];
    counter++;
  }

  bool contains(int x, int y) {  // Touch: contains
    return x >= this->x && x <= this->x + currentFrame->width && y >= this->y && y <= this->y + currentFrame->height;
  }

  void draw(Param *p) {
    p->draw(x, y, currentFrame->width, currentFrame->height, currentFrame);
  }

  void updateFrame() {  // ChangeCadr: updateFrame
    frame = (frame + 1) % 2;
    currentFrame = assets[frame];
  }

  void switchDoorState(Map gameMap) {
    int row = y / blockSize;
    int column = x / blockSize;
    isOpen = !isOpen;
    int doorId = isOpen ? OPEN_DOOR_ID : CLOSED_DOOR_ID;

    gameMap[row][column] = doorId;
    gameMap[row + 1][column] = doorId;
    gameMap[row + 2][column] = doorId;
    gameMap[row + 3][column] = doorId;
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
    assets[0] = new Sprite(CLOSED_BOOK_IMG);
    assets[1] = new Sprite(OPEN_BOOK_IMG, WHITE);
    
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

  void draw(Param *p) {
    p->draw(x, y, bookAsset->width, bookAsset->height, bookAsset);
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

    bonusAssets = loadAssets("Images/bonus/bonus%d.bmp", 10, WHITE);

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

  void draw(Param *p) {
    p->draw(x, y, currentFrame->width, currentFrame->height, currentFrame);
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
int Bonus::counter = 0;

class LockAngle { // Arrow: LockAngle
 public:
  int x, y;
  int frame = 0;

 private:
  Sprite *arrowAssets[40];

 public:
  LockAngle(int x, int y) {
    this->x = x;
    this->y = y;

    arrowAssets = loadAssets("Images/lock_angle/c%d.bmp", 40, WHITE);
  }

  void draw(Param *p) {
    Sprite *currentFrame = arrowAssets[frame];

    p->draw(x, y, currentFrame->width, currentFrame->height, currentFrame);
  }
};

class Inventory { // Inventar: Inventory
 public:
  int cells[9]; // objects: cells

  int x = 0, y = 0;
  int move;
  int selectedCell; // check: selectedCell

  bool show = false;
  bool showTooltip = false; // exp: showTooltip

 private:
  Sprite *toolTip; // InventarExp: toolTip
  Sprite *inventorySprite; // Image: inventorySprite
  Sprite *openInventory; // Open: openInventory
  
  Sprite *cellSprites[9]; // ImObjects: cellSprites

 public:
  Inventory() {
    inventorySprite = new Sprite(INVENTORY_IMG, WHITE);
    openInventory = new Sprite(OPEN_INVENTORY_IMG, WHITE);
    toolTip = new Sprite(INVENTORY_TOOLTIP_IMG, WHITE);

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
    for (int i = 0; i < 9; i++) {
      if (cells[i] == INV_EMPTY_CELL) continue;

      cellSprites[i] = item_assets[cells[i]];
    }
  }
  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + inventorySprite->width && y >= this->y && y <= this->y + inventorySprite->height;
  }
  int touchedCellIndex(int x, int y) { // TouchObject: touchedCellIndex
    // "Images/inventory/inventory_open.bmp"
    int x1 = 17;
    int x2 = 297;
    int y1 = 7;
    int y2 = 31;
    int cellSize = 25;
    int cellSpace = 7;

    int block = (x - x1) / (cellSize + cellSpace);
    bool isWithinInventory = x >= x1 && x <= x2 && y >= y1 && y <= y2;
    bool isWithinCell = x - x1 - block * (cellSize + cellSpace) <= cellSize;

    if (isWithinInventory && isWithinCell) return block;

    return NULL;
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

  void clearInventory() {
    for (int i = 0; i < 9; i++)
      if (cells[i] == INV_KEY) cells[i] = INV_EMPTY_CELL;
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
  int check = 0; // check:
  int items[chestRows * chestColumns]; // objects: items

  bool isOpen = false; // show: isOpen
  bool isChestLocked = true; // showC: showChestContents: isChestUnlocked: isChestLocked
  bool move = false; // move:

  static int counter; // counter:
  static int dt; // dt:
  static int timer; // timer:
  static int timer1; // timer1:

 private:
  Sprite *currentFrame; // ImageView: currentFrame
  Sprite *itemAssets[6 * 8]; // ImObjects: itemAssets

  Sprite *code; // code:
  Sprite *toolTip; // OpenExp: toolTip
  Sprite *chestGrid; // Content: chestGrid
  Sprite *successUnlockAsset; // OkOpen: successUnlockAsset
  
  Button *lock; // lock:
  Button *locker; // locker:
  Button *lockerLight; // LockerLight: lockerLight
  Button *circle; // lOK: circle

  LockAngle *lockAngle; // ar: lockAngle

  Text *questionText; // txt: questionText
  Text *combinationText; // codeView: combinationText
  char *question;
  int answer; // Answer: answer

  bool correctCombination = false; // BOk: correctCombination

  int frame = 0; // Cadr: frame
  int comboDigits[3] = {0, 0, 0}; // cd: comboDigits
  int currentDigit = 0; // sd: currentDigit

  int codeX, codeY;

  // Constants
  const int chestRows = 6;
  const int chestColumns = 8;

  const int inventoryChestTop = 109;
  const int inventoryChestLeft = 71;
  const int inventorySquareDim = 25;  // size of a single cell
  const int spaceBetweenCells = 5;

 public:
  Chest(int x, int y) {
    this->x = x;
    this->y = y;

    code = new Sprite(CHEST_CODE_IMG);
    toolTip = new Sprite(CHEST_TOOLTIP_IMG, WHITE);
    chestGrid = new Sprite(CHEST_GRID_IMG, WHITE);
    successUnlockAsset = new Sprite(SUCCESS_UNLOCK_IMG, WHITE);
    currentFrame = new Sprite(CLOSED_CHEST_IMG);

    for (int i = 0; i < chestRows * chestColumns; i++) items[i] = INV_EMPTY_CELL;
    items[0] = INV_KEY;
    updateChestCells();

    initializeButtons();
    setupCodeCoordinates();

    counter++;
  }

  int selectedChestCell(int x, int y) { // TouchObject: selectedChestCell
    if (!this->isWithinInventory(x, y)) return NULL; 
    if (!didClickCell(x, y))) return NULL;

    std::tie(blockX, blockY) = findCell(x, y);
    return blockY * chestColumns + blockX;
  }

  bool isWithinInventory(int x, int y) { // TouchInvChest: isWithinInventory
    // This data is pulled from "Images/chest/chest_grid.bmp"
    int inventoryChestRight = 305;
    int inventoryChestBottom = 283;

    int chestLeft = codeX + inventoryChestLeft;
    int chestRight = codeX + inventoryChestRight;
    int chestTop = codeY + inventoryChestTop;
    int chestBottom = codeY + inventoryChestBottom;

    return x >= chestLeft && x <= chestRight && y >= chestTop && y <= chestBottom;
  }
  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + currentFrame->width && y >= this->y && y <= this->y + currentFrame->height;
  }

  static void Timer() {
    if (timer1 == 0) timer1 = timeGetTime();

    timer = timeGetTime();
    dt = timer - timer1;
  }

  void updateChestCells() { // ChangeImages: updateChestCells
    for (int i = 0; i < chestRows * chestColumns; i++) {
      if (items[i] == INV_EMPTY_CELL) continue;

      itemAssets[i] = item_assets[items[i]];
    }
  }

  // ---------------------------------------------------------------
  // loadQuestion: TODO: move to its own class
  // ---------------------------------------------------------------
  void loadQuestion(const int level) { // LoadQuestion: loadQuestion
    char* questionFile = readFile("Images/Data/questions.txt", 300);

    int i = 0;
    int chunk = 0;
    int start = 0;

    while (questionFile[i] != '\0') {
      if (questionFile[i] == '%') chunk++;

      if (chunk == level) start = i + 1;

      if (chunk == level + 1) {
        int delimeter = findDelimiter(questionFile, start, i, '$');
        
        question = copyChunk(questionFile, start, delimeter); // Example: x=(2+3)*3-(2*2-4)
        answer = atoi(copyChunk(questionFile, delimeter + 1, i)); // Example: 000015
      }

      i++;
    }

    questionText = new Text(codeX + 15, codeY + 68, question, VERY_DARK_GRAY);
  }
  int findDelimiter(const char *str, int start, int end, char delimiter) {
    int i = start;

    while (str[i] != delimiter && i < end) {
      i++;
    }
    if (i == end) return -1;  // did not find

    return i;
  }
  char* copyChunk(const char* str, int start, int send) {
    char* result = new char[end - start];

    for (int i = start; i < end; i++) {
      result[i] = str[start + i];
    }
    result[end - start] = '\0'; // end of text flag

    return result;
  }
  // ---------------------------------------------------------------
  // loadQuestion {END}
  // ---------------------------------------------------------------

  void highlightLock(int cursorX, int cursorY) {
    if (!lockerLight->contains(cursorX, cursorY)) return;
    int distance = distanceFromLocker(cursorX, cursorY);

    lockerLight->show = distance > 55 && distance < 88;  // within the ring
  }

  void shouldCloseLockerView(const Inventory *inventory, int clickedX, int clickedY) {
    if (!lock->contains(clickedX, clickedY) && !inventory->containsInOpenInventory(clickedX, clickedY))  // did not clicked lock
      isOpen = false;
  }

  void openLock(int clickedX, int clickedY) { // OpenLock: openLock: 
    if (circle->contains(clickedX, clickedY)) { // left click circle
      circle->show = true;
      currentDigit = (currentDigit + 1) % 3; // this might need to be moved lower?
    } else {
      circle->show = false;
    }

    if (lockerLight->show && locker->contains(clickedX, clickedY)) {  // left click
      int lockNumer = calculatePickedNumber(clickedX, clickedY);
      comboDigits[currentDigit] = lockNumer;
      lockAngle->frame = lockNumer;
      displayCombination(comboDigits[0], comboDigits[1], comboDigits[2]);
    }
  }
  void verifyCombination() {
    if (combination() == answer && currentDigit == 0) {
      correctCombination = true;
    }

    if (correctCombination == true) {
      Timer();

      if (dt > 2000) {  // shows success checkmark for 2 seconds then dissapears
        isChestLocked = false;
        correctCombination = false;

        dt = 0;
        timer = 0;
        timer1 = 0;
      }
    }
  }
  int combination() {
      return comboDigits[0] * 10000 + comboDigits[1] * 100 + comboDigits[2];
  }

  void draw(const Param *p) {
    p->draw(x, y, currentFrame->width, currentFrame->height, currentFrame);
  }
  void drawTooltip(const Param *p, int cursorX, int cursorY) {
    bool showToolTip = contains(cursorX, cursorY);
    if (!showToolTip) return;

    p->draw(cursorX + 10, cursorY, toolTip->width, toolTip->height, toolTip);
  }
  void drawOpenChest(const Param *p) { // drawC: drawOpenChest
    lock->draw(p);

    if (isChestLocked) {
      locker->draw(p); // locked chest
    } else {
      drawChestContents(p); // unlocked chest
    }

    lockAngle->draw(p);
    combinationText->draw(p, (int[2]){currentDigit, currentDigit + 1}, 2, VIVID_RED);
    lockerLight->draw(p);
    circle->draw(p);
    questionText->draw(p);

    if (correctCombination) {
      int x = lock->x + 99 + 2;
      int y = lock->y + 107;
      p->draw(x, y, successUnlockAsset->width, successUnlockAsset->height,
              successUnlockAsset);
    }
  }

 private:
  void drawChestContents(Param *p) {
    p->draw(codeX, codeY, chestGrid->width, chestGrid->height, chestGrid);

    for (int i = 0; i < chestRows * chestColumns; i++) {
      if (items[i] == INV_EMPTY_CELL) continue;

      int column = (i % chestColumns);
      int row = (i - column) / chestColumns;

      int x = codeX + inventoryChestLeft +
              column * (spaceBetweenCells + inventorySquareDim);
      int y = codeY + inventoryChestTop +
              row * (spaceBetweenCells + inventorySquareDim);
      p->draw(x, y, itemAssets[i]->width, itemAssets[i]->height, itemAssets[i]);
    }
  }

  void initializeButtons() {
    int x = (screenPixelWidth - lock->w) / 2;
    int y = (screenPixelHeight - lock->h) / 2;

    lock = new Button(x, y, CHEST_CODE_IMG, WHITE);
    lockAngle = new LockAngle(x + 145, y + 149);
    locker = new Button(x + 99, y + 107, LOCKER_IMG, WHITE);
    lockerLight = new Button(x + 99, y + 107, LIGHT_LOCKER_IMG, WHITE);
    circle = new Button(x + 156, x + 161, CIRCLE_IMG, WHITE);

    lock->show = true;
    locker->show = true;
    lockerLight->show = false;
    circle->show = false;

    combinationText = new Text(x + 199, y + 207, "00 00 00", DARK_GRAYISH_BLUE);
  }

   int calculatePickedNumber(int clickedX, int clickedY) { // calculateAngle: calculatePickedNumber
     double x = locker->w / 2;
     double y = locker->h / 2;

     double dx = (clickedX - locker->x) - x;
     double dy = y - (clickedY - locker->y);

     double hypotenuse = sqrt(dx * dx + dy * dy);
     double angleRadians = acos(dy / hypotenuse);

     const double PI = 3.14;
     double angleDegrees = (angleRadians * 180) / PI;

     if (clickedX - locker->x < x) angleDegrees = 360 - angleDegrees;

     double degreesPerNotch = 9; // 360/40

     return angleDegrees / degreesPerNotch;
   }

   int distanceFromLocker(int cursorX, int cursorY) {
     int dx = (cursorX - locker->x) - locker->w / 2;
     int dy = locker->h / 2 - (cursorY - locker->y);
     int distance = sqrt(dx * dx + dy * dy);

     return distance;
   }

   void displayCombination(int num1, int num2, int num3) {
     char *currentCombo = new char[100];

     if (num1 < 10)
       sprintf(currentCombo, "0%d 0%d 0%d", num1, num2, num3);
     else
       sprintf(currentCombo, "%d %d %d", num1, num2, num3);

     combinationText->changeText(currentCombo);
   }

   bool didClickCell(int x, int y) { // Makes sure clicked cell and not space between
     std::tie(blockX, blockY) = findCell(x, y);

     int a = x - (codeX + inventoryChestLeft) - blockX * (inventorySquareDim + spaceBetweenCells);
     int b = y - (codeY + inventoryChestTop) - blockY * (inventorySquareDim + spaceBetweenCells);

     return a <= inventorySquareDim && b <= inventorySquareDim;
   }
   std::tuple<int, int> findCell(int x, int y) {
     int blockX = (x - (codeX + inventoryChestLeft)) / (inventorySquareDim + spaceBetweenCells);
     int blockY = (y - (codeY + inventoryChestTop)) / (inventorySquareDim + spaceBetweenCells);

     return std::tuple<int, int>(blockX, blockY);
   }

   void setupCodeCoordinates() {
     codeX = (screenPixelWidth - code->width) / 2;
     codeY = (screenPixelHeight - code->height) / 2;
   }
};
int Chest::dt = 0;
int Chest::timer = 0;
int Chest::timer1 = 0;

int Chest::counter = 0;

class PressurePlate { // ButtonON: PressurePlate
 public:
  int x, y;
  static int counter;

 private:
  Sprite *asset; // Image: asset, should this be drawn?

 public:
  PressurePlate(int x, int y) {
    this->x = x;
    this->y = y;
    asset = new Sprite(PRESSURE_PLATE_IMG);

    counter++;
  }

  bool contains(int x, int y) { // Touch: contains
    return x >= this->x && x <= this->x + asset->width && y >= this->y && y <= this->y + asset->height;
  }
};
int PressurePlate::counter = 0;

class MovingBlock { // BlockMoves: MovingBlock
 public:
  int x, y;
  int pI;
  static int counter;

 private:
  Sprite *asset; // Image: asset
  int initialX, initialY; // pX, pY
  int startTime;

 public:
  MovingBlock(int x, int y, int pi) {
    this->x = x;
    this->y = y - 1;
    this->initialX = x;
    this->initialY = y - 1;
    this->pI = pi - 1;
    asset = new Sprite(MOVING_BLOCK_IMG);

    counter++;
  }

  bool moveUp(Map gameMap) { // BlockMoveUp: moveUp
    if (!movingStairBlocks[i].shouldUpdatePosition()) return;

    if (y > pI) {
      gameMap[initialY / blockSize + 1][initialX / blockSize] = AIR_ID;

      y -= std::min(y - pI, 5);
    }

    if (y == pI) {
      gameMap[y / blockSize + 1][x / blockSize] = MOVING_BLOCK_ID;
      return false;
    }

    return true;
  }
  void moveDown(Map gameMap) { // BlockMoveDown: moveDown
    if (!movingStairBlocks[i].shouldUpdatePosition()) return;

    if (y < initialY) {
      gameMap[pI / blockSize + 1][x / blockSize] = AIR_ID;

      y += std::min(initialY - y, 5);
    }

    if (y == initialY) {
      gameMap[y / blockSize + 1][x / blockSize] = MOVING_BLOCK_ID;
    }
  }

  void draw(Param *p) {
    p->draw(x, y, asset->width, asset->height, asset);
  }

 private:
  bool shouldUpdatePosition() {
    // https://docs.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timegettime
    if (startTime == NULL) startTime = timeGetTime();
    int timeSinceLastFrame = timeGetTime() - startTime;

    if (timeSinceLastFrame > 5) {
      startTime = timeGetTime();
    }
    return timeSinceLastFrame > 5;
  }
};
int MovingBlock::counter = 0;

class FinalDoor {
 public:
  int x, y;
  bool needsKey = false;  // nk: Need key

  static int counter;

 private:
  int frame = 0; // Cadr: frame
  int isOpen = false; // num: doorId: isOpen

  int doorType; // type: doorType
  Sprite *assets[2]; // Image: assets
  Sprite *currentFrame; // ImageView: currentFrame
  Sprite *needKeyAsset; // NeedKey: needKeyAsset

 public:
  FinalDoor(int x, int y, int doorType) {
    this->x = x;
    this->y = y;
    this->doorType = doorType;

    needKeyAsset = new Sprite(NEED_KEY_IMG, WHITE);
    assets = loadAssets("Images/NextDoor%d.bmp", 2, WHITE);
    currentFrame = assets[0];

    if (doorType == 1) {
      assets[0]->flipHorizontally();
      assets[1]->flipHorizontally();
    }
    counter++;
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

  void updateFrame() {  // ChangeCadr: updateFrame
    if (doorType != 0) return;
    frame = (frame + 1) % 2;

    currentFrame = assets[frame];
  }

  void switchDoorState(Map gameMap) {
    int row = y / blockSize;
    int column = x / blockSize;
    isOpen = !isOpen;
    int doorId = isOpen ? OPEN_FINAL_DOOR_ID : CLOSED_FINAL_DOOR_ID;

    gameMap[row][column] = doorId;
    gameMap[row + 1][column] = doorId;
    gameMap[row + 2][column] = doorId;
    gameMap[row + 3][column] = doorId;
  }
};
int FinalDoor::counter = 0;