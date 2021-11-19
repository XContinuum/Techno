// Create map+++
bool createMapMode = false;  // Create_map: createMapMode
bool useTools = false;       // UseTools: useTools
bool showGrid = true;        // setka: showGrid

int firstColor = VERY_LIGHT_GRAY;  // colorFirst: firstColor
int lastColor = DARK_GRAY;         // colorLast: lastColor

int CMap[blocksInHeight][blocksInWidth]; // CMap: 
int CMapBack[blocksInHeight][blocksInWidth]; // CMapBack: 

int TypeOFBlock = 0; // TypeOFBlock: 
int TypeOFAdd = 0; // TypeOFAdd: 
int first = 0; // first: 
int Color1; // Color1: 
int Color2; // Color2: 
int MiniMap[blocksInHeight * 2][blocksInWidth * 2]; // MiniMap: 

// Assets
#define MAP_GRID_IMG "Images/create_map/map.bmp"
#define MAP_TOOLS_IMG "Images/create_map/map_tools.bmp"
#define SQUARE_IMG "Images/create_map/square.bmp"
#define RED_SQUARE_IMG "Images/create_map/redSQ.bmp"
#define WHITE_BKG_IMG "Images/create_map/white.bmp"
#define BACK_SQUARE_IMG "Images/create_map/squareBack.bmp"
#define BORDER_IMG "Images/create_map/border.bmp"

#define BUTTON_1_IMG "Images/create_map/btnT1.bmp"
#define BUTTON_2_IMG "Images/create_map/btnT2.bmp"
#define BUTTON_3_IMG "Images/create_map/btnT3.bmp"
#define BUTTON_4_IMG "Images/create_map/btnT4.bmp"
#define BUTTON_5_IMG "Images/create_map/btnT5.bmp"
#define BUTTON_6_IMG "Images/create_map/btnT6.bmp"

#define TOOL_BTN_1_IMG "Images/create_map/btn_tool1.bmp"
#define TOOL_BTN_2_IMG "Images/create_map/btn_tool2.bmp"
#define TOOL_BTN_3_IMG "Images/create_map/btn_tool3.bmp"
#define CHECK_BTN_IMG "Images/create_map/check.bmp"
#define COLOR_SQUARE_IMG "Images/create_map/colorSQ.bmp"

#define COLOR_PALLET_IMG "Images/create_map/color_tools.bmp"
#define RED_BORDER_IMG "Images/create_map/redSelect.bmp"
#define TEXTURE_GRID_IMG "Images/create_map/backTextures.bmp"
#define TEST_MAP_BTN_IMG "Images/create_map/btn3_1.bmp"

#define EMPTY_IMG "Images/create_map/empty.bmp"
#define LADDER_UP_IMG "Images/create_map/squareUP.bmp"
#define SMALL_SQUARE_IMG "Images/create_map/squareSmall.bmp"

char *block_material = SQUARE_IMG;
char *blockBack_material = BACK_SQUARE_IMG;
char *foregroundList[] = {SQUARE_IMG, "Images/create_map/squareStone.bmp",
                          "Images/create_map/squareMetal.bmp", "Images/create_map/squareWood.bmp"};
char *backList[] = {BACK_SQUARE_IMG, "Images/create_map/squareB1.bmp",
                    "Images/create_map/squareB2.bmp"};

Sprite *CMAP; // CMAP: 
Sprite *tools; // tools: 
Sprite *square; // square: 
Sprite *redSQ; // redSQ: 
Sprite *white; // white: 

NeoElement *colorSel[2]; // colorSel: 
NeoElement *colorSelector; // colorSelector: 
NeoElement *squares[blocksInHeight][blocksInWidth]; // squares: 
NeoElement *squaresBack[blocksInHeight][blocksInWidth]; // squaresBack: 

