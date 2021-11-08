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
// mX, mY - 
// blocksInHeight, blocksInWidth - 
// screenPixelWidth, screenPixelHeight - 
// isInitialState - 
// -----

// PLAYING+++
void drawScene() {
  // Global: missionMode, fireEntity, doorEntity, bonusEntity, movingStairBlocks, chest, player, inventory, bookEntity, pauseOverlay
  // posObject, isPaused, scoreText
  //
  // External: paramDraw, mX, mY
  if (missionMode == true) {
    drawMission();
    return;
  }
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

    paramDraw->Draw(mX, mY, cur->width, cur->height, cur);
  }
  // Cursor---

  // Menu Pause+++
  if (isPaused == true) {
    drawPauseMenu();
  }
  // Menu Pause---

  scoreText->Draw(paramDraw);
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

void drawMission() {
  // Global: missions, missionButtons, missionLock, backButton
  // External: paramDraw, screenPixelWidth, screenPixelHeight
  paramDraw->Draw(0, 0, screenPixelWidth, screenPixelHeight, missions);

  if (missionButtons[0]->show == true) missionButtons[0]->Draw(paramDraw);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 11; j++) {
      if (i != 0 || j != 0)
        paramDraw->Draw(22 + j * 13 + j * missionLock->width,
                        63 + i * 36 + i * missionLock->height, missionLock->width,
                        missionLock->height, missionLock);
    }
  }

  if (backButton->show == true) backButton->Draw(paramDraw);
}

// Missions+++
void loadMap(char* c, int k, int z) {
  // Global: gameMap, doorEntity, finalDoor, movingStairBlocks, bookEntity
  // External: blocksInHeight, blocksInWidth,
  char num;
  int pi = 0;
  int t = 0;

  for (int i = 0; i < blocksInHeight; i++)
    for (int j = 0; j < blocksInWidth; j++) {
      num = c[k + z];

      if (num != '\0') gameMap[i][j] = atoi(&num);

      z++;
    }

  // Load doors +++
  for (int i = 0; i < blocksInHeight; i++)
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
  // Load doors---
}
int* Read(int p1, int p2, char* c) {
  // No globals
  int* cor = NULL;
  char* line = new char[p2 - p1];

  for (int i = p1; i < p2; i++) line[i - p1] = c[i];

  if (line[0] != 'N') {
    int quantiteXY = (p2 - p1 + 2) / 4;  // quantity of coordinates

    int quantite = quantiteXY / 2;  // quantity of objects

    cor = new int[quantiteXY];

    for (int i = 0; i < quantiteXY; i++) {
      char a1, a2, a3;

      a1 = line[i * 4];
      a2 = line[i * 4 + 1];
      a3 = line[i * 4 + 2];

      cor[i] = atoi(&a1) * 100 + atoi(&a2) * 10 + atoi(&a3);
    }
  }

  return cor;
}
void readScript() {
  // Global: mapFilename, player, pressurePlate, chest, bonusEntity, fireEntity
  // External: blocksInHeight, blocksInWidth

  // Read file ---
  char* c = new char[blocksInHeight * blocksInWidth + 100];
  std::ifstream is(mapFilename);

  for (int i = 0; i < blocksInHeight * blocksInWidth + 100; i++) is >> c[i];

  is.close();
  // Read file +++

  bool LM = false;
  int z = 0;

  int pos[6] = {-1, -1, -1, -1, -1, -1};

  for (int k = 0; k < blocksInHeight * blocksInWidth + 100; k++) {
    // Load map +++++++++++++++++++++
    if (LM == true) {
      loadMap(c, k, z);
      LM = false;
    }
    // Load map --------------------------

    if (c[k] == '|' && z == 0) LM = true;

    //Загрузка перехода на следующую миссию---
    if (c[k] == ':' && pos[5] != -1) {
      int* cor = Read(pos[5], k, c);  //Выделение координат X и Y

      if (cor != NULL) {
        player->exitX = cor[0];
        player->exitY = cor[1];
      }
    }
    //Загрузка перехода на следующую миссию---

    // Load buttons ---
    if (c[k] == ':' && pos[4] != -1 && pos[5] == -1) {
      pos[5] = k + 1;

      int* cor = Read(pos[4], k, c);  //Выделение координат X и Y

      if (cor != NULL)
        for (int i = 0; i < (k - pos[4] + 2) / 8; i++)
          pressurePlate[i] = new ButtonON(cor[i * 2], cor[i * 2 + 1]);
    }
    // Load buttons ---

    // Load chests ---
    if (c[k] == ':' && pos[3] != -1 && pos[4] == -1) {
      pos[4] = k + 1;

      int* cor = Read(pos[3], k, c);  //Выделение координат X и Y

      if (cor != NULL)
        for (int i = 0; i < (k - pos[3] + 2) / 16; i++)
          chest[i] = new Chest(cor[i * 4], cor[i * 4 + 1], level,
                               cor[i * 4 + 2], cor[i * 4 + 3]);

      pos[3] = 0;
    }
    // Load chests ---

    // Load bonuses ---
    if (c[k] == ':' && pos[2] != -1 && pos[3] == -1) {
      pos[3] = k + 1;

      int* cor = Read(pos[2], k, c);  //Выделение координат X и Y

      if (cor != NULL)
        for (int i = 0; i < (k - pos[2] + 1) / 12; i++)
          bonusEntity[i] = new Bonus(cor[i * 2], cor[i * 2 + 1]);
    }
    // Load bonuses ---

    // Load books +++
    if (c[k] == ':' && pos[1] != -1 && pos[2] == -1) {
      pos[2] = k + 1;

      int* cor = Read(pos[1], k, c);  //Выделение координат X и Y

      if (cor != NULL)
        for (int i = 0; i < (k - pos[1] + 2) / 8; i++)
          bookEntity[i] = new Book(cor[i * 2], cor[i * 2 + 1]);
    }
    // Load books ---

    // Load fire +++++++++++
    if (c[k] == ':' && pos[0] != -1 && pos[1] == -1) {
      pos[1] = k + 1;

      int* cor = Read(pos[0], k, c);  //Выделение координат X и Y

      if (cor != NULL)
        for (int i = 0; i < (k - pos[0] + 2) / 8; i++)
          fireEntity[i] = new Fire(cor[i * 2], cor[i * 2 + 1]);
    }

    if (c[k] == ':') pos[0] = k + 1;
    // Load fire ------------

    // Load player coordinates+++
    if (LM == false) {
      player->x = atoi(&c[0]);
      player->y = atoi(&c[4]);
    }
    // Load player coordinates+++
  }

  for (int i = 0; i < Chest::counter; i++) chest[i]->LoadQuestion(level);
}

