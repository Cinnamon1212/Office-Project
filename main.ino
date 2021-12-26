#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Custom chars

byte synChar[] = {
  B00000,
  B00000,
  B00100,
  B00010,
  B01110,
  B00000,
  B00000,
  B00000
};

// Fans
const int INA = 11;
const int INB = 10;

// Passcode
const  char* passcode = "B00B5";
int currentPos = 0;

// LCD
LiquidCrystal_I2C lcd(0x27,16,2);

// Keypad vars
const char goBack = 'D';
const byte ROWS = 4; 
const byte COLS = 4; 
const char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
const byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// Games

void gameOfLife(){
  while(1){
    char key = keypad.getKey();
    lcd.setCursor(0,0);
    lcd.print("Here 1");
    if (int(key) != 0){
      if (key == goBack){
        lcd.clear();
        return;  
      }
    }
  }
}

//  Utils
void fans(){
  while(1){
    lcd.setCursor(0,0);
    lcd.print("Fans on!");
    char key = keypad.getKey();
    if (int(key)  != 0){
      if (key == goBack){
        lcd.clear();
        digitalWrite(INB, LOW);
        return;
      }
    }
    digitalWrite(INA, LOW);
    digitalWrite(INB, HIGH);
    delay(1000);
  }
}

// Display/Menu functions

void invalidCode(){
  lcd.clear();
  while(1){
    char key = keypad.getKey();
    lcd.setCursor(0,0);
    lcd.print("INVALID CODE!");
    if (int(key) != 0){
      lcd.clear();
      return;
    }
  }
}

void menu2(){}

void menu1(){
  lcd.clear();
  while(1){
      char key = keypad.getKey();
      lcd.setCursor(0,0);
      lcd.print("(1) Game of life");
      lcd.setCursor(0,1);
      lcd.print("(2) Turn on fan");
      if (int(key) != 0){
        switch(key){
          case '1':
            lcd.clear();
            gameOfLife();
            break;
          case '2':
            lcd.clear();
            fans();
            break;
          case 'C':
            menu2();
            break;
        }
      }
  }
}

void mainScreen(){
  lcd.setCursor(0,0);
  lcd.print("Passcode: ");
}

// Required functions
void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, synChar);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
}

void loop()
{
  int i;
  char key = keypad.getKey();
    if (int(key) != 0){
      lcd.setCursor(10, 0);
      for (i=0;i<=currentPos;  ++i){
        lcd.write(0); 
      }
      if (key == passcode[currentPos]){
        ++currentPos;
        if(currentPos == 5){
            menu1();
            currentPos = 0;
         }
       }else{
        invalidCode();
        currentPos = 0;
       }
    }
  mainScreen();
}
