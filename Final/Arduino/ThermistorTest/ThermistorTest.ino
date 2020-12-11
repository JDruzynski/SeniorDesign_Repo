/*
This is meant to get the voltage data from just thermistors
during the calibration of the thermistors
*/
#include <Wire.h>
#include <Adafruit_ADS1015.h>

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
/*
 The single address input is sampled continuously and if you connect it to 
 GND, VDD, SDA or SCL you can set the address from 0x48, 0x49, 0x4a, 0x4b respectively.
*/
Adafruit_ADS1115 ads1(0x48); //GND, can change these
Adafruit_ADS1115 ads2(0x49); //VDD

float ads1_multiplier;
float ads2_multiplier;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  ads2_Setup();
  Serial.println("Hi 1");
}

bool onoff = false, newData = false;
char receivedChar;
String readString;

void loop() {
  // put your main code here, to run repeatedly:
  
  
  float T1 = getThermistor(1);
  
  
  //float T2 = getThermistor(2);
  Serial.println(T1);
  delay(1000);
}


//ADC Setup functions
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
  ads1_multiplier = 0.000015625; //To V
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
  ads2_multiplier = 0.000125; //To V
  }

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
