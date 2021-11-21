// Create map+++
bool createMapMode = false;  // Create_map: createMapMode
bool useTools = false;       // UseTools: useTools
bool showGrid = true;        // setka: showGrid

int firstColor = VERY_LIGHT_GRAY;  // colorFirst: firstColor
int lastColor = DARK_GRAY;         // colorLast: lastColor

int forgegroundMap[blocksInHeight][blocksInWidth]; // CMap: forgegroundMap
int backgroundMap[blocksInHeight][blocksInWidth]; // CMapBack: backgroundMap

// TOOLS
enum Tool { POINT_TOOL, LINE_TOOL, RECTANGLE_TOOL };

// MODE
enum BlockMode {
  DELETE_BLOCK_MODE,
  SOLID_BLOCK_MODE,
  BACKGROUND_BLOCK_MODE,
  STAIRS_MODE,
  SMALL_SQUARE_MODE
};

BlockMode blockMode = DELETE_BLOCK_MODE; // TypeOFBlock: blockMode
Tool toolSelected = POINT_TOOL; // TypeOFAdd: toolSelected
bool secondInteration = false; // first: secondInteration
int fineMap[blocksInHeight * 2][blocksInWidth * 2]; // MiniMap: fineMap

// Panels
#define WHITE_BKG_IMG "Images/create_map/white.bmp"
#define MAP_GRID_IMG "Images/create_map/map_grid.bmp"
#define COLOR_PALETTE_IMG "Images/create_map/color_palette.bmp"
#define TEXTURE_GRID_IMG "Images/create_map/backTextures.bmp"
#define MAP_TOOLS_IMG "Images/create_map/map_tools.bmp"

// Button assets
#define BUTTON_1_IMG "Images/create_map/buttons/solid_block.bmp"
#define BUTTON_2_IMG "Images/create_map/buttons/delete_block.bmp"
#define BUTTON_3_IMG "Images/create_map/buttons/background_block.bmp"
#define BUTTON_4_IMG "Images/create_map/buttons/save_map.bmp"
#define BUTTON_5_IMG "Images/create_map/buttons/small_square.bmp"
#define BUTTON_6_IMG "Images/create_map/buttons/stairs_block.bmp"

#define TOOL_BTN_1_IMG "Images/create_map/buttons/point_tool.bmp"
#define TOOL_BTN_2_IMG "Images/create_map/buttons/line_tool.bmp"
#define TOOL_BTN_3_IMG "Images/create_map/buttons/rectangle_tool.bmp"

#define TEST_MAP_BTN_IMG "Images/create_map/buttons/test_map.bmp"
#define SHOW_GRID_BTN_IMG "Images/create_map/buttons/show_grid.bmp"

// Other assets
#define RED_SQUARE_IMG "Images/create_map/redSQ.bmp"
#define BORDER_IMG "Images/create_map/border.bmp"
#define COLOR_SQUARE_IMG "Images/create_map/colorSQ.bmp"
#define RED_BORDER_IMG "Images/create_map/redSelect.bmp"
#define EMPTY_IMG "Images/create_map/empty.bmp"
#define LADDER_UP_IMG "Images/create_map/squareUP.bmp"
#define SMALL_SQUARE_IMG "Images/create_map/squareSmall.bmp"

// Textures
#define SQUARE_IMG "Images/create_map/textures/square.bmp"
#define BACK_SQUARE_IMG "Images/create_map/textures/squareBack.bmp"

char *foregroundList[] = {SQUARE_IMG, "Images/create_map/textures/squareStone.bmp",
                          "Images/create_map/textures/squareMetal.bmp", "Images/create_map/textures/squareWood.bmp"};
char *backList[] = {BACK_SQUARE_IMG, "Images/create_map/textures/squareB1.bmp",
                    "Images/create_map/textures/squareB2.bmp"};

// Buttons
#define SOLID_BLOCK 0
#define DELETE_BLOCK 1
#define BACKGROUND_BLOCK 2
#define SAVE 3
#define SMALL_SQUARE 4
#define STAIRS 5

