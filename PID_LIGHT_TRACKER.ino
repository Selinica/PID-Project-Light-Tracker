const int ldrpin = A0;
const int ldrpin1 = A1;

//MOTOR
const int ENA = 6;
const int In1 = 8;
const int In2 = 9;

int lastE = 0;
int currenttime;
int lasttime = 0;

  
void setup() {
  //LDRS AND LED
  Serial.begin(9600);
   pinMode(ldrpin, INPUT);
   pinMode(ldrpin1, INPUT);

   //MOTOR
   pinMode(In1, OUTPUT);
   pinMode(In2, OUTPUT);
   pinMode(ENA, OUTPUT);

  Serial.println("CLEARDATA");        //This string is defined as a commmand for the Excel VBA to clear all the rows and columns
  Serial.println("LABEL, Time(ms), Error");  //LABEL command creates label for columns in the first row with bold font                                    
}

 
//PID IMPLEMENTATION


// define some constants

byte PWMOutput;
long Error[10];
long Accumulator;
long PID;
int Kp;
float Ki;
int Kd;
byte Divider;

/* GetError():
Read the analog values, shift the Error array down 
one spot, and load the new error value into the
top of array.
*/


int GetError(void)
{
    int reading = analogRead(ldrpin);
    int reading2 = analogRead(ldrpin1);
    int difference = reading - reading2; 
    return difference;
}

int CalculatePID(void)
{
    // Set constants here
    Kp = 1;
    Ki = 0.2;
    Kd = 2.5;
  
    currenttime = millis()/1000;
    int Dt = currenttime-lasttime;
  
    int E = GetError();
    int Integral =+ E;
    int Differential = E - lastE;
    //Serial.print(E);
    //Serial.print("\t");
    //Serial.print(currenttime);
    //Serial.println();

    Serial.print("DATA,TIME,");
    Serial.print(currenttime);
    Serial.print(",");
    Serial.println(E);
    
    int P = Kp * E;
    int I = Ki * Integral * Dt;
    int D = (Kd * Differential)/Dt;

    int PID = P + I + D;

    return PID;
  
}

void loop() // run over and over
{

     //Serial.println(CalculatePID());
     analogWrite(ENA,CalculatePID());
     if(GetError() >=0)
     {
        digitalWrite(In1,LOW);
        digitalWrite(In2,HIGH);
        //Serial.println("left");
     }
     else if(GetError() < 0)
     {
        digitalWrite(In1,HIGH);
        digitalWrite(In2,LOW);
        //Serial.println("Right");
     }

}
