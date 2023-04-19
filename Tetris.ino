// Tetris game in random colors for TTGO T-Display ESP32 v.1.1
// Control: GPIO0 - Left, GPIO35 - Right, GPIO0+GPIO35 - Rotate
// ...or connect custom keys)
#include <TFT_eSPI.h>

#define BUTTON_LEFT 0
#define BUTTON_RIGHT 35
#define BUTTON_ROTATE 12
#define BUTTON_DROP 13
//#define PRESS_LEVEL HIGH //Active level for sensor keys
#define PRESS_LEVEL LOW //Active level for mechanical pull_up keys


#define GAME_SPEED 500 // define game speed
#define BOUNCE_DELAY 50 // define contact bounce delay

TFT_eSPI tft = TFT_eSPI(); // Initialize the TFT display

// Constants for the screen 
const int WINDOW_WIDTH = 135;
const int WINDOW_HEIGHT = 240;

// Constants for the game board
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

// Constants for the blocks
const int BLOCK_SIZE = 4;
const int BLOCK_WIDTH = 3;
const int BLOCK_HEIGHT = 3;
const int NUM_BLOCK_TYPES = 7;
const int NUM_ORIENTATIONS = 4;

// Colors for the blocks
const uint16_t BLOCK_COLORS[NUM_BLOCK_TYPES] = {
  TFT_RED, TFT_BLUE, TFT_GREEN, TFT_CYAN, TFT_MAGENTA, TFT_YELLOW, TFT_WHITE
};

// Define the blocks
const int blocks[NUM_BLOCK_TYPES][NUM_ORIENTATIONS][BLOCK_SIZE][BLOCK_SIZE] = {
  // I block
  {
    {
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0}
    },
    {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0}
    }
  },
  // J block
  {
    {
      {1, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 1, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0}
    }
  },
  // L block
  {
    {
      {0, 0, 1, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 0, 0},
      {1, 1, 1, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    }
  },
  // O block
  {
    {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  },
  // S block
  {
    {
      {0, 1, 1, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    }
  },
  // T block
  {
    {
      {0, 1, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    }
  },
  // Z block
  {
    {
      {1, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 1, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 0}
    }
  }
};

// Variables for the game state
int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};
int currentBlockX = 0;
int currentBlockY = 0;
int currentBlockType = 0;
int currentBlockOrientation = 0;
int score = 0; // game score
int speed = GAME_SPEED; // game speed
unsigned long previous_time; // previous game time

bool left_pressed = false; // left button pressed flag
bool right_pressed = false; // right button pressed flag
bool rotate_pressed = false; // rotate button pressed flag
bool drop_pressed = false; // drop button pressed flag
unsigned long left_pressed_time; // left button pressed time
unsigned long right_pressed_time; // right button pressed time
unsigned long rotate_pressed_time; // rotate button pressed time
unsigned long drop_pressed_time; // drop button pressed time

// Check if a block is valid in its current position
bool isValidBlock(int x, int y, int blockType, int orientation) {
  // Loop through the cells of the block
  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      // If the cell is filled, check if it is within the bounds of the board and not overlapping with any other blocks
      if (blocks[blockType][orientation][i][j] != 0) {
        int boardX = x + j;
        int boardY = y + i;
        if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT || (boardY >= 0 && board[boardY][boardX] != 0)) {
          return false;
        }
      }
    }
  }
  return true;
}

// Generate a new block
void generateNewBlock() {
    //currentBlockX = BOARD_WIDTH / 2 - BLOCK_SIZE / 2;
    currentBlockX = random(BOARD_WIDTH - BLOCK_SIZE / 2);
    currentBlockY = 0;
    currentBlockType = random(NUM_BLOCK_TYPES);
    currentBlockOrientation = random(NUM_ORIENTATIONS);
}

// Add a block to the board
void addBlockToBoard(int x, int y, int blockType, int orientation) {
  // Loop through the cells of the block
  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      // If the cell is filled, add it to the board
      if (blocks[blockType][orientation][i][j] != 0) {
        board[y+i][x+j] = blockType+1; // Use blockType+1 to avoid setting board cells to 0, which is reserved for empty cells
      }
    }
  }
}