void mission() {
  // Global: backButton, missionMode, playMode, missionButtons, player, map, gameMap
  // External: isInitialState, mX, mY, blocksInHeight, blocksInWidth

  // Exit+++
  if (backButton->Touch(X, Y) == true) {
    missionMode = false;
    playMode = false;
    isInitialState = true;
  }

  backButton->show = backButton->Touch(mX, mY);
  // Exit---

  if (missionButtons[0]->Touch(X, Y) == true) {
    player = new Hero();
    player->x = 40;
    player->y = 40;

    readScript();
    setMap();

    missionMode = false;
  }

  missionButtons[0]->show = missionButtons[0]->Touch(mX, mY);
}

void setMap() {
  map = new Sprite(mapBMPfilename, 0xffffffff);

  for (int i = 0; i < blocksInHeight; i++)
    for (int j = 0; j < blocksInWidth; j++)
      player->MatMap[i][j] = gameMap[i][j];
}
// Missions---

// Play+++
void interactiveObjects() {
  // Global: bonusEntity, player, score, scoreText, inventory, posObject, chest, stateK, buffer, bookEntity, fireEntity, ...
  // External: Fire class, Bonus class, Inventar class, Chest class, Door class, FinalDoor class, ButtonON class, BlockMoves class, Book class, ...

  // BONUS+++
  for (int i = 0; i < Bonus::counter; i++) {
    if (bonusEntity[i]->show == true &&
        (bonusEntity[i]->Touch(player->x, player->y + player->h - 1) ==
             true ||
         bonusEntity[i]->Touch(player->x + player->w,
                        player->y + player->h - 1) == true)) {
      score += 10;

      char* sc = new char[100];
      sprintf(sc, "score:%d", score);

      scoreText->changeText(sc);
      bonusEntity[i]->show = false;
    }
  }
  // BONUS---

  // Inventory+++
  if (inventory->Touch(mX, mY) == true) {
    inventory->exp = true;
    inventory->mX = mX + 10;
    inventory->mY = mY;
  } else
    inventory->exp = false;

  if (inventory->Touch(X, Y) == true) inventory->show = true;

  // Move objects+++
  if (inventory->TouchInvShow(Inventar::Xi, Inventar::Yi) == true) {
    if (Inventar::iLmb == true) {
      if (inventory->move == false) {
        inventory->check = inventory->TouchObject(Inventar::Xi, Inventar::Yi);

        if (inventory->check != -1) {
          inventory->move = true;

          for (int i = 0; i < Chest::counter; i++) chest[i]->move = true;

          posObject = inventory->objects[inventory->check];
          inventory->objects[inventory->check] = 0;
        }

      } else if (inventory->move == true) {
        int nt = inventory->TouchObject(Inventar::Xi, Inventar::Yi);

        if (nt != -1 && inventory->check != nt && inventory->objects[nt] == 0) {
          inventory->objects[nt] = posObject;
          inventory->ChangeImages();
          posObject = 0;
        }
        for (int i = 0; i < Chest::counter; i++) chest[i]->move = false;
        inventory->move = false;
      }

      Inventar::Xi = 0;
      Inventar::Yi = 0;
      Inventar::iLmb = false;
    }
  }
  // Move objects---

  // Move chest's objects+++
  for (int i = 0; i < Chest::counter; i++) {
    if (chest[i]->TouchInvChest(Chest::Xi, Chest::Yi) == true &&
        chest[i]->showC == true) {
      if (Chest::iLmb == true) {
        if (chest[i]->move == false) {
          chest[i]->check = chest[i]->TouchObject(Chest::Xi, Chest::Yi);

          if (chest[i]->check != -1) {
            chest[i]->move = true;
            inventory->move = true;
            posObject = chest[i]->objects[chest[i]->check];
            chest[i]->objects[chest[i]->check] = 0;
          }
        } else if (chest[i]->move == true) {
          int nt = chest[i]->TouchObject(Chest::Xi, Chest::Yi);

          if (nt != -1 && chest[i]->check != nt && chest[i]->objects[nt] == i) {
            chest[i]->objects[nt] = posObject;
            chest[i]->ChangeImages();
            posObject = i;
          }
          chest[i]->move = false;
          inventory->move = false;
        }

        Chest::Xi = i;
        Chest::Yi = i;
        Chest::iLmb = false;
      }
    }
  }
  // Move chest's objects---

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
    chest[i]->OpenLock(inventory, lmb, X, Y, mX, mY);

    if (chest[i]->Touch(mX, mY) == true) {
      chest[i]->expO = true;
      chest[i]->mX1 = mX + 10;
      chest[i]->mY1 = mY;
    } else
      chest[i]->expO = false;
  }
  // Chest---

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
  for (int i = 0; i < Door::counter; i++) doorEntity[i]->Touch(X, Y, gameMap, lmb);
  // DOOR---

  // FinalDoor+++
  for (int i = 0; i < FinalDoor::counter; i++)
    finalDoor[i]->Touch(X, Y, gameMap, lmb, inventory->objects[0]);
  // FinalDoor---

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

    for (int i = 0; i < BlockMoves::counter; i++) movingStairBlocks[i]->BlockMoveUp(gameMap, b);

    if (b == true) BlockMoves::timer1 = 0;
  }

  if (BlockMoves::dt > 1000 * 10) BlockMoves::UP = false;

  if (BlockMoves::dt > 5 && BlockMoves::UP == false) {
    for (int i = 0; i < BlockMoves::counter; i++) movingStairBlocks[i]->BlockMoveDown(gameMap);
  }
  // Block Moves---

  // BOOKS+++
  for (int i = 0; i < Book::counter; i++)
    if (bookEntity[i]->state = 'C' && bookEntity[i]->show == true &&
            (bookEntity[i]->Touch(player->x, player->y + player->Image->height) == true ||
             bookEntity[i]->Touch(player->x + player->Image->width, player->y + player->Image->height) == true)) {
      bookEntity[i]->state = 'O';
      bookEntity[i]->ImageBack = new Sprite("Images/book1.bmp", 0xffffffff);
      bookEntity[i]->Image = bookEntity[i]->ImageBack;
      bookEntity[i]->x = (screenPixelWidth - bookEntity[i]->ImageBack->width) / 2;
      bookEntity[i]->y = (screenPixelHeight - bookEntity[i]->ImageBack->height) / 2;
      inventory->AddObject(1);
      isBookMenuOpen = true;
    }
  // BOOKS---

  // CHEST+++
  if (buffer[DIK_O] & 0x80)
    for (int i = 0; i < Chest::counter; i++)
      if (chest[i]->Touch(player->x, player->y + player->h - 1) ==
              true ||
          chest[i]->Touch(player->x + player->w,
                          player->y + player->h - 1) == true)
        chest[i]->show = true;
  // CHEST---
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
  readScript();
  setMap();
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

void setNextMapFilepath(int level) {
  mapBMPfilename = new char[100]; // global
  mapFilename = new char[100]; // global

  sprintf(mapBMPfilename, "Images/Data/map%d.bmp", level);
  sprintf(mapFilename, "Images/Data/map%d.txt", level);
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
// Play---

void playLoop() {
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
  // External: mX, mY, isInitialState
  if (buffer[DIK_ESCAPE] & 0x80) isPaused = true; // Technical ??
  if (isPaused == false) return;

  for (int j = 0; j < 4; j++) pauseMenuButtons[j]->show = false;

  for (int i = 0; i < 4; i++)
    if (pauseMenuButtons[i]->Touch(mX, mY) == true) 
      pauseMenuButtons[i]->show = true;

  isPaused = shouldContinuePause();

  if (pauseMenuButtons[PM_EXIT]->Touch(X, Y) == true) {
    playMode = false;
    isInitialState = true;
  }
}

bool shouldContinuePause() {
  return pauseMenuButtons[PM_CONTINUE]->Touch(X, Y) == false 
  && pauseMenuButtons[PM_SAVE]->Touch(X, Y) == false 
  && pauseMenuButtons[PM_SETTINGS]->Touch(X, Y) == false;
}
// PLAYING---