Button *btnT[5]; // btnT: 
Button *btn_tool[3]; // btn_tool: 
Button *check; // check: 
Button *block[4]; // block: 
Button *backBlock[3]; // backBlock: 
Button *BackTextures; // BackTextures: 
Button *btn3[1]; // btn3: 

NeoSprite *everything; // Everythink: everything
NeoSprite *Layer[2]; // Layer: 
NeoSprite *lay; // lay: 

Point p; // p: 

// CREATING+++
void drawCreateMapScreen() {  // Draw_Create: drawCreateMapScreen ★★★
  paramDraw->draw(0, 0, white->width, white->height, white);

  // MAP++++
  if (showGrid) paramDraw->draw(0, 0, CMAP->width, CMAP->height, CMAP);

  paramDraw->draw(0, 0, Layer[BACK_LAYER]->width, Layer[BACK_LAYER]->height,
                  Layer[BACK_LAYER]);
  paramDraw->draw(0, 0, lay->width, lay->height, lay);
  paramDraw->draw(0, 0, Layer[FOREGROUND_LAYER]->width,
                  Layer[FOREGROUND_LAYER]->height, Layer[FOREGROUND_LAYER]);
  // MAP----

  // RED SQUARE+++
  if (first == 1)
    paramDraw->draw(p.x * 20, p.y * 20, redSQ->width, redSQ->height, redSQ);
  // RED SQUARE---

  // TOOLS++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (useTools) {
    paramDraw->draw(0, h - tools->height, tools->width, tools->height, tools);

    // FOCUS ON PANEL'S BUTTONS+++
    for (int i = 0; i < 6; i++) {
      btnT[i]->draw(paramDraw);

      if (btnT[i]->showBorder)
        paramDraw->draw(btnT[i]->x, btnT[i]->y, btnT[i]->w, btnT[i]->h,
                        btnT[i]->Border);
    }

    for (int i = 0; i < 3; i++) {
      btn_tool[i]->draw(paramDraw);

      if (btn_tool[i]->showBorder)
        paramDraw->draw(btn_tool[i]->x, btn_tool[i]->y, btn_tool[i]->w,
                        btn_tool[i]->h, btn_tool[i]->Border);
    }
    // FOCUS ON PANEL'S BUTTONS--

    // CHOISE COLOR+++
    if (colorSelector->show)
      paramDraw->draw(colorSelector->Button::x, colorSelector->Button::y,
                      colorSelector->width, colorSelector->height,
                      colorSelector->image);

    for (int i = 1; i >= 0; i--)
      if (colorSel[i]->show)
        paramDraw->draw(colorSel[i]->Button::x, colorSel[i]->Button::y,
                        colorSel[i]->width, colorSel[i]->height,
                        colorSel[i]->image);
    // CHOISE COLOR---

    check->draw(paramDraw);

    // Textured blocks+++
    if (TypeOFBlock == 1) {
      for (int i = 0; i < 4; i++) block[i]->draw(paramDraw);
    }

    if (TypeOFBlock == 2) {
      BackTextures->draw(paramDraw);

      for (int i = 0; i < 3; i++) backBlock[i]->draw(paramDraw);
    }
    // Textured blocks---
    // TOOLS--------------------------------------------------------------------------------------
  }
}

