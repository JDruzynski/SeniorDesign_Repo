// Keyes SR1y relay blink sketch
 
#define RELAY_ON 1      // Define relay on pin state
#define RELAY_OFF 0     // Define relay off pin state
 
#define Relay  13       // Arduino pin where the relay connects

#define RELAY_RT        // Define how long relay stays 
 
void setup()  
{
  digitalWrite(Relay, RELAY_OFF);      // initialise the relay to off
  pinMode(Relay, OUTPUT);   
  Serial.begin(9600);
  delay(1000);   
}

bool onoff = false, newData = false;
char receivedChar;
String readString;
 
void loop()         
{
  recvOneChar();
  switchRelay();


  delay(1000);
}


//Relay Function(s)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void switchRelay(){
  // O for open, C for closed
  if(newData == true && receivedChar == '0'){
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