// Clear any completed rows
void clearCompletedRows() {
  // Loop through the rows of the board from the bottom up
  for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
    // Check if the row is complete
    bool rowComplete = true;
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (board[i][j] == 0) {
        rowComplete = false;
        break;
      }
    }
    
    // If the row is complete, remove it and shift down all the rows above it
    if (rowComplete) {
      score++;
      checkGameSpeed();
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
          board[k][j] = board[k-1][j];
        }
      }
      for (int j = 0; j < BOARD_WIDTH; j++) {
        board[0][j] = 0;
      }
      i++; // Re-check this row since the rows above it have shifted down
    }
  }
}
// check if game speed needs to be increased
void checkGameSpeed() {
  if (score >= 10 && score < 20) {
    speed = GAME_SPEED - 100;
  } else if (score >= 20 && score < 30) {
    speed = GAME_SPEED - 200;
  } else if (score >= 30 && score < 40) {
    speed = GAME_SPEED - 300;
  } else if (score >= 40 && score < 50) {
    speed = GAME_SPEED - 400;
  } else if (score >= 50) {
    speed = GAME_SPEED - 500;
  }
}
// Draw the game board on the TFT display
void drawBoard() {
  tft.fillScreen(TFT_BLACK);
  drawScore();
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (board[i][j] != 0) {
        tft.fillRoundRect(j*12, i*12, 12, 12, 2, BLOCK_COLORS[board[i][j]-1]);
        // Use board[i][j]-1 to get the correct color from the BLOCK_COLORS array
      }
    }
  }
}
// Draw the falling block on the TFT display
void drawFallingBlock() {
  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      if (blocks[currentBlockType][currentBlockOrientation][i][j] != 0) {
        int x = (currentBlockX + j) * 12;
        int y = (currentBlockY + i) * 12;
        tft.fillRoundRect(x, y, 12, 12, 2, BLOCK_COLORS[currentBlockType]);
      }
    }
  }
}

// Move the falling block left
void moveLeft() {
  if (isValidBlock(currentBlockX - 1, currentBlockY, currentBlockType, currentBlockOrientation)) {
    currentBlockX--;
  }
}

// Move the falling block right
void moveRight() {
  if (isValidBlock(currentBlockX + 1, currentBlockY, currentBlockType, currentBlockOrientation)) {
    currentBlockX++;
  }
}

// Rotate the falling block clockwise
void rotateClockwise() {
  int newOrientation = (currentBlockOrientation + 1) % NUM_ORIENTATIONS;
  if (isValidBlock(currentBlockX, currentBlockY, currentBlockType, newOrientation)) {
    currentBlockOrientation = newOrientation;
  }
}

