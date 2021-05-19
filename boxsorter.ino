/*this code will run the box sorting machine.
  it contains both the setup and loop functions, and acts as 
  the main arduino file
  Created by John B. Bradshaw 2019
*/
// user defined class files
#include <C:\Users\johnb\OneDrive\Documents\Arduino\boxsorter\obj\abd_class\abd_class.ino>
#include <C:\Users\johnb\OneDrive\Documents\Arduino\boxsorter\obj\motor_class\motor_class.ino>
#include <C:\Users\johnb\OneDrive\Documents\Arduino\boxsorter\obj\piston_class\piston_class.ino>
#include <C:\Users\johnb\OneDrive\Documents\Arduino\boxsorter\obj\box_class\box_class.ino>
#include <Wire.h>              //I2C library to communicate with the sensor
#include <Adafruit_TCS34725.h> //Color sensor library

const int ABDPIN = 28;
const int PUMP = 22, S_E = 24, S_R = 26;
const int CPIN1 = 4, CPIN2 = 5;
const int LEDPIN1 = 10, LEDPIN2 = 51;

int cSpeed = 175, gSpeed = 175, gTime = 50, gHold = 50;            // pwm on time 0 to 255 speed (digital)
int boxType;                                                       // box number 0 - 5
int sTime = 1000, cofTime = 1000, popTime = 1500, clearTime = 750; // gate delay
int t = 750;
unsigned long i = 0;              // counter for ABD control
uint16_t red, green, blue, clear; // create color values for sensor getRawData

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Piston piston(PUMP, S_E, S_R);                            // pump, PIN_E, PIN_R
Box clearBox(0), sBox(1), cofBox(2), popBox(3), noBox(4); // box object constructors
ABD abd(ABDPIN);                                          // NEXT pin
Motor cs(CPIN1, CPIN2, NULL);                             // conveyor belt motor constructor, set motor number to NULL
Motor g1(3, 2, 1), g2(7, 6, 2), g3(8, 9, 3);              // gate motor constructor (PINA, PINB, motor number)

void unloadDock(int t)
{
  piston.push();
  delay(t);
  piston.pull();
  delay(t);
  piston.hold();
} // piston control function that unloads the dock for the clear boxes

void setup()
{
  Serial.begin(9600);
  if (tcs.begin())
  { //sensor starts correctly
    Serial.println("Found sensor");
  }
  else
  { //sensor starts incorrectly
    Serial.println("No TCS34725 found ... restart Arduino!");
    while (1)
      ; // halt!
  }

  /*******SENSOR CALIBRATION*********/
  noBox.setRangeClear(0, 500);
  clearBox.setRangeClear(500, 1905);
  cofBox.setRangeClear(1906, 3000);
  sBox.setRangeClear(3001, 4251);
  popBox.setRangeClear(4250, 6000);

  /**********************************/

  piston.setPump(HIGH); // start piston pump

  //motor setup functions
  g1.setSpeed(gSpeed, gTime, gHold);
  g2.setSpeed(gSpeed, gTime, gHold);
  g3.setSpeed(gSpeed, gTime, gHold);
  cs.go(cSpeed); // counter strike: GO! (conveyor motor)

  pinMode(LEDPIN1, OUTPUT); // turn on supporting LED light
  digitalWrite(LEDPIN1, HIGH);
  pinMode(LEDPIN2, OUTPUT); // turn on led light on sensor
  digitalWrite(LEDPIN2, HIGH);
}

void loop()
{
  i++;
  if (i % 2 == 0)   // load a box from the abd every other loop iteration
    abd.dispense(); // this was implemented to reduce the risk of jams or overloading the machine
  delay(2000);
  tcs.getRawData(&red, &green, &blue, &clear); //read the sensor
  tcs.getRawData(&red, &green, &blue, &clear); //read the sensor
  tcs.getRawData(&red, &green, &blue, &clear); //read the sensor
  delay(100);
  tcs.getRawData(&red, &green, &blue, &clear); //read the sensor
  tcs.getRawData(&red, &green, &blue, &clear); //read the sensor
  tcs.getRawData(&red, &green, &blue, &clear); //read the sensor
  // grab a few reading from the sensor to try and improve accuracy
  Serial.print("C:\t");
  Serial.println(clear); //print color value

  // Box sorter algorithm based on clear variable
  if (clear >= sBox.getColorRangelow() && clear <= sBox.getColorRangehigh())
    boxType = sBox.getBoxNum();
  else if (clear >= cofBox.getColorRangelow() && clear <= cofBox.getColorRangehigh())
    boxType = cofBox.getBoxNum();
  else if (clear >= popBox.getColorRangelow() && clear <= popBox.getColorRangehigh())
    boxType = popBox.getBoxNum();
  else if (clear >= clearBox.getColorRangelow() && clear <= clearBox.getColorRangehigh())
    boxType = clearBox.getBoxNum();
  else
    boxType = noBox.getBoxNum();

  Serial.println(boxType);

  // Truth table for motor control
  if (boxType == g1.getMotorNumber()) // skittles box
  {
    g1.openGate();
    piston.push();
    delay(t);
    piston.pull();
    g1.hold(sTime);
    piston.hold();
  }
  else if (boxType == g2.getMotorNumber()) // coffee box
  {
    if (g1.getGateState())
      g1.closeGate();

    g2.openGate();
    piston.push();
    delay(t);
    piston.pull();
    g2.hold(cofTime);
    piston.hold();
  }
  else if (boxType == g3.getMotorNumber()) // popcorn box
  {
    if (g1.getGateState())
      g1.closeGate();
    if (g2.getGateState())
      g2.closeGate();

    g3.openGate();
    piston.push();
    delay(t);
    piston.pull();
    g3.hold(popTime);
    piston.hold();
  }
  else if (boxType == clearBox.getBoxNum()) // clear box
  {
    if (g1.getGateState())
      g1.closeGate();
    if (g2.getGateState())
      g2.closeGate();
    if (g3.getGateState())
      g3.closeGate();

    delay(100);
    unloadDock(clearTime);
  }
  else if (boxType == noBox.getBoxNum()) // no box
  {
    if (g1.getGateState())
      g1.closeGate();
    if (g2.getGateState())
      g2.closeGate();
    if (g3.getGateState())
      g3.closeGate();

    delay(100);
  }
}