// Play++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool playMode = false; // Play: playMode

// Missions+++
bool missionMode = false; // Missions: missionMode

char* mapFilename = "Images/Data/map1.txt";
char* mapBMPfilename = "Images/Data/map1.bmp";

Sprite* missions;
Button* missionButtons[1]; // btnOfMissions: missionButtons
Sprite* missionLock; // blocked: missionLock
Button* backButton; // Back: backButton
// Missions---

// Game+++
int gameMap[30][40]; // MatMap: gameMap
int level = 1;

bool isBookMenuOpen = false; // stopB: isBookMenuOpen
bool isPaused = false; // stop: isPaused

Button* pauseMenuButtons[4]; // PM: pauseMenuButtons: Continue, Save, Settings, Exit

Sprite* map;
Sprite* pauseOverlay; // pause: pauseOverlay
Sprite* pauseMenuSprite; // Menu_pause: pauseMenuSprite

// Objects in the game+++
Hero* player; // Personage: player
Fire* fireEntity[10]; // f: fireEntity
Door* doorEntity[10]; // d: doorEntity
Book* bookEntity[10]; // b: bookEntity
Bonus* bonusEntity[10]; // bons: bonusEntity
Chest* chest[10];
BlockMoves* movingStairBlocks[5]; // bm: movingStairBlocks
ButtonON* pressurePlate[10]; // bt: pressurePlate
FinalDoor* finalDoor[10]; // Fd: finalDoor
// Objects in the game---

Inventar* inventory; // Inv: inventory

// Move Invent Chest+++
int posObject = 0;
// Move Invent Chest---

int stateK = 0;

Text* scoreText; // txt: scoreText
int score = 0;
// Play------------------------------------------------------

// Globals from main.cpp
// -----
// paramDraw - global buffer sprites are rendered to
// cursorX, cursorY - current position of the cursor
// clickedX, clickedY - last cursor position after right/left click
// blocksInHeight, blocksInWidth - 
// screenPixelWidth, screenPixelHeight - 
// isInitialState - 
// -----

