//servo library
#include <Servo.h>

/*----------------------------------servo-------------------------------------*/
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
bool turned = false;    //variable to check servo's turn
/*------------------------------------------------------------------------------------*/

/*----------------------------------push button-------------------------------------*/
const int  buttonPin = 2;    // the pin that the pushbutton is attached to
const int led1Pin = 3;       // the pin that the LED is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
/*------------------------------------------------------------------------------------*/


/*----------------------------------potentiometer-------------------------------------*/
const int potentioPin = A0;   // select the input pin for the potentio
const int led2Pin = 4;        // the pin that the LED is attached to

// Variables will change:
int potentioValue = 0;        // value read from the thermistor sensor

int potentioLow = 1;
int potentioAverage = 2;
int potentioHigh = 3;
int lastpotentio = 2;   //potentio starts from potentioAverage
int currentpotentio;

int Status;    //value checking the potentioStatus and store that value into checkStatus array

int checkStatus[6];     //checkStatus has arrays of 3 and checks the potentio recordings

int arrayPotentioIndex = -1;    //arrayPotentioIndex starts from -1 so that when themperature mode changes, the arrayPotentioIndex becomes 1
/*------------------------------------------------------------------------------------*/


/*----------------------------------thermo and fsr-------------------------------------*/

const int thermoSensor = A1;   // select the input pin for the thermo sensor
const int fsrSensor = A2;   // select the input pin for the FSR
const int led3Pin = 5;      // select the pin for the LED3
const int led4Pin = 6;      // select the pin for the LED5

int thermoValue = 0;        // value read from the thermistor sensor
int thermoMapValue = 0;     // value mapped from the thermistor sensor
int fsrValue = 0;        // value read from the FSR sensor

int thermoLow = 1;
int thermoAverage = 2;
int thermoHigh = 3;
int lastThermo = 2;   //thermo starts from thermoAverage
int currentThermo;

int temperature;    //value checking the thermoStatus and store that value into checkTemp array

int checkTemp[3];     //checkTemp has arrays of 3 and checks the thermo recordings

int arrayIndex = -1;    //arrayIndex starts from -1 so that when themperature mode changes, the arrayIndex becomes 1

int currentMillis = 0;
/*------------------------------------------------------------------------------------*/

// bool
bool unlock1 = false;
bool unlock2 = false;
bool unlock3 = false;     // bool to check unlock status of step3
bool unlock4 = false;     // bool to check unlock status of step4

