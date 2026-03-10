/*
  Stacker Game
  Made by Bernie Gampel
  Made on 03/06/2026
  Updated on 03/09/2026
*/

#include "LedControl.h"
#include "pitches.h"

// Pins on 4x8x8 display, (DIN, CLK, CS, # of displays)
LedControl lc = LedControl(12, 10, 11, 4);

// Button Pin in Arduino
const int buttonPin = 2;

// Buzzer Pin
const int buzzerPin = 8;

// Game Variables
byte grid[32][8];             // creating array
int row = 0;                  // where the block is
int blockStart = 0;           // left edge start
int blockSize = 4;            // how big is initial block
int direction = 1;            // right or left
const int startSpeed = 350;   // starting speed ***LOWER IS FASTER***
int speed = startSpeed;
int perfectStreak = 0;        // counts consecutive perfect stacks

// 8x8 font library - each letter is 8 rows of 8 bits
const byte FONT_8x8[][8] = {
  { 0b00111100, 0b01100110, 0b11000011, 0b11000011, 0b11111111, 0b11000011, 0b11000011, 0b11000011 }, // A
  { 0b11111110, 0b11000011, 0b11000011, 0b11111110, 0b11000011, 0b11000011, 0b11000011, 0b11111110 }, // B
  { 0b00111110, 0b11000011, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000011, 0b00111110 }, // C
  { 0b11111100, 0b11000110, 0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b11000110, 0b11111100 }, // D
  { 0b11111111, 0b11000000, 0b11000000, 0b11111110, 0b11000000, 0b11000000, 0b11000000, 0b11111111 }, // E
  { 0b11111111, 0b11000000, 0b11000000, 0b11111110, 0b11000000, 0b11000000, 0b11000000, 0b11000000 }, // F
  { 0b00111110, 0b11000011, 0b11000000, 0b11000000, 0b11001111, 0b11000011, 0b11000011, 0b00111110 }, // G
  { 0b11000011, 0b11000011, 0b11000011, 0b11111111, 0b11111111, 0b11000011, 0b11000011, 0b11000011 }, // H
  { 0b11111111, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b11111111 }, // I
  { 0b00001111, 0b00000011, 0b00000011, 0b00000011, 0b00000011, 0b11000011, 0b11000011, 0b01111110 }, // J
  { 0b11000110, 0b11001100, 0b11011000, 0b11110000, 0b11110000, 0b11011000, 0b11001100, 0b11000110 }, // K
  { 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11111111 }, // L
  { 0b11000011, 0b11100111, 0b11111111, 0b11011011, 0b11000011, 0b11000011, 0b11000011, 0b11000011 }, // M
  { 0b11000011, 0b11100011, 0b11110011, 0b11011011, 0b11001111, 0b11000111, 0b11000011, 0b11000011 }, // N
  { 0b00111100, 0b11000110, 0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b11000110, 0b00111100 }, // O
  { 0b11111110, 0b11000011, 0b11000011, 0b11111110, 0b11000000, 0b11000000, 0b11000000, 0b11000000 }, // P
  { 0b00111100, 0b11000110, 0b11000011, 0b11000011, 0b11001011, 0b11000110, 0b11000110, 0b00111011 }, // Q
  { 0b11111110, 0b11000011, 0b11000011, 0b11111110, 0b11011000, 0b11001100, 0b11000110, 0b11000011 }, // R
  { 0b00111110, 0b11000011, 0b11000000, 0b01111110, 0b00000011, 0b00000011, 0b11000011, 0b01111110 }, // S
  { 0b11111111, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000 }, // T
  { 0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b00111110 }, // U
  { 0b11000011, 0b11000011, 0b11000011, 0b01100110, 0b01100110, 0b00111100, 0b00111100, 0b00011000 }, // V
  { 0b11000011, 0b11000011, 0b11000011, 0b11011011, 0b11111111, 0b11100111, 0b11000011, 0b11000011 }, // W
  { 0b11000011, 0b01100110, 0b00111100, 0b00011000, 0b00011000, 0b00111100, 0b01100110, 0b11000011 }, // X
  { 0b11000011, 0b01100110, 0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000 }, // Y
  { 0b11111111, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b11000000, 0b11111111 }, // Z
};

