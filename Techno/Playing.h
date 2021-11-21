bool playMode = false; // Play: playMode

// Missions+++
bool missionMode = false; // Missions: missionMode

char* mapFilename = "Images/Data/map1.txt";
char* mapBMPfilename = "Images/Data/map1.bmp";

Sprite* missions;
Button* missionButton; // btnOfMissions: missionButtons: missionButton
Sprite* missionLock; // blocked: missionLock
Button* backButton; // Back: backButton
// Missions---

// Game+++
using Map = int[blocksInHeight][blocksInWidth];

Map gameMap; // MatMap: gameMap
int level = 1;
const int blockSize = 20;

#define AIR_ID 0
#define LADDER_ID 3
#define CLOSED_DOOR_ID 4
#define OPEN_DOOR_ID 5
#define MOVING_BLOCK_ID 6
#define CLOSED_FINAL_DOOR_ID 8
#define OPEN_FINAL_DOOR_ID 9

// Inventory objects
#define INV_EMPTY_CELL 0
#define INV_BOOK 1 // Images/o1.bmp
#define INV_KEY 2 // Images/o2.bmp

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
MovingBlock* movingStairBlocks[5]; // bm: movingStairBlocks
PressurePlate* pressurePlate[10]; // bt: pressurePlate
FinalDoor* finalDoor[10]; // Fd: finalDoor
// Objects in the game---

Inventory* inventory; // Inv: inventory

// Move Invent Chest+++
int selectedObjectId = INV_EMPTY_CELL; // posObject: selectedObjectId
// Move Invent Chest---

Text* scoreText; // txt: scoreText
int score = 0;

bool shouldMoveStairsUp = false;

// Globals from main.cpp
// -----
// globalCanvas - global buffer sprites are rendered to
// blocksInHeight, blocksInWidth - 
// screenPixelWidth, screenPixelHeight - 
// mainMenuMode - 
// -----

// Dependency injected
// -----
// clickedX, clickedY - last cursor position after right/left click
// cursorX, cursorY - current position of the cursor


// ---------------------------------------------------------------------------------
// drawScene function
// ---------------------------------------------------------------------------------
void drawScene(int cursorX, int cursorY) { // ★★★
  // Global: isPaused, scoreText
  // External: globalCanvas
  drawEntities(cursorX, cursorY);

  if (isPaused) {
    drawPauseMenu();
  }

  scoreText->draw(globalCanvas);
}
void drawEntities(int cursorX, int cursorY) {
  // Global: fireEntities, doorEntities, bonusEntities, movingStairBlocks, chest, player, inventory, bookEntities, pauseOverlay
  // selectedObjectId
  // External: globalCanvas, screenPixelWidth, screenPixelHeight
  globalCanvas->draw(0, 0, screenPixelWidth, screenPixelHeight, map);

  for (int i = 0; i < Fire::counter; i++) {
    fireEntities[i]->draw(globalCanvas);
  }

  for (int i = 0; i < Door::counter; i++) {
    doorEntities[i]->draw(globalCanvas);
  }

  for (int i = 0; i < FinalDoor::counter; i++) {
    finalDoor[i]->draw(globalCanvas);
  }

  for (int i = 0; i < Bonus::counter; i++) {
    if (!bonusEntities[i]->show) continue;

    bonusEntities[i]->draw(globalCanvas);
  }

  for (int i = 0; i < MovingBlock::counter; i++) {
    movingStairBlocks[i]->draw(globalCanvas);
  }

  // Chest++++
  player->draw(globalCanvas);

  for (int i = 0; i < Chest::counter; i++) {
    chest[i]->draw(globalCanvas);
    chest[i]->drawTooltip(globalCanvas, cursorX, cursorY);

    if (!chest[i]->isOpen) continue;

    globalCanvas->draw(0, 0, pauseOverlay->width, pauseOverlay->height,
                    pauseOverlay);
    chest[i]->drawOpenChest(globalCanvas); // does not mutate parameter
  }
  // Chest---

  inventory->draw(globalCanvas, cursorX, cursorY);

  // Book++++
  for (int i = 0; i < Book::counter; i++) {
    if (!bookEntities[i]->show) continue;

    if (bookEntities[i]->isOpen) {
      globalCanvas->draw(0, 0, pauseOverlay->width, pauseOverlay->height, pauseOverlay);
    }

    bookEntities[i]->draw(globalCanvas);
  }
  // Book---

  updateFrames();

  // Cursor
  if (selectedObjectId != INV_EMPTY_CELL) {
    Sprite* cursorIcon = item_assets[selectedObjectId];

    globalCanvas->draw(cursorX, cursorY, cursorIcon->width, cursorIcon->height, cursorIcon);
  }
}
void updateFrames() {
  // Global: fireEntities, bonusEntities, gameMap, movingStairBlocks
  // Classes: Fire, Bonus, MovingBlock

  // Fire
  for (int i = 0; i < Fire::counter; i++) {
    fireEntities[i]->updateFrame();
  }

  // Bonus
  for (int i = 0; i < Bonus::counter; i++) {
    bonusEntities[i]->udpateFrame();
  }

  // Block Moves
  for (int i = 0; i < MovingBlock::counter; i++) {
    if (shouldMoveStairsUp) {
      movingStairBlocks[i]->moveUp(gameMap);  // mutates parameter
    } else {
      movingStairBlocks[i]->moveDown(gameMap);  // mutates parameter
    }
  }
}
void drawPauseMenu() {
  // Global: pauseOverlay, pauseMenuSprite, pauseMenuButtons
  // External: globalCanvas, screenPixelWidth, screenPixelHeight
  int leftCentered = (screenPixelWidth - pauseMenuSprite->width) / 2;
  int topCentered = (screenPixelHeight - pauseMenuSprite->height) / 2;
  
  globalCanvas->draw(0, 0, pauseOverlay->width, pauseOverlay->height,
                  pauseOverlay);
  globalCanvas->draw(leftCentered, topCentered, pauseMenuSprite->width,
                  pauseMenuSprite->height, pauseMenuSprite);

  for (int i = 0; i < 4; i++) {
    pauseMenuButtons[i]->draw(globalCanvas);
  }
}
// ---------------------------------------------------------------------------------
// drawScene {end}
// ---------------------------------------------------------------------------------