// Drop the falling block all the way down
void drop() {
  while (isValidBlock(currentBlockX, currentBlockY + 1, currentBlockType, currentBlockOrientation)) {
    currentBlockY++;
  }
}
/*
void handleButtonPress() {
  // handle button presses
  if ((digitalRead(BUTTON_LEFT) == PRESS_LEVEL) && (digitalRead(BUTTON_RIGHT) == PRESS_LEVEL) || (digitalRead(BUTTON_ROTATE) == PRESS_LEVEL)) {
    rotateClockwise();
  }
  if (digitalRead(BUTTON_LEFT) == PRESS_LEVEL) {
    moveLeft();
  }
  if (digitalRead(BUTTON_RIGHT) == PRESS_LEVEL) {
    moveRight();
  }
  if (digitalRead(BUTTON_DROP) == PRESS_LEVEL) {
    // move the block down quickly
    drop();
  }
}
*/
// handle button presses
void handleButtonPress() {
  // LEFT
  if (digitalRead(BUTTON_LEFT) == PRESS_LEVEL && !left_pressed) {
    left_pressed = true;
    left_pressed_time = millis();
  }
  if (digitalRead(BUTTON_LEFT) != PRESS_LEVEL && left_pressed) {
    if (millis() - left_pressed_time > BOUNCE_DELAY) {
      moveLeft();
    }
    left_pressed = false;
  }
  // RIGHT
  if (digitalRead(BUTTON_RIGHT) == PRESS_LEVEL && !right_pressed) {
    right_pressed = true;
    right_pressed_time = millis();
  }
  if (digitalRead(BUTTON_RIGHT) != PRESS_LEVEL && right_pressed) {
    if (millis() - right_pressed_time > BOUNCE_DELAY) {
      moveRight();
    }
    right_pressed = false;
  }
  // ROTATE
/*  //for separate button Rotate
  if (digitalRead(BUTTON_ROTATE) == PRESS_LEVEL && rotate_pressed) {
    rotate_pressed = true;
    rotate_pressed_time = millis();
    //rotateClockwise();
  }
  if (digitalRead(BUTTON_ROTATE) != PRESS_LEVEL && rotate_pressed) {
    if (millis() - rotate_pressed_time > BOUNCE_DELAY) {
      rotateClockwise();
    }
    rotate_pressed = false;
  }
*/
  if ((digitalRead(BUTTON_LEFT) == PRESS_LEVEL) && (digitalRead(BUTTON_RIGHT) == PRESS_LEVEL) && !rotate_pressed) {
    rotate_pressed = true;
    rotate_pressed_time = millis();
    //rotateClockwise();
  }
  if ((digitalRead(BUTTON_LEFT) != PRESS_LEVEL) && (digitalRead(BUTTON_RIGHT) != PRESS_LEVEL) && rotate_pressed) {
    if (millis() - rotate_pressed_time > BOUNCE_DELAY) {
      rotateClockwise();
    }
    rotate_pressed = false;
  }
// DROP
  if (digitalRead(BUTTON_DROP) == PRESS_LEVEL) {
    drop_pressed = true;
    drop_pressed_time = millis();
    //drop();
  }
  if (digitalRead(BUTTON_DROP) != PRESS_LEVEL && drop_pressed) {
    if (millis() - drop_pressed_time > BOUNCE_DELAY) {
      drop();
    }
    drop_pressed = false;
  }
}

// Initialize the game board
void initBoard() {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      board[i][j] = 0;
    }
  }
}
// update score on TFT
void drawScore() {
//  tft.setRotation(3);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.print("Score: ");
  tft.setTextColor(TFT_YELLOW);
  tft.print(score);
//  tft.setRotation(0);
}

void GameOver() {
  // draw game over message on TFT
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, TFT_HEIGHT / 2 - 10);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(2);
  tft.print("GAME OVER");
  score = 0;
}

void setup() {
  
//  Serial.begin(115200); // initialize serial port for debug

  // Initialize the TFT display
  tft.begin();
  tft.setRotation(0);
  
  // Initialize the buttons pins
  //IF USE SENSOR KEYS - CHANGE ON 'INPUT' and  define ACTIVE_LEVEL HIGH
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_ROTATE, INPUT_PULLUP);
  pinMode(BUTTON_DROP, INPUT_PULLUP);
  
  // Seed the random number generator
  randomSeed(analogRead(34));
  initBoard();
  generateNewBlock();
}

void loop() {

  handleButtonPress();

  // get current time
  unsigned long current_time = millis();
  
  // check if it's time to move block down
  if (current_time - previous_time > speed) {
    // Move the block down by one row
    currentBlockY++;
    // Check if the block is valid in its new position
    if (!isValidBlock(currentBlockX, currentBlockY, currentBlockType, currentBlockOrientation)) {
      
      // If the block is not valid, move it back up and add it to the board
      currentBlockY--;
      addBlockToBoard(currentBlockX, currentBlockY, currentBlockType, currentBlockOrientation);
      clearCompletedRows();
      generateNewBlock();
      if (!isValidBlock(currentBlockX, currentBlockY, currentBlockType, currentBlockOrientation)) {
        // GAME OVER
        GameOver();
        delay(2000);
        initBoard();
        return;
      }
    }
    previous_time = current_time;
   
  // Draw the game board and the falling block
  drawBoard();
  drawFallingBlock();
/*  
  // Print debug information to the serial monitor 
  Serial.print("Current block type: ");
  Serial.println(currentBlockType);
  Serial.print("Current block orientation: ");
  Serial.println(currentBlockOrientation);
  Serial.print("Current block position: (");
  Serial.print(currentBlockX);
  Serial.print(", ");
  Serial.print(currentBlockY);
  Serial.print(", ");
  Serial.print(analogRead(34));
  Serial.println(")");
*/  
  }
}
