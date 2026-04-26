#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Pines ESP32 DevKit
#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4

#define TFT_SCLK 18
#define TFT_MOSI 23

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const int W = 160;
const int H = 128;

int modoActual = 1;

unsigned long lastMouthFrame = 0;
unsigned long lastBlinkFrame = 0;
unsigned long lastScanFrame = 0;
unsigned long lastPulseFrame = 0;

int mouthFrame = 0;
int scanY = 36;
int oldScanY = 36;

bool alertPulse = false;
bool pulseState = false;

// Colores
uint16_t BLACK_FACE;
uint16_t CYAN_AI;
uint16_t CYAN_LOW;
uint16_t BLUE_DARK;
uint16_t WHITE_SOFT;
uint16_t GRAY_BODY;
uint16_t GREEN_AI;
uint16_t GREEN_LOW;
uint16_t RED_AI;
uint16_t RED_DARK;
uint16_t RED_LOW;
uint16_t YELLOW_WARN;
uint16_t PINK_AI;
uint16_t PINK_LOW;

// ----------------------------------------------------
// LIMPIEZA DE ZONAS PEQUEÑAS
// ----------------------------------------------------

void clearMouthArea(uint16_t bg) {
  tft.fillRect(55, 72, 50, 24, bg);
}

void clearEyeArea(uint16_t bg) {
  tft.fillRect(45, 42, 72, 30, bg);
}

// ----------------------------------------------------
// MARCO Y BASE DEL ROBOT
// ----------------------------------------------------

void drawPanelFrame(uint16_t color) {
  tft.drawRoundRect(4, 4, 152, 120, 10, color);
  tft.drawRoundRect(8, 8, 144, 112, 8, color);

  tft.drawLine(12, 20, 28, 20, color);
  tft.drawLine(12, 20, 12, 34, color);

  tft.drawLine(132, 20, 148, 20, color);
  tft.drawLine(148, 20, 148, 34, color);

  tft.drawLine(12, 94, 12, 108, color);
  tft.drawLine(12, 108, 28, 108, color);

  tft.drawLine(148, 94, 148, 108, color);
  tft.drawLine(132, 108, 148, 108, color);
}

void drawRobotBase(uint16_t accentColor, uint16_t faceColor, uint16_t bodyColor) {
  // Laterales
  tft.fillRoundRect(14, 45, 20, 42, 8, bodyColor);
  tft.drawRoundRect(14, 45, 20, 42, 8, accentColor);

  tft.fillRoundRect(126, 45, 20, 42, 8, bodyColor);
  tft.drawRoundRect(126, 45, 20, 42, 8, accentColor);

  // Cabeza
  tft.fillRoundRect(28, 18, 104, 92, 18, bodyColor);
  tft.drawRoundRect(28, 18, 104, 92, 18, accentColor);

  // Visor
  tft.fillRoundRect(38, 30, 84, 64, 12, faceColor);
  tft.drawRoundRect(38, 30, 84, 64, 12, accentColor);

  // Luces
  tft.fillRoundRect(62, 13, 36, 6, 3, accentColor);
  tft.fillRoundRect(62, 104, 36, 7, 3, accentColor);
}

void drawSmallText(const char* text, uint16_t color, int x) {
  tft.setTextColor(color);
  tft.setTextSize(1);
  tft.setCursor(x, 114);
  tft.print(text);
}

// ----------------------------------------------------
// OJOS
// ----------------------------------------------------

void drawNormalEyes(bool blink) {
  clearEyeArea(BLACK_FACE);

  if (blink) {
    tft.drawFastHLine(51, 60, 22, CYAN_AI);
    tft.drawFastHLine(87, 60, 22, CYAN_AI);
  } else {
    tft.fillRoundRect(51, 54, 22, 12, 5, CYAN_AI);
    tft.fillRoundRect(87, 54, 22, 12, 5, CYAN_AI);

    tft.fillCircle(58, 57, 2, WHITE_SOFT);
    tft.fillCircle(94, 57, 2, WHITE_SOFT);
  }
}