// note pitch table - pitch rises with each row to build tension
const int rowNotes[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4,
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5,
  NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6,
  NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7,
  NOTE_C8, NOTE_D8, NOTE_C8, NOTE_D8  // loop back for last 2 rows
};

// helper to get letter index (A=0, B=1, etc.)
int letterIndex(char c) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= 'a' && c <= 'z') return c - 'a';
  return -1; // unknown character, blank display
}

// figures out which display and LED to talk to
void setPixel(int r, int c, bool state) {
  if (r < 0 || r > 31 || c < 0 || c > 7) return;
  int display = 3 - (r / 8); // determines display 0-3
  int localRow = r % 8;       // gives row within display
  lc.setLed(display, c, localRow, state);
}

// draws a full 8x8 letter on a given display (0=top, 3=bottom)
void drawLetterOnDisplay(int displayIndex, char letter) {
  int idx = letterIndex(letter);
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      bool lit = idx >= 0 ? (FONT_8x8[idx][7 - r] >> (7 - c)) & 1 : false;
      setPixel((3 - displayIndex) * 8 + r, c, lit); // flipped display mapping
    }
  }
}

// --- SOUND FUNCTIONS ---

// short satisfying two note click on successful stack
void soundStack() {
  tone(buzzerPin, NOTE_G4, 40);
  delay(50);
  tone(buzzerPin, NOTE_C5, 40);
}

// two quick high beeps for a perfect stack
void soundPerfect() {
  tone(buzzerPin, NOTE_E6, 80);
  delay(100);
  tone(buzzerPin, NOTE_G6, 80);
}

// short low buzz when you lose a dot
void soundHit() {
  tone(buzzerPin, NOTE_C2, 100);
}

// sad descending three note tone for game over
void soundGameOver() {
  tone(buzzerPin, NOTE_G4, 200);
  delay(250);
  tone(buzzerPin, NOTE_E4, 200);
  delay(250);
  tone(buzzerPin, NOTE_C4, 400);
  delay(450);
}

// ascending three note fanfare for win
void soundWin() {
  tone(buzzerPin, NOTE_C5, 150);
  delay(180);
  tone(buzzerPin, NOTE_E5, 150);
  delay(180);
  tone(buzzerPin, NOTE_G5, 300);
  delay(350);
}

// short startup jingle on power on
void soundStartup() {
  tone(buzzerPin, NOTE_C5, 100);
  delay(120);
  tone(buzzerPin, NOTE_E5, 100);
  delay(120);
  tone(buzzerPin, NOTE_G5, 100);
  delay(120);
  tone(buzzerPin, NOTE_C6, 200);
  delay(250);
}

// --- ANIMATION FUNCTIONS ---

// randomly fills all pixels then randomly clears them
void startupAnimation() {
  // build a shuffled list of all 256 pixel positions
  int pixels[256];
  for (int i = 0; i < 256; i++) pixels[i] = i;

  // fisher-yates shuffle to randomize order
  randomSeed(analogRead(0));
  for (int i = 255; i > 0; i--) {
    int j = random(i + 1);
    int temp = pixels[i];
    pixels[i] = pixels[j];
    pixels[j] = temp;
  }

  // light up pixels one by one in random order
  for (int i = 0; i < 256; i++) {
    int r = pixels[i] / 8;
    int c = pixels[i] % 8;
    setPixel(r, c, true);
    delay(10);
  }

  delay(500); // hold full screen

  // shuffle again for random clear order
  for (int i = 255; i > 0; i--) {
    int j = random(i + 1);
    int temp = pixels[i];
    pixels[i] = pixels[j];
    pixels[j] = temp;
  }

  // turn off pixels one by one in random order
  for (int i = 0; i < 256; i++) {
    int r = pixels[i] / 8;
    int c = pixels[i] % 8;
    setPixel(r, c, false);
    delay(10);
  }

  delay(300); // brief pause before game starts
}