void initializeCreateMap() {  // Init_CreateMap: initializeCreateMap // ★★★
  everything = new NeoSprite(WHITE_BKG_IMG, WHITE);

  for (int i = 0; i < blocksInHeight; i++) {
    for (int j = 0; j < blocksInWidth; j++) {
      CMap[i][j] = 0;
      CMapBack[i][j] = 0;
    }
  }

  CMAP = new Sprite(MAP_GRID_IMG, WHITE);
  tools = new Sprite(MAP_TOOLS_IMG, WHITE);
  square = new Sprite(SQUARE_IMG, WHITE);
  redSQ = new Sprite(RED_SQUARE_IMG, WHITE);
  white = new Sprite(WHITE_BKG_IMG, WHITE);
  Sprite* borderSprite = new Sprite(BORDER_IMG, WHITE);

  const int toolsY = (screenPixelHeight - tools->height);

  btnT[SOLID_BLOCK] = new Button(8, toolsY + 8, BUTTON_1_IMG);
  btnT[SOLID_BLOCK]->Border = borderSprite;

  btnT[DELETE_BLOCK] = new Button(8, toolsY + 38, BUTTON_2_IMG);
  btnT[DELETE_BLOCK]->Border = borderSprite;

  btnT[BACKGROUND_BLOCK] = new Button(8, toolsY + 68, BUTTON_3_IMG);
  btnT[BACKGROUND_BLOCK]->Border = borderSprite;

  btnT[SAVE] = new Button(40, toolsY + 8, BUTTON_4_IMG);
  btnT[SAVE]->Border = borderSprite;

  btnT[SMALL_SQUARE] = new Button(40, toolsY + 38, BUTTON_5_IMG);
  btnT[SMALL_SQUARE]->Border = borderSprite;

  btnT[STAIRS] = new Button(40, toolsY + 68, BUTTON_6_IMG);
  btnT[STAIRS]->Border = borderSprite;

  btn_tool[POINT] = new Button(82, toolsY + 8, TOOL_BTN_1_IMG);
  btn_tool[POINT]->Border = borderSprite;

  btn_tool[LINE] = new Button(82, toolsY + 38, TOOL_BTN_2_IMG);
  btn_tool[LINE]->Border = borderSprite;

  btn_tool[RECTANGLE] = new Button(82, toolsY + 68, TOOL_BTN_3_IMG);
  btn_tool[RECTANGLE]->subShow = false;
  btn_tool[RECTANGLE]->Border = borderSprite;

  check = new Button(123, toolsY + 11, CHECK_BTN_IMG);
  check->show = true;
  check->subShow = true;

  colorSel[0] = new NeoElement(COLOR_SQUARE_IMG);
  colorSel[0]->Button::x = 731;
  colorSel[0]->Button::y = toolsY + 24;
  colorSel[0]->changeColor(colorSel[0]->followColor(2, 2), firstColor);

  colorSel[1] = new NeoElement(COLOR_SQUARE_IMG);
  colorSel[1]->Button::x = 747;
  colorSel[1]->Button::y = toolsY + 40;
  colorSel[1]->changeColor(colorSel[1]->followColor(2, 2), lastColor);

  colorSelector = new NeoElement(COLOR_PALLET_IMG);
  colorSelector->Button::x = w - colorSelector->width;
  colorSelector->Button::y = h - tools->height - colorSelector->height;

  //++++
  for (int i = 0; i < 4; i++) {
    block[i] = new Button(237 + 24 * i, toolsY + 17, RED_BORDER_IMG, WHITE);
  }
  //----

  //+++
  for (int i = 0; i < 3; i++) {
    backBlock[i] = new Button(237 + 24 * i, toolsY + 17, RED_BORDER_IMG, WHITE);
  }
  //---

  BackTextures = new Button(229, toolsY + 10, TEXTURE_GRID_IMG);

  Layer[BACK_LAYER] = new NeoSprite(WHITE_BKG_IMG, WHITE);
  Layer[FOREGROUND_LAYER] = new NeoSprite(WHITE_BKG_IMG, WHITE);
  lay = new NeoSprite(WHITE_BKG_IMG, WHITE);

  btn3[TEST_THE_MAP] = new Button(425, toolsY + 10, TEST_MAP_BTN_IMG);
}

