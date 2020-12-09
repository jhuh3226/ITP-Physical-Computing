/*--------rotary encoder----------*/
#include <Encoder.h>

// encoder on pins 2 and 3
Encoder myEncoder(2, 3);
// previous position of the encoder:
int lastPosition  = 0;

// steps of the encoder's shaft:
int steps = 100;
/*--------------------------------*/

/*---- potentiometer smoothing----*/
int potentioPin = A0;
int potentioValue;
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;
/*--------------------------------*/

void setup() {
  Serial.begin(9600);

  /*---- potentiometer smoothing----*/
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  /*--------------------------------*/
}

void loop() {
  /*--------rotary encoder----------*/
  int newPosition = myEncoder.read();
  // compare current and last encoder state:
  int change = newPosition - lastPosition;
  // if it's changed by 4 or more (one detent step):
  if (abs(change) >= 4) {
    // get the direction (-1 or 1):
    int encoderDirection = (change / abs(change));
    steps += encoderDirection;
    //Serial.println(steps);
    Serial.print(steps); // stepper directon
    Serial.print(",");
    Serial.println(potentioValue);  // send potentio value to servo

    // save knobState for next time through loop:
    lastPosition = newPosition;

    delay(1);        // delay in between reads for stability
  }
  /*--------------------------------*/

  /*---- potentiometer smoothing----*/
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

  //map 0~1023 (1005) to 0~180
  potentioValue  = map(average, 0, 1023, 0, 180);

  //printing value
  if (abs(change) < 4)
  {
    Serial.print(steps); // stepper directon
    Serial.print(",");
    Serial.println(potentioValue);  // send potentio value to servo
  }
  /*--------------------------------*/
//
//     Serial.print(steps); // stepper directon
//    Serial.print(",");
//    Serial.println(potentioValue);  // send potentio value to servo
}