// ---------------------------------------------------------------------------------
// drawScene function
// ---------------------------------------------------------------------------------
void drawScene(int cursorX, int cursorY) { // ★★★
  // Global: missionMode, isPaused, scoreText
  //
  // External: paramDraw
  if (missionMode == true) {
    drawMission();
    return;
  }
  
  drawEntities(cursorX, cursorY);

  if (isPaused == true) {
    drawPauseMenu();
  }

  scoreText->Draw(paramDraw);
}
void drawMission() {
  // Global: missions, missionButtons, missionLock, backButton
  // External: paramDraw, screenPixelWidth, screenPixelHeight
  paramDraw->Draw(0, 0, screenPixelWidth, screenPixelHeight, missions);

  if (missionButtons[0]->show == true) missionButtons[0]->Draw(paramDraw);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 11; j++) {
      if (!(i == 0 && j == 0))
        paramDraw->Draw(22 + j * 13 + j * missionLock->width,
                        63 + i * 36 + i * missionLock->height, missionLock->width,
                        missionLock->height, missionLock);
    }
  }

  if (backButton->show == true) backButton->Draw(paramDraw);
}
void drawEntities(int cursorX, int cursorY) {
  // Global: fireEntity, doorEntity, bonusEntity, movingStairBlocks, chest, player, inventory, bookEntity, pauseOverlay
  // posObject
  // External: paramDraw, screenPixelWidth, screenPixelHeight
  paramDraw->Draw(0, 0, screenPixelWidth, screenPixelHeight, map);

  for (int i = 0; i < Fire::counter; i++) {
    paramDraw->Draw(fireEntity[i]->x, fireEntity[i]->y,
                    fireEntity[i]->ImageView->width,
                    fireEntity[i]->ImageView->height, fireEntity[i]->ImageView);
  }

  for (int i = 0; i < Door::counter; i++) {
    paramDraw->Draw(doorEntity[i]->x, doorEntity[i]->y,
                    doorEntity[i]->ImageView->width,
                    doorEntity[i]->ImageView->height, doorEntity[i]->ImageView);
  }

  for (int i = 0; i < FinalDoor::counter; i++) {
    finalDoor[i]->Draw(paramDraw);
  }

  for (int i = 0; i < Bonus::counter; i++) {
    if (bonusEntity[i]->show == true)
      paramDraw->Draw(bonusEntity[i]->x, bonusEntity[i]->y,
                      bonusEntity[i]->ImageView->width,
                      bonusEntity[i]->ImageView->height,
                      bonusEntity[i]->ImageView);
  }

  for (int i = 0; i < BlockMoves::counter; i++) {
    paramDraw->Draw(movingStairBlocks[i]->x, movingStairBlocks[i]->y, movingStairBlocks[i]->Image->width,
                    movingStairBlocks[i]->Image->height, movingStairBlocks[i]->Image);
  }

  // Chest++++
  for (int i = 0; i < Chest::counter; i++) {
    chest[i]->Draw(paramDraw);
  }

  player->Draw(paramDraw);

  for (int i = 0; i < Chest::counter; i++) {
    if (chest[i]->show == true) {
      paramDraw->Draw(0, 0, pauseOverlay->width, pauseOverlay->height,
                      pauseOverlay);
      chest[i]->DrawC(paramDraw);
    }
  }
  // Chest---

  inventory->Draw(paramDraw);

  // Book++++
  for (int i = 0; i < Book::counter; i++) {
    if (bookEntity[i]->show == true) {
      if (bookEntity[i]->state == 'O') {
        paramDraw->Draw(0, 0, pauseOverlay->width, pauseOverlay->height,
                        pauseOverlay);
      }

      paramDraw->Draw(bookEntity[i]->x, bookEntity[i]->y,
                      bookEntity[i]->Image->width, bookEntity[i]->Image->height,
                      bookEntity[i]->Image);
    }
  }
  // Book---

  // Cursor+++
  if (posObject != 0) {
    char* path = new char[20];
    sprintf(path, "Images/o%d.bmp", posObject);

    Sprite* cur = new Sprite(path, 0xffffffff);

    paramDraw->Draw(cursorX, cursorY, cur->width, cur->height, cur);
  }
  // Cursor---
}
void drawPauseMenu() {
  // Global: pauseOverlay, pauseMenuSprite, pauseMenuButtons
  // External: paramDraw, screenPixelWidth, screenPixelHeight
  paramDraw->Draw(0, 0, pauseOverlay->width, pauseOverlay->height,
                  pauseOverlay);
  paramDraw->Draw((screenPixelWidth - pauseMenuSprite->width) / 2,
                  (screenPixelHeight - pauseMenuSprite->height) / 2, pauseMenuSprite->width,
                  pauseMenuSprite->height, pauseMenuSprite);

  for (int i = 0; i < 4; i++) {
    pauseMenuButtons[i]->Draw(paramDraw);
  }
}
// ---------------------------------------------------------------------------------
// drawScene {end}
// ---------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------
// readScript function
// ---------------------------------------------------------------------------------
void readScript(char* filename) {
  // Global: mapFilename, player
  // External: blocksInHeight, blocksInWidth
  int bufferSize = blocksInHeight * blocksInWidth + 100;
  char* fileBuffer = readFile(filename, bufferSize);
  int readingChunk = 0;
  int prevColonIndex = 0;

  for (int index = 0; index < bufferSize; index++) {
    if (fileBuffer[index] == ':') {
      int* coordinates = readCoordinates(prevColonIndex, index, fileBuffer);

      if (coordinates != NULL) {
        int coord_quantity = (index - prevColonIndex + 2) / 4;
        createEntity(readingChunk, coordinates, coord_quantity);
      }

      prevColonIndex = index + 1;
      readingChunk++;
    }
  }

  int mapDeliminatorPos = findDelimiter(fileBuffer, bufferSize);
  loadMap(fileBuffer, mapDeliminatorPos);
  setMap();

  loadDoors();

  for (int i = 0; i < Chest::counter; i++) chest[i]->LoadQuestion(level);
}
void readFile(char* filename, int bufferSize) {
  char* fileBuffer = new char[bufferSize];
  std::ifstream is(filename);

  for (int i = 0; i < bufferSize; i++) {
    is >> fileBuffer[i];
  }

  is.close();

  return fileBuffer;
}
int* readCoordinates(int begin, int end, char* fileBuffer) {
  // No globals
  if (fileBuffer[0] == 'N')
    return NULL; // no info flag

  int quantity = (end - begin + 2) / 4;  // quantity of coordinates
  int* coordinates = new int[quantity];

  for (int i = 0; i < quantity; i++) {
    int a1 = fileBuffer[begin + i * 4] - '0';
    int a2 = fileBuffer[begin + i * 4 + 1] - '0';
    int a3 = fileBuffer[begin + i * 4 + 2] - '0';

    coordinates[i] = a1 * 100 + a2 * 10 + a3;
  }

  return coordinates;
}
void createEntity(int readingChunk, int* coordinates, int coord_quantity) {
  // Global: pressurePlate, chest, bonusEntity, fireEntity
  switch (readingChunk) {
    case 0: // Load player coordinates
      player->x = coordinates[0];
      player->y = coordinates[1];
      break;
    
    case 1:
      for (int i = 0; i < coord_quantity / 2; i++)
        fireEntity[i] = new Fire(coordinates[i * 2], coordinates[i * 2 + 1]);
      break;

    case 2:
      for (int i = 0; i < coord_quantity / 2; i++)
        bookEntity[i] = new Book(coordinates[i * 2], coordinates[i * 2 + 1]);
      break;

    case 3:
      for (int i = 0; i < coord_quantity / 3; i++) // TODO: this should be 2 and not 3
        bonusEntity[i] = new Bonus(coordinates[i * 2], coordinates[i * 2 + 1]);
      break;

    case 4:
      for (int i = 0; i < coord_quantity / 4; i++)
        chest[i] = new Chest(coordinates[i * 4], coordinates[i * 4 + 1], level,
                             coordinates[i * 4 + 2], coordinates[i * 4 + 3]);
      break;

    case 5: 
      for (int i = 0; i < coord_quantity / 2; i++)
        pressurePlate[i] = new ButtonON(coordinates[i * 2], coordinates[i * 2 + 1]);
      break;

    case 6: // Загрузка перехода на следующую миссию
      player->exitX = coordinates[0];
      player->exitY = coordinates[1];
      break;
  }
}
int findDelimiter(char* fileBuffer, int bufferSize) {
  for (int index = 0; index < bufferSize; index++) {
    if (fileBuffer[index] == '|') {
      return index;
    }
  }

  return 0;
}
void loadMap(char* fileBuffer, int mapDeliminatorPos) {
  // Global: gameMaps
  // External: blocksInHeight, blocksInWidth,
  for (int i = 0; i < blocksInHeight; i++) {
    for (int j = 0; j < blocksInWidth; j++) {
      char num = fileBuffer[mapDeliminatorPos + i * blocksInWidth + j];

      if (num != '\0') {
        gameMap[i][j] = atoi(&num);
      }
    }
  }
}
void setMap() {
  map = new Sprite(mapBMPfilename, 0xffffffff);

  for (int i = 0; i < blocksInHeight; i++)
    for (int j = 0; j < blocksInWidth; j++)
      player->MatMap[i][j] = gameMap[i][j];
}
void loadDoors() {
  // Global: gameMap, doorEntity, finalDoor, movingStairBlocks
  int pi = 0;
  int t = 0;

  for (int i = 0; i < blocksInHeight; i++) {
    for (int j = 0; j < blocksInWidth; j++) {
      if (gameMap[i][j] == 4 && gameMap[i - 1][j] != 4)
        doorEntity[Door::counter - 1] = new Door(j * 20, i * 20);

      // Load final doors +++
      if (j == 0)
        t = 1;
      else if (j == blocksInWidth - 1)
        t = 0;

      if (gameMap[i][j] == 8 && gameMap[i - 1][j] != 8)
        finalDoor[FinalDoor::counter - 1] = new FinalDoor(j * 20, i * 20, t);
      // Load final doors ---

      // Block moves+++
      if (gameMap[i][j] == 6) {
        if (BlockMoves::counter == 0) pi = (i - 1) * 20;

        movingStairBlocks[BlockMoves::counter - 1] = new BlockMoves(j * 20, i * 20, pi);
      }
      // Block moves---
    }
  }
}
// ---------------------------------------------------------------------------------
// readScript {end}
// ---------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------
// playLoop function
// ---------------------------------------------------------------------------------
void playLoop() { // ★★★
  if (playMode == false) return;
  if (missionMode == true) {
    mission();
    return;
  }
  nextLevel();

  if (isBookMenuOpen == false && isPaused == false) {
    interactiveObjects();
    playerEvents();
  }

  closeTheBook();
  menuPause();
}
void mission() {
  // Global: backButton, missionMode, playMode, missionButtons, player, map, gameMap
  // External: isInitialState, cursorX, cursorY, blocksInHeight, blocksInWidth

  // Exit+++
  if (backButton->Touch(clickedX, clickedY) == true) {
    missionMode = false;
    playMode = false;
    isInitialState = true;
  }

  backButton->show = backButton->Touch(cursorX, cursorY);
  // Exit---

  if (missionButtons[0]->Touch(clickedX, clickedY) == true) {
    player = new Hero();
    player->x = 40;
    player->y = 40;

    readScript(mapFilename);

    missionMode = false;
  }

  missionButtons[0]->show = missionButtons[0]->Touch(cursorX, cursorY);
}
void nextLevel() {
  // Global: player, level, inventory, map, gameMap
  // External: blocksInHeight, blocksInWidth
  if (player->ChangeLevel() == false) return;

  level++;

  for (int i = 0; i < 9; i++)
    if (inventory->objects[i] == 2) inventory->objects[i] = 0;

  clearClassInformation();
  resetEntities();
  clearMap(); // this might be redundant, as we are loading the new map next
  setNextMapFilepath(level);
  readScript(mapFilename);
}
// Not sure what those values mean and why they should be reset
void clearClassInformation() {
  Fire::counter = 0;
  Door::counter = 0;
  Book::counter = 0;
  Bonus::counter = 0;
  //++++++
  Chest::dt = 0;
  Chest::timer = 0;
  Chest::timer1 = 0;

  Chest::counter = 0;
  Chest::Xi = 0;
  Chest::Yi = 0;

  Chest::iLmb = false;
  //------
  BlockMoves::counter = 0;
  ButtonON::counter = 0;
  FinalDoor::counter = 0;
}
void resetEntities() {
  for (int i = 0; i < 10; i++) {
    fireEntity[i] = NULL;
    doorEntity[i] = NULL;
    bookEntity[i] = NULL;
    bonusEntity[i] = NULL;
    chest[i] = NULL;
    movingStairBlocks[i] = NULL;
    pressurePlate[i] = NULL;
    finalDoor[i] = NULL;
  }
}
void clearMap() {
  // Global: gameMap
  // External: blocksInHeight, blocksInWidth
  for (int i = 0; i < blocksInHeight; i++) {
    for (int j = 0; j < blocksInWidth; j++) {
      gameMap[i][j] = 0;
    }
  }
}
void setNextMapFilepath(int level) {
  mapBMPfilename = new char[100]; // global
  mapFilename = new char[100]; // global

  sprintf(mapBMPfilename, "Images/Data/map%d.bmp", level);
  sprintf(mapFilename, "Images/Data/map%d.txt", level);
}
void playerEvents() {
  // Global: player, gameMap, buffer, inventory
  // External: Hero class
  player->ChargeMatMap(gameMap);

  Hero::Timer();
  Hero::TimerG();

  if (buffer[DIK_N] & 0x80) inventory->AddObject(2);

  if ((buffer[DIK_RIGHT] & 0x80) || (buffer[DIK_D] & 0x80)) {
    player->R = true;
    player->L = false;
  }

  if ((buffer[DIK_LEFT] & 0x80) || (buffer[DIK_A] & 0x80)) {
    player->L = true;
    player->R = false;
  }

  if (buffer[DIK_SPACE] & 0x80) {
    int nx = player->x / 20;
    int nx1 = (player->x + player->w) / 20;
    int ny = (player->y + player->h) / 20;

    if (gameMap[ny][nx] != 0 || gameMap[ny][nx1] != 0) {
      player->J = true;
      player->velocityJ = 15;
    }
  }

  if ((buffer[DIK_UP] & 0x80) || (buffer[DIK_W] & 0x80)) {
    player->U = true;
    player->D = false;
  }

  if ((buffer[DIK_DOWN] & 0x80) || (buffer[DIK_S] & 0x80)) {
    player->D = true;
    player->U = false;
  }

  player->Jump();

  if (Hero::dtG > 20) player->Gravitaton();

  if (Hero::dt > 15) {
    player->MoveL();
    player->MoveR();
    player->UD('U');
    player->UD('D');

    int nx = player->x / 20;
    int ny = (player->y + player->h) / 20;

    if (gameMap[ny][nx] == 3) {
      player->G = false;
    } else
      player->G = true;
  }
}