// win snake: fast, celebratory, sweeps down then back up
void snakeAnimation() {
  // snake down - light up
  for (int r = 0; r < 32; r++) {
    if (r % 2 == 0) {
      for (int c = 0; c < 8; c++) { setPixel(r, c, true); delay(8); }
    } else {
      for (int c = 7; c >= 0; c--) { setPixel(r, c, true); delay(8); }
    }
  }
  delay(300);

  // snake back up - turn off
  for (int r = 31; r >= 0; r--) {
    if (r % 2 == 0) {
      for (int c = 7; c >= 0; c--) { setPixel(r, c, false); delay(8); }
    } else {
      for (int c = 0; c < 8; c++) { setPixel(r, c, false); delay(8); }
    }
  }
  delay(300);
}

// lose snake: slow, ominous, creeps up from bottom then dies top to bottom
void snakeAnimationLose() {
  // snake creeps up from bottom - slow and ominous
  for (int r = 31; r >= 0; r--) {
    if (r % 2 == 0) {
      for (int c = 0; c < 8; c++) { setPixel(r, c, true); delay(20); }
    } else {
      for (int c = 7; c >= 0; c--) { setPixel(r, c, true); delay(20); }
    }
  }

  delay(600); // hold the full lit screen - heavy, ominous pause

  // everything dies out row by row top to bottom - slow drip
  for (int r = 0; r < 32; r++) {
    for (int c = 0; c < 8; c++)
      setPixel(r, c, false);
    delay(60);
  }
  delay(400);
}

// displays GAME then OVER
void gameOver() {
  // show GAME on all 4 displays (one letter per display)
  drawLetterOnDisplay(0, 'G');
  drawLetterOnDisplay(1, 'A');
  drawLetterOnDisplay(2, 'M');
  drawLetterOnDisplay(3, 'E');
  delay(1000);

  // show OVER on all 4 displays
  drawLetterOnDisplay(0, 'O');
  drawLetterOnDisplay(1, 'V');
  drawLetterOnDisplay(2, 'E');
  drawLetterOnDisplay(3, 'R');
  delay(1000);

  // clear all displays
  for (int d = 0; d < 4; d++) drawLetterOnDisplay(d, ' ');
}

void gameOverAnimation() {
  // Step 1: play game over sound
  soundGameOver();

  // Step 2: slow ominous snake creeps up then dies
  snakeAnimationLose();

  // Step 3: display GAME OVER
  gameOver();
  delay(500);

  // reset game state
  for (int r = 0; r < 32; r++)
    for (int c = 0; c < 8; c++)
      grid[r][c] = 0;

  row = 0;
  blockStart = 0;
  blockSize = 4;
  direction = 1;
  speed = startSpeed;
  perfectStreak = 0; // reset perfect streak
}

void gameWinAnimation() {
  // Step 1: play win sound
  soundWin();

  // Step 2: fast celebratory snake
  snakeAnimation();

  // Step 3: flash all LEDs 3 times
  for (int flash = 0; flash < 3; flash++) {
    for (int r = 0; r < 32; r++)
      for (int c = 0; c < 8; c++)
        setPixel(r, c, true);
    delay(200);
    for (int r = 0; r < 32; r++)
      for (int c = 0; c < 8; c++)
        setPixel(r, c, false);
    delay(200);
  }

  // Step 4: display W, I, N on displays 1-3, blank on display 4
  drawLetterOnDisplay(0, 'W');
  drawLetterOnDisplay(1, 'I');
  drawLetterOnDisplay(2, 'N');
  drawLetterOnDisplay(3, ' ');

  // flash WIN 2 times
  for (int flash = 0; flash < 2; flash++) {
    delay(300);
    for (int d = 0; d < 4; d++) drawLetterOnDisplay(d, ' ');
    delay(300);
    drawLetterOnDisplay(0, 'W');
    drawLetterOnDisplay(1, 'I');
    drawLetterOnDisplay(2, 'N');
    drawLetterOnDisplay(3, ' ');
  }

  delay(1000);

  // clear all displays
  for (int d = 0; d < 4; d++) drawLetterOnDisplay(d, ' ');

  // reset game state
  for (int r = 0; r < 32; r++)
    for (int c = 0; c < 8; c++)
      grid[r][c] = 0;

  row = 0;
  blockStart = 0;
  blockSize = 4;
  direction = 1;
  speed = startSpeed;
  perfectStreak = 0; // reset perfect streak
}

