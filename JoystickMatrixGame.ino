#include <LedControl.h>

int dataPin = 12;
int loadPin = 11;
int clockPin = 10;
int matrixAmount = 1;
LedControl matrix = LedControl(dataPin, clockPin, loadPin, matrixAmount);
int j = 0;

int directions[4][8][8] = {
  {
    {1, 1, 0, 0, 0, 0, 1, 1}, //Up
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {1, 1, 1, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0}, //Down
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 0, 0}
  },
  {
    {1, 1, 0, 0, 0, 0, 0, 0}, //Left
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
  },
  {
    {1, 1, 1, 1, 1, 1, 1, 0}, //Right
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 1, 1, 0, 0},
    {1, 1, 0, 0, 0, 1, 1, 0},
    {1, 1, 0, 0, 0, 0, 1, 1}
  }
};

int win [8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
int lose [8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int xPin = A0;
int yPin = A1;
int xVal;
int yVal;

int seqLength = 1;
int current = 0;
String sequence;
String userInput;
int curInputLength = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  matrix.shutdown(0, false);
  matrix.setIntensity(0, 5);
  matrix.clearDisplay(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  matrix.clearDisplay(0);
  delay(2000);

  while (current < seqLength) {
    int num = SelectDirection();
    MatrixWrite(num);
    current++;
    delay(1000);
    matrix.clearDisplay(0);
  }

  while (curInputLength < seqLength) {
    xVal = analogRead(xPin);
    yVal = analogRead(yPin);
    if(xVal < 1000 && xVal > 20 && yVal < 1000 && yVal > 20){
    }
    else if (xVal >= 1000) {
      userInput = userInput + "3";
      curInputLength++;
    }
    else if (xVal <= 20) {
      userInput = userInput + "2";
      curInputLength++;
    }
    else if (yVal >= 1000) {
      userInput = userInput + "1";
      curInputLength++;
    }
    else if (yVal <= 20) {
      userInput = userInput + "0";
      curInputLength++;
    }
    delay(250);
    Serial.println(userInput);
  }

  if (userInput == sequence) {
    matrix.clearDisplay(0);
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (win[i][j] == 1) {
          matrix.setLed(0, i, j, true);
        }
      }
    }
    seqLength++;
    current = 0;
    sequence = "";
    curInputLength = 0;
    userInput = "";
    delay(3000);
  }
  else {
    matrix.clearDisplay(0);
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (lose[i][j] == 1) {
          matrix.setLed(0, i, j, true);
        }
      }
    }
    seqLength = 1;
    current = 0;
    sequence = "";
    curInputLength = 0;
    userInput = "";
    delay(10000);
  }
}

int SelectDirection() {
  int randomNum = rand() % 4; //Random number between 0-3
  sequence = sequence + randomNum;
  Serial.println(sequence);
  return randomNum;
}

void MatrixWrite(int i) {
  matrix.clearDisplay(0);
  delay(250);
  for (int j = 0; j < 8; j++) {
    for (int k = 0; k < 8; k++) {
      if (directions[i][j][k] == 1) {
        matrix.setLed(0, j, k, true);
      }
    }
  }
}
