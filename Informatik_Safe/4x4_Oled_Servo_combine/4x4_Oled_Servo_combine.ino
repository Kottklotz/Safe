#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>
#include <Keypad.h>
#include <ESP32Servo.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define ROW_NUM 4
#define COLUMN_NUM 4

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {19, 18, 5, 17};
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

String txt = "";
const String code = "123A";
bool showMessage = false;
String messageText = "";
unsigned long messageTime = 0;

Servo myservo;
#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
int servoPin = 17;
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
int servoPin = 7;
#else
int servoPin = 14;
#endif

void drawOnDisplay(String txt) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(0, 20, txt.c_str());
  } while (u8g2.nextPage());
}

void setup() {
  u8g2.begin();
  Serial.begin(9600);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 1000, 2000);
  myservo.write(0);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '*') {
      txt = "";
      messageText = "geloescht";
      showMessage = true;
      messageTime = millis();
    } else if (key == '#') {
      Serial.println("Eingabe abgeschlossen: " + txt);

      if (txt == code) {
        messageText = "Zugriff";
        showMessage = true;
        messageTime = millis();
        myservo.write(180);
        delay(1000);
        myservo.write(0);
      } else {
        messageText = "Falsch";
        showMessage = true;
        messageTime = millis();
      }

      txt = "";
    } else {
      txt += key;
    }
  }

  if (showMessage) {
    drawOnDisplay(messageText);
    if (millis() - messageTime > 1000) {
      showMessage = false;
    }
  } else {
    drawOnDisplay(txt);
  }

  delay(50);
}
