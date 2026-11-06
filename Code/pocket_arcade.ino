#include <TFT_eSPI.h>
#include <string.h>
TFT_eSPI tft;

#define JOY_X 34
#define JOY_Y 35
#define JOY_SW 32
#define BTN_A 13
#define BTN_B 14
#define BTN_START 27
#define BTN_SELECT 26
#define BUZZER 25
#define LED_R 15
#define LED_G 2
#define LED_B 33

#define COLS 30
#define ROWS 15
#define CELL 10
#define FX 10
#define FY 45
#define MAX_SNAKE 450

#define T_COLS 10
#define T_ROWS 18
#define T_CELL 12
#define T_FX 100
#define T_FY 21

#define SI_ALIEN_ROWS 4
#define SI_ALIEN_COLS 8
#define SI_ALIEN_W 15
#define SI_ALIEN_H 10
#define SI_PLAYER_Y 207
#define SI_PLAYER_W 20
#define SI_PLAYER_H 12

enum Screen {
  MENU, SNAKE_SETUP, SNAKE_COUNTDOWN, SNAKE_GAME, GAME_OVER,
  PONG_SETUP, PONG_GAME, PONG_OVER,
  TETRIS_GAME, TETRIS_OVER,
  SPACE_GAME, SPACE_OVER, SETTINGS, JOY_CALIBRATION, BUTTON_TEST, PLACEHOLDER
};
Screen screen = MENU;

const char* games[] = {"SNAKE", "PONG", "TETRIS", "SPACE INVADERS", "SETTINGS"};
int selected = 0, menuYLock = 0;

bool oldA = HIGH, oldB = HIGH, oldStart = HIGH, oldSelect = HIGH;

#define TFT_BACKLIGHT_PIN -1
bool soundEnabled = true;
int brightnessLevel = 100;
int themeIndex = 0;
const uint16_t themeColors[] = {TFT_GREEN, TFT_CYAN, TFT_RED, TFT_MAGENTA};
const char* themeNames[] = {"GREEN", "BLUE", "RED", "PURPLE"};
uint16_t accentColor() { return themeColors[themeIndex]; }
int joyCenterX = 2048, joyCenterY = 2048;
int joyMinX = 0, joyMaxX = 4095, joyMinY = 0, joyMaxY = 4095;
int joyDeadZone = 800;
int joyLowX() { return max(100, joyCenterX - joyDeadZone); }
int joyHighX() { return min(3995, joyCenterX + joyDeadZone); }
int joyLowY() { return max(100, joyCenterY - joyDeadZone); }
int joyHighY() { return min(3995, joyCenterY + joyDeadZone); }
int settingsSelection = 0;
unsigned long settingsLastMove = 0;
unsigned long ledOffAt = 0;
bool ledFeedbackActive = false;

void setLED(bool r, bool g, bool b, unsigned long duration = 100) {
  digitalWrite(LED_R, r ? HIGH : LOW);
  digitalWrite(LED_G, g ? HIGH : LOW);
  digitalWrite(LED_B, b ? HIGH : LOW);
  ledFeedbackActive = (r || g || b);
  ledOffAt = millis() + duration;
}
void ledBlue() { setLED(false, false, true, 90); }
void ledGreen() { setLED(false, true, false, 450); }
void ledRed() { setLED(true, false, false, 650); }
void updateLED() {
  if (ledFeedbackActive && (long)(millis() - ledOffAt) >= 0) {
    digitalWrite(LED_R, LOW); digitalWrite(LED_G, LOW); digitalWrite(LED_B, LOW);
    ledFeedbackActive = false;
  }
}

bool pressed(int pin, bool &oldState) {
  bool current = digitalRead(pin);
  bool result = oldState == HIGH && current == LOW;
  oldState = current;
  return result;
}
void syncButtons() {
  oldA = digitalRead(BTN_A);
  oldB = digitalRead(BTN_B);
  oldStart = digitalRead(BTN_START);
  oldSelect = digitalRead(BTN_SELECT);
}
void beep(int frequency, int duration) {
  if (soundEnabled) tone(BUZZER, frequency, duration);
}

void drawBootScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("FLUXO BOTS PRESENTS", 160, 45);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(3);
  tft.drawString("POCKET", 160, 90);
  tft.drawString("ARCADE", 160, 125);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("INITIALIZING SYSTEM...", 160, 165);
  tft.drawRoundRect(60, 185, 200, 16, 4, TFT_WHITE);
  for (int i = 0; i <= 192; i += 4) {
    tft.fillRect(64, 189, i, 8, TFT_GREEN);
    delay(30);
  }
  tft.fillRect(50, 210, 220, 20, TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("SYSTEM READY!", 160, 220);
  beep(1200, 100);
  delay(600);
}