// loops through grid and calls setPixel()
void drawGrid() {
  for (int r = 0; r < 32; r++) {
    for (int c = 0; c < 8; c++) {
      setPixel(r, c, grid[r][c]);
    }
  }
}

// turn off all LEDs in row
void clearRow(int r) {
  for (int c = 0; c < 8; c++) {
    grid[r][c] = 0;
    setPixel(r, c, false);
  }
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // set button default to high, low when pressed
  pinMode(buzzerPin, OUTPUT);       // set buzzer pin as output

  // initialize display
  for (int i = 0; i < 4; i++) {
    lc.shutdown(i, false);  // turn on display
    lc.setIntensity(i, 1);  // set brightness (0-15)
    lc.clearDisplay(i);     // clears all LEDs
  }

  soundStartup();     // play startup jingle
  startupAnimation(); // run startup animation once on power on
}

void loop() {

  // clears current row
  clearRow(row);

  // light up LED based on block size and block position
  for (int i = 0; i < blockSize; i++) {
    grid[row][blockStart + i] = 1;
    setPixel(row, blockStart + i, true);
  }

  // wait speed milliseconds before doing anything else
  delay(speed);

  // check if button is pressed
  if (digitalRead(buttonPin) == LOW) {
    // check overlap with previous row
    if (row > 0) {
      int newSize = 0;
      int newStart = -1;
      for (int i = 0; i < blockSize; i++) {
        if (grid[row - 1][blockStart + i]) {
          if (newStart == -1) newStart = blockStart + i;
          newSize++;
        } else {
          grid[row][blockStart + i] = 0;
          setPixel(row, blockStart + i, false);
        }
      }

      // flash remaining block and play hit sound if we lost any dots
      if (newSize < blockSize) {
        soundHit();          // play hit sound
        perfectStreak = 0;   // reset perfect streak on hit
        for (int flash = 0; flash < 3; flash++) {
          for (int i = 0; i < newSize; i++)
            setPixel(row, newStart + i, false);
          delay(80);
          for (int i = 0; i < newSize; i++)
            setPixel(row, newStart + i, true);
          delay(80);
        }
      } else {
        soundPerfect();  // play perfect stack sound if no dots lost
        perfectStreak++; // increment perfect streak
      }

      blockSize = newSize;

      if (blockSize == 0) {
        gameOverAnimation(); // play lose animation then restart game
        return;              // exit loop() immediately after reset
      }

      blockStart = newStart;
    } else {
      soundStack(); // play stack sound for first row
    }

    row++;

    // shrink block by 1 after every 10 consecutive perfect stacks
    if (perfectStreak >= 10 && blockSize > 1) {
      blockSize--;
      perfectStreak = 0; // reset counter after shrinking
    }

    blockStart = 0;
    direction = 1;
    speed -= 10;
    delay(300);

    // after row 10, 35% chance to flip direction each new row
    if (row > 10 && random(100) < 35) {
      direction = -direction; // flip direction
    }

    if (row > 31) {
      gameWinAnimation(); // play win animation then restart game
      return;
    }

    soundStack(); // play stack sound after successful placement
  }

  // change direction (bounce when touching walls)
  blockStart += direction;

  if (blockStart < 0) {
    blockStart = 0;
    direction = 1;
  } else if (blockStart + blockSize > 8) {
    blockStart = 8 - blockSize;
    direction = -1;
  }
}