void drawHappyEyes() {
  clearEyeArea(BLACK_FACE);

  // Ojo izquierdo feliz
  tft.drawLine(50, 61, 55, 55, GREEN_AI);
  tft.drawLine(55, 55, 68, 55, GREEN_AI);
  tft.drawLine(68, 55, 74, 61, GREEN_AI);

  tft.drawLine(50, 62, 55, 56, GREEN_AI);
  tft.drawLine(55, 56, 68, 56, GREEN_AI);
  tft.drawLine(68, 56, 74, 62, GREEN_AI);

  // Ojo derecho feliz
  tft.drawLine(86, 61, 92, 55, GREEN_AI);
  tft.drawLine(92, 55, 105, 55, GREEN_AI);
  tft.drawLine(105, 55, 110, 61, GREEN_AI);

  tft.drawLine(86, 62, 92, 56, GREEN_AI);
  tft.drawLine(92, 56, 105, 56, GREEN_AI);
  tft.drawLine(105, 56, 110, 62, GREEN_AI);
}

void drawStealthEyes() {
  uint16_t stealthFace = tft.color565(12, 0, 0);
  clearEyeArea(stealthFace);

  // Cejas agresivas
  tft.drawLine(48, 43, 72, 53, RED_AI);
  tft.drawLine(48, 44, 72, 54, RED_AI);

  tft.drawLine(112, 43, 88, 53, RED_AI);
  tft.drawLine(112, 44, 88, 54, RED_AI);

  // Ojos tipo rendija
  tft.fillRoundRect(52, 59, 22, 7, 3, RED_AI);
  tft.fillRoundRect(86, 59, 22, 7, 3, RED_AI);

  tft.drawFastHLine(56, 62, 14, ST77XX_BLACK);
  tft.drawFastHLine(90, 62, 14, ST77XX_BLACK);
}

// ----------------------------------------------------
// BOCAS
// ----------------------------------------------------

void drawNormalMouth(int frame) {
  clearMouthArea(BLACK_FACE);

  if (frame == 0) {
    tft.drawFastHLine(68, 80, 24, CYAN_AI);
    tft.drawFastHLine(68, 81, 24, CYAN_AI);
  } 
  else if (frame == 1) {
    tft.drawRoundRect(69, 77, 22, 8, 4, CYAN_AI);
  } 
  else if (frame == 2) {
    tft.fillRoundRect(70, 75, 20, 12, 5, CYAN_AI);
    tft.fillRoundRect(73, 78, 14, 6, 3, BLACK_FACE);
  } 
  else {
    tft.drawLine(67, 78, 74, 84, CYAN_AI);
    tft.drawLine(74, 84, 86, 84, CYAN_AI);
    tft.drawLine(86, 84, 93, 78, CYAN_AI);
  }
}

void drawHappyMouth(int frame) {
  clearMouthArea(BLACK_FACE);

  if (frame == 0) {
    tft.drawLine(64, 76, 72, 84, GREEN_AI);
    tft.drawLine(72, 84, 88, 84, GREEN_AI);
    tft.drawLine(88, 84, 96, 76, GREEN_AI);
  } 
  else if (frame == 1) {
    tft.drawLine(61, 74, 70, 90, GREEN_AI);
    tft.drawLine(70, 90, 90, 90, GREEN_AI);
    tft.drawLine(90, 90, 99, 74, GREEN_AI);
  } 
  else if (frame == 2) {
    tft.fillRoundRect(66, 77, 28, 14, 6, GREEN_AI);
    tft.fillRoundRect(70, 79, 20, 8, 4, BLACK_FACE);
  } 
  else {
    tft.drawFastHLine(68, 84, 24, GREEN_AI);
    tft.drawFastHLine(68, 85, 24, GREEN_AI);
  }
}

void drawStealthMouth(int frame) {
  uint16_t stealthFace = tft.color565(12, 0, 0);
  clearMouthArea(stealthFace);

  if (frame == 0) {
    tft.drawFastHLine(66, 82, 28, RED_AI);
    tft.drawFastHLine(66, 83, 28, RED_AI);
  } 
  else if (frame == 1) {
    tft.fillRect(68, 80, 24, 5, RED_AI);
    tft.drawFastHLine(70, 82, 20, ST77XX_BLACK);
  } 
  else if (frame == 2) {
    tft.fillRoundRect(68, 78, 24, 10, 3, RED_AI);
    tft.fillRect(72, 81, 16, 4, ST77XX_BLACK);
  } 
  else {
    tft.drawLine(66, 86, 94, 82, RED_AI);
    tft.drawLine(66, 87, 94, 83, RED_AI);
  }
}

