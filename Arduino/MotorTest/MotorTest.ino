/*
Motor Testing Program

Functions Made here will go into main program

Exercise the motor using the L293D chip and Serial commands

*/

//These are the default pins we will be using for the motor, 
//but we may change these later
#define ENABLE 5
#define DIRA 3
#define DIRB 4
#define SPEED 255 // Motor speed (0-255)
#define MOTOR_RT 500// How long the motor runs, ms

void setup() {
  // put your setup code here, to run once:
  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);
}

bool onoff = false, newData = false;
char receivedChar;
String readString;

void loop() {
  // put your main code here, to run repeatedly:
  recvOneChar();
  runMotor();

  delay(1000);
}


//Motor Function(s)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void runMotor(){
  // F for forward, R for reverse
  if(newData == true && receivedChar == 'F'){
    //Runs motor forward for the motor run time at the set speed
    analogWrite(ENABLE,SPEED); //enable on
    digitalWrite(DIRA,HIGH); //one way
    digitalWrite(DIRB,LOW);
    
    delay(MOTOR_RT);
    digitalWrite(ENABLE,LOW); //all done
    Reset();
    }
  
  else if(newData == true && receivedChar == 'R'){
    //Runs motor backward for the motor run time at the set speed
    analogWrite(ENABLE,SPEED); //enable on, set at a certain speed
    digitalWrite(DIRA,LOW); 
    digitalWrite(DIRB,HIGH); //one way

    delay(MOTOR_RT);
    digitalWrite(ENABLE,LOW); //all done
    Reset();
    }
}








//Stock Functions:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setOnoff(){
  if(newData == true && receivedChar == 'B'){
    onoff = true;
    Reset();
    }
  else if(newData == true && receivedChar == 'E'){
    onoff = false;
    Reset();
    }
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void recvFullLine(){
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
  }
  
  if (readString.length()>0){
  Serial.print(readString);
  newData = true;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void recvOneChar() {
 if (Serial.available() > 0) {
  receivedChar = Serial.read();
  newData = true;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Reset() {
 if (newData == true) {
  newData = false;
  }

  if(readString.length()>0){
   readString = "";
  }
}