void drawMenuItem(int i) {
  int y = 55 + i * 34;
  tft.fillRect(35, y, 250, 28, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  if (i == selected) {
    tft.fillRoundRect(35, y, 250, 28, 5, accentColor());
    tft.setTextColor(TFT_BLACK, accentColor());
  } else {
    tft.drawRoundRect(35, y, 250, 28, 5, TFT_DARKGREY);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  tft.setTextSize(2);
  tft.drawString(games[i], 160, y + 14);
}
void drawMenu() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.fillRect(0, 0, 320, 42, themeIndex == 0 ? TFT_DARKGREEN : accentColor());
  tft.setTextColor(TFT_WHITE, themeIndex == 0 ? TFT_DARKGREEN : accentColor());
  tft.setTextSize(2);
  tft.drawString("POCKET ARCADE", 160, 21);
  for (int i = 0; i < 5; i++) drawMenuItem(i);
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("UP/DOWN: MOVE   A: SELECT", 160, 235);
}

int sx[MAX_SNAKE], sy[MAX_SNAKE], snakeLength = 3;
int foodX, foodY, score = 0;
int direction = 1, nextDirection = 1;
bool turnLocked = false;
int speedLevel = 3;
const int speeds[] = {280, 220, 170, 120, 80};
unsigned long lastMove = 0, lastSetupInput = 0;
int setupOption = 0;

void drawSnakeSetupButtons() {
  tft.setTextDatum(MC_DATUM);
  tft.fillRect(65, 90, 190, 38, TFT_BLACK);
  if (setupOption == 0) {
    tft.fillRoundRect(65, 90, 190, 38, 6, TFT_GREEN);
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
  } else {
    tft.drawRoundRect(65, 90, 190, 38, 6, TFT_WHITE);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  tft.setTextSize(2);
  tft.drawString("PLAY", 160, 109);
  tft.fillRect(65, 140, 190, 38, TFT_BLACK);
  if (setupOption == 1) {
    tft.fillRoundRect(65, 140, 190, 38, 6, TFT_GREEN);
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
  } else {
    tft.drawRoundRect(65, 140, 190, 38, 6, TFT_WHITE);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  tft.drawString("SPEED", 110, 159);
  tft.setTextColor(setupOption == 1 ? TFT_BLACK : TFT_YELLOW,
                   setupOption == 1 ? TFT_GREEN : TFT_BLACK);
  tft.drawString(String(speedLevel), 220, 159);
}
void drawSnakeSetup() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(3);
  tft.drawString("SNAKE", 160, 35);
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("GAME CONFIGURATION", 160, 65);
  drawSnakeSetupButtons();
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("LEFT / RIGHT: CHANGE SPEED", 160, 198);
  tft.drawString("A: CONFIRM    B: BACK", 160, 220);
}
void spawnFood() {
  bool occupied;
  do {
    occupied = false;
    foodX = random(COLS);
    foodY = random(ROWS);
    for (int i = 0; i < snakeLength; i++)
      if (sx[i] == foodX && sy[i] == foodY) { occupied = true; break; }
  } while (occupied);
}
void drawCell(int x, int y, uint16_t color) {
  tft.fillRect(FX + x * CELL + 1, FY + y * CELL + 1, CELL - 2, CELL - 2, color);
}
void drawFood() {
  tft.fillCircle(FX + foodX * CELL + 5, FY + foodY * CELL + 5, 4, TFT_RED);
}
void drawSnakeScore() {
  tft.fillRect(100, 25, 120, 16, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("SCORE: " + String(score), 160, 32);
}
void drawGame() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("SNAKE", 160, 13);
  drawSnakeScore();
  tft.drawRect(FX - 1, FY - 1, COLS * CELL + 2, ROWS * CELL + 2, TFT_RED);
  drawFood();
  for (int i = 0; i < snakeLength; i++)
    drawCell(sx[i], sy[i], i == 0 ? TFT_GREEN : TFT_DARKGREEN);
}
void countdownSnake() {
  screen = SNAKE_COUNTDOWN;
  for (int i = 3; i >= 1; i--) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(7);
    tft.drawString(String(i), 160, 110);
    beep(700 + i * 150, 100);
    delay(700);
  }
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(5);
  tft.drawString("GO!", 160, 110);
  beep(1500, 150);
  delay(400);
  screen = SNAKE_GAME;
  drawGame();
  lastMove = millis();
  syncButtons();
}
void startSnake() {
  score = 0; snakeLength = 3;
  direction = 1; nextDirection = 1; turnLocked = false;
  sx[0] = 15; sy[0] = 7;
  sx[1] = 14; sy[1] = 7;
  sx[2] = 13; sy[2] = 7;
  spawnFood();
  countdownSnake();
}
void showGameOver() {
  ledRed();
  screen = GAME_OVER;
  noTone(BUZZER); beep(250, 400);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(3);
  tft.drawString("GAME OVER", 160, 70);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("SCORE: " + String(score), 160, 120);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("A: RESTART", 160, 170);
  tft.drawString("B: MAIN MENU", 160, 195);
}
void readGameDirection() {
  if (turnLocked) return;
  int x = analogRead(JOY_X), y = analogRead(JOY_Y);
  int candidate = -1;
  if (y < joyLowY()) candidate = 0;
  else if (x > joyHighX()) candidate = 1;
  else if (y > joyHighY()) candidate = 2;
  else if (x < joyLowX()) candidate = 3;
  if (candidate == -1 || (candidate + 2) % 4 == direction) return;
  if (candidate != direction) { nextDirection = candidate; turnLocked = true; }
}
void updateSnake() {
  direction = nextDirection;
  int nx = sx[0], ny = sy[0];
  if (direction == 0) ny--;
  if (direction == 1) nx++;
  if (direction == 2) ny++;
  if (direction == 3) nx--;
  if (nx < 0 || nx >= COLS || ny < 0 || ny >= ROWS) { showGameOver(); return; }
  bool eating = nx == foodX && ny == foodY;
  int limit = eating ? snakeLength : snakeLength - 1;
  for (int i = 0; i < limit; i++)
    if (sx[i] == nx && sy[i] == ny) { showGameOver(); return; }
  if (eating && snakeLength >= MAX_SNAKE) { showGameOver(); return; }
  int oldTailX = sx[snakeLength - 1], oldTailY = sy[snakeLength - 1];
  if (eating) { snakeLength++; score += 10; beep(1400, 70); ledBlue(); }
  for (int i = snakeLength - 1; i > 0; i--) { sx[i] = sx[i - 1]; sy[i] = sy[i - 1]; }
  sx[0] = nx; sy[0] = ny;
  if (eating) spawnFood();
  else drawCell(oldTailX, oldTailY, TFT_BLACK);
  drawCell(sx[1], sy[1], TFT_DARKGREEN);
  drawCell(sx[0], sy[0], TFT_GREEN);
  drawFood();
  if (eating) drawSnakeScore();
  turnLocked = false;
}

float ballX = 160, ballY = 120, ballVX = 2.5, ballVY = 1.5;
int playerPaddleY = 100, aiPaddleY = 100;
const int PADDLE_H = 40, PADDLE_W = 6;
int playerScore = 0, aiScore = 0;
const int WIN_SCORE = 5;
unsigned long lastPongUpdate = 0;
int prevBallX = 160, prevBallY = 120, prevPlayerY = 100, prevAiY = 100;

void drawPongSetup() {
  tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); tft.setTextSize(3);
  tft.drawString("PONG", 160, 35);
  tft.setTextColor(TFT_CYAN, TFT_BLACK); tft.setTextSize(1);
  tft.drawString("PLAYER VS COMPUTER", 160, 65);
  tft.fillRoundRect(65, 100, 190, 42, 6, TFT_GREEN);
  tft.setTextColor(TFT_BLACK, TFT_GREEN); tft.setTextSize(2);
  tft.drawString("PLAY", 160, 121);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(1);
  tft.drawString("FIRST TO 5 POINTS WINS", 160, 175);
  tft.drawString("JOYSTICK: MOVE", 160, 200);
  tft.drawString("A: PLAY    B: BACK", 160, 220);
}
void drawPongScores() {
  tft.fillRect(100, 6, 120, 26, TFT_BLACK);
  tft.setTextDatum(MC_DATUM); tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString(String(playerScore), 115, 18);
  tft.drawString(String(aiScore), 205, 18);
}
void drawPongCenterLine() {
  for (int y = 42; y < 220; y += 12) tft.drawFastVLine(160, y, 6, TFT_DARKGREY);
}
void drawPongStatic() {
  tft.fillScreen(TFT_BLACK);
  drawPongScores(); drawPongCenterLine();
  tft.drawFastHLine(5, 40, 310, TFT_RED);
  tft.drawFastHLine(5, 220, 310, TFT_RED);
  tft.fillRect(15, playerPaddleY, PADDLE_W, PADDLE_H, TFT_GREEN);
  tft.fillRect(299, aiPaddleY, PADDLE_W, PADDLE_H, TFT_CYAN);
  prevPlayerY = playerPaddleY; prevAiY = aiPaddleY;
  prevBallX = (int)ballX; prevBallY = (int)ballY;
  tft.fillCircle(prevBallX, prevBallY, 4, TFT_WHITE);
}
void movePaddle(int x, int oldY, int newY, uint16_t color) {
  if (oldY == newY) return;
  tft.fillRect(x, newY, PADDLE_W, PADDLE_H, color);
  if (newY < oldY) tft.fillRect(x, newY + PADDLE_H, PADDLE_W, oldY - newY, TFT_BLACK);
  else tft.fillRect(x, oldY, PADDLE_W, newY - oldY, TFT_BLACK);
}
void renderPong() {
  movePaddle(15, prevPlayerY, playerPaddleY, TFT_GREEN);
  movePaddle(299, prevAiY, aiPaddleY, TFT_CYAN);
  prevPlayerY = playerPaddleY; prevAiY = aiPaddleY;
  tft.fillCircle(prevBallX, prevBallY, 4, TFT_BLACK);
  if (prevBallX > 150 && prevBallX < 170) drawPongCenterLine();
  tft.fillRect(15, playerPaddleY, PADDLE_W, PADDLE_H, TFT_GREEN);
  tft.fillRect(299, aiPaddleY, PADDLE_W, PADDLE_H, TFT_CYAN);
  prevBallX = (int)ballX; prevBallY = (int)ballY;
  tft.fillCircle(prevBallX, prevBallY, 4, TFT_WHITE);
}
void resetPongBall(int dir) {
  ballX = 160; ballY = 120;
  ballVX = 2.5 * dir; ballVY = random(-15, 16) / 10.0;
  if (abs(ballVY) < 0.8) ballVY = 1.2;
}
void startPong() {
  playerScore = 0; aiScore = 0;
  playerPaddleY = 100; aiPaddleY = 100;
  resetPongBall(random(0, 2) == 0 ? -1 : 1);
  for (int i = 3; i >= 1; i--) {
    tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_GREEN, TFT_BLACK); tft.setTextSize(7);
    tft.drawString(String(i), 160, 110); beep(700 + i * 150, 100); delay(700);
  }
  tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); tft.setTextSize(5);
  tft.drawString("GO!", 160, 110); beep(1500, 150); delay(400);
  screen = PONG_GAME; lastPongUpdate = millis(); syncButtons(); drawPongStatic();
}
void showPongOver() {
  screen = PONG_OVER; noTone(BUZZER); tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  if (playerScore >= WIN_SCORE) {
    ledGreen();
    tft.setTextColor(TFT_GREEN, TFT_BLACK); tft.setTextSize(3);
    tft.drawString("YOU WIN!", 160, 65); beep(1500, 150); delay(180); beep(1800, 200);
  } else {
    ledRed();
    tft.setTextColor(TFT_RED, TFT_BLACK); tft.setTextSize(3);
    tft.drawString("YOU LOST!", 160, 65); beep(250, 400);
  }
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(2);
  tft.drawString(String(playerScore) + " - " + String(aiScore), 160, 120);
  tft.setTextColor(TFT_CYAN, TFT_BLACK); tft.setTextSize(1);
  tft.drawString("A: PLAY AGAIN", 160, 175); tft.drawString("B: MAIN MENU", 160, 200);
}
void updatePong() {
  int joy = analogRead(JOY_Y);
  if (joy < joyLowY()) playerPaddleY -= 5;
  else if (joy > joyHighY()) playerPaddleY += 5;
  playerPaddleY = constrain(playerPaddleY, 42, 178);
  int aiTarget = ballY - PADDLE_H / 2;
  if (aiPaddleY < aiTarget) aiPaddleY += 3;
  else if (aiPaddleY > aiTarget) aiPaddleY -= 3;
  aiPaddleY = constrain(aiPaddleY, 42, 178);
  ballX += ballVX; ballY += ballVY;
  if (ballY <= 45 || ballY >= 215) { ballVY = -ballVY; ballY = constrain(ballY, 45, 215); beep(800, 25); }
  if (ballVX < 0 && ballX <= 21 && ballX >= 15 &&
      ballY >= playerPaddleY - 4 && ballY <= playerPaddleY + PADDLE_H + 4) {
    ballVX = abs(ballVX) + 0.15;
    float hitPosition = (ballY - (playerPaddleY + PADDLE_H / 2)) / 20.0;
    ballVY = hitPosition * 3.5; beep(1200, 35); ledBlue();
  }
  if (ballVX > 0 && ballX >= 295 && ballX <= 305 &&
      ballY >= aiPaddleY - 4 && ballY <= aiPaddleY + PADDLE_H + 4) {
    ballVX = -(abs(ballVX) + 0.15);
    float hitPosition = (ballY - (aiPaddleY + PADDLE_H / 2)) / 20.0;
    ballVY = hitPosition * 3.5; beep(1200, 35); ledBlue();
  }
  if (ballX > 320) {
    playerScore++; beep(1500, 100);
    if (playerScore >= WIN_SCORE) { showPongOver(); return; }
    drawPongScores(); resetPongBall(-1);
  }
  if (ballX < 0) {
    aiScore++; beep(300, 150);
    if (aiScore >= WIN_SCORE) { showPongOver(); return; }
    drawPongScores(); resetPongBall(1);
  }
  renderPong();
}

