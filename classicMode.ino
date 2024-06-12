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

const int buttonStartGame = 13;

// Define the range of track numbers on the SD card
const int firstTrack = 1;
const int lastTrack = 26; 

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

bool isTimerPlaying = true;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  mySoftwareSerial.begin(9600);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);
  pinMode(purpleLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  pinMode(pinkLED, OUTPUT);
  pinMode(cyanLED, OUTPUT);
  pinMode(newLED1, OUTPUT);
  pinMode(newLED2, OUTPUT);
  pinMode(newLED3, OUTPUT);

  pinMode(redButton, INPUT_PULLUP);
  pinMode(greenButton, INPUT_PULLUP);
  pinMode(yellowButton, INPUT_PULLUP);
  pinMode(blueButton, INPUT_PULLUP);
  pinMode(whiteButton, INPUT_PULLUP);
  pinMode(purpleButton, INPUT_PULLUP);
  pinMode(orangeButton, INPUT_PULLUP);
  pinMode(pinkButton, INPUT_PULLUP);
  pinMode(cyanButton, INPUT_PULLUP);
  pinMode(newButton1, INPUT_PULLUP);
  pinMode(newButton2, INPUT_PULLUP);
  pinMode(newButton3, INPUT_PULLUP);
  pinMode(buttonStartGame, INPUT_PULLUP);


  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) { 
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(20); 
}

void loop() {
  // Check if the start game button is pressed
  if (digitalRead(buttonStartGame) == LOW) { 
    delay(200);

    // Wait for the button to be released
    while (digitalRead(buttonStartGame) == LOW);

    // Delay for 2 seconds
    delay(2000); 

    // Loading animation
    for (int i = 0; i < 3; i++) {
      digitalWrite(redLED, HIGH);
      delay(100);
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, HIGH);
      delay(100);
      digitalWrite(yellowLED, LOW);
      digitalWrite(blueLED, HIGH);
      delay(100);
      digitalWrite(blueLED, LOW);
      digitalWrite(greenLED, HIGH);
      delay(100);
      digitalWrite(greenLED, LOW);
      digitalWrite(whiteLED, HIGH);
      delay(100);
      digitalWrite(whiteLED, LOW);
      digitalWrite(purpleLED, HIGH);
      delay(100);
      digitalWrite(purpleLED, LOW);
      digitalWrite(orangeLED, HIGH);
      delay(100);
      digitalWrite(orangeLED, LOW);
      digitalWrite(pinkLED, HIGH);
      delay(100);
      digitalWrite(pinkLED, LOW);
      digitalWrite(cyanLED, HIGH);
      delay(100);
      digitalWrite(cyanLED, LOW);
      digitalWrite(newLED1, HIGH);
      delay(100);
      digitalWrite(newLED1, LOW);
      digitalWrite(newLED2, HIGH);
      delay(100);
      digitalWrite(newLED2, LOW);
      digitalWrite(newLED3, HIGH);
      delay(100);
      digitalWrite(newLED3, LOW);
    }

    // Play a random track in the background
    int randomTrack = random(firstTrack, lastTrack + 1);
    myDFPlayer.play(randomTrack);

    // Wait for 2 seconds before starting the game
    delay(2000);
    game();
  }
}

void game() {
  Serial.println(">> Começando o jogo...");

  unsigned long lastButtonPressTime = millis();
  bool gameRunning = true;

  const int leds[] = {2, 3, 4, 8, 9, 12, 14, 15, 16, 30, 32, 34};
  const int buttons[] = {5, 6, 7, 31, 33, 35, 19, 20, 21, 39, 41, 43};
  const int ledCount = sizeof(leds) / sizeof(leds[0]);

  while (gameRunning) {
    // Randomly select the number of LEDs to turn on (between 1 and 5)
    int numLedsToTurnOn = random(1, 6);

    // Store the indices of the LEDs to turn on
    int ledIndices[5] = {0};

    // Select random LEDs
    for (int i = 0; i < numLedsToTurnOn; i++) {
      int ledIndex;
      bool alreadySelected;

      // Ensure no duplicate LEDs are selected
      do {
        alreadySelected = false;
        ledIndex = random(0, ledCount);

        // Check if this LED is already selected
        for (int j = 0; j < i; j++) {
          if (ledIndices[j] == ledIndex) {
            alreadySelected = true;
            break;
          }
        }
      } while (alreadySelected);

      ledIndices[i] = ledIndex;
      digitalWrite(leds[ledIndex], HIGH);
    }

    // Wait for a button press or timeout
    while (true) {
      if (millis() - lastButtonPressTime > 1500) {
        Serial.println("Eroou!");
        gameRunning = false;  // End game if no button pressed within 1.5 seconds
        myDFPlayer.stop();    // Stop the track
        Serial.println(">> A musica parou");
        turnOffAllLEDs();     // Turn off all LEDs
        return;  // Exit the game function to reset the game
      }

      bool allLEDsTurnedOff = true;

      // Check if corresponding buttons are pressed for all LEDs
      for (int i = 0; i < numLedsToTurnOn; i++) {
        int ledIndex = ledIndices[i];
        if (digitalRead(leds[ledIndex]) == HIGH && digitalRead(buttons[ledIndex]) == LOW) { 
          digitalWrite(leds[ledIndex], LOW);
          lastButtonPressTime = millis();  // Update last button press time
        }
        if (digitalRead(leds[ledIndex]) == HIGH) {
          allLEDsTurnedOff = false;
        }
      }

      // If all LEDs are turned off, break the loop and select new LEDs
      if (allLEDsTurnedOff) {
        break;
      }

      delay(100);
    }
  }
}

void turnOffAllLEDs() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(whiteLED, LOW);
  digitalWrite(purpleLED, LOW);
  digitalWrite(orangeLED, LOW);
  digitalWrite(pinkLED, LOW);
  digitalWrite(cyanLED, LOW);
  digitalWrite(newLED1, LOW);
  digitalWrite(newLED2, LOW);
  digitalWrite(newLED3, LOW);
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