void saveMap() {  // Save: saveMap
  std::ofstream oText("Save/map1.txt");
  char a = '0';

  for (int i = 0; i < blocksInHeight; i++) {
    for (int j = 0; j < blocksInWidth; j++) {
      if (CMap[i][j] == 0) a = '0';

      if (CMap[i][j] == 1) a = '1';

      if (CMap[i][j] == 2) a = '2';

      if (CMap[i][j] == 3) a = '3';

      oText.write(&a, sizeof(a));
    }
  }

  a = '|';
  oText.write(&a, sizeof(a));

  for (int i = 0; i < blocksInHeight * 2; i++) {
    for (int j = 0; j < blocksInWidth * 2; j++) {
      if (MiniMap[i][j] == 0) a = '0';

      if (MiniMap[i][j] == 1) a = '1';

      if (MiniMap[i][j] == 2) a = '2';

      if (MiniMap[i][j] == 3) a = '3';

      oText.write(&a, sizeof(a));
    }
  }

  oText.close();

  everything->addImage(0, 0, Layer[BACK_LAYER]);
  everything->addImage(0, 0, lay);
  everything->addImage(0, 0, Layer[FOREGROUND_LAYER]);
  everything->save("Save/map1.bmp");

  btnT[SAVE]->showBorder = false;
}
void focusButtons() {  // Focus: focusButtons
  for (int i = 0; i < 6; i++) {
    btnT[i]->show = btnT[i]->contains(cursorX, cursorY);
  }

  for (int i = 0; i < 3; i++) {
    btn_tool[i]->show = btn_tool[i]->contains(cursorX, cursorY);
  }

  for (int i = 0; i < 1; i++) {
      btn3[i]->show = btn3[i]->contains(cursorX, cursorY);
  }
}
void panelToolsActions() {  // UseToolsInPanel: panelToolsActions
  if (useTools) {
    focusButtons();

    // CLICK+++
    if (btnT[SOLID_BLOCK]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 6; i++) btnT[i]->showBorder = false;

      btnT[SOLID_BLOCK]->showBorder = true;

      TypeOFBlock = 1;
    }

    if (btnT[DELETE_BLOCK]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 6; i++) btnT[i]->showBorder = false;

      btnT[DELETE_BLOCK]->showBorder = true;

      TypeOFBlock = 0;
    }

    if (btnT[BACKGROUND_BLOCK]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 6; i++) btnT[i]->showBorder = false;

      btnT[BACKGROUND_BLOCK]->showBorder = true;

      TypeOFBlock = 2;
    }

    if (btnT[SAVE]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 6; i++) btnT[i]->showBorder = false;

      btnT[SAVE]->showBorder = true;

      // Saving+++
      saveMap();
      // Saving---
    }

    if (btnT[SMALL_SQUARE]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 6; i++) btnT[i]->showBorder = false;

      btnT[SMALL_SQUARE]->showBorder = true;

      TypeOFBlock = 4;
    }

    if (btnT[STAIRS]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 6; i++) btnT[i]->showBorder = false;

      btnT[STAIRS]->showBorder = true;

      TypeOFBlock = 3;
    }

    //+++++++++
    if (btn_tool[POINT]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 3; i++) btn_tool[i]->showBorder = false;

      btn_tool[POINT]->showBorder = true;

      TypeOFAdd = 0;
    }

    if (btn_tool[LINE]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 3; i++) btn_tool[i]->showBorder = false;

      btn_tool[LINE]->showBorder = true;

      TypeOFAdd = 1;
      first = 0;
    }

    if (btn_tool[RECTANGLE]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 3; i++) btn_tool[i]->showBorder = false;

      btn_tool[RECTANGLE]->showBorder = true;

      TypeOFAdd = 2;
      first = 0;
    }

    //
    if (btn3[TEST_THE_MAP]->contains(clickedX, clickedY)) {
      for (int i = 0; i < 3; i++) btn3[i]->showBorder = false;

      btn3[TEST_THE_MAP]->showBorder = true;
    }

    //Кнопка скрытия сетки++++
    if (check->contains(clickedX, clickedY) && didClickLeftButton) {
      if (check->subShow) {
        check->subShow = false;
        check->show = false;
        showGrid = false;
      } else {
        check->subShow = true;
        check->show = true;
        showGrid = true;
      }
    }
    //Кнопка скрытия сетки++++

    //Выбор материала блока+++
    for (int i = 0; i < 4; i++) {
      if (block[i]->contains(clickedX, clickedY)) {
        for (int j = 0; j < 4; j++) block[j]->show = false;

        block[i]->show = true;

        block_material = foregroundList[i];
      }
    }
    //++++++++++++++++
    for (int i = 0; i < 3; i++) {
      if (backBlock[i]->contains(clickedX, clickedY)) {
        for (int j = 0; j < 3; j++) backBlock[j]->show = false;

        backBlock[i]->show = true;

        blockBack_material = backList[i];
      }
    }
    //Выбор материала блока---
    // CLICK---
  }
}

