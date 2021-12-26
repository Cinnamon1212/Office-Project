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

// Calculator
boolean presentValue = false;
boolean next = false;
boolean final = false;
String num1, num2;
int answer;
char op;

void calc(){
  while(1){
    char key = keypad.getKey();
    if (key != NO_KEY && (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0')){
       if (presentValue != true){
        num1 = num1 + key;
        int numLength = num1.length();
        lcd.setCursor(15 - numLength, 0); //to adjust one whitespace for operator
        lcd.print(num1);
       }else{
        num2 = num2 + key;
        int numLength = num2.length();
        lcd.setCursor(15 - numLength, 1);
        lcd.print(num2);
        final = true;
       }
    }else if(presentValue == false && key != NO_KEY && (key == '#' || key == '*' || key == 'B' || key == 'A')){
      if (presentValue == false){
        presentValue = true;
        op = key;
        lcd.setCursor(15,0);
        switch(op){
          case 'A':
            lcd.print('+');
            break;
          case 'B':
            lcd.print('-');
            break;
          case '#':
            lcd.print('/');
            break;
          case '*':
            lcd.print('*');
            break;
        }
      }  
    }else if(final == true && key != NO_KEY && key == 'D'){
      lcd.clear();
      if (op == 'A'){
        answer = num1.toInt() + num2.toInt();
      }
      else if (op == 'B'){
        answer = num1.toInt() - num2.toInt();
      }
      else if (op == '*'){
        answer = num1.toInt() * num2.toInt();
      }
      else if (op == '#'){
        answer = num1.toInt() / num2.toInt();
      }
      unsigned long startTime = millis();
      while((millis() - startTime) <= 10000){
        lcd.setCursor(0,0);
        lcd.print(answer);
      }
      presentValue = false;
      final = false;
      num1 = "";
      num2 = "";
      answer = 0;
      op = ' ';
      lcd.clear();
      return;
     }
  }
}
// Games
void jumpGame(){
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

void rNums(){}

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

void menu2(){
  lcd.clear();
  while(1){
      char key = keypad.getKey();
      lcd.setCursor(0,0);
      lcd.print("(3) Calculator");
      lcd.setCursor(0,1);
      lcd.print("(4) Random numbers");
      if (int(key) != 0){
        switch(key){
          case '3':
            lcd.clear();
            calc();
            break;
          case '4':
            lcd.clear();
            rNums();
            break;
          case 'D':
            menu1();
            break;
        }
      }
  }
}

void menu1(){
  lcd.clear();
  while(1){
      char key = keypad.getKey();
      lcd.setCursor(0,0);
      lcd.print("(1) Jump game");
      lcd.setCursor(0,1);
      lcd.print("(2) Turn on fan");
      if (int(key) != 0){
        switch(key){
          case '1':
            lcd.clear();
            jumpGame();
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
