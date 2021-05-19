/* This file defines the piston class by setting relay Pins
   contains a state variable piston position and pump state
   or retracted, and contains member functions for push, pull and hold 
   Created by John B. Bradshaw 2019   
*/
class Piston{ 
    private:
        int pump_relay,
            relay_extend,
            relay_retract; // Relay Set Pins
        int time; // used for arduinos delay function
        volatile int switchState; // piston state
        volatile int pumpState; // pump state control variable
    public: // constructor
    Piston(int rP, int rE, int rR)// PUMP, EXTEND, RETRACT
    {
        pump_relay = rP;
        relay_extend = rE;
        relay_retract = rR;
        switchState = LOW;
        time = 0;

        pinMode(pump_relay, OUTPUT);
        pinMode(relay_extend, OUTPUT);
        pinMode(relay_retract, OUTPUT);

        digitalWrite(pump_relay, switchState);
        digitalWrite(relay_extend, switchState);
        digitalWrite(relay_retract, switchState); // Set all to off
    }  
    void setPump(volatile int S) // turn on pump
    {
        pumpState = S;
        digitalWrite(pump_relay, pumpState);
    }
    void push()
    {
        switchState = HIGH;
        digitalWrite(relay_extend, !switchState);
        digitalWrite(relay_retract, switchState);
    }
    void pull()
    {
        switchState = HIGH;
        digitalWrite(relay_extend, switchState);
        digitalWrite(relay_retract, !switchState);
    }
    void hold()
    {
        switchState = LOW;
        digitalWrite(relay_extend,switchState);
        digitalWrite(relay_retract,switchState);
    }
};