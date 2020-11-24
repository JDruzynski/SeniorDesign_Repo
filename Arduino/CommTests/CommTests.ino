/*
This is to test bare bones communication with Matlab. 
Just needs an arduino and matlab, no wiring.

Testing Ground for functions to copy into the main scripts we'll be using.
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  digitalWrite(13,HIGH);
}

bool onoff = false, newData = false;
char receivedChar;
String readString;
float dataArray[3];

void loop() {
  // put your main code here, to run repeatedly:
  //recvFullLine();

  
  recvOneChar();
  //AHAH();
  setOnoff();
  if(onoff){    
    digitalWrite(13,HIGH);    

  dataArray[0] = random(10);
  dataArray[1] = random(10);
  dataArray[2] = random(10);
  sendArr();
  }
  else{
    digitalWrite(13,HIGH);
  }


  delay(1000);
}


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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sendArr(){
    
  if(onoff){
    for(int i=0; i<sizeof(dataArray) / sizeof(dataArray[0]); i++){
      //Serial.print("<");
      //Serial.print(i);
      //Serial.print(",");
      Serial.print(dataArray[i]);
      Serial.print("|");
   }
   Serial.print("\n");
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AHAH(){
  if(newData == true && receivedChar == 'B'){
    onoff = true;
    Reset();
    }
  else if(newData == true && receivedChar == 'E'){
    onoff = false;
    Reset();
    }
    
  if(onoff){
    Serial.println("1.5 AHAHAH");
    delay(100);
    Serial.println("2.5 AHAHAH");
    delay(100);
    Serial.println("3 AHAHAH");
    delay(100);
    Serial.println("4.2 AHAHAH");
    delay(100);
    Serial.println("5 AHAHAH");
    delay(100);  
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
