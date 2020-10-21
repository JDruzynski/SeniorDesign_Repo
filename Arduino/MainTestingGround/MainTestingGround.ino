#include <Wire.h>;
#include <Adafruit_ADS1015.h>;
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h";
#include <Adafruit_I2CDevice.h>;
#include <Adafruit_I2CRegister.h>;
#include "Adafruit_MCP9600.h";

#define I2C_ADDRESS_MCP9600 (0x67); //
#define I2C_ADDRESS_ADS1115_1 (0x48);
#define I2C_ADDRESS_ADS1115_2 (0x49);
#define I2C_ADDRESS_Pressure (0x77);
#define I2C_ADDRESS_Scale (0x2A);

Adafruit_MCP9600 mcp;
Adafruit_ADS1115 ads1 (0x48);
Adafruit_ADS1115 ads2 (0x49);
NAU7802 myScale;

#define RELAY_ON 1      // Define relay on pin state
#define RELAY_OFF  0    // Define relay off pin state 
#define Relay  13       // Arduino pin where the relay connects


//These are the default pins we will be using for the motor, 
//but we may change these later
#define ENABLE 5
#define DIRA 3
#define DIRB 4
#define SPEED 255 // Motor speed (0-255)
#define MOTOR_RT 500// How long the motor runs, ms

float dataArray[3] ;
int addressArray[] = {0x67,0x48,0x49,0x77,0x2A};
String deviceNameArray[] = {"Thermocoupl 1","ADC 1","ADC 2","Pressure Sensor","Scale"};

float ads1_multiplier;
float ads2_multiplier;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //Initialize each sensor, make sure they are wired
  Wire.begin();
  
  digitalWrite(Relay, RELAY_OFF);      // initialise the relay to off
  pinMode(Relay, OUTPUT); 

  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  
}


bool onoff = false, newData = false;
char receivedChar;
String readString;



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

  
  }
  
//Thermistor Function(s)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

float getThermistor(int num){
  //input 1 or 2 for thermistor 1 or 2
  int thermistor_25 = 5000; //Change later
  float refCurrent = 0.0001; //Change later
  float Voltage; //Change later
  float VCC = 5.0; //Change Later
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

  float resistance = (Voltage / refCurrent); // Using Ohm's Law to calculate resistance of thermistor
  
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
