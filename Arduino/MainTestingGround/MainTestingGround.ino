#include <Wire.h>;
#include <EEPROM.h>;
#include <Adafruit_ADS1015.h>;
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h";
#include <Adafruit_I2CDevice.h>;
#include <Adafruit_I2CRegister.h>;
#include "Adafruit_MCP9600.h";

#define I2C_ADDRESS_MCP9600 0x67 //
#define I2C_ADDRESS_ADS1115_1 0x48
#define I2C_ADDRESS_ADS1115_2 0x49
#define PSEN_ADDRESS 0x77
#define I2C_ADDRESS_Scale 0x2A

//Pressure sensor setup
#define CMD_RESET 0x1E // ADC reset command
#define CMD_ADC_READ 0x00 // ADC read command
#define CMD_ADC_CONV 0x40 // ADC conversion command
#define CMD_ADC_D1 0x00 // ADC D1 conversion
#define CMD_ADC_D2 0x10 // ADC D2 conversion
#define CMD_ADC_256 0x00 // ADC OSR=256
#define CMD_ADC_512 0x02 // ADC OSR=512
#define CMD_ADC_1024 0x04 // ADC OSR=1024
#define CMD_ADC_2048 0x06 // ADC OSR=2048
#define CMD_ADC_4096 0x08 // ADC OSR=4096
#define CMD_PROM_RD 0xA0 // Prom read command

float Q[7] = {9, 11, 9, 15, 15, 16 ,16};
uint32_t D1, D2;
float P,TOP, BOT;

struct PSenReadings {
   float T;
   float P;
};

uint16_t C[8];
struct PSenProm {
   float C0;
   float C1;
   float C2;
   float C3;
   float C4;
   float C5;
   float C6;
   float A0;
   float A1;
   float A2;
};
PSenProm PPROM;

//Other I2C initializations
Adafruit_MCP9600 mcp;
Adafruit_ADS1115 ads1 (0x48);
Adafruit_ADS1115 ads2 (0x49);
NAU7802 myScale;

//EEPROM locations to store 4-byte variables
#define LOCATION_CALIBRATION_FACTOR 0 //Float, requires 4 bytes of EEPROM
#define LOCATION_ZERO_OFFSET 10 //Must be more than 4 away from previous spot. Long, requires 4 bytes of EEPROM

bool settingsDetected = false; //Used to prompt user to calibrate their scale

//Create an array to take average of weights. This helps smooth out jitter.
#define AVG_SIZE 50
float avgWeights[AVG_SIZE];
byte avgWeightSpot = 0;

//Relay setup
#define RELAY_ON 1      // Define relay on pin state
#define RELAY_OFF  0    // Define relay off pin state 
#define Relay  13       // Arduino pin where the relay connects


//These are the default pins we will be using for the motor, 
//but we may change these later
#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int rolePerMinute = 15;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

float dataArray[3] ;
int addressArray[] = {0x67,0x48,0x49,0x77,0x2A};
String deviceNameArray[] = {"Thermocoupl 1","ADC 1","ADC 2","Pressure Sensor","Scale"};

float ads1_multiplier;
float ads2_multiplier;

//Spline coefficients for the thermistors
float therm_x[] = {0.0337,  0.0384,  0.044,   0.0506,  0.0583,  0.0674,  0.0782, 0.0912, 0.1066,
     0.1252,  0.1476,  0.1748,  0.2079,  0.2484,  0.2982,  0.3599,  0.4365,  0.5324,
     0.653,   0.8056,  1. ,     1.2493,  1.5711,  1.9897,  2.5381,  3.2624,  4.2268,
     5.5218,  7.2765,  9.6761, 12.99,   17.613,  24.132,  33.427};

