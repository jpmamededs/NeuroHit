#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int greenLED = 2;
const int redLED = 3;
const int yellowLED = 4;
const int blueLED = 8;
const int whiteLED = 9;
const int purpleLED = 12;
const int orangeLED = 14;
const int pinkLED = 15;
const int cyanLED = 16;
const int newLED1 = 30;
const int newLED2 = 32;
const int newLED3 = 34;

const int leds[] = {greenLED, redLED, yellowLED, blueLED, whiteLED, purpleLED, orangeLED, pinkLED, cyanLED, newLED1, newLED2, newLED3};
const int ledCount = sizeof(leds) / sizeof(leds[0]);

const int greenButton = 5;
const int redButton = 6;
const int yellowButton = 7;
const int blueButton = 31;
const int whiteButton = 33;
const int purpleButton = 35;
const int orangeButton = 19;
const int pinkButton = 20;
const int cyanButton = 21;
const int newButton1 = 39; 
const int newButton2 = 41;
const int newButton3 = 43;

const int buttons[] = {greenButton, redButton, yellowButton, blueButton, whiteButton, purpleButton, orangeButton, pinkButton, cyanButton, newButton1, newButton2, newButton3};

const int buttonStartGame = 13;

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

bool gameStarted = false;

void setup() {

  for (int i = 0; i < ledCount; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
  pinMode(buttonStartGame, INPUT_PULLUP);

  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  myDFPlayer.volume(10); 

  randomSeed(analogRead(0));
}

void loop() {
  if (digitalRead(buttonStartGame) == LOW) {
    delay(50); 
    if (digitalRead(buttonStartGame) == LOW) {
      gameStarted = false; 

      myDFPlayer.stop();

      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < ledCount; j++) {
          digitalWrite(leds[j], HIGH);
        }
        delay(200);
        for (int j = 0; j < ledCount; j++) {
          digitalWrite(leds[j], LOW);
        }
        delay(200);
      }
    }
  }

  if (!gameStarted) {
    bool allButtonsHeld = true;
    for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
      if (digitalRead(buttons[i]) == HIGH) {
        allButtonsHeld = false;
        break;
      }
    }

    if (allButtonsHeld) {
      // Start animation
      for (int i = 0; i < 10; i++) {
        for (int j = 0; j < ledCount; j++) {
          digitalWrite(leds[j], HIGH);
          delay(50);
          digitalWrite(leds[j], LOW);
        }
      }

      // Light a random LED
      int randomIndex = random(ledCount);
      digitalWrite(leds[randomIndex], HIGH);

      // Play track 1
      myDFPlayer.play(1);

      // Keep the LED on for 3 seconds
      delay(3000);
      digitalWrite(leds[randomIndex], LOW);

      // Wait until all buttons are released
      bool allButtonsReleased = false;
      while (!allButtonsReleased) {
        allButtonsReleased = true;
        for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
          if (digitalRead(buttons[i]) == LOW) {
            allButtonsReleased = false;
            break;
          }
        }
        delay(50);
      }

      gameStarted = true;
    }
  }
}
