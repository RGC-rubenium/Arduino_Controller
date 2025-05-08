#include <Joystick.h>

// Initialize the Joystick_ object with desired capabilities
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_GAMEPAD,
                   10,  // Number of buttons
                   1,  // Number of hat switches
                   true, true, false,  // X, Y, Z Axis
                   false, false, false,  // Rx, Ry, Rz
                   false, false, false, false, false);  // Rudder, Throttle, etc.

// Dpad
const int buttonMovement[] = {2, 3, 4, 5};  // Up, Down, Left, Right

//ABXY L R Select Pause
const int extraButtons[] = {6, 7, 8, 9, 10, 16, A0, A1, 14, 15};//{Up ,Down, Left, Right, A, B, X, Y, L1, R1, L2, R2, Start, Select}1-10 button mapping (Mandyak sure)

// Debouncing Values
const unsigned long debounceDelay = 50;

//Dpad Mapping

unsigned long lastDebounceTime[sizeof(extraButtons) / sizeof(extraButtons[0])] = {0};
int lastButtonState[sizeof(extraButtons) / sizeof(extraButtons[0])] = {HIGH};

const int xpos = 500;
const int ypos = 500;

void setup() {

  //BTNMove
  for (int i = 0; i < sizeof(buttonMovement) / sizeof(buttonMovement[0]); i++) {
    pinMode(buttonMovement[i], INPUT_PULLUP);
  }

  //Attacks
  for (int i = 0; i < sizeof(extraButtons) / sizeof(extraButtons[0]); i++) {
    pinMode(extraButtons[i], INPUT_PULLUP);
  }

  Joystick.begin();

  Joystick.setXAxis(xpos);
  Joystick.setYAxis(ypos);
}

void loop() {
  //Dpad or Lever
  int hatState = -1;
  int up = digitalRead(buttonMovement[0]);
  int down = digitalRead(buttonMovement[1]);
  int left = digitalRead(buttonMovement[2]);
  int right = digitalRead(buttonMovement[3]);
  
  if (up == LOW && right == LOW) hatState = 45;           //up-right
  else if (up == LOW && left == LOW) hatState = 315;      //up-left
  else if (down == LOW && right == LOW) hatState = 135;   //down-right
  else if (down == LOW && left == LOW) hatState = 225;    //down-left
  else if (up == LOW) hatState = 0;                       //up
  else if (down == LOW) hatState = 180;                   //down
  else if (left == LOW) hatState = 270;                   //left
  else if (right == LOW) hatState = 90;                   //right

  Joystick.setHatSwitch(0, hatState);

//Attack with buttons
  for (int i = 0; i < sizeof(extraButtons) / sizeof(extraButtons[0]); i++) {
    int currentButtonState = digitalRead(extraButtons[i]);

    if (currentButtonState != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      Joystick.setButton(i, currentButtonState == LOW);
    }

    lastButtonState[i] = currentButtonState;
  }
}
