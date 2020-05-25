#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
int relay = 4;
LiquidCrystal_I2C lcd(0x27, 16, 2);
volatile int flow_frequency;               // Water flow
unsigned int l_hour, f_val;                // Water flow
unsigned long ctTime, flow_val, cloopTime; // Water flow
int ml = 0, inputml = 0 , CK = 1;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {12, 11, 10, 9};
byte colPins[COLS] = {8, 7, 6, 5};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void flow ()
{
  flow_frequency++;  f_val++;
}

void setup() {
  lcd.begin();
  lcd.backlight();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, 1);
  Serial.begin(9600);
  attachInterrupt(1, flow, RISING);
  sei();
  ctTime = millis();
  cloopTime = ctTime; // Water flow
}

void loop() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
    if (key == 'A') {
      inputml = 330;
    } else if (key == 'B') {
      inputml = 625;
    }
    if (key == '#') {
      if (CK == 0) {
        digitalWrite(relay, 1);
        f_val = 0;
        flow_val = 0;
        CK = 1;
        lcd.clear();
      } else if (CK == 1) {
        digitalWrite(relay, 0);
        CK = 0;
      }
    } else if (key == '*') {
      digitalWrite(relay, 1);
      CK = 1;
      inputml = 0;
      f_val = 0;
      flow_val = 0;
      ml = 0;
    } else {
      switch (key) {
        case '1':
          inputml = (inputml * 10) + 1;
          break;
        case '2':
          inputml = (inputml * 10) + 2;
          break;
        case '3':
          inputml = (inputml * 10) + 3;
          break;
        case '4':
          inputml = (inputml * 10) + 4;
          break;
        case '5':
          inputml = (inputml * 10) + 5;
          break;
        case '6':
          inputml = (inputml * 10) + 6;
          break;
        case '7':
          inputml = (inputml * 10) + 7;
          break;
        case '8':
          inputml = (inputml * 10) + 8;
          break;
        case '9':
          inputml = (inputml * 10) + 9;
          break;
        case '0':
          inputml = (inputml * 10) + 0;
          break;
      }
      ml = inputml;
    }
  }

  ctTime = millis();
  if (ctTime >= (cloopTime + 1000))
  {
    cloopTime = ctTime;
    //l_hour = (flow_frequency * 60 / 7.5);
    //flow_frequency = 0;
    //Serial.print(l_hour, DEC);     Serial.print("  L/hour    "); Serial.println(key);

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(ml);
    lcd.setCursor(1, 1);
    lcd.print(flow_val);
    lcd.setCursor(10, 0);
    lcd.print("ml");
    lcd.setCursor(10, 1);
    lcd.print("ml");
  }
  flow_val = f_val * 2.67;

  //Serial.print(flow_val); Serial.println(" ml");
  if (flow_val >= ml) {
    digitalWrite(relay, 1);
    flow_val = 0;
    f_val = 0;
    CK = 1;
  }
}
