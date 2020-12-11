const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int rolePerMinute = 15;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
#include <Stepper.h>
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

#define RELAY_ON 1      // Define relay on pin state
#define RELAY_OFF 0     // Define relay off pin state
 
#define Relay  13       // Arduino pin where the relay connects

#define RELAY_RT        // Define how long relay stays 

void setup() {
  // put your setup code here, to run once:
  //---set pin direction
  myStepper.setSpeed(rolePerMinute);

  digitalWrite(Relay, RELAY_OFF);      // initialise the relay to off
  pinMode(Relay, OUTPUT);  
  
  Serial.begin(9600);

  delay(1000);   
}

bool onoff = false, newData = false;
char receivedChar;
String readString;

void loop() {
  // put your main code here, to run repeatedly:
  recvOneChar();
  runMotor();
  switchRelay();

  delay(1000);
}

//Relay Function(s)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void switchRelay(){
  // O for open, C for closed
  if(newData == true && receivedChar == 'O'){
    //Turns relay on
    digitalWrite(Relay, RELAY_ON);
    
    Reset();
    }
  
  else if(newData == true && receivedChar == 'C'){
    //Turns relay off
    digitalWrite(Relay, RELAY_OFF);     
    Reset();
    }
}

//Motor Function(s)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void runMotor(){
  // F for forward, R for reverse
  if(newData == true && receivedChar == 'F'){
    //Runs motor forward for the motor run time at the set speed
    myStepper.step(stepsPerRevolution/4);
    Reset();
    }
  
  else if(newData == true && receivedChar == 'R'){
    //Runs motor backward for the motor run time at the set speed
    myStepper.step(-stepsPerRevolution/4);
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