#define BACK_LAYER 0
#define FOREGROUND_LAYER 1

char *selectedFgTexture = SQUARE_IMG; // block_material: selectedFgTexture
char *selectedBgTexture = BACK_SQUARE_IMG; // blockBack_material: selectedBgTexture

Sprite *mapGridSprite; // CMAP: mapGridSprite
Sprite *toolsPanel; // tools: toolsPanel
Sprite *square; // square: 
Sprite *redSQ; // redSQ: 
Sprite *white; // white: 

NeoElement *colorSel[2]; // colorSel: 
NeoElement *colorPalette; // colorSelector: colorPalette

Button *toolButtons[5]; // btnT: toolButtons
Button *blockShapeToolButton[3]; // btn_tool: blockShapeToolButton (~ layingShapeToolBtn?)
Button *showGridButton; // check: showGridButton
Button *foregroundTextureButton[4]; // block: foregroundTextureButton
Button *backgroundTextureButton[3]; // backBlock: backgroundTextureButton
Button *backgroundBlockPanel; // BackTextures: backgroundBlockPanel
Button *testMapButton; // btn3: testMapButton

NeoSprite *layers[2]; // Layer: layers
NeoSprite *finerMapLayer; // lay: finerMapLayer

Point previousClick; // p: previousClick

// ---------------------------------------------------------------------------------
// drawCreateMapScreen function
// ---------------------------------------------------------------------------------
void drawCreateMapScreen() {  // Draw_Create: drawCreateMapScreen ★★★
  paramDraw->draw(0, 0, white->width, white->height, white);

  // Map
  if (showGrid) paramDraw->draw(0, 0, mapGridSprite->width, mapGridSprite->height, mapGridSprite);

  paramDraw->draw(0, 0, layers[BACK_LAYER]->width, layers[BACK_LAYER]->height,
                  layers[BACK_LAYER]);
  paramDraw->draw(0, 0, finerMapLayer->width, finerMapLayer->height, finerMapLayer);
  paramDraw->draw(0, 0, layers[FOREGROUND_LAYER]->width,
                  layers[FOREGROUND_LAYER]->height, layers[FOREGROUND_LAYER]);

  // Red square
  if (secondInteration)
    paramDraw->draw(previousClick.x * blockSize, previousClick.y * blockSize, redSQ->width, redSQ->height, redSQ);

  if (useTools) {
    drawTools();
  }
}
void drawTools() {
  paramDraw->draw(0, screenPixelHeight - toolsPanel->height, toolsPanel->width, toolsPanel->height, toolsPanel);

  // Focus on panel's buttons
  for (int i = 0; i < 6; i++) {
    toolButtons[i]->draw(paramDraw);
    toolButtons[i]->drawBorder(paramDraw);
  }

  for (int i = 0; i < 3; i++) {
    blockShapeToolButton[i]->draw(paramDraw);
    blockShapeToolButton[i]->drawBorder(paramDraw);
  }

  // Choose color
  if (colorPalette->show) colorPalette->draw(paramDraw);

  if (colorSel[1]->show) colorSel[1]->draw(paramDraw);
  if (colorSel[0]->show) colorSel[0]->draw(paramDraw);
  // Choose color---

  showGridButton->draw(paramDraw);

  // Textured blocks
  if (blockMode == SOLID_BLOCK_MODE) {
    for (int i = 0; i < 4; i++) foregroundTextureButton[i]->draw(paramDraw);
  }

  if (blockMode == BACKGROUND_BLOCK_MODE) {
    backgroundBlockPanel->draw(paramDraw);

    for (int i = 0; i < 3; i++) backgroundTextureButton[i]->draw(paramDraw);
  }
}
// ---------------------------------------------------------------------------------
// drawCreateMapScreen {END}
// ---------------------------------------------------------------------------------