uint16_t board[T_ROWS][T_COLS];
uint16_t shown[T_ROWS][T_COLS];
const byte shapes[7][4][4] = {
  {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
  {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
  {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
  {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
  {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
  {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
  {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}
};
const byte pieceSize[7] = {4,3,3,2,3,3,3};
const uint16_t pieceColors[7] = {TFT_CYAN,TFT_BLUE,TFT_ORANGE,TFT_YELLOW,TFT_GREEN,TFT_MAGENTA,TFT_RED};
byte activeShape[4][4];
int pieceType = 0, pieceX = 3, pieceY = 0;
int tetrisScore = 0, clearedLines = 0, tetrisLevel = 1;
int shownScore = -1, shownLines = -1, shownLevel = -1;
bool tetrisPaused = false, gameOverFlag = false;
unsigned long lastFall = 0, lastHorizontal = 0;
int fallSpeed = 600;

bool validPiece(int px, int py, const byte shape[4][4]) {
  for (int y = 0; y < 4; y++) for (int x = 0; x < 4; x++) {
    if (!shape[y][x]) continue;
    int bx = px + x, by = py + y;
    if (bx < 0 || bx >= T_COLS || by >= T_ROWS) return false;
    if (by >= 0 && board[by][bx]) return false;
  }
  return true;
}
bool canMove(int nx, int ny) { return validPiece(nx, ny, activeShape); }
bool rotatePiece() {
  if (pieceType == 3) return false;
  int n = pieceSize[pieceType];
  byte rotated[4][4] = {{0}};
  for (int y = 0; y < n; y++) for (int x = 0; x < n; x++)
    rotated[y][x] = activeShape[n - 1 - x][y];
  const int kicks[3] = {0, -1, 1};
  for (int k = 0; k < 3; k++) if (validPiece(pieceX + kicks[k], pieceY, rotated)) {
    pieceX += kicks[k];
    for (int y = 0; y < 4; y++) for (int x = 0; x < 4; x++) activeShape[y][x] = rotated[y][x];
    beep(1100, 40); return true;
  }
  return false;
}
void spawnPiece() {
  pieceType = random(0, 7); pieceX = 3; pieceY = 0;
  for (int y = 0; y < 4; y++) for (int x = 0; x < 4; x++) activeShape[y][x] = shapes[pieceType][y][x];
  if (!canMove(pieceX, pieceY)) gameOverFlag = true;
}
void drawTetrisCell(int x, int y, uint16_t color) {
  tft.fillRect(T_FX + x * T_CELL + 1, T_FY + y * T_CELL + 1, T_CELL - 2, T_CELL - 2, color);
}
void drawTetrisStats() {
  tft.setTextDatum(MC_DATUM); tft.setTextSize(2);
  if (tetrisScore != shownScore) {
    tft.fillRect(2, 76, 92, 18, TFT_BLACK); tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(String(tetrisScore), 48, 85); shownScore = tetrisScore;
  }
  if (clearedLines != shownLines) {
    tft.fillRect(2, 136, 92, 18, TFT_BLACK); tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(String(clearedLines), 48, 145); shownLines = clearedLines;
  }
  if (tetrisLevel != shownLevel) {
    tft.fillRect(2, 196, 92, 18, TFT_BLACK); tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString(String(tetrisLevel), 48, 205); shownLevel = tetrisLevel;
  }
}
void drawTetrisFrame() {
  tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); tft.setTextSize(2); tft.drawString("TETRIS", 160, 10);
  tft.setTextColor(TFT_CYAN, TFT_BLACK); tft.setTextSize(1);
  tft.drawString("SCORE", 48, 65); tft.drawString("LINES", 48, 125); tft.drawString("LEVEL", 48, 185);
  tft.drawRect(T_FX - 1, T_FY - 1, T_COLS * T_CELL + 2, T_ROWS * T_CELL + 2, TFT_DARKGREY);
  for (int x = 1; x < T_COLS; x++) tft.drawFastVLine(T_FX + x * T_CELL, T_FY, T_ROWS * T_CELL, TFT_DARKGREY);
  for (int y = 1; y < T_ROWS; y++) tft.drawFastHLine(T_FX, T_FY + y * T_CELL, T_COLS * T_CELL, TFT_DARKGREY);
  for (int y = 0; y < T_ROWS; y++) for (int x = 0; x < T_COLS; x++) shown[y][x] = 0;
  shownScore = shownLines = shownLevel = -1; drawTetrisStats();
}
void renderTetris() {
  static uint16_t view[T_ROWS][T_COLS];
  for (int y = 0; y < T_ROWS; y++) for (int x = 0; x < T_COLS; x++) view[y][x] = board[y][x];
  for (int y = 0; y < 4; y++) for (int x = 0; x < 4; x++) if (activeShape[y][x]) {
    int bx = pieceX + x, by = pieceY + y;
    if (by >= 0 && by < T_ROWS && bx >= 0 && bx < T_COLS) view[by][bx] = pieceColors[pieceType];
  }
  for (int y = 0; y < T_ROWS; y++) for (int x = 0; x < T_COLS; x++)
    if (view[y][x] != shown[y][x]) { drawTetrisCell(x, y, view[y][x]); shown[y][x] = view[y][x]; }
  drawTetrisStats();
}
void drawTetrisPaused() {
  tft.fillRoundRect(105, 100, 110, 45, 5, TFT_DARKGREEN);
  tft.setTextDatum(MC_DATUM); tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.setTextSize(2); tft.drawString("PAUSED", 160, 122);
}
void clearLines() {
  int count = 0;
  for (int y = T_ROWS - 1; y >= 0; y--) {
    bool full = true;
    for (int x = 0; x < T_COLS; x++) if (!board[y][x]) { full = false; break; }
    if (full) {
      count++;
      for (int row = y; row > 0; row--) for (int x = 0; x < T_COLS; x++) board[row][x] = board[row - 1][x];
      for (int x = 0; x < T_COLS; x++) board[0][x] = 0;
      y++;
    }
  }
  if (count > 0) {
    ledGreen();
    int points[] = {0,100,300,500,800};
    tetrisScore += points[count] * tetrisLevel;
    clearedLines += count; tetrisLevel = 1 + clearedLines / 5;
    fallSpeed = max(100, 600 - (tetrisLevel - 1) * 50); beep(1500, 100);
  }
}
void lockPiece() {
  for (int y = 0; y < 4; y++) for (int x = 0; x < 4; x++) if (activeShape[y][x]) {
    int bx = pieceX + x, by = pieceY + y;
    if (by < 0) { gameOverFlag = true; return; }
    if (by < T_ROWS && bx >= 0 && bx < T_COLS) board[by][bx] = pieceColors[pieceType];
  }
  beep(500, 30); ledBlue(); clearLines(); spawnPiece();
}
void dropPiece() {
  if (canMove(pieceX, pieceY + 1)) pieceY++;
  else lockPiece();
}
void showTetrisOver();
void startTetris() {
  memset(board, 0, sizeof(board));
  tetrisScore = 0; clearedLines = 0; tetrisLevel = 1; fallSpeed = 600;
  tetrisPaused = false; gameOverFlag = false;
  lastFall = millis(); lastHorizontal = millis(); spawnPiece();
  screen = TETRIS_GAME; syncButtons();
  if (gameOverFlag) showTetrisOver();
  else { drawTetrisFrame(); renderTetris(); }
}
void showTetrisOver() {
  ledRed();
  screen = TETRIS_OVER; noTone(BUZZER); beep(250, 400);
  tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_RED, TFT_BLACK); tft.setTextSize(3); tft.drawString("GAME OVER", 160, 65);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(2); tft.drawString("SCORE: " + String(tetrisScore), 160, 115);
  tft.setTextColor(TFT_CYAN, TFT_BLACK); tft.setTextSize(1); tft.drawString("LINES: " + String(clearedLines), 160, 145);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); tft.drawString("A: RESTART", 160, 185); tft.drawString("B: MAIN MENU", 160, 210);
}
void updateTetris() {
  if (tetrisPaused) return;
  int x = analogRead(JOY_X), y = analogRead(JOY_Y);
  if (millis() - lastHorizontal > 115) {
    if (x < joyLowX() && canMove(pieceX - 1, pieceY)) { pieceX--; lastHorizontal = millis(); }
    else if (x > joyHighX() && canMove(pieceX + 1, pieceY)) { pieceX++; lastHorizontal = millis(); }
  }
  if (y > joyHighY()) {
    if (millis() - lastFall > 65) {
      if (canMove(pieceX, pieceY + 1)) { pieceY++; tetrisScore++; }
      else lockPiece();
      lastFall = millis();
    }
  } else if (millis() - lastFall >= (unsigned long)fallSpeed) {
    dropPiece(); lastFall = millis();
  }
}

bool siAliens[SI_ALIEN_ROWS][SI_ALIEN_COLS];
int siBaseX = 25, siBaseY = 52, siDirection = 1;
int siPlayerX = 150, siOldPlayerX = 150;
int siScore = 0, siLives = 3, siWave = 1;
bool siPlayerBullet = false, siEnemyBullet = false, siPaused = false;
int siBulletX = 0, siBulletY = 0, siOldBulletY = -1;
int siEnemyBulletX = 0, siEnemyBulletY = 0, siOldEnemyBulletY = -1;
bool siShields[3][2][5];
unsigned long siLastAlienMove = 0, siLastUpdate = 0, siLastEnemyShot = 0;
int siAlienDelay = 45, siShownScore = -1, siShownLives = -1, siShownWave = -1;
int siAliensRemaining = SI_ALIEN_ROWS * SI_ALIEN_COLS;

void showSpaceInvadersOver();

void drawSIAlien(int x, int y, int type) {
  uint16_t color = type == 0 ? TFT_GREEN : type == 1 ? TFT_CYAN : type == 2 ? TFT_YELLOW : TFT_MAGENTA;
  tft.fillRect(x + 3, y, 9, 3, color);
  tft.fillRect(x, y + 3, 15, 5, color);
  tft.fillRect(x + 2, y + 8, 3, 2, color);
  tft.fillRect(x + 10, y + 8, 3, 2, color);
  tft.fillRect(x + 3, y + 4, 2, 2, TFT_BLACK);
  tft.fillRect(x + 10, y + 4, 2, 2, TFT_BLACK);
}
void eraseSIAlien(int x, int y) {
  tft.fillRect(x - 1, y - 1, SI_ALIEN_W + 2, SI_ALIEN_H + 2, TFT_BLACK);
}
int siAlienX(int col) { return siBaseX + col * 32; }
int siAlienY(int row) { return siBaseY + row * 20; }
void drawSIPlayer(int x) {
  int y = SI_PLAYER_Y;
  tft.fillRect(x + 8, y, 4, 3, TFT_GREEN);
  tft.fillRect(x + 5, y + 3, 10, 3, TFT_GREEN);
  tft.fillRect(x + 2, y + 6, 16, 3, TFT_GREEN);
  tft.fillRect(x, y + 9, 20, 3, TFT_GREEN);
}
void eraseSIPlayer(int x) {
  tft.fillRect(x - 1, SI_PLAYER_Y - 1, SI_PLAYER_W + 2, SI_PLAYER_H + 2, TFT_BLACK);
}
void drawSIStats() {
  tft.setTextDatum(MC_DATUM); tft.setTextSize(1);
  if (siScore != siShownScore) {
    tft.fillRect(5, 5, 85, 22, TFT_BLACK); tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("SCORE " + String(siScore), 45, 15); siShownScore = siScore;
  }
  if (siLives != siShownLives) {
    tft.fillRect(112, 5, 90, 22, TFT_BLACK); tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("LIVES " + String(siLives), 157, 15); siShownLives = siLives;
  }
  if (siWave != siShownWave) {
    tft.fillRect(230, 5, 85, 22, TFT_BLACK); tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("WAVE " + String(siWave), 272, 15); siShownWave = siWave;
  }
}
int siShieldX(int shield) { return 48 + shield * 90; }
int siShieldY() { return 178; }
void drawSIShieldCell(int shield, int row, int col) {
  int x = siShieldX(shield) + col * 5, y = siShieldY() + row * 5;
  tft.fillRect(x, y, 4, 4, siShields[shield][row][col] ? TFT_GREEN : TFT_BLACK);
}
void drawSIShields() {
  for (int s = 0; s < 3; s++) for (int r = 0; r < 2; r++) for (int c = 0; c < 5; c++)
    drawSIShieldCell(s, r, c);
}
bool hitSIShield(int x, int y) {
  for (int s = 0; s < 3; s++) for (int r = 0; r < 2; r++) for (int c = 0; c < 5; c++) {
    int cx = siShieldX(s) + c * 5, cy = siShieldY() + r * 5;
    if (siShields[s][r][c] && x >= cx && x <= cx + 4 && y >= cy && y <= cy + 4) {
      siShields[s][r][c] = false; drawSIShieldCell(s, r, c); beep(500, 20); return true;
    }
  }
  return false;
}
void drawSIFormation() {
  for (int r = 0; r < SI_ALIEN_ROWS; r++) for (int c = 0; c < SI_ALIEN_COLS; c++)
    if (siAliens[r][c]) drawSIAlien(siAlienX(c), siAlienY(r), r);
}
void moveSIFormation() {
  for (int r = 0; r < SI_ALIEN_ROWS; r++) for (int c = 0; c < SI_ALIEN_COLS; c++)
    if (siAliens[r][c]) eraseSIAlien(siAlienX(c), siAlienY(r));
  siBaseX += siDirection * 2;
  if (siBaseX >= 66 || siBaseX <= 15) {
    siDirection = -siDirection; siBaseY += 8; beep(350, 20);
  }
  for (int r = 0; r < SI_ALIEN_ROWS; r++) for (int c = 0; c < SI_ALIEN_COLS; c++)
    if (siAliens[r][c] && siAlienY(r) + SI_ALIEN_H >= 195) {
      showSpaceInvadersOver(); return;
    }
  drawSIFormation();
}
void drawSIPlayerBullet() {
  if (siPlayerBullet) tft.fillRect(siBulletX - 1, siBulletY, 3, 7, TFT_YELLOW);
}
void eraseSIPlayerBullet() {
  if (siOldBulletY >= 0) tft.fillRect(siBulletX - 1, siOldBulletY, 3, 7, TFT_BLACK);
}
void drawSIEnemyBullet() {
  if (siEnemyBullet) tft.fillRect(siEnemyBulletX - 1, siEnemyBulletY, 3, 7, TFT_RED);
}
void eraseSIEnemyBullet() {
  if (siOldEnemyBulletY >= 0) tft.fillRect(siEnemyBulletX - 1, siOldEnemyBulletY, 3, 7, TFT_BLACK);
}
void fireSIPlayerBullet() {
  if (siPlayerBullet) return;
  siPlayerBullet = true; siBulletX = siPlayerX + SI_PLAYER_W / 2;
  siBulletY = SI_PLAYER_Y - 7; siOldBulletY = -1; beep(1300, 25); ledBlue();
}
void fireSIEnemyBullet() {
  if (siEnemyBullet) return;
  int available[SI_ALIEN_COLS], count = 0;
  for (int c = 0; c < SI_ALIEN_COLS; c++) {
    for (int r = SI_ALIEN_ROWS - 1; r >= 0; r--) if (siAliens[r][c]) { available[count++] = c; break; }
  }
  if (!count) return;
  int col = available[random(count)], row = -1;
  for (int r = SI_ALIEN_ROWS - 1; r >= 0; r--) if (siAliens[r][col]) { row = r; break; }
  if (row < 0) return;
  siEnemyBullet = true; siEnemyBulletX = siAlienX(col) + SI_ALIEN_W / 2;
  siEnemyBulletY = siAlienY(row) + SI_ALIEN_H; siOldEnemyBulletY = -1;
}
void updateSIPlayerBullet() {
  if (!siPlayerBullet) return;
  eraseSIPlayerBullet(); siOldBulletY = siBulletY; siBulletY -= 6;
  if (siBulletY < 30) { siPlayerBullet = false; siOldBulletY = -1; return; }
  for (int r = 0; r < SI_ALIEN_ROWS; r++) for (int c = 0; c < SI_ALIEN_COLS; c++) {
    if (!siAliens[r][c]) continue;
    int ax = siAlienX(c), ay = siAlienY(r);
    if (siBulletX >= ax - 1 && siBulletX <= ax + SI_ALIEN_W + 1 &&
        siBulletY <= ay + SI_ALIEN_H && siBulletY + 7 >= ay) {
      eraseSIAlien(ax, ay); siAliens[r][c] = false; siAliensRemaining--;
      siScore += (4 - r) * 10; siPlayerBullet = false; siOldBulletY = -1;
      beep(1600, 35); drawSIStats();
      if (siAliensRemaining <= 0) {
        ledGreen();
        siWave++; siBaseX = 25; siBaseY = 52; siDirection = 1;
        siAlienDelay = max(18, 45 - (siWave - 1) * 5);
        siAliensRemaining = SI_ALIEN_ROWS * SI_ALIEN_COLS;
        for (int rr = 0; rr < SI_ALIEN_ROWS; rr++) for (int cc = 0; cc < SI_ALIEN_COLS; cc++) siAliens[rr][cc] = true;
        drawSIFormation(); drawSIStats(); beep(1800, 120);
      }
      return;
    }
  }
  drawSIPlayerBullet();
}
void updateSIEnemyBullet() {
  if (!siEnemyBullet) return;
  eraseSIEnemyBullet(); siOldEnemyBulletY = siEnemyBulletY; siEnemyBulletY += 4;
  if (siEnemyBulletY > 230) { siEnemyBullet = false; siOldEnemyBulletY = -1; return; }
  if (hitSIShield(siEnemyBulletX, siEnemyBulletY + 7)) {
    siEnemyBullet = false; siOldEnemyBulletY = -1; return;
  }
  if (siEnemyBulletX >= siPlayerX && siEnemyBulletX <= siPlayerX + SI_PLAYER_W &&
      siEnemyBulletY + 7 >= SI_PLAYER_Y && siEnemyBulletY <= SI_PLAYER_Y + SI_PLAYER_H) {
    siEnemyBullet = false; siOldEnemyBulletY = -1; siLives--; drawSIStats(); beep(180, 180);
    eraseSIPlayer(siPlayerX);
    if (siLives <= 0) { showSpaceInvadersOver(); return; }
    tft.drawRoundRect(siPlayerX - 3, SI_PLAYER_Y - 3, SI_PLAYER_W + 6, SI_PLAYER_H + 6, 3, TFT_RED);
    delay(100);
    tft.drawRoundRect(siPlayerX - 3, SI_PLAYER_Y - 3, SI_PLAYER_W + 6, SI_PLAYER_H + 6, 3, TFT_BLACK);
    drawSIPlayer(siPlayerX); return;
  }
  drawSIEnemyBullet();
}
void drawSpaceInvadersFrame() {
  tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); tft.setTextSize(2);
  tft.drawString("SPACE INVADERS", 160, 15);
  tft.drawFastHLine(5, 29, 310, TFT_DARKGREY);
  tft.drawFastHLine(5, 232, 310, TFT_DARKGREY);
  siShownScore = siShownLives = siShownWave = -1;
  drawSIStats(); drawSIShields(); drawSIFormation(); drawSIPlayer(siPlayerX);
}
void startSpaceInvaders() {
  siScore = 0; siLives = 3; siWave = 1;
  siBaseX = 25; siBaseY = 52; siDirection = 1; siAlienDelay = 45;
  siPlayerX = 150; siOldPlayerX = 150;
  siPlayerBullet = false; siEnemyBullet = false;
  siOldBulletY = -1; siOldEnemyBulletY = -1; siPaused = false;
  siAliensRemaining = SI_ALIEN_ROWS * SI_ALIEN_COLS;
  for (int r = 0; r < SI_ALIEN_ROWS; r++) for (int c = 0; c < SI_ALIEN_COLS; c++) siAliens[r][c] = true;
  for (int s = 0; s < 3; s++) for (int r = 0; r < 2; r++) for (int c = 0; c < 5; c++) siShields[s][r][c] = true;
  screen = SPACE_GAME; syncButtons();
  siLastAlienMove = millis(); siLastUpdate = millis(); siLastEnemyShot = millis();
  drawSpaceInvadersFrame();
}
void showSpaceInvadersOver() {
  ledRed();
  screen = SPACE_OVER; noTone(BUZZER); beep(250, 400);
  tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_RED, TFT_BLACK); tft.setTextSize(3); tft.drawString("GAME OVER", 160, 65);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(2); tft.drawString("SCORE: " + String(siScore), 160, 115);
  tft.setTextColor(TFT_CYAN, TFT_BLACK); tft.setTextSize(1); tft.drawString("WAVE: " + String(siWave), 160, 145);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); tft.drawString("A: RESTART", 160, 185); tft.drawString("B: MAIN MENU", 160, 210);
}
void drawSIPaused() {
  tft.fillRoundRect(100, 105, 120, 42, 5, TFT_DARKGREEN);
  tft.setTextDatum(MC_DATUM); tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.setTextSize(2); tft.drawString("PAUSED", 160, 126);
}
void updateSpaceInvaders() {
  if (siPaused) return;
  int joy = analogRead(JOY_X);
  if (joy < joyLowX()) siPlayerX -= 4;
  else if (joy > joyHighX()) siPlayerX += 4;
  siPlayerX = constrain(siPlayerX, 8, 292);
  if (siPlayerX != siOldPlayerX) {
    eraseSIPlayer(siOldPlayerX); drawSIPlayer(siPlayerX); siOldPlayerX = siPlayerX;
  }
  if (millis() - siLastAlienMove >= (unsigned long)siAlienDelay) {
    siLastAlienMove = millis(); moveSIFormation();
    if (screen != SPACE_GAME) return;
  }
  updateSIPlayerBullet();
  if (screen != SPACE_GAME) return;
  updateSIEnemyBullet();
  if (screen != SPACE_GAME) return;
  if (!siEnemyBullet && millis() - siLastEnemyShot > 650) {
    siLastEnemyShot = millis();
    if (random(0, 100) < 65) fireSIEnemyBullet();
  }
}

