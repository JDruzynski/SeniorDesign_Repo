/***Imports***/
/***************************************************************/
#include <Wire.h>;
#include <EEPROM.h>;
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h";

#define I2C_ADDRESS_Scale 0x2A
NAU7802 myScale;

/***Scale Setup***/
/***************************************************************/
//EEPROM locations to store 4-byte variables
#define LOCATION_CALIBRATION_FACTOR 0 //Float, requires 4 bytes of EEPROM
#define LOCATION_ZERO_OFFSET 10 //Must be more than 4 away from previous spot. Long, requires 4 bytes of EEPROM

bool settingsDetected = false; //Used to prompt user to calibrate their scale

//Create an array to take average of weights. This helps smooth out jitter.
#define AVG_SIZE 50
float avgWeights[AVG_SIZE];
byte avgWeightSpot = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //Initialize each sensor, make sure they are wired
  Wire.begin();
  
  myScale.begin();

  //This loads any settings for the scale stored on the arduino
  readSystemSettings(); //Load zeroOffset and calibrationFactor from EEPROM
  myScale.setSampleRate(NAU7802_SPS_320); //Increase to max sample rate
  myScale.calibrateAFE(); //Re-cal analog front end when we change gain, sample rate, or channel

  myScale_Setup();
}


/*Variables used in the loops*/
/***************************************************************/
bool onoff = false, newData = false;
char receivedChar;

void loop() {
  // put your main code here, to run repeatedly:
  recvOneChar();

  setOnoff();
  
  myScale_Tare();

  if (onoff)
  {
    Serial.print("Weight(g): "); Serial.println(getScale()); 
  }
  
  Reset();
  delay(1000);
}

//Scale Function(s)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

float getScale(){
  float currentWeight = myScale.getWeight();

  avgWeights[avgWeightSpot++] = currentWeight;
  if(avgWeightSpot == AVG_SIZE) {
    avgWeightSpot = 0;
    }

  float avgWeight = 0;
    for (int x = 0 ; x < AVG_SIZE ; x++){
    avgWeight += avgWeights[x];
    //avgWeight /= AVG_SIZE;
    }
  
  avgWeight /= AVG_SIZE;
  return avgWeight;
}


/***************************************************************/

void myScale_Tare(){
  if(newData == true && receivedChar == 't'){
    myScale.calculateZeroOffset();        
    Reset();
    }  
  }

//Miscellaneous Functions
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

/***************************************************************/

void recvOneChar() {
 if (Serial.available() > 0) {
  receivedChar = Serial.read();
  newData = true;
  }
}

/***************************************************************/

void Reset() {
 if (newData == true) {
  newData = false;
 }
}

//EEPROM Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Record the current system settings to EEPROM
void recordSystemSettings(void)
{
  //Get various values from the library and commit them to NVM
  EEPROM.put(LOCATION_CALIBRATION_FACTOR, myScale.getCalibrationFactor());
  EEPROM.put(LOCATION_ZERO_OFFSET, myScale.getZeroOffset());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Reads the current system settings from EEPROM
//If anything looks weird, reset setting to default value
void readSystemSettings(void)
{
  float settingCalibrationFactor; //Value used to convert the load cell reading to lbs or kg
  long settingZeroOffset; //Zero value that is found when scale is tared

  //Look up the calibration factor
  EEPROM.get(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
  if (settingCalibrationFactor == 0xFFFFFFFF)
  {
    settingCalibrationFactor = 0; //Default to 0
    EEPROM.put(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
  }

  //Look up the zero tare point
  EEPROM.get(LOCATION_ZERO_OFFSET, settingZeroOffset);
  if (settingZeroOffset == 0xFFFFFFFF)
  {
    settingZeroOffset = 1000L; //Default to 1000 so we don't get inf
    EEPROM.put(LOCATION_ZERO_OFFSET, settingZeroOffset);
  }

  //Pass these values to the library
  myScale.setCalibrationFactor(settingCalibrationFactor);
  myScale.setZeroOffset(settingZeroOffset);

  settingsDetected = true; //Assume for the moment that there are good cal values
  if (settingCalibrationFactor < 0.1 || settingZeroOffset == 1000)
    settingsDetected = false; //Defaults detected. Prompt user to cal scale.
}