void drawMission() { // ★★★
  // Global: missions, missionButton, missionLock, backButton
  // External: globalCanvas, screenPixelWidth, screenPixelHeight
  globalCanvas->draw(0, 0, screenPixelWidth, screenPixelHeight, missions);

  if (missionButton->show) missionButton->draw(globalCanvas);

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

      globalCanvas->draw(x, y, missionLock->width, missionLock->height,
                      missionLock);
    }
  }

  if (backButton->show) backButton->draw(globalCanvas);
}

// ---------------------------------------------------------------------------------
// readScript function
// ---------------------------------------------------------------------------------
void readScript(char* filename) {
  // Global: mapFilename
  // External: blocksInHeight, blocksInWidth
  int bufferSize = blocksInHeight * blocksInWidth + 100;
  char* fileBuffer = readFile(filename, bufferSize);

  readEntities(fileBuffer, bufferSize);
  int mapDeliminatorPos = findDelimiter(fileBuffer, bufferSize);
  loadMap(fileBuffer, mapDeliminatorPos);
  setMap();
  loadDoors();

  for (int i = 0; i < Chest::counter; i++) chest[i]->loadQuestion(level);
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
    case 0:
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

    case 6:
      player->setExit(coordinates[0], coordinates[1]);
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
  map = new Sprite(mapBMPfilename, WHITE);
}
void loadDoors() {
  // Global: gameMap, doorEntities, finalDoor, movingStairBlocks, blockSize
  // moving block
  int movingBlockCount = 0;
  int finalYposition = 0;

  for (int i = 0; i < blocksInHeight; i++) {
    for (int j = 0; j < blocksInWidth; j++) {
      switch (gameMap[i][j]) {
        case CLOSED_DOOR_ID: // Load door
          if (gameMap[i - 1][j] == CLOSED_DOOR_ID) continue;
          
          doorEntities[Door::counter - 1] = new Door(j * blockSize, i * blockSize); 
          break;

        case CLOSED_FINAL_DOOR_ID: // Load final doors
          if (gameMap[i - 1][j] == CLOSED_FINAL_DOOR_ID) continue;

          finalDoor[FinalDoor::counter - 1] = new FinalDoor(j * blockSize, i * blockSize, j != 0);
          break;

        case MOVING_BLOCK_ID: // Load moving blocks
          if (movingBlockCount == 0) finalYposition = (i - 1) * blockSize - 1;
          
          movingStairBlocks[movingBlockCount] = new MovingBlock(j * blockSize, i * blockSize - 1, finalYposition);

          movingBlockCount++;
          break;
      }
    }
  }
}
// ---------------------------------------------------------------------------------
// readScript {end}
// ---------------------------------------------------------------------------------

