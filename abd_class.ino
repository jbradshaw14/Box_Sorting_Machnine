/* Defines the automatic box dispenser class
   this class has a pin value and a pulse function
   Created by John B. Bradshaw 2019
*/
class ABD
{
    private:
        int npin;
        //volatile int state;
    public:
        ABD(int pin)
        {
            npin = pin; // accpet and set pin
            pinMode(npin, OUTPUT);
            digitalWrite(npin, LOW); // default pin off
        }

    void dispense(){
        digitalWrite(npin, HIGH);
        delay(15);
        digitalWrite(npin, LOW);
    }
};