bool unlocked1 = false;
bool unlocked2 = false;
bool unlocked3 = false;     // bool to check unlock status of step3
bool unlocked4 = false;     // bool to check unlock status of step4

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);

  // declare the ledPin as an OUTPUT:
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);

  //servo
  myservo.attach(7);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  //Serial.println(millis());


  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  // read the value from the sensors
  thermoValue = analogRead(thermoSensor);

  /*----------------------------------push button-------------------------------------*/
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;


  // turns on the LED every three button pushes by checking the modulo of the
  // button push counter. the modulo function gives you the remainder of the
  // division of two numbers:
  if (buttonPushCounter == 0) {
    digitalWrite(led1Pin, LOW);
  } else if (buttonPushCounter % 3 == 0) {
    digitalWrite(led1Pin, HIGH);

    //unlock the step 1
    unlock1 = true;

    //Serial.println("unlocked step1");
  } else {
    digitalWrite(led1Pin, LOW);
  }
  /*----------------------------------push button-------------------------------------*/


  /*----------------------------------potentiometer-------------------------------------*/
  // read the value from the sensors
  potentioValue = analogRead(potentioPin);

  //------------------------------------------------------------------
  if (potentioValue > 0 && potentioValue <= 311)   //  if potentio is low
  {
    currentpotentio = potentioLow;
    if (currentpotentio != lastpotentio)   //if last state is avg or high
    {
      arrayPotentioIndex ++;
      Status = 1;
      //      tone(2, 440, 500);
      //      delay(500);
      Serial.println("potentioGot lower");
      //      Serial.print("\t arrayPotentioIndex: ");
      //      Serial.println(arrayPotentioIndex);
    }
  }

  //------------------------------------------------------------------
  if (potentioValue > 371 && potentioValue <= 652) // if potentio value is in average
  {
    currentpotentio = potentioAverage;
    if (currentpotentio != lastpotentio)   //if last state is low or high
    {
      arrayPotentioIndex ++;
      Status = 2;

      //out put sound of MIDDLE only when it is used at 5th step
      //      if (unlock3 == true) {
      //tone(2, 660, 500);
      //      delay(500);
      //      }
      Serial.println("potentioGot average");
    }
  }

  //------------------------------------------------------------------
  if (potentioValue > 712) // if potentio value is high
  {
    currentpotentio = potentioHigh;
    if (currentpotentio != lastpotentio) //if last state is low or avg
    {
      arrayPotentioIndex ++;
      Status = 3;
      //      tone(2, 880, 500);
      //      delay(500);
      Serial.println("potentioGot higher");
      //      Serial.print("\t arrayPotentioIndex: ");
      //      Serial.println(arrayPotentioIndex);
    }
  }

  //------------------------------------------------------------------
  lastpotentio = currentpotentio;
  //------------------------------------------------------------------

  //if user tried out three times, then it sets index to 0 again
  arrayPotentioIndex = arrayPotentioIndex % 6;

  //record Statuserautre change
  checkStatus[arrayPotentioIndex] = Status;


  //------------------------------------------------------------------
  //if potentio sensor changes from low->middle->high-->middle->low-->middle
  if (checkStatus[0] == 1 && checkStatus[1] == 2 && checkStatus[2] == 3 && checkStatus[3] == 2 && checkStatus[4] == 1 && checkStatus[5] == 2)
  {
    //Serial.println("unlocked step2");
    digitalWrite(led2Pin, HIGH);
    unlock3 = true;
  }

  //if user tried 3 times but didn't get the puzzle solved, print console "try again"
  if (arrayPotentioIndex == 5)
  {
    if (unlock3 == false)
    {
      //looping and stucking
      for (int i; i < 6; i++)
      {
        checkStatus[i] = 0;
      }
      //beep
      lastpotentio = 2;
      //      Serial.println("try again");
    }
  }
  /*----------------------------------potentiometer-------------------------------------*/


  /*----------------------------------thermo-------------------------------------*/
  //------------------------------------------------------------------
  //Serial.println(thermoValue);

  if (thermoValue <= 460)   //  if thermo is low
  {
    currentThermo = thermoLow;
    if (currentThermo != lastThermo)   //if last state is avg or high
    {
      if (millis() - currentMillis > 2000)
      {
        arrayIndex ++;
        temperature = 0;
        Serial.println("thermoGot lower");
        //      Serial.print("\t arrayIndex: ");
        //      Serial.println(arrayIndex);
        currentMillis = millis();
      }
    }
  }

  //------------------------------------------------------------------
  if (thermoValue > 500 && thermoValue < 510)   // if thermo value is in average
  {
    currentThermo = thermoAverage;
  }

  //------------------------------------------------------------------
  if (thermoValue > 580)   // if thermo value is high
  {
    currentThermo = thermoHigh;
    if (currentThermo != lastThermo) //if last state is low or avg
    {
      if (millis() - currentMillis > 2000)
      {
        arrayIndex ++;
        temperature = 1;
        Serial.println("thermoGot higher");
        //      Serial.print("\t arrayIndex: ");
        //      Serial.println(arrayIndex);
        currentMillis = millis();
      }
    }
  }
  //------------------------------------------------------------------
  lastThermo = currentThermo;
  //------------------------------------------------------------------

  //if user tried out three times, then it sets index to 0 again
  arrayIndex = arrayIndex % 3;

  //record temperautre change
  checkTemp[arrayIndex] = temperature;

  //  Serial.print("checkTemp[0] = ");
  //  Serial.print(checkTemp[0]);
  //
  //  Serial.print("\t checkTemp[1] = ");
  //  Serial.println(checkTemp[1]);
  //
  //  Serial.print("\t checkTemp[2] = ");
  //  Serial.println(checkTemp[2]);

  //------------------------------------------------------------------
  //if thermo sensor changes from high -> low -> high, unlock3 and light up the led3
  if (checkTemp[0] == 1 && checkTemp[1] == 0 && checkTemp[2] == 1)
  {
    //Serial.println("unlocked step3");
    digitalWrite(led3Pin, HIGH);
    unlock3 = true;
  }

  //------------------------------------------------------------------
  //if user tried 3 times but didn't get the puzzle solved, print console "try again"
  if (arrayIndex == 2)
  {
    if (unlock3 == false)
    {
      //beep
      //Serial.println("try again");
    }
  }
  /*------------------------------------------------------------------------------------*/


  /*----------------------------------FSR-------------------------------------*/
  //------------------------------------------------------------------
  fsrValue = analogRead(fsrSensor);
  //Serial.println(fsrValue);
  //if FSR value "sensor value" is bigger than 600, turn on the LED4
  if (fsrValue >= 100)
  {
    digitalWrite(led4Pin, HIGH);
    unlock4 = true;
  }
  /*------------------------------------------------------------------------------------*/

  /*----------------------------------servo-------------------------------------*/

  //if unlock 1,2,3,4, is true, then move the servo
  if (unlock4)
  {
    //turn the servo only if it is not turned previously
    if (turned == false)
    {
      myservo.write(180);
      //change the boolean so that servo only turns once
      turned = true;
    }
  }
  /*------------------------------------------------------------------------------------*/

}
