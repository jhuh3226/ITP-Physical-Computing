#include <Encoder.h>

// encoder on pins 2 and 3
Encoder myEncoder(2, 3);

// previous position of the encoder:
long newPosition = 0;
int lastPosition  = 0;
int checkLastPosition  = 0;
long checkNewPosition = 0;

/*---------smoothing value---------*/
int potentioPin = A0;
int potentioValue;
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;
/*--------------------------------*/

int difference;
int stepperDirection;
boolean turnDetected;
int turned;

void setup() {
  Serial.begin(9600);

  /*---------smoothing value---------*/
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  /*--------------------------------*/
}

void loop() {

  newPosition = myEncoder.read();
  if (newPosition != lastPosition) {
    lastPosition = newPosition;
    checkNewPosition = newPosition; // if ther's change record that

    difference = checkNewPosition - checkLastPosition;

    turnDetected = true;
    turned = 1;

    Serial.print(checkNewPosition); // rotary encoder position
    Serial.print(",");
    Serial.print(turned); // rotaryturning true or false
    Serial.print(",");
    Serial.print(stepperDirection); // stepper directon
    Serial.print(",");
    Serial.println(potentioValue);  // send potentio value to servo

    //    Serial.print("checkLastPosition: ");
    //    Serial.println(checkLastPosition);
    //    Serial.print("checkNewPosition: ");
    //    Serial.println(checkNewPosition);
  }
  else {
    turnDetected = false;
    turned = 0;
  }
  checkLastPosition = checkNewPosition;

  if (turnDetected) {
    if (difference == 1)
    {
      stepperDirection = 1;
    } else if (difference == -1) {
      stepperDirection = -1;
    }
  }

  /*---------smoothing value---------*/
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(potentioPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  delay(1);        // delay in between reads for stability
  /*--------------------------------*/

  //map 0~1023 (1005) to 0~180
  potentioValue  = map(average, 0, 1023, 0, 180);

  if (newPosition == lastPosition)
  {
    Serial.print(checkNewPosition); // rotary encoder position
    Serial.print(",");
    Serial.print(turned); // rotaryturning true or false
    Serial.print(",");
    Serial.print(stepperDirection); // stepper directon
    Serial.print(",");
    Serial.println(potentioValue);  // send potentio value to servo
  }
}
