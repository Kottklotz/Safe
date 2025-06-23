#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>

// OLED Display initialisieren
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Variable für seriellen Input
String srl = "";

void setup(void) {
  u8g2.begin();
  Serial.begin(9600);
  Serial.setTimeout(100);  // Wartezeit für Serial.readString()
}

void loop() {
  // Prüfen, ob etwas über Serial eingegeben wurde
  if (Serial.available() > 0) {
    srl = Serial.readString();  // Eingabe lesen
    srl.trim();                 // Leerzeichen entfernen
  }

  // OLED Display zeichnen
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr); // Lesbare Schriftart
    u8g2.drawStr(0, 20, srl.c_str());   // String anzeigen
  } while (u8g2.nextPage());

  delay(200);  // Kurze Pause (verhindert Flackern)
}