void addBlock(int i, int j) {  // AddBlock: addBlock
  Sprite *Empty;
  Sprite *mini;

  switch (TypeOFBlock) {
    case 0:  // Delete block
      CMap[i][j] = 0;
      CMapBack[i][j] = 0;

      Empty = new Sprite(EMPTY_IMG);

      Layer[FOREGROUND_LAYER]->addImage(j * 20, i * 20, Empty, 0);
      Layer[BACK_LAYER]->addImage(j * 20, i * 20, Empty, 0);
      break;

    case 1:  // Add block
      CMap[i][j] = 1;

      squares[i][j] = new NeoElement(block_material);

      if (block_material == SQUARE_IMG) {
        squares[i][j]->changeColor(squares[i][j]->followColor(1, 1),
                                   firstColor);
        squares[i][j]->changeColor(squares[i][j]->followColor(0, 0), lastColor);
      }
      Layer[FOREGROUND_LAYER]->addImage(j * 20, i * 20, squares[i][j]);
      break;

    case 2:  // Add back block
      CMapBack[i][j] = 2;
      squaresBack[i][j] = new NeoElement(blockBack_material);

      if (blockBack_material == BACK_SQUARE_IMG)
        squaresBack[i][j]->changeColor(squaresBack[i][j]->followColor(0, 0),
                                       firstColor);

      Layer[BACK_LAYER]->addImage(j * 20, i * 20, squaresBack[i][j]);
      break;

    case 3:  // Add stairs
      CMap[i][j] = 3;
      squares[i][j] = new NeoElement(LADDER_UP_IMG);

      squares[i][j]->changeColor(squares[i][j]->followColor(1, 1), firstColor);
      squares[i][j]->changeColor(squares[i][j]->followColor(0, 0), lastColor);

      Layer[FOREGROUND_LAYER]->addImage(j * 20, i * 20, squares[i][j]);
      break;

    case 4:
      int Xj = clickedX / 10;
      int Yi = clickedY / 10;

      MiniMap[Yi][Xj] = 1;

      mini = new Sprite(SMALL_SQUARE_IMG);

      lay->addImage(Xj * 10, Yi * 10, mini);
      break;
  }
}
void modifyMap() {  // ModifyTheMap: modifyMap
  if (didClickLeftButton && !useTools) {
    int mapX = clicked / 20;
    int mapY = clickedY / 20;

    // POINT+++
    if (TypeOFAdd == 0) addBlock(mapY, mapX);
    // POINT---

    // LINE+++
    if (TypeOFAdd == 1) {
      int mapX = clickedX / 20;
      int mapY = clickedY / 20;

      if (first == 0) {
        p.x = mapX;
        p.y = mapY;

        first = 1;
      } else if (first == 1) {
        int X1[2];
        int Y1[2];

        X1[0] = mapX;
        Y1[0] = mapY;

        X1[1] = p.x;
        Y1[1] = p.y;

        if (mapX > p.x) {
          X1[0] = p.x;
          X1[1] = mapX;
        }

        if (mapY > p.y) {
          Y1[0] = p.y;
          Y1[1] = mapY;
        }

        if (X1[0] == X1[1] || Y1[0] == Y1[1])
          for (int i = Y1[0]; i < Y1[1] + 1; i++)
            for (int j = X1[0]; j < X1[1] + 1; j++) addBlock(i, j);

        first = 0;
      }
    }
    // LINE---

    // RECTANGLE+++
    if (TypeOFAdd == 2) {
      int mapX = clickedX / 20;
      int mapY = clickedY / 20;

      if (first == 0) {
        p.x = mapX;
        p.y = mapY;

        first = 1;
      } else if (first == 1) {
        int X1[2];
        int Y1[2];

        X1[0] = mapX;
        Y1[0] = mapY;

        X1[1] = p.x;
        Y1[1] = p.y;

        if (mapX > p.x) {
          X1[0] = p.x;
          X1[1] = mapX;
        }

        if (mapY > p.y) {
          Y1[0] = p.y;
          Y1[1] = mapY;
        }

        for (int i = Y1[0]; i < Y1[1] + 1; i++)
          for (int j = X1[0]; j < X1[1] + 1; j++) addBlock(i, j);

        first = 0;
      }
    }
    // RECTANGLE---
  }
}

