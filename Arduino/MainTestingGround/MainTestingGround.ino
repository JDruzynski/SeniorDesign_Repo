#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"

#define I2C_ADDRESS_MCP9600 (0x67) //
#define I2C_ADDRESS_ADS1115_1 (0x48)
#define I2C_ADDRESS_ADS1115_2 (0x49)

Adafruit_MCP9600 mcp;
Adafruit_ADS1115 ads1(0x48);
Adafruit_ADS1115 ads2(0x49);
NAU7802 myScale;

HEX addressArray[] = {0x67,0x48,0x49};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //Initialize each sensor, make sure they are wired
  Wire.begin();
  
  
  
  
  
  
}


bool onoff = false, newData = false;
char receivedChar;
String readString;
float ads1_multiplier;



void loop() {
  // put your main code here, to run repeatedly:
  
  recvOneChar();

  wireTestAndSetup();

  setOnoff();
  
  if (onoff){
    dataArray[0] = 1.1; //Sensor readings go here
    dataArray[1] = 2.2;
    dataArray[2] = 3.3;
    sendArr();
    
    }
  
  Reset();
 
  delay(1000);
 }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This ADS1115 will be use in differential mode, for the radiometer
// The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1115
  //                                                                -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
  
void ads1_Setup(){
  /*if (!ads1.begin()){
    Serial.println("ADS 1 not detected. Please check wiring. Freezing...");
    while (1);
    }
  Serial.println("ADS 1 detected!");*/
  ads1.begin();
  ads1.setGain(GAIN_EIGHT);
  ads1_multiplier = 0.015625F; //To mV
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ads2_Setup(){
  /*if (!ads2.begin()){
    Serial.println("ADS 2 not detected. Please check wiring. Freezing...");
    while (1);
    }
  Serial.println("ADS 2 detected!");*/
  ads2.begin();
  ads2.setGain(GAIN_ONE);
  float ads2_multiplier = 0.125F; //To mV
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void myScale_Setup(){
  if (!myScale.begin()){
    Serial.println("Scale not detected. Please check wiring. Freezing...");
    while (1);
    }
  Serial.println("Scale detected!");
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mcp_Setup(){
  if (!mcp.begin(I2C_ADDRESS_MCP9600)) {
    Serial.println("MCP9600 not detected. Please check wiring. Freezing...");
    while (1);
    }
  Serial.println("MCP9600 detected!");
  
  mcp.setThermocoupleType(MCP9600_TYPE_K);
  mcp.setFilterCoefficient(3);
  mcp.enable(true);
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

void wireTestAndSetup(){
  if(newData == true && receivedChar == 'T'){
    onoff = true;
    Reset();
    }  
  
  for(int i=0; i<sizeof(dataArray) / sizeof(dataArray[0]); i++){
    Wire.beginTransmission (addressArray[i]);
    if (Wire.endTransmission () == 0){
      Serial.print ("Found address: ");
      Serial.print (addressArray[i], DEC);
      Serial.print (" (0x");
      Serial.print (addressArray[i], HEX);
      Serial.println (")");      
    }
    else{
      Serial.print ("Did not find: ");
      Serial.print (addressArray[i], DEC);
      Serial.print (" (0x");
      Serial.print (addressArray[i], HEX);
      Serial.println (")");
    }    
  }

  //Setup all the sensors
  ads1_Setup();
  ads2_Setup();
  myScale_Setup();
  mcp_Setup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AHAH(){
  if(newData == true && receivedChar == 'B'){
    onoff = true;
    Reset();
    }
  else if(newData == true && receivedChar == 'E'){
    onoff = false;
    }
    
  if(onoff){
    Serial.println("1 AHAHAH");
    delay(100);
    Serial.println("2 AHAHAH");
    delay(100);
    Serial.println("3 AHAHAH");
    delay(100);
    Serial.println("4 AHAHAH");
    delay(100);
    Serial.println("5 AHAHAH");
    delay(100);  
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sendArr(){
  if(newData == true && receivedChar == 'B'){
    onoff = true;
    Reset();
    }
  else if(newData == true && receivedChar == 'E'){
    onoff = false;
    Reset();
    }
    
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