const char* settingsNames[] = {"SOUND", "BRIGHTNESS", "JOYSTICK CAL", "BUTTON TEST", "THEME"};
void drawSettings() {
  tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
  tft.fillRect(0, 0, 320, 42, themeIndex == 0 ? TFT_DARKGREEN : accentColor());
  tft.setTextColor(TFT_WHITE, themeIndex == 0 ? TFT_DARKGREEN : accentColor());
  tft.setTextSize(2); tft.drawString("SETTINGS", 160, 21);
  for (int i=0; i<5; i++) {
    int y=52+i*33;
    uint16_t c = (i==settingsSelection) ? accentColor() : TFT_DARKGREY;
    if (i==settingsSelection) tft.fillRoundRect(22,y,276,27,5,c);
    else tft.drawRoundRect(22,y,276,27,5,c);
    tft.setTextColor(i==settingsSelection ? TFT_BLACK : TFT_WHITE,
                     i==settingsSelection ? c : TFT_BLACK);
    tft.setTextSize(1); tft.drawString(settingsNames[i], 100, y+14);
    String value="";
    if (i==0) value=soundEnabled?"ON":"OFF";
    if (i==1) value=(TFT_BACKLIGHT_PIN < 0)?"NOT WIRED":String(brightnessLevel)+"%";
    if (i==2) value="CALIBRATE";
    if (i==3) value="OPEN";
    if (i==4) value=themeNames[themeIndex];
    tft.drawString(value, 235, y+14);
  }
  tft.setTextColor(TFT_CYAN,TFT_BLACK); tft.setTextSize(1);
  tft.drawString("UP/DOWN: SELECT   A: CHANGE   B: BACK",160,226);
}
void drawCalibrationPage() {
  tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
  tft.setTextColor(accentColor(),TFT_BLACK); tft.setTextSize(2);
  tft.drawString("JOYSTICK CALIBRATION",160,42);
  tft.setTextColor(TFT_WHITE,TFT_BLACK); tft.setTextSize(1);
  tft.drawString("RELEASE THE JOYSTICK",160,92);
  tft.drawString("KEEP IT CENTERED",160,112);
  tft.drawString("THEN PRESS A TO CALIBRATE",160,145);
  tft.drawString("B: BACK",160,205);
}
void performJoystickCalibration() {
  long totalX=0,totalY=0;
  for(int i=0;i<32;i++){ totalX+=analogRead(JOY_X); totalY+=analogRead(JOY_Y); delay(4); }
  joyCenterX=totalX/32; joyCenterY=totalY/32;
  joyDeadZone=800;
  tft.fillRect(20,165,280,25,TFT_BLACK);
  tft.setTextColor(TFT_GREEN,TFT_BLACK); tft.setTextSize(1);
  tft.drawString("CALIBRATED! CENTER: "+String(joyCenterX)+", "+String(joyCenterY),160,177);
  beep(1400,70);
}
void drawButtonTest() {
  tft.fillScreen(TFT_BLACK); tft.setTextDatum(MC_DATUM);
  tft.setTextColor(accentColor(),TFT_BLACK); tft.setTextSize(2); tft.drawString("BUTTON TEST",160,30);
  const int pins[5]={BTN_A,BTN_B,BTN_START,BTN_SELECT,JOY_SW};
  const char* labels[5]={"A","B","START","SELECT","JOYSTICK"};
  for(int i=0;i<5;i++){
    int y=65+i*29; bool down=(digitalRead(pins[i])==LOW);
    tft.fillRoundRect(65,y,190,23,4,down?TFT_GREEN:TFT_DARKGREY);
    tft.setTextColor(down?TFT_BLACK:TFT_WHITE,down?TFT_GREEN:TFT_DARKGREY);
    tft.setTextSize(1); tft.drawString(String(labels[i])+ (down?"  PRESSED":"  RELEASED"),160,y+12);
  }
  tft.setTextColor(TFT_CYAN,TFT_BLACK); tft.drawString("B + START: BACK",160,224);
}
void applyBrightness() {
#if TFT_BACKLIGHT_PIN >= 0
  analogWrite(TFT_BACKLIGHT_PIN, map(brightnessLevel,0,100,0,255));
#endif
}
void handleSettings(bool a, bool b, bool start) {
  int y=analogRead(JOY_Y);
  if(millis()-settingsLastMove>190){
    if(y<joyLowY()){settingsSelection=(settingsSelection+4)%5;settingsLastMove=millis();drawSettings();beep(950,25);}
    else if(y>joyHighY()){settingsSelection=(settingsSelection+1)%5;settingsLastMove=millis();drawSettings();beep(950,25);}
  }
  if(b){screen=MENU;menuYLock=0;drawMenu();syncButtons();return;}
  if(!(a||start)) return;
  switch(settingsSelection){
    case 0: soundEnabled=!soundEnabled; if(soundEnabled) beep(1300,60); else noTone(BUZZER); drawSettings(); break;
    case 1:
      if(TFT_BACKLIGHT_PIN < 0){
        tft.fillRect(15,205,290,20,TFT_BLACK);tft.setTextColor(TFT_YELLOW,TFT_BLACK);tft.setTextSize(1);
        tft.drawString("BACKLIGHT PIN NOT CONFIGURED",160,215);
      } else { brightnessLevel+=20;if(brightnessLevel>100)brightnessLevel=20;applyBrightness();drawSettings(); }
      break;
    case 2: screen=JOY_CALIBRATION;drawCalibrationPage();syncButtons();break;
    case 3: screen=BUTTON_TEST;drawButtonTest();syncButtons();break;
    case 4: themeIndex=(themeIndex+1)%4;drawSettings();break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(JOY_SW, INPUT_PULLUP);
  pinMode(BTN_A, INPUT_PULLUP); pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP); pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_R, OUTPUT); pinMode(LED_G, OUTPUT); pinMode(LED_B, OUTPUT);
  digitalWrite(LED_R, LOW); digitalWrite(LED_G, LOW); digitalWrite(LED_B, LOW);
  tft.init(); tft.setRotation(1);
  randomSeed(analogRead(34) + micros());
  drawBootScreen();
  selected = 0; screen = MENU; drawMenu();
}