void mission(int cursorX, int cursorY, int clickedX, int clickedY) {  // ★★★
  // Global: backButton, missionMode, playMode, missionButton, player, map, gameMap
  // External: mainMenuMode

  // Exit+++
  if (backButton->contains(clickedX, clickedY)) {
    missionMode = false;
    playMode = false;
    mainMenuMode = true;
  }

  backButton->show = backButton->contains(cursorX, cursorY);
  // Exit---

  if (missionButton->contains(clickedX, clickedY)) {
    player = new Player(40, 40);

    readScript(mapFilename);

    missionMode = false;
  }

  missionButton->show = missionButton->contains(cursorX, cursorY);
}

// ---------------------------------------------------------------------------------
// playLoop function
// ---------------------------------------------------------------------------------
void playLoop(int cursorX, int cursorY, int clickedX, int clickedY) { // ★★★
  // Global: isBookMenuOpen, isPaused
  keyboardEvents();

  if (player->didReachExitDoor()) {
    loadNextLevel();
  }

  if (!isBookMenuOpen && !isPaused) {
    interactiveObjects(cursorX, cursorY, clickedX, clickedY);
    playerEvents();
  }

  if (isPaused) {
    pauseMenuInteractions(cursorX, cursorY, clickedX, clickedY);
  }
}
void loadNextLevel() { // nextLevel: loadNextLevel
  // Global: player, level, inventory, mapFilename
  // External: 
  level++;

  inventory->clearKeyFromInventory();
  clearClassInformation();
  resetEntities();

  setNextMapFilepath(level);
  readScript(mapFilename);
}
void clearClassInformation() {
  // TODO: remove counters for vector
  Fire::counter = 0;
  Door::counter = 0;
  Book::counter = 0;
  Bonus::counter = 0;
  Chest::counter = 0;
  MovingBlock::counter = 0;
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
void setNextMapFilepath(int level) {
  mapBMPfilename = new char[100]; // global
  mapFilename = new char[100]; // global

  sprintf(mapBMPfilename, "Images/Data/map%d.bmp", level);
  sprintf(mapFilename, "Images/Data/map%d.txt", level);
}
void playerEvents() {
  // Global: player, gameMap, buffer, inventory
  // External: Player class
  if (player->shouldUpdateVerticalPosition()) {
    player->jump(gameMap);
    player->gravity(gameMap);
  }

  if (player->shouldUpdatePlayerActions()) {
    player->moveLeft(gameMap);
    player->moveRight(gameMap);
    player->moveUpLadder(gameMap);
    player->moveDownLadder(gameMap);

    int leftPlayerSide = player->x / blockSize;
    int bottom = (player->y + player->height) / blockSize;

    player->canFall = gameMap[bottom][leftPlayerSide] != LADDER_ID;
  }
}
void keyboardEvents() {
  Keyboard key = keyboardMapping(buffer);

  switch (key) {
    case KEY_E:
      if (isBookMenuOpen || isPaused) return;

      inventory->show = !inventory->show;
      break;
    
    case KEY_O:
      if (isBookMenuOpen || isPaused) return;

      for (int i = 0; i < Chest::counter; i++) {
        bool bottomLeft = chest[i]->contains(player->x, player->y + player->height - 1);
        bool bottomRight = chest[i]->contains(player->x + player->width, player->y + player->height - 1);

        if (bottomLeft || bottomRight) chest[i]->isOpen = true;
      }
      break;

    case KEY_RETURN:
      if (!isBookMenuOpen) return;

      for (int i = 0; i < Book::counter; i++) {
        if (!bookEntities[i]->isOpen) continue;
        
        bookEntities[i]->show = false;
        isBookMenuOpen = false;

        bookEntities[i]->closeBook();
      }
      break;

    case KEY_ESCAPE:
      isPaused = true;
      break;

    case KEY_N:
      inventory->addItem(INV_KEY);
      break;

    case KEY_RIGHT:
    case KEY_D:
      if (isBookMenuOpen || isPaused) return;

      player->isMovingRight = true;
      player->isMovingLeft = false;
      break;

    case KEY_LEFT:
    case KEY_A:
      if (isBookMenuOpen || isPaused) return;

      player->isMovingLeft = true;
      player->isMovingRight = false;
      break;

    case KEY_SPACE:
      if (isBookMenuOpen || isPaused) return;

      int leftPlayerSide = player->x / blockSize;
      int rightPlayerSide = (player->x + player->width) / blockSize;
      int bottom = (player->y + player->height) / blockSize;

      if (!(gameMap[bottom][leftPlayerSide] == AIR_ID && gameMap[bottom][rightPlayerSide] == AIR_ID)) {
        player->isJumping = true;
        player->jumpVelocity = 15;
      }
      break;
    
    case KEY_UP:
    case KEY_W:
      if (isBookMenuOpen || isPaused) return;

      player->isClimbingUp = true;
      player->isClimbingDown = false;
      break;

    case KEY_DOWN:
    case KEY_S:
      if (isBookMenuOpen || isPaused) return;

      player->isClimbingDown = true;
      player->isClimbingUp = false;
      break;
  }
}

void pauseMenuInteractions(int cursorX, int cursorY, int clickedX, int clickedY) { // menuPause: pauseMenuInteractions
  // Global: isPaused, pauseMenuButtons, playMode
  // External: mainMenuMode
  for (int i = 0; i < 4; i++) {
    pauseMenuButtons[i]->show = pauseMenuButtons[i]->contains(cursorX, cursorY);
  }

  isPaused = shouldContinuePause(clickedX, clickedY);

  if (pauseMenuButtons[PM_EXIT]->contains(clickedX, clickedY)) {
    playMode = false;
    mainMenuMode = true;
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
  // Global: inventory
  inventory->showTooltip = inventory->contains(cursorX, cursorY); // show or hide tool tip
  if (inventory->contains(clickedX, clickedY)) inventory->show = true;

  didPlayerTouchBonus();
  didPlayerPickupBook();
  didTouchPressurePlate();
  
  if (didClickLeftButton) {
    inventoryMoveEvents(clickedX, clickedY);
    chestMoveEvents(clickedX, clickedY);
  }

  if (didClickRightButton) {
    doorInteractions(clickedX, clickedY);
  }

  chestLockerInteractions(cursorX, cursorY, clickedX, clickedY);
}
void chestLockerInteractions(int cursorX, int cursorY, int clickedX, int clickedY) {
  for (int i = 0; i < Chest::counter; i++) {
    if (!chest[i]->isOpen) continue;
    if (!chest[i]->isChestLocked) continue;

    chest[i]->highlightLock(cursorX, cursorY);

    if (didClickLeftButton) {
      chest[i]->shouldCloseLockerView(inventory, clickedX, clickedY);
      chest[i]->goToNextDigit(clickedX, clickedY);
      chest[i]->openLock(clickedX, clickedY);
    }

    chest[i]->verifyCombination();
  }
}
void didPlayerPickupBook() {
  for (int i = 0; i < Book::counter; i++) {
    if (bookEntities[i]->isOpen) continue;
    if (!bookEntities[i]->show) continue;

    bool bottomLeft = bookEntities[i]->contains(player->x, player->y + player->height);
    bool bottomRight = bookEntities[i]->contains(player->x + player->width, player->y + player->height);

    if (bottomLeft || bottomRight) {
      bookEntities[i]->openBook();
      inventory->addItem(INV_BOOK);
      isBookMenuOpen = true;
    }
  }
}
void didPlayerTouchBonus() {
  // Checks if player intersected with bonus entity.
  // if intersected, then increase score and hide bonus entity
  for (int i = 0; i < Bonus::counter; i++) {
    if (!bonusEntities[i]->show) continue;

    bool bottomLeft = bonusEntities[i]->contains(player->x, player->y + player->height - 1);
    bool bottomRight = bonusEntities[i]->contains(player->x + player->width, player->y + player->height - 1);

    if (bottomLeft || bottomRight) {
      score += 10;
      bonusEntities[i]->show = false;

      char* sc = new char[100];
      sprintf(sc, "score:%d", score);

      scoreText->changeText(sc); // does not mutate parameter
    }
  }
}
void inventoryMoveEvents(int clickedX, int clickedY) {
  if (!inventory->containsInOpenInventory(clickedX, clickedY) return;

  // left clicked open inventory
  int inventoryCell = inventory->touchedCellIndex(clickedX, clickedY);
  
  if (inventory->move) { // Attempt to put a selected item into the clicked cell
    bool didSelectCell = inventoryCell != NULL;
    bool movingToADifferentCell = inventory->selectedCell != inventoryCell;
    bool targetCellEmpty = didSelectCell && inventory->cells[inventoryCell] == INV_EMPTY_CELL;

    if (didSelectCell &&  movingToADifferentCell && targetCellEmpty) {
      inventory->cells[inventoryCell] = selectedObjectId;
      inventory->updateCellSprites();
      selectedObjectId = INV_EMPTY_CELL;
    }
    inventory->move = false;
  } else { // Attempt to pickup an item from a clicked cell
    if (inventoryCell != NULL) {
      inventory->selectedCell = inventoryCell;
      selectedObjectId = inventory->cells[inventoryCell];
      inventory->cells[inventoryCell] = INV_EMPTY_CELL;

      inventory->move = true;
    }
  }

  for (int i = 0; i < Chest::counter; i++) chest[i]->move = !inventory->move;
}
void chestMoveEvents(int clickedX, int clickedY) {
  for (int i = 0; i < Chest::counter; i++) {
    if (!chest[i]->isWithinInventory(clickedX, clickedY)) continue;
    if (chest[i]->isChestLocked) continue;

    int selectedChestCell = chest[i]->selectedChestCell(clickedX, clickedY);

    if (chest[i]->move) { // Attempt to put a selected item into the clicked cell
      bool didSelectCell = selectedChestCell != NULL;
      bool movingToADifferentCell = chest[i]->selectedChestCell != selectedChestCell;
      bool targetCellEmpty = didSelectCell && chest[i]->items[selectedChestCell] == INV_EMPTY_CELL;
    
      if (didSelectCell && movingToADifferentCell && targetCellEmpty) {
        chest[i]->items[selectedChestCell] = selectedObjectId;
        chest[i]->updateChestCells();
        selectedObjectId = i;
      }

      chest[i]->move = false;
      inventory->move = false;
    } else { // Attempt to pickup an item from a clicked cell
      if (selectedChestCell != NULL) {
        chest[i]->selectedChestCell = selectedChestCell;
        chest[i]->move = true;
        inventory->move = true;
        selectedObjectId = chest[i]->items[selectedChestCell];
        chest[i]->items[selectedChestCell] = INV_EMPTY_CELL;
      }
    }
  }
}
void didTouchPressurePlate() {
  for (int i = 0; i < PressurePlate::counter; i++) {
    if (pressurePlate[i]->contains(player->x, player->y + player->height - 1))
      shouldMoveStairsUp = true;
  }
}
void doorInteractions(int clickedX, int clickedY) {
  // Door
  for (int i = 0; i < Door::counter; i++) {
    if (!doorEntities[i]->contains(clickedX, clickedY)) continue;

    doorEntities[i]->updateFrame();
    doorEntities[i]->switchDoorState(gameMap); // mutates parameter
  }

  // FinalDoor
  bool isHoldingKey = inventory->cells[0] == INV_KEY;

  for (int i = 0; i < FinalDoor::counter; i++) {
    if (finalDoor[i]->contains(clickedX, clickedY)) {
      if (isHoldingKey) {
        finalDoor[i]->updateFrame();
        finalDoor[i]->switchDoorState(gameMap);
      } else {
        finalDoor[i]->needsKey = true;
      }
    } else {
      finalDoor[i]->needsKey = false;
    }
  }
}
// ---------------------------------------------------------------------------------
// interactiveObjects {end}
// ---------------------------------------------------------------------------------

enum Keyboard {
  KEY_A,
  KEY_E,
  KEY_H,
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
  if (buffer[DIK_H] & 0x80) return Keyboard::KEY_H;
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

  return NULL;
}