void initializeCreateMap() {  // Init_CreateMap: initializeCreateMap // ★★★
  for (int i = 0; i < blocksInHeight; i++) {
    for (int j = 0; j < blocksInWidth; j++) {
      forgegroundMap[i][j] = 0;
      backgroundMap[i][j] = 0;
    }
  }

  mapGridSprite = new Sprite(MAP_GRID_IMG, WHITE);
  toolsPanel = new Sprite(MAP_TOOLS_IMG, WHITE);
  square = new Sprite(SQUARE_IMG, WHITE);
  redSQ = new Sprite(RED_SQUARE_IMG, WHITE);
  white = new Sprite(WHITE_BKG_IMG, WHITE);
  Sprite* borderSprite = new Sprite(BORDER_IMG, WHITE);

  const int toolsY = (screenPixelHeight - toolsPanel->height);

  toolButtons[SOLID_BLOCK] = new Button(8, toolsY + 8, BUTTON_1_IMG);
  toolButtons[SOLID_BLOCK]->Border = borderSprite;

  toolButtons[DELETE_BLOCK] = new Button(8, toolsY + 38, BUTTON_2_IMG);
  toolButtons[DELETE_BLOCK]->Border = borderSprite;

  toolButtons[BACKGROUND_BLOCK] = new Button(8, toolsY + 68, BUTTON_3_IMG);
  toolButtons[BACKGROUND_BLOCK]->Border = borderSprite;

  toolButtons[SAVE] = new Button(40, toolsY + 8, BUTTON_4_IMG);
  toolButtons[SAVE]->Border = borderSprite;

  toolButtons[SMALL_SQUARE] = new Button(40, toolsY + 38, BUTTON_5_IMG);
  toolButtons[SMALL_SQUARE]->Border = borderSprite;

  toolButtons[STAIRS] = new Button(40, toolsY + 68, BUTTON_6_IMG);
  toolButtons[STAIRS]->Border = borderSprite;

  blockShapeToolButton[POINT_TOOL] = new Button(82, toolsY + 8, TOOL_BTN_1_IMG);
  blockShapeToolButton[POINT_TOOL]->Border = borderSprite;

  blockShapeToolButton[LINE_TOOL] = new Button(82, toolsY + 38, TOOL_BTN_2_IMG);
  blockShapeToolButton[LINE_TOOL]->Border = borderSprite;

  blockShapeToolButton[RECTANGLE_TOOL] = new Button(82, toolsY + 68, TOOL_BTN_3_IMG);
  blockShapeToolButton[RECTANGLE_TOOL]->subShow = false;
  blockShapeToolButton[RECTANGLE_TOOL]->Border = borderSprite;

  showGridButton = new Button(123, toolsY + 11, SHOW_GRID_BTN_IMG);
  showGridButton->show = true;
  showGridButton->subShow = true;

  colorSel[0] = new NeoElement(COLOR_SQUARE_IMG);
  colorSel[0]->Button::x = 731;
  colorSel[0]->Button::y = toolsY + 24;
  colorSel[0]->changeColor(colorSel[0]->followColor(2, 2), firstColor);

  colorSel[1] = new NeoElement(COLOR_SQUARE_IMG);
  colorSel[1]->Button::x = 747;
  colorSel[1]->Button::y = toolsY + 40;
  colorSel[1]->changeColor(colorSel[1]->followColor(2, 2), lastColor);

  colorPalette = new NeoElement(COLOR_PALETTE_IMG);
  colorPalette->Button::x = screenPixelWidth - colorPalette->width;
  colorPalette->Button::y = toolsY - colorPalette->height;

  //++++
  for (int i = 0; i < 4; i++) {
    foregroundTextureButton[i] = new Button(237 + 24 * i, toolsY + 17, RED_BORDER_IMG, WHITE);
  }
  //----

  //+++
  for (int i = 0; i < 3; i++) {
    backgroundTextureButton[i] = new Button(237 + 24 * i, toolsY + 17, RED_BORDER_IMG, WHITE);
  }
  //---

  backgroundBlockPanel = new Button(229, toolsY + 10, TEXTURE_GRID_IMG);

  layers[BACK_LAYER] = new NeoSprite(WHITE_BKG_IMG, WHITE);
  layers[FOREGROUND_LAYER] = new NeoSprite(WHITE_BKG_IMG, WHITE);
  finerMapLayer = new NeoSprite(WHITE_BKG_IMG, WHITE);

  testMapButton = new Button(425, toolsY + 10, TEST_MAP_BTN_IMG);
}