void closeTheBook() {
  if (buffer[DIK_RETURN] & 0x80) { // Technical ??
    for (int i = 0; i < Book::counter; i++) {
      if (bookEntity[i]->state == 'O') {
        bookEntity[i]->show = false;
        bookEntity[i]->state = 'C';
        isBookMenuOpen = false;
      }
    }
  }
}
void menuPause() {
  // Global: buffer, isPaused, pauseMenuButtons, playMode
  // External: cursorX, cursorY, isInitialState
  if (buffer[DIK_ESCAPE] & 0x80) isPaused = true; // Technical ??
  if (isPaused == false) return;

  for (int j = 0; j < 4; j++) pauseMenuButtons[j]->show = false;

  for (int i = 0; i < 4; i++)
    if (pauseMenuButtons[i]->Touch(cursorX, cursorY) == true) 
      pauseMenuButtons[i]->show = true;

  isPaused = shouldContinuePause();

  if (pauseMenuButtons[PM_EXIT]->Touch(clickedX, clickedY) == true) {
    playMode = false;
    isInitialState = true;
  }
}
bool shouldContinuePause() {
  return pauseMenuButtons[PM_CONTINUE]->Touch(clickedX, clickedY) == false 
  && pauseMenuButtons[PM_SAVE]->Touch(clickedX, clickedY) == false 
  && pauseMenuButtons[PM_SETTINGS]->Touch(clickedX, clickedY) == false;
}
// ---------------------------------------------------------------------------------
// playLoop {end}
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// interactiveObjects functions
// ---------------------------------------------------------------------------------
void interactiveObjects() {
  // Global: player, inventory, chest, stateK, buffer, bookEntity, ...
  // External: Chest class, ButtonON class, BlockMoves class, Book class

  didPlayerTouchBonus();
  showInventoryToolTip(cursorX, cursorY);

  if (inventory->Touch(clickedX, clickedY) == true) inventory->show = true;

  // clicked showing result = clicked || showing 
  // T        T       T
  // T        F       T 
  // F        T       T
  // F        F       F

  inventoryMoveEvents();
  chestMoveEvents();

  //++++++
  if (stateK == 1) {
    inventory->show = !inventory->show;

    stateK = 2;
  }

  if (buffer[DIK_E] & 0x80 && stateK == 0) stateK = 1;

  if (stateK == 2) stateK = 0;
  //-----

  // Chest+++
  for (int i = 0; i < Chest::counter; i++) {
    chest[i]->OpenLock(inventory, lmb, clickedX, clickedY, cursorX, cursorY);

    showChestToolTip(i, cursorX, cursorY);
  }
  // Chest---

  updateFrames();

  // ButtonON+++
  for (int i = 0; i < ButtonON::counter; i++) {
    if (pressurePlate[i]->Touch(player->x, player->y + player->h - 1) == true)
      BlockMoves::UP = true;
  }
  // ButtonON---

  // Block Moves+++
  BlockMoves::Timer();

  if (BlockMoves::dt > 5 && BlockMoves::UP == true) {
    bool b = true;

    for (int i = 0; i < BlockMoves::counter; i++) {
      bool tmp = movingStairBlocks[i]->BlockMoveUp(gameMap);

      if (!tmp) b = false;
    }
    /// tmp b  A = tmp & b
    /// T   T  T
    /// T   F  F
    /// F   T  F
    /// F   F  F

    if (b == true) BlockMoves::timer1 = 0;
  }

  if (BlockMoves::dt > 1000 * 10) BlockMoves::UP = false;

  if (BlockMoves::dt > 5 && BlockMoves::UP == false) {
    for (int i = 0; i < BlockMoves::counter; i++) movingStairBlocks[i]->BlockMoveDown(gameMap);
  }
  // Block Moves---

  // BOOKS+++
  for (int i = 0; i < Book::counter; i++) {
    bool bottomLeft = bookEntity[i]->Touch(player->x, player->y + player->Image->height);
    bool bottomRight = bookEntity[i]->Touch(player->x + player->Image->width, player->y + player->Image->height);

    if (bookEntity[i]->state = 'C' && bookEntity[i]->show == true && (bottomLeft || bottomRight)) {
      bookEntity[i]->state = 'O';
      bookEntity[i]->ImageBack = new Sprite("Images/book1.bmp", 0xffffffff);
      bookEntity[i]->Image = bookEntity[i]->ImageBack;
      bookEntity[i]->x = (screenPixelWidth - bookEntity[i]->ImageBack->width) / 2;
      bookEntity[i]->y = (screenPixelHeight - bookEntity[i]->ImageBack->height) / 2;

      inventory->AddObject(1);
      isBookMenuOpen = true;
    }
  }
  // BOOKS---

  // CHEST+++
  if (buffer[DIK_O] & 0x80) {
    for (int i = 0; i < Chest::counter; i++) {
      bool bottomLeft = chest[i]->Touch(player->x, player->y + player->h - 1);
      bool bottomRight = chest[i]->Touch(player->x + player->w, player->y + player->h - 1);

      if (bottomLeft || bottomRight)
        chest[i]->show = true;
    }
  }
  // CHEST---
}
void didPlayerTouchBonus() {
  // Checks if player intersected with bonus entity.
  // if intersected, then increase score and hide bonus entity
  for (int i = 0; i < Bonus::counter; i++) {
    bool bottomLeft = bonusEntity[i]->Touch(player->x, player->y + player->h - 1);
    bool bottomRight = bonusEntity[i]->Touch(player->x + player->w, player->y + player->h - 1);

    if (bonusEntity[i]->show && (bottomLeft || bottomRight)) {
      score += 10;

      char* sc = new char[100];
      sprintf(sc, "score:%d", score);

      scoreText->changeText(sc);
      bonusEntity[i]->show = false;
    }
  }
}
void showInventoryToolTip(int mouseX, int mouseY) {
  inventory->exp = inventory->Touch(mouseX, mouseY); // show or hide tool tip
  inventory->mX = mouseX + 10;
  inventory->mY = mouseY;
}
void inventoryMoveEvents() {
  if (inventory->TouchInvShow(Inventar::Xi, Inventar::Yi) == false) {
    return;
  }

  if (Inventar::iLmb == false) { 
    return;
  }

  if (inventory->move == true) {
    int nt = inventory->TouchObject(Inventar::Xi, Inventar::Yi);

    if (nt != -1 && inventory->check != nt && inventory->objects[nt] == 0) {
      inventory->objects[nt] = posObject;
      inventory->ChangeImages();
      posObject = 0;
    }
    for (int i = 0; i < Chest::counter; i++) chest[i]->move = false;
    inventory->move = false;
  } else {
    inventory->check = inventory->TouchObject(Inventar::Xi, Inventar::Yi);

    if (inventory->check != -1) {
      inventory->move = true;

      for (int i = 0; i < Chest::counter; i++) chest[i]->move = true;

      posObject = inventory->objects[inventory->check];
      inventory->objects[inventory->check] = 0;
    }
  }

  Inventar::Xi = 0;
  Inventar::Yi = 0;
  Inventar::iLmb = false;
}
void chestMoveEvents() {
  for (int i = 0; i < Chest::counter; i++) {
    bool clickedChest = chest[i]->TouchInvChest(Chest::Xi, Chest::Yi);

    if (!(clickedChest && chest[i]->showC && Chest::iLmb)) {
      continue;
    }

    if (chest[i]->move) {
      int nt = chest[i]->TouchObject(Chest::Xi, Chest::Yi);

      if (nt != -1 && chest[i]->check != nt && chest[i]->objects[nt] == i) {
        chest[i]->objects[nt] = posObject;
        chest[i]->ChangeImages();
        posObject = i;
      }
      chest[i]->move = false;
      inventory->move = false;
    } else {
      chest[i]->check = chest[i]->TouchObject(Chest::Xi, Chest::Yi);

      if (chest[i]->check != -1) {
        chest[i]->move = true;
        inventory->move = true;
        posObject = chest[i]->objects[chest[i]->check];
        chest[i]->objects[chest[i]->check] = 0;
      }
    }

    Chest::Xi = i;
    Chest::Yi = i;
    Chest::iLmb = false;
  }
}
void showChestToolTip(int i, int mouseX, int mouseY) {
  chest[i]->expO = chest[i]->Touch(mouseX, mouseY);
  chest[i]->mX1 = mouseX + 10;
  chest[i]->mY1 = mouseY;
}
void updateFrames() {
  // FIRE---
  Fire::Timer();

  if (Fire::dt > 50) {
    for (int i = 0; i < Fire::counter; i++) fireEntity[i]->ChangeCadr();

    Fire::timer1 = 0;
    Fire::dt = 0;
  }
  // FIRE+++

  // BONUS---
  Bonus::Timer();

  if (Bonus::dt > 50) {
    for (int i = 0; i < Bonus::counter; i++) bonusEntity[i]->ChangeCadr();

    Bonus::timer1 = 0;
    Bonus::dt = 0;
  }
  // BONUS+++

  // DOOR+++
  for (int i = 0; i < Door::counter; i++)
    doorEntity[i]->Touch(clickedX, clickedY, gameMap, lmb);
  // DOOR---

  // FinalDoor+++
  for (int i = 0; i < FinalDoor::counter; i++)
    finalDoor[i]->Touch(clickedX, clickedY, gameMap, lmb,
                        inventory->objects[0]);
  // FinalDoor---
}
// ---------------------------------------------------------------------------------
// interactiveObjects {end}
// ---------------------------------------------------------------------------------
