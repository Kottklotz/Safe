#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>
#include <Keypad.h>

// === OLED Display ===
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// === Keypad-Konfiguration ===
#define ROW_NUM     4
#define COLUMN_NUM  4

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM]      = {19, 18, 5, 17};
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// === Texteingabe & Statusvariablen ===
String txt = "";

bool showCleared = false;
unsigned long clearedTime = 0;

bool showSubmitted = false;
unsigned long submittedTime = 0;

// === Funktion zur Anzeige auf dem OLED ===
void drawOnDisplay(String txt) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_profont22_tr);  // Gut lesbare Schrift
    u8g2.drawStr(0, 20, txt.c_str());
  } while (u8g2.nextPage());
}

// === Setup ===
void setup() {
  u8g2.begin();
  Serial.begin(9600);
}

// === Loop ===
void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '*') {
      txt = "";
      showCleared = true;
      clearedTime = millis();
    } else if (key == '#') {
      Serial.println("Eingabe abgeschlossen: " + txt);
      txt = "";
      showSubmitted = true;
      submittedTime = millis();
    } else {
      txt += key;
    }
  }

  // === Anzeige steuern ===
  if (showCleared) {
    drawOnDisplay("geloescht");
    if (millis() - clearedTime > 1000) {
      showCleared = false;
    }
  } else if (showSubmitted) {
    drawOnDisplay("Eingabe");
    if (millis() - submittedTime > 1000) {
      showSubmitted = false;
    }
  } else {
    drawOnDisplay(txt);
  }

  delay(50);  // Kurze Pause zur Entlastung des Systems
}