float therm_c[][33] = {
  {247640.6454, 247640.6454, 140577.1722, 92753.7746, 59073.6772, 36563.3981, 
  22764.5891, 13744.9501, 8481.6329, 4908.2624, 3003.7212, 1689.7223, 973.2692, 559.565, 303.7518, 
  169.7436, 90.2566, 47.9205, 25.0624, 12.6791, 6.4348, 3.1021, 1.5233, 0.7039, 0.3277, 0.1463, 
  0.0641, 0.0274, 0.0114, 0.0046, 0.0019, 0.0006, 0.0006},
  {-18505.882, -14965.1161, -10821.0975, -8059.1778, -5912.1134, -4294.6171, -3106.1238, -2224.2464, 
  -1587.1267, -1114.615, -784.6325, -539.7091, -371.9196, -253.6382, -169.9553, -113.7855, -74.7479, 
  -48.7919, -31.4571, -19.9813, -12.5868, -7.7743, -4.7794, -2.8665, -1.7085, -0.9965, -0.5731, -0.3241, 
  -0.1798, -0.098, -0.0525, -0.0268, -0.016},
  {1131.6717, 972.1489, 828.3134, 704.6665, 596.864, 503.7072, 423.5202, 354.6891, 295.7996, 245.6282, 
  203.0661, 167.0705, 136.8956, 111.5542, 90.4381, 72.9483, 58.4953, 46.6528, 36.9764, 29.1254, 22.7941, 
  17.7181, 13.6783, 10.4777, 7.9688, 6.0095, 4.4957, 3.3338, 2.4496, 1.7832, 1.2846, 0.9181, 0.6392},
  {-40., -35., -30., -25., -20., -15., -10., -5., 0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 50., 
  55., 60., 65., 70., 75., 80., 85., 90., 95., 100., 105., 110., 115., 120.}
  };

//Spline coefficients for the pressure sensor 
float p_x[] = {-2.53, 1.0, 5.5, 10.0, 14.6, 18.85, 23.09, 27.55, 31.52, 35.62, 40.56};
float p_c[][10] = {{0.0    , 0.0    , 0.    ,  0.    , -0.    , 0.    ,  0.0001, -0.0001, 0.    , 0.    },
         { 0.0002,  0.0001,  0.0    , -0.0002,  0.0003,  0.    , -0.0004, 0.0006, -0.0002, -0.0004},
         {0.0206, 0.0218, 0.0224, 0.0217, 0.0225, 0.0241, 0.0226, 0.0238, 0.0255, 0.0229},
         {0.038, 0.113, 0.213, 0.313, 0.413, 0.513, 0.613, 0.713, 0.813, 0.913}};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //Initialize each sensor, make sure they are wired
  Wire.begin();

  readSystemSettings(); //Load zeroOffset and calibrationFactor from EEPROM

  myScale.setSampleRate(NAU7802_SPS_320); //Increase to max sample rate
  myScale.calibrateAFE(); //Re-cal analog front end when we change gain, sample rate, or channel 

  
  myStepper.setSpeed(rolePerMinute);
  
  digitalWrite(Relay, RELAY_OFF);      // initialise the relay to off
  pinMode(Relay, OUTPUT); 
  
}


bool onoff = false, newData = false;
char receivedChar;
String readString;



void loop() {
  // put your main code here, to run repeatedly:
  
  recvOneChar();

  wireTestAndSetup();
  
  setOnoff();
  myScale_Tare();
  
  if (onoff){
    dataArray[0] = 1.1; //Sensor readings go here
    dataArray[1] = 2.2;
    dataArray[2] = 3.3;
    sendArr();
    
    }

  switchRelay();

  runMotor();
  
  Reset();
 
  delay(1000);
 }

