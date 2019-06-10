#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

void setup() {
    lcd.begin(16, 2);

    byte c[8] = {
        0b01110,
        0b11111,
        0b11101,
        0b11111,
        0b11000,
        0b11111,
        0b11111,
        0b01010
    };
    lcd.createChar(0, c);
    
    c[0] = 0b01110;
    c[1] = 0b11111;
    c[2] = 0b11101;
    c[3] = 0b11111;
    c[4] = 0b11000;
    c[5] = 0b11111;
    c[6] = 0b11111;
    c[7] = 0b10001;
    lcd.createChar(1, c);
    
    c[0] = 0b00000;
    c[1] = 0b00000;
    c[2] = 0b00000;
    c[3] = 0b00000;
    c[4] = 0b00000;
    c[5] = 0b01110;
    c[6] = 0b11111;
    c[7] = 0b11101;
    lcd.createChar(2, c);
    
    c[0] = 0b11111;
    c[1] = 0b11000;
    c[2] = 0b11111;
    c[3] = 0b11111;
    c[4] = 0b01010;
    c[5] = 0b00000;
    c[6] = 0b00000;
    c[7] = 0b00000;
    lcd.createChar(3, c);
    
    c[0] = 0b00000;
    c[1] = 0b00000;
    c[2] = 0b00000;
    c[3] = 0b01110;
    c[4] = 0b11111;
    c[5] = 0b11101;
    c[6] = 0b11111;
    c[7] = 0b11000;
    lcd.createChar(4, c);
    
    c[0] = 0b11111;
    c[1] = 0b01010;
    c[2] = 0b00000;
    c[3] = 0b00000;
    c[4] = 0b00000;
    c[5] = 0b00000;
    c[6] = 0b00000;
    c[7] = 0b00000;
    lcd.createChar(5, c);

    c[0] = 0b01110;
    c[1] = 0b11111;
    c[2] = 0b10101;
    c[3] = 0b11011;
    c[4] = 0b10101;
    c[5] = 0b11111;
    c[6] = 0b11111;
    c[7] = 0b11111;
    lcd.createChar(6, c);
    
    c[0] = 0b00100;
    c[1] = 0b00111;
    c[2] = 0b00100;
    c[3] = 0b11111;
    c[4] = 0b10001;
    c[5] = 0b11111;
    c[6] = 0b00000;
    c[7] = 0b10101;
    lcd.createChar(7, c);
}

int time = 0;
int jumpState = 0;
int pos[8];
int posLastIndex = 0;
boolean gameOver = false;
int gameOverTime = 0;
void loop() {
    int buttonState = digitalRead(2);
    if(gameOver){
      if(buttonState == HIGH) {
        gameOverTime += 1;
      }
      if(60<gameOverTime){
        time = 0;
        gameOver = false;
        posLastIndex = 0;
        gameOverTime = 0;
        pos[0] = 0;
        pos[1] = 0;
        pos[2] = 0;
        pos[3] = 0;
        pos[4] = 0;
        pos[5] = 0;
        pos[6] = 0;
        pos[7] = 0;
      }
      delay(16.6);
      return;
    }
    if(time%60 == 0) {
      pos[posLastIndex] = 0;
      posLastIndex += 1;
    }
    if(time%(10-time/600) == 0){
      for(int i=0; i <= posLastIndex; i++) {
        pos[i] += 1;
      }
      if(15 < pos[0]) {
        posLastIndex -= 1;
        pos[0] = pos[1];
        pos[1] = pos[2];
        pos[2] = pos[3];
        pos[3] = pos[4];
        pos[4] = pos[5];
        pos[5] = pos[6];
        pos[6] = pos[7];
      }      
    }
    if(time%10 == 0 && 0 < jumpState){
      jumpState += 1;
      if(5 < jumpState) {
        jumpState = 0;
      }
    }
    if(buttonState == HIGH && jumpState == 0) {
      jumpState = 1;
    }
    if((jumpState == 0 || jumpState == 1 || jumpState == 5) && pos[0] == 14) {
      gameOver = true;
    }
    
    lcd.setCursor(0,0);
    if(buttonState == HIGH) {
      lcd.print("J");
    } else {
      lcd.print(" ");
    }
    
    printPlayer();
    printStoneList();
    printPoint();
    background();
    time += 1;
    delay(16.6);
}

void printPlayer() {
    if(jumpState == 0) {
      lcd.setCursor(1, 0);
      lcd.print(' ');
      lcd.setCursor(1, 1);
      lcd.write(time / 20 % 2);
    } else if(jumpState == 1 || jumpState == 5 ) {
      lcd.setCursor(1, 0);
      lcd.write(2);
      lcd.setCursor(1, 1);
      lcd.write(3);
    } else if(jumpState == 2 || jumpState == 4) {
      lcd.setCursor(1, 0);
      lcd.write(4);
      lcd.setCursor(1, 1);
      lcd.write(5);
    } else {
      lcd.setCursor(1, 0);
      lcd.write(time / 20 % 2);
      lcd.write((byte)0);
      lcd.setCursor(1, 1);
      lcd.write(' ');
    }
}

void printStoneList() {
  int lastIndex = 0;
  for(int i=0; i < 15; i++) {
    lcd.setCursor(i, 1);
    if(i == 15 - pos[lastIndex]) {
      lcd.write(6);
      lastIndex += 1;     
    } else {
      if(i!=1) {
        lcd.print(' ');      
      }
    }
  }
}

void printPoint() {
  lcd.setCursor(10, 0);
  int point = time / 6;
  if(point < 10) {
    lcd.print("  ");
    lcd.print(point);
  } else if (point < 100) {
    lcd.print(" ");
    lcd.print(point);
  } else {
    lcd.print(point); 
  }
  lcd.write(7);
}

void background() {
  if(gameOver) {
    lcd.setRGB(255,0,0);
    return;
  }
  int world = time / 600 % 4;
  switch(world) {
    case 0:
      lcd.setRGB(255,255,255);
      break;
    case 1:
      lcd.setRGB(53,216,28);
      break;
    case 2:
      lcd.setRGB(33, 198, 245);
      break;
    case 3:
      lcd.setRGB(220, 163, 41);
      break;
  }
}