// ----------------------------------------------------
// BASES DE CADA MODO
// ----------------------------------------------------

void drawNormalBase() {
  tft.fillScreen(BLUE_DARK);

  drawPanelFrame(CYAN_AI);
  drawRobotBase(CYAN_AI, BLACK_FACE, GRAY_BODY);

  drawNormalEyes(false);
  drawNormalMouth(0);

  drawSmallText("NORMAL TALK", CYAN_AI, 42);
}

void drawHappyBase() {
  tft.fillScreen(BLUE_DARK);

  drawPanelFrame(GREEN_AI);
  drawRobotBase(GREEN_AI, BLACK_FACE, GRAY_BODY);

  drawHappyEyes();
  drawHappyMouth(0);

  tft.fillCircle(50, 76, 3, PINK_AI);
  tft.fillCircle(110, 76, 3, PINK_AI);

  drawSmallText("HAPPY TALK", GREEN_AI, 44);
}

void drawStealthBase() {
  uint16_t stealthFace = tft.color565(12, 0, 0);

  tft.fillScreen(RED_DARK);

  // Fondo rojo con líneas, pero la cara se dibuja encima
  for (int y = 0; y < H; y += 12) {
    tft.drawFastHLine(0, y, W, RED_LOW);
  }

  drawPanelFrame(RED_AI);
  drawRobotBase(RED_AI, stealthFace, ST77XX_BLACK);

  scanY = 36;
  oldScanY = 36;

  drawStealthEyes();
  drawStealthMouth(mouthFrame);

  drawSmallText("STEALTH TALK", RED_AI, 40);
}

// ----------------------------------------------------
// MICROANIMACIONES
// ----------------------------------------------------

void updateNormalPulse() {
  pulseState = !pulseState;

  if (pulseState) {
    tft.fillCircle(58, 57, 2, WHITE_SOFT);
    tft.fillCircle(94, 57, 2, WHITE_SOFT);

    tft.drawRoundRect(38, 30, 84, 64, 12, CYAN_AI);
    tft.fillRoundRect(62, 13, 36, 6, 3, CYAN_AI);
  } else {
    tft.fillCircle(58, 57, 2, CYAN_LOW);
    tft.fillCircle(94, 57, 2, CYAN_LOW);

    tft.drawRoundRect(38, 30, 84, 64, 12, CYAN_LOW);
    tft.fillRoundRect(62, 13, 36, 6, 3, CYAN_LOW);
  }
}

void updateHappyPulse() {
  pulseState = !pulseState;

  if (pulseState) {
    tft.fillCircle(50, 76, 4, PINK_AI);
    tft.fillCircle(110, 76, 4, PINK_AI);

    tft.fillRoundRect(62, 13, 36, 6, 3, GREEN_AI);
    tft.drawRoundRect(38, 30, 84, 64, 12, GREEN_AI);
  } else {
    tft.fillCircle(50, 76, 3, PINK_LOW);
    tft.fillCircle(110, 76, 3, PINK_LOW);

    tft.fillRoundRect(62, 13, 36, 6, 3, GREEN_LOW);
    tft.drawRoundRect(38, 30, 84, 64, 12, GREEN_LOW);
  }
}

void updateStealthScan() {
  uint16_t stealthFace = tft.color565(12, 0, 0);

  // Borra la línea anterior SOLO dentro del visor
  tft.drawFastHLine(42, oldScanY, 76, stealthFace);
  tft.drawFastHLine(42, oldScanY + 1, 76, stealthFace);

  oldScanY = scanY;

  // Dibuja la nueva línea SOLO dentro del visor
  tft.drawFastHLine(42, scanY, 76, RED_AI);
  tft.drawFastHLine(42, scanY + 1, 76, RED_AI);

  scanY += 3;

  if (scanY > 90) {
    scanY = 36;
  }

  // Pulso superior rojo
  alertPulse = !alertPulse;

  if (alertPulse) {
    tft.fillRoundRect(60, 13, 40, 6, 3, RED_AI);
    tft.drawTriangle(12, 12, 22, 12, 17, 22, YELLOW_WARN);
    tft.drawTriangle(138, 12, 148, 12, 143, 22, YELLOW_WARN);
  } else {
    tft.fillRoundRect(60, 13, 40, 6, 3, RED_DARK);
    tft.fillRoundRect(65, 13, 30, 6, 3, RED_LOW);

    // Limpiar triángulos de alerta
    tft.fillRect(10, 10, 16, 15, RED_DARK);
    tft.fillRect(136, 10, 16, 15, RED_DARK);

    // Restaurar marco por si se borró algún borde
    drawPanelFrame(RED_AI);
  }

  // Redibujar ojos y boca encima del scanner para que no se destruyan
  drawStealthEyes();
  drawStealthMouth(mouthFrame);
}