//Setup Functions
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
  myScale.begin();
  }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void myScale_Tare(){
  if(newData == true && receivedChar == 't'){
    myScale.calculateZeroOffset();
        
    Reset();
    }  
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mcp_Setup(){
  mcp.setThermocoupleType(MCP9600_TYPE_K);
  mcp.setFilterCoefficient(3);
  mcp.enable(true);
  }
  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void pressure_Setup(){
  PPROM = readPSenProm();
  
  }

//Thermistor Function(s)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

float getThermistor(int num){
  //input 1 or 2 for thermistor 1 or 2
  int thermistor_25 = 5000; //Change later
  float refCurrent = 0.0001; //Change later
  float Voltage; //Change later
  float VCC = 3.3; //Change Later
  int16_t adc; // we read from the ADC, we have a sixteen bit integer as a result

  if(num == 1){
    //Reads 1st thermistor
    adc = ads2.readADC_SingleEnded(0); 
  }
  if(num == 2){
    //Reads 2nd thermistor
    adc = ads2.readADC_SingleEnded(1); 
  }

  Voltage = adc * ads2_multiplier; //Look more into this *****

  float resistance = (-10000 * (Voltage / VCC)) / ((Voltage / VCC) - 1); // Using Ohm's Law to calculate resistance of thermistor
  
  float Rfrac = resistance / thermistor_25; // Log of the ratio of thermistor resistance and resistance at 25 deg. C

  float temp;
  int len = 34;
  for (int i=0; i< len ; i++){
    if (Rfrac >= therm_x[i] && Rfrac < therm_x[i+1]){
      float d = therm_c[0][i];
      float ci = therm_c[1][i];
      float b = therm_c[2][i];
      float y = therm_c[3][i];

      temp = (2*42.5) - ((d * (pow((Rfrac-therm_x[i]),3))) + (ci * (pow((Rfrac-therm_x[i]),2))) + (b * (Rfrac-therm_x[i])) + y);
      break;
    }
  }   
  
  return temp;
}

//Radiometer Function(s)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

float getRadiometer(){
  float VCC = 5.0; //Change Later
  //Will update these values later
  float cosTh = 1.0;
  float a = 1.0 ;
  float b = 1.0 ;
  float c = 1.0 ;
  float n = 1.0 ;  

  float T_opt = getThermistor(1);
  float T_d = getThermistor(2);

  int16_t  diffV = ads1.readADC_Differential_0_1();  

  float Voltage = diffV * ads1_multiplier; //Look more into this *****

  float L = (a*Voltage - b*pow(T_opt,n) + c*pow(T_d,n)) / cosTh ;  
  
  return L;
}

//Scale Function(s)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

float getScale(){
  long currentReading = myScale.getReading();
  float currentWeight = myScale.getWeight();

  avgWeights[avgWeightSpot++] = currentWeight;
  if(avgWeightSpot == AVG_SIZE) {
    avgWeightSpot = 0;
    }

  float avgWeight = 0;
    for (int x = 0 ; x < AVG_SIZE ; x++){
    avgWeight += avgWeights[x];
    avgWeight /= AVG_SIZE;
    }
  
  return avgWeight;
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

//Pressure Sensor Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct PSenReadings readPSen(PSenProm PPROM) {
   struct PSenReadings Psen;
   GetD1D2();

   Psen.T = (PPROM.A0 / 3) + (PPROM.A1 * 2 * D2 / pow(2,24)) + (PPROM.A2 * 2 * pow((D2 / pow(2, 24)), 2));
   
   TOP = D1 + (PPROM.C0 * pow(2, Q[0])) + (PPROM.C3 * pow(2, Q[3]) * D2 / pow(2, 24)) + (PPROM.C4 * pow(2,Q[4]) * pow((D2 / pow(2, 24)), 2));
   BOT = (PPROM.C1 * pow(2, Q[1])) + (PPROM.C5 * pow(2, Q[5]) * D2 / pow(2, 24)) + (PPROM.C6 * pow(2, Q[6]) * pow((D2 / pow(2, 24)), 2));   
   float Y = TOP/BOT;
   float P = (Y*(1-PPROM.C2*pow(2, Q[2])/pow(2, 24))) + PPROM.C2*pow(2, Q[2])/pow(2, 24)*pow(Y, 2);
   Psen.P = 14.5038 * (((P-0.1)/0.8)*(12-0));

   float p;
   int len = 11;//**INPUT LENGTH OF SPLINE ARRAYS**
   for (int i=0; i< len ; i++){
    if (Psen.P >= p_x[i] && Psen.P < p_x[i+1]){
      float d = p_c[0][i];
      float ci = p_c[1][i];
      float b = p_c[2][i];
      float y = p_c[3][i];
      
      p = (d * (pow((Psen.P-p_x[i]),3))) + (ci * (pow((Psen.P-p_x[i]),2))) + (b * (Psen.P-p_x[i])) + y;
      Psen.P = p;
      break;
    }
  }

   return Psen;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t PSEN_CMD_PROM(int addrs){
    
  int16_t reading;
  Wire.beginTransmission(PSEN_ADDRESS); // transmit to device #112
  Wire.write(addrs);      // sets register pointer to echo #1 register (0x02)
  Wire.endTransmission();      // stop transmitting

  // step 4: request reading from sensor
  Wire.requestFrom(PSEN_ADDRESS, 2);    // request 2 bytes from slave device #112

  // step 5: receive reading from sensor
  if(2 <= Wire.available())    // if two bytes were received
  {
    reading = Wire.read();  // receive high byte (overwrites previous reading)
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read(); // receive low byte as lower 8 bits
    
    return reading;   // print the reading
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int16_t FourteenBitConversion(int16_t Value){
 
  int16_t Converted = Value;
 
   if(Value > 8192){
 
     Converted = ((Value - 8192 -1)^8191)*-1;
 }
 
  return Converted;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int16_t TenBitConvertion(int16_t Value){
 
  int16_t Converted = Value;
 
 if(Value > 512){
 
  Converted = ((Value - 512 -1)^511)*-1;
 }
 
 
  return Converted;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct PSenProm readPSenProm() {
   PSenProm Psen;

   for (int i=0;i < 8; i++ ){
    int val = CMD_PROM_RD+(i*2);
    C[i] = PSEN_CMD_PROM(val);
    
  }

  Psen.C0 = (C[1] & 0xFFFC) >> 2;
  Psen.C0 = FourteenBitConversion(Psen.C0);

  Psen.C1 = ((C[1] & 0x03) << 12) + ((C[2] & 0xFFF0) >> 4);
  Psen.C1 = FourteenBitConversion(Psen.C1);

  Psen.C2 = (((C[2] & 0xF) << 6) + (C[3] >> 10)) & 0x3FF;
  Psen.C2 = TenBitConvertion(Psen.C2);

  Psen.C3 = C[3] & 0x3FF;
  Psen.C3 = TenBitConvertion(Psen.C3);

  Psen.C4 = (C[4] >> 6) & 0x3FF;
  Psen.C4 = TenBitConvertion(Psen.C4);

  Psen.C5 =  (((C[4] & 0x3F) << 4) + (C[5] >> 12)) & 0x3FF;
  Psen.C5 = TenBitConvertion(Psen.C5);

  Psen.C6 = (C[5] >> 2) & 0x3FF;
  Psen.C6 = TenBitConvertion(Psen.C6);

  Psen.A0 = (((C[5] & 0x3) << 8) + (C[6] >> 8)) & 0x3FF;
  Psen.A0 = TenBitConvertion(Psen.A0);

  Psen.A1 = (((C[6] & 0xFF) << 2) + (C[7] >> 14)) & 0x3FF;
  Psen.A1 = TenBitConvertion(Psen.A1);

  Psen.A2 = (C[7] >> 4) & 0x3FF;
  Psen.A2 = TenBitConvertion(Psen.A2);
 
   return Psen;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GetD1D2(){
  D2=cmd_adc(CMD_ADC_D2+CMD_ADC_4096); // read D2
  D1=cmd_adc(CMD_ADC_D1+CMD_ADC_4096); // read D1
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Read ADC
uint32_t cmd_adc(int cmd){
  uint16_t ret;
  uint32_t reading = 0;
  
  Wire.beginTransmission(PSEN_ADDRESS); // transmit to device #112
  Wire.write(CMD_ADC_CONV + cmd);      // sets register pointer to echo #1 register (0x02)
  Wire.endTransmission();      // stop transmitting

  switch(cmd & 0x0F){
      case CMD_ADC_256 : delayMicroseconds(900); break;
      case CMD_ADC_512 : delay(3); break;
      case CMD_ADC_1024: delay(4); break;
      case CMD_ADC_2048: delay(6); break;
      case CMD_ADC_4096: delay(10); break;

   }
  
  Wire.beginTransmission(PSEN_ADDRESS); // transmit to device #112
  Wire.write(CMD_ADC_READ);      // sets register pointer to echo #1 register (0x02)
  Wire.endTransmission();      // stop transmitting
  
  // step 4: request reading from sensor
  Wire.requestFrom(PSEN_ADDRESS, 3);    // request 2 bytes from slave device #112

  // step 5: receive reading from sensor
  if(2 <= Wire.available())    // if two bytes were received
  {
    ret = Wire.read();  // receive high byte (overwrites previous reading)
    reading = 65536 * ret;    // shift high byte to be high 8 bits
    ret = Wire.read() << 8; // receive low byte as lower 8 bits
    reading = reading + 256*ret;
    ret = Wire.read();
    reading = reading + ret;
    
    return reading;   // print the reading
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

//Wiring Test Function
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void wireTestAndSetup(){
  //This function checks which sensors are connected. 
  if(newData == true && receivedChar == 'T'){
    for(int i=0; i<sizeof(addressArray) / sizeof(addressArray[0]); i++){
    Wire.beginTransmission (addressArray[i]);
    if (Wire.endTransmission () == 0){
      Serial.print ("Found address ");
      Serial.print (deviceNameArray[i]);//prints the name of the sensors found
      Serial.print (" at: (0x");
      Serial.print (addressArray[i], HEX); //prints the name of the sensors found
      Serial.println (")"); 
      delay(100);    
    }
    else{
      Serial.print ("Did not find: ");
      Serial.println (deviceNameArray[i]);
      delay(100);  
    }    
  }

  //Setup all the sensors
  ads1_Setup();
  ads2_Setup();
  myScale_Setup();
  mcp_Setup();
  pressure_Setup();
  Reset();
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
