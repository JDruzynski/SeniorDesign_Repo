/*
This is meant to get the voltage data from just the radiometer and thermistors
during the calibration of the radiometer
*/
#include <Wire.h>
#include <Adafruit_ADS1015.h>


/*
 The single address input is sampled continuously and if you connect it to 
 GND, VDD, SDA or SCL you can set the address from 0x48, 0x49, 0x4a, 0x4b respectively.
*/
Adafruit_ADS1115 ads1(0x48); //GND, can change these
Adafruit_ADS1115 ads2(0x49); //VDD


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  
  ads1_Setup(); //See setup functions  below
  ads2_Setup();
  }


bool onoff = false, newData = false;
char receivedChar;
String readString;
float ads1_multiplier;

void loop() {
  // put your main code here, to run repeatedly:
  
  recvOneChar();
  
  if (newData == true){
    setOnoff();
    
    }
  
  if (onoff == true){
    Serial.print("Radiometer mV: "); Serial.println(ads1.readADC_Differential_0_1() * ads1_multiplier);
    Serial.print("Thermistor 1 mV: "); Serial.println(ads2.readADC_SingleEnded(0));
    Serial.print("Thermistor 2 mV: "); Serial.println(ads2.readADC_SingleEnded(1));
    //Matlab will split up those strings and extract just the numerical portion
    
    }
  
  
  
  //recvOneChar();
  //recvFullLine();
  //AHAH();
  
  Reset();
 
  delay(100);
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

void setOnoff(){
  if(newData == true && receivedChar == 'B'){
    onoff = true;
    }
  else if(newData == true && receivedChar == 'E'){
    onoff = false;
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