void loop() {
  updateLED();

  bool a = pressed(BTN_A, oldA);
  bool b = pressed(BTN_B, oldB);
  bool start = pressed(BTN_START, oldStart);
  bool selectBtn = pressed(BTN_SELECT, oldSelect);
  (void)selectBtn;

  if (screen == MENU) {
    int y = analogRead(JOY_Y);
    if (y < joyLowY() && menuYLock != 1) {
      int old = selected; selected = (selected + 4) % 5; menuYLock = 1;
      beep(1000, 40); drawMenuItem(old); drawMenuItem(selected);
    } else if (y > joyHighY() && menuYLock != 2) {
      int old = selected; selected = (selected + 1) % 5; menuYLock = 2;
      beep(1000, 40); drawMenuItem(old); drawMenuItem(selected);
    } else if (y >= 1200 && y <= 2800) menuYLock = 0;

    if (a || start) {
      if (selected == 0) { setupOption = 0; screen = SNAKE_SETUP; drawSnakeSetup(); }
      else if (selected == 1) { screen = PONG_SETUP; drawPongSetup(); }
      else if (selected == 2) startTetris();
      else if (selected == 3) startSpaceInvaders();
      else {
        screen = SETTINGS; settingsSelection = 0;
        settingsLastMove = millis(); drawSettings(); syncButtons();
      }
    }
  }
  else if (screen == SNAKE_SETUP) {
    if (b) { screen = MENU; menuYLock = 0; drawMenu(); }
    int x = analogRead(JOY_X), y = analogRead(JOY_Y);
    if (millis() - lastSetupInput > 220) {
      bool changed = false;
      if (y < joyLowY() && setupOption != 0) { setupOption = 0; changed = true; }
      else if (y > joyHighY() && setupOption != 1) { setupOption = 1; changed = true; }
      if (setupOption == 1) {
        if (x < joyLowX() && speedLevel > 1) { speedLevel--; changed = true; }
        else if (x > joyHighX() && speedLevel < 5) { speedLevel++; changed = true; }
      }
      if (changed) { lastSetupInput = millis(); beep(1000, 40); drawSnakeSetupButtons(); }
    }
    if (a || start) {
      if (setupOption == 0) startSnake();
      else beep(1200, 70);
    }
  }
  else if (screen == SNAKE_GAME) {
    if (b) { noTone(BUZZER); screen = MENU; menuYLock = 0; drawMenu(); return; }
    readGameDirection();
    if (millis() - lastMove >= (unsigned long)speeds[speedLevel - 1]) {
      lastMove = millis(); updateSnake();
    }
  }
  else if (screen == GAME_OVER) {
    if (b) { screen = MENU; menuYLock = 0; drawMenu(); }
    else if (a || start) startSnake();
  }
  else if (screen == PONG_SETUP) {
    if (b) { screen = MENU; menuYLock = 0; drawMenu(); }
    else if (a || start) startPong();
  }
  else if (screen == PONG_GAME) {
    if (b) { noTone(BUZZER); screen = MENU; menuYLock = 0; drawMenu(); return; }
    if (millis() - lastPongUpdate >= 30) {
      lastPongUpdate = millis(); updatePong();
    }
  }
  else if (screen == PONG_OVER) {
    if (b) { screen = MENU; menuYLock = 0; drawMenu(); }
    else if (a || start) startPong();
  }
  else if (screen == TETRIS_GAME) {
    if (b) {
      noTone(BUZZER); screen = MENU; menuYLock = 0;
      tetrisPaused = false; drawMenu(); return;
    }
    if (start) {
      tetrisPaused = !tetrisPaused; beep(1000, 60);
      if (tetrisPaused) drawTetrisPaused();
      else { drawTetrisFrame(); renderTetris(); lastFall = millis(); }
    }
    if (!tetrisPaused) {
      if (a) rotatePiece();
      updateTetris();
      if (gameOverFlag) showTetrisOver();
      else renderTetris();
    }
  }
  else if (screen == TETRIS_OVER) {
    if (b) { screen = MENU; menuYLock = 0; drawMenu(); }
    else if (a || start) startTetris();
  }
  else if (screen == SPACE_GAME) {
    if (b) {
      noTone(BUZZER); screen = MENU; menuYLock = 0;
      siPaused = false; drawMenu(); return;
    }
    if (a) fireSIPlayerBullet();
    if (start) {
      siPaused = !siPaused; beep(1000, 60);
      if (siPaused) drawSIPaused();
      else {
        drawSpaceInvadersFrame();
        siOldPlayerX = siPlayerX;
        if (siPlayerBullet) drawSIPlayerBullet();
        if (siEnemyBullet) drawSIEnemyBullet();
        siLastUpdate = millis(); siLastAlienMove = millis();
      }
    }
    if (!siPaused && millis() - siLastUpdate >= 20) {
      siLastUpdate = millis(); updateSpaceInvaders();
    }
  }
  else if (screen == SPACE_OVER) {
    if (b) { screen = MENU; menuYLock = 0; drawMenu(); }
    else if (a || start) startSpaceInvaders();
  }
  else if (screen == SETTINGS) {
    handleSettings(a,b,start);
  }
  else if (screen == JOY_CALIBRATION) {
    if(b){screen=SETTINGS;drawSettings();syncButtons();}
    else if(a || start) performJoystickCalibration();
  }
  else if (screen == BUTTON_TEST) {
    if(b && start){screen=SETTINGS;drawSettings();syncButtons();}
    else if(millis()-settingsLastMove>80){
      settingsLastMove=millis();drawButtonTest();
    }
  }
  else if (screen == PLACEHOLDER) {
    if (b) { screen = MENU; menuYLock = 0; drawMenu(); }
  }

  delay(5);
}