void saveMap() {  // Save: saveMap
  std::ofstream oText("Save/map1.txt");
  char a = '0';

  for (int i = 0; i < blocksInHeight; i++) {
    for (int j = 0; j < blocksInWidth; j++) {
      if (forgegroundMap[i][j] == 0) a = '0';
      if (forgegroundMap[i][j] == 1) a = '1';
      if (forgegroundMap[i][j] == 2) a = '2';
      if (forgegroundMap[i][j] == 3) a = '3';

      oText.write(&a, sizeof(a));
    }
  }

  a = '|';
  oText.write(&a, sizeof(a));

  for (int i = 0; i < blocksInHeight * 2; i++) {
    for (int j = 0; j < blocksInWidth * 2; j++) {
      if (fineMap[i][j] == 0) a = '0';
      if (fineMap[i][j] == 1) a = '1';
      if (fineMap[i][j] == 2) a = '2';
      if (fineMap[i][j] == 3) a = '3';

      oText.write(&a, sizeof(a));
    }
  }

  oText.close();

  NeoSprite *everything = new NeoSprite(WHITE_BKG_IMG, WHITE);
  everything->addImage(0, 0, layers[BACK_LAYER]);
  everything->addImage(0, 0, finerMapLayer);
  everything->addImage(0, 0, layers[FOREGROUND_LAYER]);
  everything->save("Save/map1.bmp");

  toolButtons[SAVE]->showBorder = false;
}
void panelToolsActions(int clickedX, int clickedY) {  // UseToolsInPanel: panelToolsActions
  focusButtons(cursorX, cursorY);

  // CLICK+++
  if (toolButtons[SOLID_BLOCK]->contains(clickedX, clickedY)) {
    showBorderOnlyFor(SOLID_BLOCK);

    blockMode = SOLID_BLOCK_MODE;
  }

  if (toolButtons[DELETE_BLOCK]->contains(clickedX, clickedY)) {
    showBorderOnlyFor(DELETE_BLOCK);

    blockMode = DELETE_BLOCK_MODE;
  }

  if (toolButtons[BACKGROUND_BLOCK]->contains(clickedX, clickedY)) {
    showBorderOnlyFor(BACKGROUND_BLOCK);

    blockMode = BACKGROUND_BLOCK_MODE;
  }

  if (toolButtons[SAVE]->contains(clickedX, clickedY)) {
    showBorderOnlyFor(SAVE);

    saveMap();
  }

  if (toolButtons[SMALL_SQUARE]->contains(clickedX, clickedY)) {
    showBorderOnlyFor(SMALL_SQUARE);

    blockMode = SMALL_SQUARE_MODE;
  }

  if (toolButtons[STAIRS]->contains(clickedX, clickedY)) {
    showBorderOnlyFor(STAIRS);

    blockMode = STAIRS_MODE;
  }

  //+++++++++
  if (blockShapeToolButton[POINT_TOOL]->contains(clickedX, clickedY)) {
    showToolBorderOnlyFor(POINT_TOOL);

    toolSelected = POINT_TOOL;
  }

  if (blockShapeToolButton[LINE_TOOL]->contains(clickedX, clickedY)) {
    showToolBorderOnlyFor(LINE_TOOL);

    toolSelected = LINE_TOOL;
    secondInteration = false;
  }

  if (blockShapeToolButton[RECTANGLE_TOOL]->contains(clickedX, clickedY)) {
    showToolBorderOnlyFor(RECTANGLE_TOOL);

    toolSelected = RECTANGLE_TOOL;
    secondInteration = false;
  }

  if (testMapButton->contains(clickedX, clickedY)) {
    testMapButton->showBorder = true;
  }

  //Кнопка скрытия сетки
  if (showGridButton->contains(clickedX, clickedY) && didClickLeftButton) {
    showGridButton->show = !showGridButton->subShow;
    showGrid = !showGridButton->subShow;
    showGridButton->subShow = !showGridButton->subShow;
  }

  //Выбор материала блока+++
  for (int i = 0; i < 4; i++) {
    if (!foregroundTextureButton[i]->contains(clickedX, clickedY)) continue;
    for (int j = 0; j < 4; j++) foregroundTextureButton[j]->show = false;

    foregroundTextureButton[i]->show = true;

    selectedFgTexture = foregroundList[i];
  }
  //++++++++++++++++
  for (int i = 0; i < 3; i++) {
    if (!backgroundTextureButton[i]->contains(clickedX, clickedY)) continue;
    for (int j = 0; j < 3; j++) backgroundTextureButton[j]->show = false;

    backgroundTextureButton[i]->show = true;

    selectedBgTexture = backList[i];
  }
  //Выбор материала блока---
  // CLICK---
}
void focusButtons(int cursorX, int cursorY) {  // Focus: focusButtons
  for (int i = 0; i < 6; i++) {
    toolButtons[i]->show = toolButtons[i]->contains(cursorX, cursorY);
  }

  for (int i = 0; i < 3; i++) {
    blockShapeToolButton[i]->show = blockShapeToolButton[i]->contains(cursorX, cursorY);
  }

  testMapButton->show = testMapButton->contains(cursorX, cursorY);
}
void showBorderOnlyFor(int target) {
  for (int i = 0; i < 6; i++) {
    toolButtons[i]->showBorder = false;
  }

  toolButtons[target]->showBorder = true;
}
void showToolBorderOnlyFor(int target) {
  for (int i = 0; i < 3; i++) {
    blockShapeToolButton[i]->showBorder = false;
  }

  blockShapeToolButton[target]->showBorder = true;
}

