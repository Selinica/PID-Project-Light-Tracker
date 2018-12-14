const int ldrpin = A0;
const int ldrpin1 = A1;

//MOTOR
const int ENA = 6;
const int In1 = 8;
const int In2 = 9;
  
void setup() {
  //LDRS AND LED
  Serial.begin(9600);
   pinMode(ldrpin, INPUT);
   pinMode(ldrpin1, INPUT);
   pinMode(LED, OUTPUT);

   //MOTOR
   pinMode(In1, OUTPUT);
   pinMode(In2, OUTPUT);
   pinMode(ENA, OUTPUT);
}

//PID IMPLEMENTATION


// define some constants

byte PWMOutput;
long Error[10];
long Accumulator;
long PID;
int Kp;
int Ki;
int Kd;
byte Divider;

/* GetError():
Read the analog values, shift the Error array down 
one spot, and load the new error value into the
top of array.
*/

int actpos =0;   
int despos = 0;

void GetError(void)
{
  int i = 0;
// read analogs, values will be used to graph data
  actpos = analogRead(ldrpin1) - analogRead(ldrpin); 
// comment out to speed up PID loop
//  Serial.print("ActPos= ");
//  Serial.println(ActualPosition,DEC);
/*
  Serial.print("test");
     Serial.print(actpos);
     Serial.print('\n');
     delay(250);
*/
// comment out to speed up PID loop
//  Serial.print("despos= ");
//  Serial.println(despos,DEC);

  // shift error values
  for(i=9;i>0;i--)
    Error[i] = Error[i-1];
  // load new error into top array spot  
  Error[0] = actpos - despos;
// comment out to speed up PID loop
//  Serial.print("Error= ");
//  Serial.println(Error[0],DEC);
   

}

/* CalculatePID():
Error[0] is used for latest error, Error[9] with the Kd
*/
void CalculatePID(void)
{
// Set constants here
  Kp = 2000;
  Ki = 25;
  Kd = 0;
  Divider = 10;
  
// Calculate the PID  
  PID = Error[0]*Kp;     // start with proportional gain
  Accumulator += Error[0];  // accumulator is sum of errors
  PID += Ki*Accumulator; // add integral gain and error accumulation
  PID += Kd*(Error[0]-Error[9]); // differential gain comes next
  PID = PID>>Divider; // scale PID down with divider

  
//comment out to speed up the PID
//Serial.print("PID= ");
//Serial.println(PID,DEC);

// stay within the PWM range (1-254)

  if(PID>=127)
    PID = 127;
  if(PID<=-126)
    PID = -126;   
  PWMOutput = PID + 126; //was 127
//comment this out to speed up program
//  Serial.print("PWMOutput= ");
//  Serial.println(PWMOutput,DEC);

}

void loop() // run over and over
{
     
     GetError();       // Get position error
     CalculatePID();   // Calculate the PID output from the error
     analogWrite(ENA,PWMOutput);  // Set motor speed
     
     if (actpos > 0) {
      //move right
        digitalWrite(In1, LOW);
        digitalWrite(In2, HIGH);
        Serial.print(digitalRead(9));
        Serial.print(digitalRead(0)); 
        Serial.print('\n');
     } 
     else {
      digitalWrite(In1, HIGH);
      digitalWrite(In2, LOW);
     }
}
