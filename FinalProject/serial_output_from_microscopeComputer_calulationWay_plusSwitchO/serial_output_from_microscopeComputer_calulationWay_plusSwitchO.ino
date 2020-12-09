#include "Stepper.h"
const int stepsPerRevolution = 64;  //***** point

const int wireSwitch = 7; // wire switch pin
int switchValue;
int resetNewPosition;
int switchHigh = false;

// previous position of the encoder:
long newPosition = 0;
int lastPosition  = 0;
int checkLastPosition  = 0;
long checkNewPosition = 0;

int difference;

int stepperInput = 149; //***** point
int stepperDirection;

volatile boolean turnDetected;

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    Serial.write(inByte);

    newPosition = inByte;
    if (newPosition != lastPosition) {
      lastPosition = newPosition;
      checkNewPosition = newPosition; // if ther's change record that

      //    Serial.print("checkLastPosition: ");
      //    Serial.println(checkLastPosition);
      //      Serial.print("checkNewPosition: ");
      //      Serial.println(checkNewPosition);

      difference = checkNewPosition - checkLastPosition;
      //    Serial.println(difference);

      turnDetected = true;
    }
    else {
      turnDetected = false;
    }
    checkLastPosition = checkNewPosition;

    if (turnDetected) {
      //    Serial.println(stepperInput);

      if (difference == 1)
      {
        stepperDirection = 1;
      } else if (difference == -1) {
        stepperDirection = -1;
      }

      //important part where it rotates stepper
      myStepper.step(stepperInput * stepperDirection);
      delay(3);
    }
  }

  switchValue = digitalRead(wireSwitch);
  if (switchValue == HIGH)
  {
    if (!switchHigh)
    {
      //      Serial.println("switch on");
      switchHigh = true;

      reset();
    }
  } else {
    switchHigh = false;
  }

}

// reset encoder value on switch on
void reset()
{
  // Serial.println("reset");
  //  lastPosition  = 0;
  //  newPosition = 0;
  //  checkLastPosition  = 0;
  //  checkNewPosition = 0;

  Serial.println(7); // send this to p5.js
}
