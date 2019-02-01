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
// 3: Ox
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
char controlKeys[pinCount] = {'i', 'p', 'o', 'u', 'a', 'd'};
// Array of strings, use of * means it is a array of strings
// Used for debugging
char* controlNames[pinCount - 2] = {"Dog", "Ox", "Ram", "Boar"};

int leftMin = 1023;
int leftMax = 0;

int rightMin = 1023;
int rightMax = 0;

// Setup function
void setup() {
  // Starts up serial communciation
  Serial.begin(9600);

  // Starts keyboard library
  Keyboard.begin();

  // Loops through inputPins and sets them to INPUT_PULLUP mode
  // Uses internal pullup resistor so we don't have ot use one on the breadboard
  for (int i = 0; i < pinCount; i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
  }


  // Calibration phase
  while (millis() < 5000) {
    int leftValue = analogRead(A0);
    int rightValue = analogRead(A1);

    // record the maximum sensor value
    if (leftValue > leftMax) {
      leftMax = leftValue;
    }

    // record the minimum sensor value
    if (leftValue < leftMin) {
      leftMin = leftValue;
    }

    // record the maximum sensor value
    if (rightValue > rightMax) {
      rightMax = rightValue;
    }

    // record the minimum sensor value
    if (rightValue < rightMin) {
      rightMin = rightValue;
    }
  }

}

// Main loop
void loop() {
  int leftValue = analogRead(A0);
  int rightValue = analogRead(A1);

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
    //    if (i == 3 && inputState[0] == LOW) {
    //      continue;
    //    }
    //    if (i == 3 && inputState[1] == LOW) {
    //      continue;
    //    }
    if (i == 2 && inputState[1] == LOW) {
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

  leftValue = map(leftValue, leftMin, leftMax, 0, 255);
  rightValue = map(rightValue, rightMin, rightMax, 0, 255);

  leftValue = constrain(leftValue, 0, 255);
  rightValue = constrain(rightValue, 0, 255);


  // If statements that check if a person has pressed on the movement pad
  // Left control
  if (leftValue > 200) {
    Serial.println("Pressing : A");
    Keyboard.press('a');
    keyPressed = true;

  }
  Serial.println(leftValue);
  Serial.println(rightValue);
  Serial.println(analogRead(A0));
  Serial.println(analogRead(A1));

  // Right control
  if (rightValue > 200) {
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