void addBlock(int i, int j, BlockMode mode) {  // AddBlock: addBlock
  switch (mode) {
    case DELETE_BLOCK_MODE:
      forgegroundMap[i][j] = 0;
      backgroundMap[i][j] = 0;

      Sprite* empty = new Sprite(EMPTY_IMG);

      layers[FOREGROUND_LAYER]->addImage(j * blockSize, i * blockSize, empty, 0);
      layers[BACK_LAYER]->addImage(j * blockSize, i * blockSize, empty, 0);
      break;

    case SOLID_BLOCK_MODE:
      forgegroundMap[i][j] = 1;

      NeoElement* texture = new NeoElement(selectedFgTexture);

      if (selectedFgTexture == SQUARE_IMG) {
        texture->changeColor(texture->followColor(1, 1), firstColor);
        texture->changeColor(texture->followColor(0, 0), lastColor);
      }

      layers[FOREGROUND_LAYER]->addImage(j * blockSize, i * blockSize, texture);
      break;

    case BACKGROUND_BLOCK_MODE:
      backgroundMap[i][j] = 2;
      NeoElement *texture = new NeoElement(selectedBgTexture);

      if (selectedBgTexture == BACK_SQUARE_IMG)
        texture->changeColor(texture->followColor(0, 0), firstColor);

      layers[BACK_LAYER]->addImage(j * blockSize, i * blockSize, texture);
      break;

    case STAIRS_MODE:
      forgegroundMap[i][j] = 3;
      NeoElement* texture = new NeoElement(LADDER_UP_IMG);

      texture->changeColor(texture->followColor(1, 1), firstColor);
      texture->changeColor(texture->followColor(0, 0), lastColor);

      layers[FOREGROUND_LAYER]->addImage(j * blockSize, i * blockSize, texture);
      break;

    case SMALL_SQUARE_MODE:
      int mapX = clickedX / (blockSize/2);
      int mapY = clickedY / (blockSize/2);

      fineMap[mapY][mapX] = 1;

      Sprite* mini = new Sprite(SMALL_SQUARE_IMG);

      finerMapLayer->addImage(mapX * (blockSize/2), mapY * (blockSize/2), mini);
      break;
  }
}
void modifyMap(int clickedX, int clickedY) {  // ModifyTheMap: modifyMap
  if (useTools) return;

  int mapX = clickedX / blockSize;
  int mapY = clickedY / blockSize;
  
  switch (toolSelected) {
    case POINT_TOOL:
      addBlock(mapY, mapX, blockMode);
      break;

    case LINE_TOOL:
      if (!secondInteration) {
        secondInteration = true;

        previousClick.x = mapX;
        previousClick.y = mapY;
      } else {
        secondInteration = false;

        int left = std::min(mapX, previousClick.x);
        int top = std::min(mapY, previousClick.y);

        int right = std::max(mapX, previousClick.x);
        int bottom = std::max(mapY, previousClick.y);

        if (left != right && top != bottom) break;
    
        for (int i = top; i < bottom + 1; i++) {
            for (int j = left; j < right + 1; j++) {
                addBlock(i, j, blockMode);
            }
        }
      }
      break;

    case RECTANGLE_TOOL:
      if (!secondInteration) {
        secondInteration = true;

        previousClick.x = mapX;
        previousClick.y = mapY;
      } else {
        secondInteration = false;

        int left = std::min(mapX, previousClick.x);
        int top = std::min(mapY, previousClick.y);

        int right = std::max(mapX, previousClick.x);
        int bottom = std::max(mapY, previousClick.y);

        for (int i = top; i < bottom + 1; i++) {
          for (int j = left; j < right + 1; j++) {
              addBlock(i, j, blockMode);
          }
        }
      }
      break;
  }
}

