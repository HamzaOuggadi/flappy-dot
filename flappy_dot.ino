#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

// OLED Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
// The I2C address is usually 0x3C for these 0.96 screens
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin Definitions (Using MightyCore pin macros for safety)
#define BUTTON_PIN PIN_PD2
#define BUZZER_PIN PIN_PD3

// --- Game Variables ---
// Player (Dot)
float dotY = 32.0;
float velocity = 0.0;
const float gravity = 0.6;
const float jumpStrength = -4.0;
const int dotRadius = 3;

// Obstacle (The Gap)
float obstacleX = 128.0;
int gapY = 32;          // Center of the gap
const int gapSize = 25; // How wide the gap is
const float obstacleSpeed = 3.0;
const int obstacleWidth = 8;

// Game State
int score = 0;
bool gameOver = false;
bool pause = true;
bool lastButtonState = HIGH;
int highscore = 0;

int eepromAddress = 0;

void setup() {
  // Initialize inputs/outputs
  pinMode(BUTTON_PIN, INPUT_PULLUP); // HIGH when unpressed, LOW when pressed
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize the OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while(true); // If screen fails, halt here forever
  }

  EEPROM.get(eepromAddress, highscore);
  
  if (highscore == -1) {
    highscore = 0;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 20);
  display.print("FLAPPY DOT!");
  display.setCursor(20, 40);
  display.print("Press to Start");
  display.display();
  
  tone(BUZZER_PIN, 1000, 200); // Startup beep
  delay(2000);
}

void resetGame() {
  dotY = 32.0;
  velocity = 0.0;
  obstacleX = 128.0;
  score = 0;
  gameOver = false;
  gapY = random(15, 49); // Pick a new gap height
}

void setHighscore() {
  if (score > highscore) {
    highscore = score;
    EEPROM.put(eepromAddress, highscore);
  }
}

void loop() {

  // 1. INPUT
  bool currentButtonState = digitalRead(BUTTON_PIN);
  bool buttonPressed = (currentButtonState == LOW && lastButtonState == HIGH);
  lastButtonState = currentButtonState;

  if (!pause) {
    if (gameOver) {
      if (buttonPressed) {
        resetGame();
      }
      return; // Skip the rest of the loop
    }

    // 2. UPDATE PHYSICS
    if (buttonPressed) {
      velocity = jumpStrength;
      tone(BUZZER_PIN, 1500, 50); // Small jump sound
    }

    // Apply gravity
    velocity += gravity;
    dotY += velocity;

    // Move obstacle
    obstacleX -= obstacleSpeed;

    // Obstacle reset & Score increment
    if (obstacleX < -obstacleWidth) {
      obstacleX = 128.0;
      gapY = random(15, 49); // Randomize next gap
      score++;
      tone(BUZZER_PIN, 2000, 100); // Score beep
    }

    // 3. COLLISION DETECTION
    // Check Floor and Ceiling
    if (dotY <= 0 || dotY >= SCREEN_HEIGHT) {
      gameOver = true;
    }
    
    // Check Obstacle
    // If dot is horizontally inside the obstacle...
    if ((10 + dotRadius > obstacleX) && (10 - dotRadius < obstacleX + obstacleWidth)) {
      // And vertically OUTSIDE the gap...
      if ((dotY - dotRadius < gapY - gapSize/2) || (dotY + dotRadius > gapY + gapSize/2)) {
        gameOver = true;
      }
    }

    if (gameOver) {
      tone(BUZZER_PIN, 300, 500); // Death sound
      setHighscore();
    }

    // 4. RENDER GRAPHICS
    display.clearDisplay();

    // Draw Dot (fixed horizontal position at X=10)
    display.fillCircle(10, (int)dotY, dotRadius, SSD1306_WHITE);

    // Draw Obstacle (Top block)
    display.fillRect((int)obstacleX, 0, obstacleWidth, gapY - gapSize/2, SSD1306_WHITE);
    // Draw Obstacle (Bottom block)
    display.fillRect((int)obstacleX, gapY + gapSize/2, obstacleWidth, SCREEN_HEIGHT - (gapY + gapSize/2), SSD1306_WHITE);

    // Draw Score
    display.setCursor(0, 0);
    display.print(score);
    // Draw High Score
    display.setCursor(0, 10); 
    display.print("HS:");
    display.print(highscore);

    if (gameOver) {
      display.setCursor(35, 20);
      display.print("GAME OVER");
      display.setCursor(20, 40);
      display.print("Press to Restart");
    }

    // Push the memory buffer to the screen
    display.display(); 

  } else {
    if (buttonPressed) {
      pause = false;
    }
  }

  // 5. DELAY (Regulate Frame Rate to roughly 30 FPS)
  delay(30); 
}