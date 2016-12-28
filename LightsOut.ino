#include <Arduboy2.h>

Arduboy2 arduboy;

unsigned char state = 0;
unsigned int level = 1;
unsigned int moves = 0;
bool last_press = false;
bool board[5][5] = {};
int sx = 2;
int sy = 2;

void generateBoard() {
  moves = 0;
  sx = 2;
  sy = 2;

  for (unsigned char x = 0; x < 5; x++) {
    for (unsigned char y = 0; y < 5; y++) {
      board[x][y] = 0;
    }
  }

  unsigned char stx = random(0, 5);
  unsigned char sty = random(0, 5);
  flipSquares(stx, sty);

  unsigned char flips = 3 * level - 1;
  do {
    switch (random(0, 4)) {
      case 0: // Top
        if (stx - 1 >= 0) {
          flipSquares(stx - 1, sty);
          stx = stx - 1;
          flips--;
        }
        break;
      case 1: // Bottom
        if (stx + 1 <= 4) {
          flipSquares(stx + 1, sty);
          stx = stx + 1;
          flips--;
        }
        break;
      case 2: // Left
        if (sty - 1 >= 0) {
          flipSquares(stx, sty - 1);
          sty = sty - 1;
          flips--;
        }
        break;
      case 3: // Right
        if (sty + 1 <= 4) {
          flipSquares(stx, sty + 1);
          sty = sty + 1;
          flips--;
        }
        break;
    }
  } while (flips > 0); 
}

void drawBoard() {    
  for (unsigned char x = 0; x < 5; x++) {
    for (unsigned char y = 0; y < 5; y++) {
      if (board[x][y]) {
        arduboy.fillRect(x * 13 + 2, y * 13 + 2, 8, 8, WHITE);
      } else {
        arduboy.drawRect(x * 13 + 2, y * 13 + 2, 8, 8, WHITE);
      }
    }
  }
  
  arduboy.drawRect(sx * 13, sy * 13, 12, 12, WHITE);
}

void drawHud() {
  arduboy.setTextSize(1);  
  arduboy.setCursor(76, 0);
  arduboy.print("Level");
  
  arduboy.setTextSize(2);
  arduboy.setCursor(80, 12);
  arduboy.print(level);

  arduboy.drawFastHLine(76, 31, 52, WHITE);

  arduboy.setTextSize(1);
  arduboy.setCursor(76, 35);
  arduboy.print("Moves");

  arduboy.setTextSize(2);
  arduboy.setCursor(80, 47);
  arduboy.print(moves);
}

void handleNoInput() {
  if (
    !arduboy.pressed(UP_BUTTON) && 
    !arduboy.pressed(DOWN_BUTTON) &&
    !arduboy.pressed(LEFT_BUTTON) && 
    !arduboy.pressed(RIGHT_BUTTON) &&
    !arduboy.pressed(A_BUTTON) &&
    !arduboy.pressed(B_BUTTON)
  ) { 
    last_press = false; 
  }
}

void handleInput() {  
  if (arduboy.pressed(UP_BUTTON) && !last_press) { 
    sy--; 
    last_press = true; 
  }
  
  if (arduboy.pressed(DOWN_BUTTON) && !last_press) { 
    sy++; 
    last_press = true; 
  }
  
  if (arduboy.pressed(LEFT_BUTTON) && !last_press) { 
    sx--; 
    last_press = true; 
  }
  
  if (arduboy.pressed(RIGHT_BUTTON) && !last_press) { 
    sx++; 
    last_press = true; 
  }

  if (sy < 0) { sy = 0; }
  if (sy > 4) { sy = 4; }
  if (sx < 0) { sx = 0; }
  if (sx > 4) { sx = 4; }

  if (arduboy.pressed(A_BUTTON) && !last_press) {
    last_press = true;
     
    flipSquares(sx, sy);
    moves++;
    
    if (checkSquares()) {
      level++;
      generateBoard(); 
    }
  }

  if (arduboy.pressed(B_BUTTON) && !last_press) {
    last_press = true;
    
    state = 2;
  }
}

void flipSquares(int x, int y) {
  // Top
  if (y - 1 >= 0) { board[x][y - 1] = !board[x][y - 1]; }

  // Left
  if (x - 1 >= 0) { board[x - 1][y] = !board[x - 1][y]; }

  // Right
  if (x + 1 <= 4) { board[x + 1][y] = !board[x + 1][y]; }

  // Bottom
  if (y + 1 <= 4) { board[x][y + 1] = !board[x][y + 1]; }

  // Center
  board[x][y] = !board[x][y];
}

bool checkSquares() {
  unsigned char ct = 0;
  for (unsigned char x = 0; x < 5; x++) {
    for (unsigned char y = 0; y < 5; y++) {
      ct += board[x][y];
    }
  }

  return ct == 0;
}

void drawHome() {
  arduboy.setTextSize(2);
  arduboy.setCursor(4, 24);
  arduboy.print("LIGHTS/OUT");
  
  arduboy.setTextSize(1);
  arduboy.setCursor(16, 44);
  arduboy.print("Press A to start");

  if (arduboy.pressed(A_BUTTON) && !last_press) {
    last_press = true;
    
    level = 1;
    generateBoard();
    state = 1;
  }
}

void drawGiveUp() {
  arduboy.setTextSize(2);
  arduboy.setCursor(17, 18);
  arduboy.print("Give up?");
  
  arduboy.setTextSize(1);
  arduboy.setCursor(13, 45);
  arduboy.print("Press A to resume");

  arduboy.setTextSize(1);
  arduboy.setCursor(7, 55);
  arduboy.print("Press B to recreate");

  if (arduboy.pressed(A_BUTTON) && !last_press) {
    last_press = true;
    
    state = 1;
  }

  if (arduboy.pressed(B_BUTTON) && !last_press) {
    last_press = true;
    
    generateBoard();
    state = 1;
  }
}

void setup() {
  arduboy.initRandomSeed();
  arduboy.begin();
}

void loop() {
  if (!arduboy.nextFrame()) { return; }

  arduboy.clear();

  handleNoInput();
  
  switch (state) {
    case 0:
      drawHome();
      break;
    case 1:
      handleInput();
      drawBoard();
      drawHud();
      break;
    case 2:
      drawGiveUp();
      break;
  }

  arduboy.display();
}
