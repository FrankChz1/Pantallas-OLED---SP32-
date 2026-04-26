#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int progreso = 0;

void dibujarBarra(int valor) {
  display.clearDisplay();

  // Título
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(18, 5);
  display.print("CARGA DEL SISTEMA");

  // Porcentaje grande
  display.setTextSize(2);
  display.setCursor(42, 20);
  display.print(valor);
  display.print("%");

  // Marco de la barra
  int x = 10;
  int y = 48;
  int ancho = 108;
  int alto = 10;

  display.drawRect(x, y, ancho, alto, SSD1306_WHITE);

  // Ancho interno proporcional
  int anchoRelleno = map(valor, 0, 100, 0, ancho - 4);

  display.fillRect(x + 2, y + 2, anchoRelleno, alto - 4, SSD1306_WHITE);

  display.display();
}

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("No se encontro la pantalla OLED SSD1306");
    while (true);
  }

  display.clearDisplay();
  display.display();

  Serial.println("Sistema iniciado.");
  Serial.println("Envia numeros de 0 a 100 por Serial.");
  Serial.println("Los valores se acumulan hasta llegar a 100.");

  dibujarBarra(progreso);
}

void loop() {
  if (Serial.available()) {
    int valorRecibido = Serial.parseInt();

    if (valorRecibido < 0) {
      valorRecibido = 0;
    }

    progreso += valorRecibido;

    if (progreso > 100) {
      progreso = 100;
    }

    Serial.print("Valor recibido: ");
    Serial.println(valorRecibido);

    Serial.print("Progreso acumulado: ");
    Serial.print(progreso);
    Serial.println("%");

    dibujarBarra(progreso);

    while (Serial.available()) {
      Serial.read();
    }
  }
}