void createMapInteractions(int clickedX, int clickedY) {  // ★★★ createMap: createMapInteractions
  keyboardEvents();

  selectColor(clickedX, clickedY);

  // Hide panel
  bool clickedOutsidePanel = clickedX >= 0 && clickedX <= screenPixelWidth &&
                             clickedY >= 0 &&
                             clickedY <= screenPixelHeight - toolsPanel->height;
  bool clickedColorSelector =
      colorPalette->contains(clickedX, clickedY) && colorPalette->show;

  if (didClickLeftButton && clickedOutsidePanel && !clickedColorSelector) {
    useTools = false;

    colorSel[0]->show = false;
    colorSel[1]->show = false;
    colorPalette->show = false;
  }

  // Use tools in panel
  if (useTools) {
    if (colorSel[0]->contains(clickedX, clickedY) colorPalette->show = true;

    panelToolsActions(clickedX, clickedY);
  }

  // Modify the map
  if (didClickLeftButton) {
    modifyMap(clickedX, clickedY);
  }
}

void keyboardEvents() {
  auto key = keyboardMapping(buffer);

  switch (key) {
    case KEY_H:  // show grid
      showGridButton->subShow = false;
      showGridButton->show = false;
      showGrid = false;
      break;

    case KEY_S:  // hide grid
      showGridButton->subShow = true;
      showGridButton->show = true;
      showGrid = true;
      break;

    case KEY_SPACE:  // show panel
      useTools = true;

      colorSel[0]->show = true;
      colorSel[1]->show = true;

      if (showGridButton->subShow) showGridButton->show = true;
      break;

    case KEY_ESCAPE:  // exit
      createMapMode = false;
      isInitialState = true;
      break;
  }
}

void selectColor(int clickedX, int clickedY) {
  if (!colorPalette->contains(clickedX, clickedY)) return;
  if (!colorPalette->show) return;

  // First
  if (didClickLeftButton) {
    colorSel[0]->changeColor(
        colorSel[0]->followColor(2, 2),
        colorPalette->followColor(clickedX - colorPalette->Button::x,
                                   clickedY - colorPalette->Button::y));

    firstColor =
        colorPalette->followColor(clickedX - colorPalette->Button::x,
                                   clickedY - colorPalette->Button::y);
  }

  // Last
  if (didClickRightButton) {
    colorSel[1]->changeColor(
        colorSel[1]->followColor(2, 2),
        colorPalette->followColor(clickedX - colorPalette->Button::x,
                                   clickedY - colorPalette->Button::y));

    lastColor = colorPalette->followColor(clickedX - colorPalette->Button::x,
                                           clickedY - colorPalette->Button::y);
  }
}