void createMap() {  // ★★★
  if (createMapMode) {
    // SELECT COLOR+++
    if (useTools)
      if (colorSel[0]->contains(clickedX, clickedY))
        colorSelector->show = true;

    // FIRST+++
    if (colorSelector->contains(clickedX, clickedY) &&
        colorSelector->show && didClickLeftButton) {
      colorSel[0]->changeColor(
          colorSel[0]->followColor(2, 2),
          colorSelector->followColor(clickedX - colorSelector->Button::x,
                                     clickedY - colorSelector->Button::y));

      firstColor =
          colorSelector->followColor(clickedX - colorSelector->Button::x,
                                     clickedY - colorSelector->Button::y);
    }
    // FIRST---

    // LAST+++
    if (colorSelector->contains(clickedX, clickedY) &&
        colorSelector->show && didClickRightButton) {
      colorSel[1]->changeColor(
          colorSel[1]->followColor(2, 2),
          colorSelector->followColor(clickedX - colorSelector->Button::x,
                                     clickedY - colorSelector->Button::y));

      lastColor =
          colorSelector->followColor(clickedX - colorSelector->Button::x,
                                     clickedY - colorSelector->Button::y);
    }
    // LAST---
    // SELECT COLOR---

    // NET+++
    auto key = keyboardMapping(buffer);
    if (key == KEY_H) {
      check->subShow = false;
      check->show = false;
      showGrid = false;
    }
    if (key == KEY_S) {
      check->subShow = true;
      check->show = true;
      showGrid = true;
    }
    // NET---

    // Show panel+++
    if (key == KEY_SPACE) {
      useTools = true;

      colorSel[0]->show = true;
      colorSel[1]->show = true;

      if (check->subShow) check->show = true;
    }
    // Show panel+++

    // Hide panel+++
    bool clickedOutsidePanel= clickedX >= 0 && clickedX <= screenPixelWidth && clickedY >= 0 &&
        clickedY <= screenPixelHeight - tools->height;
    bool clickedColorSelector = colorSelector->contains(clickedX, clickedY) && colorSelector->show;

    if (didClickLeftButton && clickedOutsidePanel && !clickedColorSelector) {
      useTools = false;

      colorSel[0]->show = false;
      colorSel[1]->show = false;
      colorSelector->show = false;
    }
    // Hide panel---

    // Use tools in panel+++
    panelToolsActions();
    // Use tools in panel---

    // MODIFY THE MAP+++
    modifyMap();
    // MODIFY THE MAP---

    // EXIT++++
    if (key == KEY_ESCAPE) {
      createMapMode = false;
      isInitialState = true;
    }
    // EXIT----
  }
}
// CREATING---