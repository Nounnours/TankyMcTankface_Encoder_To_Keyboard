/* Tanky McTankFace Knobby HandWheel Knob of Knobbyness
 * Look at my code.
 * My code is amazing
 * Give it a lick!
 * It smells just like raisins...
 */

#include <Encoder.h>
#include <Keyboard.h>
#include <Mouse.h>
uint8_t buf[8] = { 0 }; //Keyboard report buffer

uint16_t blankKeyTime = 10;
uint16_t blankKeyTimeMouse = 100;

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobLeft(2, 13);
Encoder knobRight(5, 4);
//   avoid using pins with LEDs attached
const int leftClickInput = 8;
const int rightClickInput = 9;

void setup() {
  Serial.begin(115200);
  pinMode(leftClickInput, INPUT);
  pinMode(rightClickInput, INPUT);
  Keyboard.begin();
  Mouse.begin();
  Serial.println("Tanky McTankFace Knobby HandWheel Knob of Knobbyness Look at my code. My code is amazing Give it a lick! It smells just like raisins...");
}

long positionLeft  = -999;
long positionRight = -999;
uint8_t mouseLeftClickLast;
uint8_t mouseRightClickLast;

long lastCounter = 0;
uint32_t lastPosition = 0;
bool KeypressedWS = false;
bool KeypressedAD = false;
bool wPressed = false;
bool sPressed = false;
bool aPressed = false;
bool dPressed = false;
bool leftClickPressed = false;
bool rightClickPressed = false;
bool MousePressedAny = false;
uint32_t keyPressedTimeW = 0;
uint32_t keyPressedTimeS = 0;
uint32_t keyPressedTimeA = 0;
uint32_t keyPressedTimeD = 0;
uint32_t leftClickPressedTime  = 0;
uint32_t rightClickPressedTime = 0;
bool leftForward = false;
bool rightForward = false;

long newLeft, newRight;
bool newMouseLeft, newMouseRight;
void loop() 
{
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  // if (Serial.available())
  // {
  //   Serial.read();
  //   Serial.println("Reset both knobs to zero");
  //   knobLeft.write(0);
  //   knobRight.write(0);
  // }
  uint32_t potatoes = millis();
  // Serial.print("Direction: ");
  // Serial.print(leftForward);
  // Serial.print(" :");
  // Serial.print(newLeft);
  // Serial.print(" :");
  // Serial.print(rightForward);
  // Serial.print(" :");
  // swswswswswswswswswsw.println(newRight);
  newMouseLeft  = (digitalRead(leftClickInput) == LOW);
  newMouseRight = (digitalRead(rightClickInput) == LOW);
  if (newMouseLeft == mouseLeftClickLast) {}
  else if (!leftClickPressed && (newMouseLeft != mouseLeftClickLast) && (mouseLeftClickLast == LOW))
  {
    if (!MousePressedAny)
    {
      Mouse.press(MOUSE_LEFT);
      leftClickPressed = true;
      MousePressedAny = true;
      leftClickPressedTime = millis();
      mouseLeftClickLast = newMouseLeft;
      Serial.println("mouse click left");
    }
  }
  if (newMouseRight == mouseRightClickLast) {}
  else if (!rightClickPressed && (newMouseRight != mouseRightClickLast) && (mouseRightClickLast == LOW))
  {
    if (!MousePressedAny)
    {
      Mouse.press(MOUSE_RIGHT);
      rightClickPressed = true;
      MousePressedAny = true;
      rightClickPressedTime = millis();
      mouseRightClickLast = newMouseRight;
      Serial.println("mouse click right");
    }
  }
  
  if (newLeft > positionLeft)
  {
    leftForward = true;
    if(!KeypressedWS)
    {
      Keyboard.press('w');
      wPressed = true;
      KeypressedWS = true;
      keyPressedTimeW = millis();
      // Serial.println("Key W Pressed");
    }
  }
  else if (newLeft == positionLeft) {}  //skip
  else
  {
    leftForward = false;
    if(!KeypressedWS)
    {
      Keyboard.press('s');
      sPressed = true;
      KeypressedWS = true;
      keyPressedTimeS = millis();
      // Serial.println("Key S Pressed");
    }
  }

  if (newRight > positionRight)
  {
    rightForward = true;
    if(!dPressed)
    {
      Keyboard.press('d');
      dPressed = true;
      KeypressedAD = true;
      keyPressedTimeD = millis();
      // Serial.println("Key D Pressed");
    }
  }
  else if (newRight == positionRight) {}  //skip
  else
  {
    rightForward = false;
    if(!aPressed)
    {
      Keyboard.press('a');
      aPressed = true;
      KeypressedAD = true;
      keyPressedTimeA = millis();
      // Serial.println("Key A Pressed");
    }
  }

  uint32_t timeDiffW = potatoes - keyPressedTimeW;
  uint32_t timeDiffS = potatoes - keyPressedTimeS;
  uint32_t timeDiffA = potatoes - keyPressedTimeA;
  uint32_t timeDiffD = potatoes - keyPressedTimeD;
  uint32_t timeDiffML = potatoes - leftClickPressedTime;
  uint32_t timeDiffMR = potatoes - rightClickPressedTime;

  if (KeypressedWS && ((timeDiffW > blankKeyTime) || (timeDiffS > blankKeyTime)))
  {
    if (wPressed) Keyboard.release('w');
    if (sPressed) Keyboard.release('s');
    wPressed = false;
    sPressed = false;
    KeypressedWS = false;
    // Serial.println("Key WS released");
  }
  if (KeypressedAD && ((timeDiffA > blankKeyTime) || (timeDiffD > blankKeyTime)))
  {
    if (aPressed) Keyboard.release('a');
    if (dPressed) Keyboard.release('d');
    aPressed = false;
    dPressed = false;
    KeypressedAD = false;
    // Serial.println("Key AD released");
  }
  if (MousePressedAny)
  {
    if (leftClickPressed && (timeDiffML > blankKeyTimeMouse))
    {
      Mouse.release(MOUSE_LEFT);
      leftClickPressed = false;
    }
    if (rightClickPressed && (timeDiffMR > blankKeyTimeMouse))
    {
      Mouse.release(MOUSE_RIGHT);
      rightClickPressed = false;
    }
    if (!leftClickPressed && !rightClickPressed) MousePressedAny = false;
    // Serial.println("Key Mouse released");
  }


  if (newLeft != positionLeft)
  {
    positionLeft = newLeft;
  }  
  if (newRight != positionRight)
  {
    positionRight = newRight;
  }
  if (mouseLeftClickLast != newMouseLeft) mouseLeftClickLast = newMouseLeft;
  if (mouseRightClickLast != newMouseRight) mouseRightClickLast = newMouseRight;
}
