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
int blockSize = 20;

int AIR_ID = 0;
int LADDER_ID = 3;
int MOVING_BLOCK_ID = 6;
int DOOR_ID = 4;
int CLOSED_DOOR_ID = 5; // I think so?
int FINAL_DOOR_ID = 8;
int CLOSED_FINAL_DOOR_ID = 9; // Final open door?

// Inventory objects
int INV_EMPTY_CELL = 0;
int INV_BOOK = 1; // Images/o1.bmp
int INV_KEY = 2; // Images/o2.bmp

bool isBookMenuOpen = false; // stopB: isBookMenuOpen
bool isPaused = false; // stop: isPaused

Button* pauseMenuButtons[4]; // PM: pauseMenuButtons: Continue, Save, Settings, Exit

Sprite* map;
Sprite* pauseOverlay; // pause: pauseOverlay
Sprite* pauseMenuSprite; // Menu_pause: pauseMenuSprite

// Objects in the game+++
Player* player; // Personage: player
Fire* fireEntities[10]; // f: fireEntities - TODO: change array data structure to list (so we dont need to store the length)
Door* doorEntities[10]; // d: doorEntities
Book* bookEntities[10]; // b: bookEntities
Bonus* bonusEntities[10]; // bons: bonusEntities
Chest* chest[10];
BlockMoves* movingStairBlocks[5]; // bm: movingStairBlocks
PressurePlate* pressurePlate[10]; // bt: pressurePlate
FinalDoor* finalDoor[10]; // Fd: finalDoor
// Objects in the game---

Inventory* inventory; // Inv: inventory

// Move Invent Chest+++
int selectedObjectId = INV_EMPTY_CELL; // posObject: selectedObjectId
// Move Invent Chest---

Text* scoreText; // txt: scoreText
int score = 0;

// Globals from main.cpp
// -----
// paramDraw - global buffer sprites are rendered to
// blocksInHeight, blocksInWidth - 
// screenPixelWidth, screenPixelHeight - 
// isInitialState - 
// -----

