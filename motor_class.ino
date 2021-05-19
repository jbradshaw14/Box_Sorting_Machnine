/* This defines the gate motor class
    the data structer sets the motor pins
    and sets the default state to closed (a box can pass through)
    member functions spin the motor cw or ccw based on given inputs
    and change weather or not the gate is open or closed based on a boolean value
    ***The go function is used ONLY for the conveor belt motor***
    Created by John B. Bradshaw 2019
    */

class Motor{
    private:
        int PinCW, PinCCW; // pin A and B (analog)
        int speed; // 0 to 255 speed
        int onTime; // time the motor spins
        int motorNum; // recieve an input from the sensor based on the box type for each motor
        int holdval; // set holding strength

        bool gateState; // tell is open(can pass) or closed(cannot pass)
     
    public:    //Consructor --> create motor, initalize member vars and state
    Motor(int a, int b, int num){ 
        PinCW = a;  // accept and set encoder pin A and B
        PinCCW = b;  
        pinMode(PinCW, OUTPUT);
        pinMode(PinCCW, OUTPUT);

        motorNum = num; // number 1, 2, 3...
        gateState = false; // set the gate to be closed by default
    }

    void go(int s) // Make it go fast
    {
        speed = s;
        analogWrite(PinCW, speed);
        analogWrite(PinCCW, 0);
    }
    void openGate()
    { //move the motor a bit cw
        analogWrite(PinCW, speed);
        analogWrite(PinCCW, 0);
        delay(onTime); 
        analogWrite(PinCW, holdval);
        analogWrite(PinCCW, 0);
        gateState = true;
    }
    void closeGate()
    { //move the motor a bit ccw
        analogWrite(PinCW, 0);
        analogWrite(PinCCW, speed);
        delay(onTime);
        analogWrite(PinCW, 0);
        analogWrite(PinCCW, holdval);
        gateState = false;
    }
    void hold(int t){
      analogWrite(PinCW, holdval);
      analogWrite(PinCCW, 0);
      delay(t);
    }
    
    int getMotorNumber() 
    { return motorNum; }

    void setSpeed(int s, int t, int h)
    {
        speed = s; // 0 to 255
        onTime = t; // time of pulse
        holdval = h; // set hold strength
    }

    void setGateState(bool state) 
    { gateState = state; }
    bool getGateState() 
    {  return gateState;  }
};