// ----------------------------------------------------
// CAMBIO DE MODO
// ----------------------------------------------------

void cambiarModo(int nuevoModo) {
  modoActual = nuevoModo;
  mouthFrame = 0;
  scanY = 36;
  oldScanY = 36;
  pulseState = false;
  alertPulse = false;

  if (modoActual == 1) {
    Serial.println("Modo NORMAL seleccionado");
    drawNormalBase();
  } 
  else if (modoActual == 2) {
    Serial.println("Modo FELIZ seleccionado");
    drawHappyBase();
  } 
  else if (modoActual == 3) {
    Serial.println("Modo FURTIVO seleccionado");
    drawStealthBase();
  }
}

void mostrarMenu() {
  Serial.println();
  Serial.println("========== ASISTENTE IA ==========");
  Serial.println("Escribe 1, 2 o 3 y presiona Enter:");
  Serial.println("1 -> Modo NORMAL hablando");
  Serial.println("2 -> Modo FELIZ hablando");
  Serial.println("3 -> Modo FURTIVO hablando");
  Serial.println("==================================");
  Serial.println();
}

// ----------------------------------------------------
// SETUP
// ----------------------------------------------------

void setup() {
  Serial.begin(115200);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  BLACK_FACE = tft.color565(3, 8, 14);
  CYAN_AI = tft.color565(0, 220, 255);
  CYAN_LOW = tft.color565(0, 110, 140);
  BLUE_DARK = tft.color565(0, 7, 18);
  WHITE_SOFT = tft.color565(230, 240, 255);
  GRAY_BODY = tft.color565(180, 190, 210);

  GREEN_AI = tft.color565(0, 255, 120);
  GREEN_LOW = tft.color565(0, 120, 60);

  RED_AI = tft.color565(255, 0, 20);
  RED_DARK = tft.color565(18, 0, 0);
  RED_LOW = tft.color565(80, 0, 0);

  YELLOW_WARN = tft.color565(255, 180, 0);
  PINK_AI = tft.color565(255, 80, 120);
  PINK_LOW = tft.color565(120, 20, 60);

  mostrarMenu();
  cambiarModo(1);
}

// ----------------------------------------------------
// LOOP
// ----------------------------------------------------

void loop() {
  // Leer comando serial
  if (Serial.available()) {
    char comando = Serial.read();

    if (comando == '1') {
      cambiarModo(1);
      mostrarMenu();
    } 
    else if (comando == '2') {
      cambiarModo(2);
      mostrarMenu();
    } 
    else if (comando == '3') {
      cambiarModo(3);
      mostrarMenu();
    }

    while (Serial.available()) {
      Serial.read();
    }
  }

  // Animación de boca hablando
  if (millis() - lastMouthFrame > 130) {
    lastMouthFrame = millis();

    mouthFrame++;

    if (mouthFrame > 3) {
      mouthFrame = 0;
    }

    if (modoActual == 1) {
      drawNormalMouth(mouthFrame);
    } 
    else if (modoActual == 2) {
      drawHappyMouth(mouthFrame);
    } 
    else if (modoActual == 3) {
      drawStealthMouth(mouthFrame);
    }
  }

  // Parpadeo natural solo en modo normal
  if (modoActual == 1 && millis() - lastBlinkFrame > 2400) {
    lastBlinkFrame = millis();

    drawNormalEyes(true);
    delay(90);
    drawNormalEyes(false);
  }

  // Pulso visual para normal y feliz
  if (millis() - lastPulseFrame > 500) {
    lastPulseFrame = millis();

    if (modoActual == 1) {
      updateNormalPulse();
    } 
    else if (modoActual == 2) {
      updateHappyPulse();
    }
  }

  // Scanner corregido en modo furtivo
  if (modoActual == 3 && millis() - lastScanFrame > 80) {
    lastScanFrame = millis();
    updateStealthScan();
  }
}