// Dependency injected
// -----
// clickedX, clickedY - last cursor position after right/left click
// cursorX, cursorY - current position of the cursor
// ---------------------------------------------------------------------------------
// drawScene function
// ---------------------------------------------------------------------------------
void drawScene(int cursorX, int cursorY) { // ★★★
  // Global: missionMode, isPaused, scoreText
  //
  // External: paramDraw
  if (missionMode) {
    drawMission();
    return;
  }
  
  drawEntities(cursorX, cursorY);

  if (isPaused) {
    drawPauseMenu();
  }

  scoreText->draw(paramDraw);
}
void drawMission() {
  // Global: missions, missionButtons, missionLock, backButton
  // External: paramDraw, screenPixelWidth, screenPixelHeight
  paramDraw->draw(0, 0, screenPixelWidth, screenPixelHeight, missions);

  if (missionButtons[0]->show) missionButtons[0]->draw(paramDraw);

  // missions.bmp
  int missionsRows = 4;
  int missionsColumns = 11;

  // px
  int missionsTableX = 22;
  int missionsTableY = 63;
  int horizontalSpace = 13;
  int verticalSpace = 36;

  for (int i = 0; i < missionsRows; i++) {
    for (int j = 0; j < missionsColumns; j++) {
      if (i == 0 && j == 0) continue;

      int x = missionsTableX + j * (missionLock->width + horizontalSpace);
      int y = missionsTableY + i * (missionLock->height + verticalSpace);

      paramDraw->draw(x, y, missionLock->width, missionLock->height,
                      missionLock);
    }
  }

  if (backButton->show) backButton->draw(paramDraw);
}
void drawEntities(int cursorX, int cursorY) {
  // Global: fireEntities, doorEntities, bonusEntities, movingStairBlocks, chest, player, inventory, bookEntities, pauseOverlay
  // selectedObjectId
  // External: paramDraw, screenPixelWidth, screenPixelHeight
  paramDraw->draw(0, 0, screenPixelWidth, screenPixelHeight, map);

  for (int i = 0; i < Fire::counter; i++) {
    paramDraw->draw(fireEntities[i]->x, fireEntities[i]->y,
                    fireEntities[i]->currentSprite->width,
                    fireEntities[i]->currentSprite->height, fireEntities[i]->currentSprite);
  }

  for (int i = 0; i < Door::counter; i++) {
    paramDraw->draw(doorEntities[i]->x, doorEntities[i]->y,
                    doorEntities[i]->currentFrame->width,
                    doorEntities[i]->currentFrame->height, doorEntities[i]->currentFrame);
  }

  for (int i = 0; i < FinalDoor::counter; i++) {
    finalDoor[i]->draw(paramDraw);
  }

  for (int i = 0; i < Bonus::counter; i++) {
    if (!bonusEntities[i]->show) continue;

    paramDraw->draw(
        bonusEntities[i]->x, bonusEntities[i]->y, bonusEntities[i]->currentFrame->width,
        bonusEntities[i]->currentFrame->height, bonusEntities[i]->currentFrame);
  }

  for (int i = 0; i < BlockMoves::counter; i++) {
    paramDraw->draw(movingStairBlocks[i]->x, movingStairBlocks[i]->y, movingStairBlocks[i]->Image->width,
                    movingStairBlocks[i]->Image->height, movingStairBlocks[i]->Image);
  }

  // Chest++++
  player->draw(paramDraw);

  for (int i = 0; i < Chest::counter; i++) {
    chest[i]->draw(paramDraw);

    if (!chest[i]->isOpen) continue;

    paramDraw->draw(0, 0, pauseOverlay->width, pauseOverlay->height,
                    pauseOverlay);
    chest[i]->drawOpenChest(paramDraw); // does not mutate parameter
  }
  // Chest---

  inventory->draw(paramDraw, cursorX, cursorY);

  // Book++++
  for (int i = 0; i < Book::counter; i++) {
    if (!bookEntities[i]->show) continue;

    if (bookEntities[i]->isOpen) {
      paramDraw->draw(0, 0, pauseOverlay->width, pauseOverlay->height,
                      pauseOverlay);
    }

    paramDraw->draw(bookEntities[i]->x, bookEntities[i]->y,
                    bookEntities[i]->bookAsset->width, bookEntities[i]->bookAsset->height,
                    bookEntities[i]->bookAsset);
  }
  // Book---

  // Cursor+++
  if (selectedObjectId != INV_EMPTY_CELL) {
    char* path = new char[20];
    sprintf(path, "Images/o%d.bmp", selectedObjectId);

    Sprite* cursorIcon = new Sprite(path, 0xffffffff);

    paramDraw->draw(cursorX, cursorY, cursorIcon->width, cursorIcon->height, cursorIcon);
  }
  // Cursor---
}
void drawPauseMenu() {
  // Global: pauseOverlay, pauseMenuSprite, pauseMenuButtons
  // External: paramDraw, screenPixelWidth, screenPixelHeight
  int leftCentered = (screenPixelWidth - pauseMenuSprite->width) / 2;
  int topCentered = (screenPixelHeight - pauseMenuSprite->height) / 2;
  
  paramDraw->draw(0, 0, pauseOverlay->width, pauseOverlay->height,
                  pauseOverlay);
  paramDraw->draw(leftCentered, topCentered, pauseMenuSprite->width,
                  pauseMenuSprite->height, pauseMenuSprite);

  for (int i = 0; i < 4; i++) {
    pauseMenuButtons[i]->draw(paramDraw);
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

  readEntities(fileBuffer, bufferSize);
  int mapDeliminatorPos = findDelimiter(fileBuffer, bufferSize);
  loadMap(fileBuffer, mapDeliminatorPos);
  setMap();
  loadDoors();

  for (int i = 0; i < Chest::counter; i++) chest[i]->loadQuestion(level); // does not mutate parameter
}
void readEntities(char* fileBuffer, int bufferSize) {
  int readingChunk = 0;
  int prevColonIndex = 0;

  for (int index = 0; index < bufferSize; index++) {
    if (fileBuffer[index] == ':') {
      int* coordinates = readCoordinates(prevColonIndex, index, fileBuffer);

      if (coordinates != NULL) {
        int coord_quantity = (index - prevColonIndex + 2) / 4;
        createEntities(readingChunk, coordinates, coord_quantity);
      }

      prevColonIndex = index + 1;
      readingChunk++;
    }
  }
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
int* readCoordinates(int begin, int end, char* fileBuffer) { // pure function
  // No globals
  if (fileBuffer[0] == 'N')
    return NULL; // no info flag

  int quantity = (end - begin + 2) / 4;  // quantity of coordinates
  int* coordinates = new int[quantity];
  int charsPerNumber = 3; // 3 characters left for number, then comma

  for (int i = 0; i < quantity; i++) {
    int offset = begin + i * (charsPerNumber + 1);

    int firstDigit = fileBuffer[offset] - '0'; // convert to int
    int secondDigit = fileBuffer[offset + 1] - '0';
    int thirdDigit = fileBuffer[offset + 2] - '0';

    coordinates[i] = firstDigit * 100 + secondDigit * 10 + thirdDigit;
  }

  return coordinates;
}
void createEntities(int readingChunk, int* coordinates, int coordQuantity) {
  // Global: pressurePlate, chest, bonusEntities, fireEntities
  switch (readingChunk) {
    case 0: // Load player coordinates
      player->x = coordinates[0];
      player->y = coordinates[1];
      break;
    
    case 1:
      for (int i = 0; i < coordQuantity / 2; i++)
        fireEntities[i] = new Fire(coordinates[i * 2], coordinates[i * 2 + 1]);
      break;

    case 2:
      for (int i = 0; i < coordQuantity / 2; i++)
        bookEntities[i] = new Book(coordinates[i * 2], coordinates[i * 2 + 1]);
      break;

    case 3:
      for (int i = 0; i < coordQuantity / 3; i++) // TODO: this should be 2 and not 3
        bonusEntities[i] = new Bonus(coordinates[i * 2], coordinates[i * 2 + 1]);
      break;

    case 4:
      for (int i = 0; i < coordQuantity / 4; i++) // ignore the last two coordinates, we load the answer from the questions folder
        chest[i] = new Chest(coordinates[i * 4], coordinates[i * 4 + 1]);
      break;

    case 5: 
      for (int i = 0; i < coordQuantity / 2; i++)
        pressurePlate[i] = new PressurePlate(coordinates[i * 2], coordinates[i * 2 + 1]);
      break;

    case 6: // Next mission position
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
      player->gameMap[i][j] = gameMap[i][j];
}
void loadDoors() {
  // Global: gameMap, doorEntities, finalDoor, movingStairBlocks, blockSize
  // moving block
  int movingBlockCount = 0;

  for (int i = 0; i < blocksInHeight; i++) {
    for (int j = 0; j < blocksInWidth; j++) {
      switch (gameMap[i][j]) {
        case DOOR_ID: // Load door
          if (gameMap[i - 1][j] == DOOR_ID) continue;
          
          doorEntities[Door::counter - 1] = new Door(j * blockSize, i * blockSize); 
          break;

        case FINAL_DOOR_ID: // Load final doors
          if (gameMap[i - 1][j] == FINAL_DOOR_ID) continue;

          int door_type = (j == 0) ? 1 : 0;

          finalDoor[FinalDoor::counter - 1] = new FinalDoor(j * blockSize, i * blockSize, door_type);
          break;

        case MOVING_BLOCK_ID: // Load moving blocks
          int pi = 0;
          if (movingBlockCount == 0) pi = (i - 1) * blockSize;

          movingStairBlocks[movingBlockCount] = new BlockMoves(j * blockSize, i * blockSize, pi);

          movingBlockCount++;
          break;
      }
    }
  }
}
// ---------------------------------------------------------------------------------
// readScript {end}
// ---------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------
// playLoop function
// ---------------------------------------------------------------------------------
void playLoop(int cursorX, int cursorY, int clickedX, int clickedY) { // ★★★
  // Global: playMode, missionMode,isBookMenuOpen, isPaused
  if (!playMode) return;
  if (missionMode) {
    mission(cursorX, cursorY, clickedX, clickedY);
    return;
  }

  if (player->didReachExitDoor()) {
    loadNextLevel();
  }

  if (!isBookMenuOpen && !isPaused) {
    interactiveObjects(cursorX, cursorY, clickedX, clickedY);
    playerEvents();
  }

  closeTheBook();
  menuPause(cursorX, cursorY, clickedX, clickedY);
}
void mission(int cursorX, int cursorY, int clickedX, int clickedY) {
  // Global: backButton, missionMode, playMode, missionButtons, player, map, gameMap
  // External: isInitialState

  // Exit+++
  if (backButton->contains(clickedX, clickedY)) {
    missionMode = false;
    playMode = false;
    isInitialState = true;
  }

  backButton->show = backButton->contains(cursorX, cursorY);
  // Exit---

  if (missionButtons[0]->contains(clickedX, clickedY)) {
    player = new Player(); // TODO: pass x & y into Player
    player->x = 40;
    player->y = 40;

    readScript(mapFilename);

    missionMode = false;
  }

  missionButtons[0]->show = missionButtons[0]->contains(cursorX, cursorY);
}
void loadNextLevel() { // nextLevel: loadNextLevel
  // Global: player, level, inventory, mapFilename
  // External: 
  level++;

  for (int i = 0; i < 9; i++)
    if (inventory->cells[i] == INV_KEY) inventory->cells[i] = INV_EMPTY_CELL;

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
  PressurePlate::counter = 0;
  FinalDoor::counter = 0;
}
void resetEntities() {
  for (int i = 0; i < 10; i++) {
    fireEntities[i] = NULL;
    doorEntities[i] = NULL;
    bookEntities[i] = NULL;
    bonusEntities[i] = NULL;
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
  // External: Player class
  player->duplicateMap(gameMap); // does not mutate parameter
  Keyboard key = keyboardMapping(buffer);

  switch (key) {
    case KEY_N:
      inventory->addItem(INV_KEY);
      break;

    case KEY_RIGHT:
    case KEY_D:
      player->isMovingRight = true;
      player->isMovingLeft = false;
      break;

    case KEY_LEFT:
    case KEY_A:
      player->isMovingLeft = true;
      player->isMovingRight = false;
      break;

    case KEY_SPACE:
      int leftPlayerSide = player->x / blockSize;
      int rightPlayerSide = (player->x + player->w) / blockSize;
      int bottom = (player->y + player->h) / blockSize;

      if (!(gameMap[bottom][leftPlayerSide] == AIR_ID && gameMap[bottom][rightPlayerSide] == AIR_ID)) {
        player->isJumping = true;
        player->jumpVelocity = 15;
      }
      break;
    
    case KEY_UP:
    case KEY_W:
      player->isClimbingUp = true;
      player->isClimbingDown = false;
      break;

    case KEY_DOWN:
    case KEY_S:
      player->isClimbingDown = true;
      player->isClimbingUp = false;
      break;
  }

  if (player->shouldUpdateVerticalPosition()) {
    player->jump();
    player->gravity();
  }

  if (player->shouldUpdatePlayerActions()) {
    player->moveLeft();
    player->moveRight();
    player->moveUpLadder();
    player->moveDownLadder();

    int leftPlayerSide = player->x / blockSize;
    int bottom = (player->y + player->h) / blockSize;

    player->canFall = gameMap[bottom][leftPlayerSide] != LADDER_ID;
  }
}

void closeTheBook() {
  Keyboard key = keyboardMapping(buffer);
  if (key != KEY_RETURN) return;

  for (int i = 0; i < Book::counter; i++) {
    if (bookEntities[i]->isOpen) {
      bookEntities[i]->show = false;
      isBookMenuOpen = false;
      
      bookEntities[i]->closeBook();
    }
  }
}
void menuPause(int cursorX, int cursorY, int clickedX, int clickedY) {
  // Global: buffer, isPaused, pauseMenuButtons, playMode
  // External: isInitialState
  Keyboard key = keyboardMapping(buffer);
  if (key == KEY_ESCAPE) isPaused = true;
  if (!isPaused) return;

  for (int i = 0; i < 4; i++) {
    pauseMenuButtons[i]->show = pauseMenuButtons[i]->contains(cursorX, cursorY);
  }

  isPaused = shouldContinuePause(clickedX, clickedY);

  if (pauseMenuButtons[PM_EXIT]->contains(clickedX, clickedY)) {
    playMode = false;
    isInitialState = true;
  }
}
bool shouldContinuePause(int clickedX, int clickedY) {
  return pauseMenuButtons[PM_CONTINUE]->contains(clickedX, clickedY) == false 
  && pauseMenuButtons[PM_SAVE]->contains(clickedX, clickedY) == false 
  && pauseMenuButtons[PM_SETTINGS]->contains(clickedX, clickedY) == false;
}
// ---------------------------------------------------------------------------------
// playLoop {end}
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// interactiveObjects functions
// ---------------------------------------------------------------------------------
void interactiveObjects(int cursorX, int cursorY, int clickedX, int clickedY) {
  // Global: inventory, chest, player, buffer, bookEntities, isBookMenuOpen, 
  // External: didClickLeftButton, screenPixelWidth, screenPixelHeight
  // Class: Chest class, Book class

  didPlayerTouchBonus();
  inventory->showTooltip = inventory->contains(cursorX, cursorY); // show or hide tool tip

  if (inventory->contains(clickedX, clickedY)) inventory->show = true;

  // clicked showing result = clicked || showing 
  // T        T       T
  // T        F       T 
  // F        T       T
  // F        F       F

  inventoryMoveEvents(clickedX, clickeY);
  chestMoveEvents();

  //++++++
  Keyboard key = keyboardMapping(buffer);
  
  if (key == KEY_E) inventory->show = !inventory->show;
  //-----

  // Chest+++
  for (int i = 0; i < Chest::counter; i++) {
    chest[i]->openLock(inventory, didClickLeftButton, clickedX, clickedY, cursorX, cursorY); // mutates didClickLeftButton

    showChestToolTip(i, cursorX, cursorY);
  }
  // Chest---

  updateFrames(clickedX, clickedY);

  // BOOKS+++
  for (int i = 0; i < Book::counter; i++) {
    if (bookEntities[i]->isOpen) continue;
    if (!bookEntities[i]->show) continue;

    bool bottomLeft = bookEntities[i]->contains(player->x, player->y + player->currentFrame->height);
    bool bottomRight = bookEntities[i]->contains(player->x + player->currentFrame->width, player->y + player->currentFrame->height);

    if (bottomLeft || bottomRight) {
      bookEntities[i]->openBook();
      inventory->addItem(INV_BOOK);
      isBookMenuOpen = true;
    }
  }
  // BOOKS---

  // CHEST+++
  if (key == KEY_O) {
    for (int i = 0; i < Chest::counter; i++) {
      bool bottomLeft = chest[i]->contains(player->x, player->y + player->h - 1);
      bool bottomRight = chest[i]->contains(player->x + player->w, player->y + player->h - 1);

      if (bottomLeft || bottomRight)
        chest[i]->isOpen = true;
    }
  }
  // CHEST---
}
void didPlayerTouchBonus() {
  // Checks if player intersected with bonus entity.
  // if intersected, then increase score and hide bonus entity
  for (int i = 0; i < Bonus::counter; i++) {
    if (!bonusEntities[i]->show) continue;

    bool bottomLeft = bonusEntities[i]->contains(player->x, player->y + player->h - 1);
    bool bottomRight = bonusEntities[i]->contains(player->x + player->w, player->y + player->h - 1);

    if (bottomLeft || bottomRight) {
      score += 10;

      char* sc = new char[100];
      sprintf(sc, "score:%d", score);

      scoreText->changeText(sc); // does not mutate parameter
      bonusEntities[i]->show = false;
    }
  }
}
void inventoryMoveEvents(int clickedX, int clickedY) {
  if (!didClickLeftButton) return;
  if (!inventory->containsInOpenInventory(clickedX, clickedY) return;

  // left clicked open inventory
  int inventoryCell = inventory->touchedCellIndex(clickedX, clickedY);
  
  if (inventory->move) {
    if (inventoryCell != -1 && inventory->selectedCell != inventoryCell && inventory->cells[inventoryCell] == INV_EMPTY_CELL) {
      inventory->cells[inventoryCell] = selectedObjectId;
      inventory->updateCellSprites();
      selectedObjectId = INV_EMPTY_CELL;
    }
    inventory->move = false;
  } else {
    inventory->selectedCell = inventoryCell;

    if (inventoryCell != -1) {
      inventory->move = true;
      selectedObjectId = inventory->cells[inventoryCell];
      inventory->cells[inventoryCell] = INV_EMPTY_CELL;
    }
  }

  for (int i = 0; i < Chest::counter; i++) chest[i]->move = !inventory->move;
}
void chestMoveEvents() {
  for (int i = 0; i < Chest::counter; i++) {
    bool clickedChest = chest[i]->isWithinInventory(Chest::Xi, Chest::Yi);

    if (!(clickedChest && chest[i]->isChestUnlocked && Chest::iLmb)) {
      continue;
    }

    int selectedChestCell = chest[i]->selectedChestCell(Chest::Xi, Chest::Yi);

    if (chest[i]->move) {
      if (selectedChestCell != -1 && chest[i]->check != selectedChestCell && chest[i]->items[selectedChestCell] == i) {
        chest[i]->items[selectedChestCell] = selectedObjectId;
        chest[i]->updateChestCells();
        selectedObjectId = i;
      }

      chest[i]->move = false;
      inventory->move = false;
    } else {
      chest[i]->check = selectedChestCell;

      if (selectedChestCell != -1) {
        chest[i]->move = true;
        inventory->move = true;
        selectedObjectId = chest[i]->items[selectedChestCell];
        chest[i]->items[selectedChestCell] = INV_EMPTY_CELL;
      }
    }

    Chest::Xi = i;
    Chest::Yi = i;
    Chest::iLmb = false;
  }
}
void showChestToolTip(int i, int mouseX, int mouseY) {
  chest[i]->showToolTip = chest[i]->contains(mouseX, mouseY);
  chest[i]->mX1 = mouseX + 10;
  chest[i]->mY1 = mouseY;
}
void updateFrames(int clickedX, int clickedY) {
  // Global: fireEntities, bonusEntities, doorEntities, gameMap, finalDoor, inventory, pressurePlate, player, movingStairBlocks
  // External: didClickLeftButton
  // Classes: Fire, Bonus, Door, FinalDoor, PressurePlate, BlockMoves

  // FIRE---
  // update frame every 50ms
  for (int i = 0; i < Fire::counter; i++) {
    fireEntities[i]->updateFrame();
  }
  // FIRE+++

  // BONUS---
  for (int i = 0; i < Bonus::counter; i++) {
    bonusEntities[i]->udpateFrame();
  }
  // BONUS+++

  // DOOR+++
  for (int i = 0; i < Door::counter; i++) {
    if (didClickRightButton && doorEntities[i]->contains(clickedX, clickedY)) {
      updateFrame(gameMap);
      didClickRightButton = false;
    }
  }
  // DOOR---

  // FinalDoor+++
  bool isHoldingKey = inventory->cells[0] == 2;

  for (int i = 0; i < FinalDoor::counter; i++) {
    if (didClickRightButton && finalDoor[i]->contains(clickedX, clickedY)) {
      if (isHoldingKey) {
        finalDoor[i]->updateFrame(gameMap);
        didClickRightButton = false;
      } else {
        finalDoor[i]->needsKey = true;
      }
    }

    if (!finalDoor[i]->contains(clickedX, clickedY)) {
      finalDoor[i]->needsKey = false;
    }
  }
  // FinalDoor---

  for (int i = 0; i < PressurePlate::counter; i++) {
    if (pressurePlate[i]->contains(player->x, player->y + player->h - 1))
      BlockMoves::UP = true;
  }

  // Block Moves+++
  BlockMoves::Timer();

  if (BlockMoves::dt > 5 && BlockMoves::UP) {
    bool b = true;

    for (int i = 0; i < BlockMoves::counter; i++) {
      bool tmp = movingStairBlocks[i]->BlockMoveUp(gameMap); // mutates parameter

      if (!tmp) b = false;
    }
    /// tmp b  A = tmp & b
    /// T   T  T
    /// T   F  F
    /// F   T  F
    /// F   F  F

    if (b) BlockMoves::timer1 = 0;
  }

  if (BlockMoves::dt > 1000 * 10) BlockMoves::UP = false;

  if (BlockMoves::dt > 5 && !BlockMoves::UP) {
    for (int i = 0; i < BlockMoves::counter; i++) movingStairBlocks[i]->BlockMoveDown(gameMap); // mutates parameter
  }
  // Block Moves---
}
// ---------------------------------------------------------------------------------
// interactiveObjects {end}
// ---------------------------------------------------------------------------------

enum Keyboard {
  KEY_A,
  KEY_E,
  KEY_D,
  KEY_N,
  KEY_O,
  KEY_S,
  KEY_W,
  KEY_SPACE,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_RETURN,
  KEY_ESCAPE
};

Keyboard keyboardMapping(char* buffer) {
  if (buffer[DIK_A] & 0x80) return Keyboard::KEY_A;
  if (buffer[DIK_E] & 0x80) return Keyboard::KEY_E;
  if (buffer[DIK_D] & 0x80) return Keyboard::KEY_D;
  if (buffer[DIK_N] & 0x80) return Keyboard::KEY_N;
  if (buffer[DIK_O] & 0x80) return Keyboard::KEY_O;
  if (buffer[DIK_S] & 0x80) return Keyboard::KEY_S;
  if (buffer[DIK_W] & 0x80) return Keyboard::KEY_W;

  if (buffer[DIK_SPACE] & 0x80) return Keyboard::KEY_SPACE;
  if (buffer[DIK_UP] & 0x80) return Keyboard::KEY_UP;
  if (buffer[DIK_DOWN] & 0x80) return Keyboard::KEY_DOWN;
  if (buffer[DIK_LEFT] & 0x80) return Keyboard::KEY_LEFT;
  if (buffer[DIK_RIGHT] & 0x80) return Keyboard::KEY_RIGHT;

  if (buffer[DIK_RETURN] & 0x80) return Keyboard::KEY_RETURN;
  if (buffer[DIK_ESCAPE] & 0x80) return Keyboard::KEY_ESCAPE;
}