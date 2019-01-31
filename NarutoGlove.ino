/*
   DIGF 2004 Atelier 1
   Brian Nguyen, Madelaine Fischer-Bernhut, Salisa Jatuweerapong 
   Experiment 3 - Final Prototype
   Naruto Glove Game Controller
*/

// Includes Keyboard library, used to simulate a keyboard
#include <Keyboard.h>

// Pins for sensors on hands
// 2: Dog
// 3: Hare
// 4: Ram
// 5: Boar

// Movement Pins
// Left: A0
// Right: A1

// Integer variable to change sizes of arrays
const int pinCount = 6;
// Integer array that holds the pin numbers of the glove controller
int inputPins[pinCount - 2] = {2, 3, 4, 5};
// Character array that holds the keys to be pressed
char controlKeys[pinCount] = {'i', 'o', 'p', 'u', 'a', 'd'};
// Array of strings, use of * means it is a array of strings
// Used for debugging
char* controlNames[pinCount - 2] = {"Dog", "Hare", "Ram", "Boar"};

// Setup function
void setup() {
  // Starts up serial communciation
  Serial.begin(9600);

  // Starts keyboard library
  Keyboard.begin();

  // Loops through inputPins and sets them to INPUT_PULLUP mode
  // Uses internal pullup resistor so we don't have to use one on the breadboard
  for (int i = 0; i < pinCount; i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
  }
}

// Main loop
void loop() {
  // Integer array that contians the state of every glove pin
  // 0 = Off
  // 1 = On
  int inputState[pinCount] = {0, 0, 0, 0};

  // keyPressed boolean that tracks if a key has been pressed this loop
  bool keyPressed = false;

  // For loop that loops through every pin and updates its value in inputState
  for (int i = 0; i < pinCount; i++) {
    int switchVal = digitalRead(inputPins[i]);
    inputState[i] = switchVal;
  }

  // For loop that loops through inputState and presses corresponding keyboard keys
  // depending if the state of the pin is 0(OFF) or 1(ON)
  for (int i = 0; i < pinCount; i++) {
    // Edgecases that stops a key from being pressed accidentally
    // Happens when some hand signs have two sensors touching at the same time
    if (i == 3 && inputState[0] == LOW) {
      continue;
    }
    if (i == 3 && inputState[1] == LOW) {
      continue;
    }
    // When looping through inputState, if the pin is LOW (On)
    if (inputState[i] == LOW) {
      // Printing debug lines
      Serial.print("Pressing: ");
      // Index of loop
      Serial.print(i);
      Serial.print(' ');
      // Name of the hand sign
      Serial.print(controlNames[i]);
      Serial.print(' ');
      // Name of the key that is being pressed
      Serial.println(controlKeys[i]);
      // Presses the key
      Keyboard.press(controlKeys[i]);
      // Notes that a key was pressed this loop
      keyPressed = true;
    }
  }

  // If statements that check if a person has pressed on the movement pad
  // Left control
  if(analogRead(A0) > 500) {
    Serial.println("Pressing : A");
    Keyboard.press('a');
    keyPressed = true;
    
  }
  // Right control
  if(analogRead(A1) > 500) {
    Serial.println("Pressing : D");
    Keyboard.press('d');
    keyPressed = true;
  }

  // Delay of 10ms 
  // Required otherwise the game wont recognize a key has been pressed
  delay(10);

  // If keyPressed is true, loop through all the keys and releases them
  if (keyPressed == true) {
    for (int i = 0; i < pinCount; i++) {
     // Debug lines
//     Serial.print("Releasing: ");
//     Serial.println(controlKeys[i]);
     // Releases the key
     Keyboard.release(controlKeys[i]);
    }
  }
}