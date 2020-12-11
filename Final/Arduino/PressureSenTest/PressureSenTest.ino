#include <Wire.h>
#define PSEN_ADDRESS 0x77
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
//uint16_t PSEN_CMD_PROM(int addrs);
//int16_t FourteenBitConversion(int16_t Value);
//int16_t TenBitConvertion(int16_t Value);

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

void setup()
{
  Wire.begin();                // join i2c bus (address optional for master)
  Serial.begin(9600);          // start serial communication at 9600bps
  PPROM = readPSenProm();
  
  Serial.println(PPROM.C0);
  Serial.println(PPROM.C1);
  Serial.println(PPROM.C2);
  Serial.println(PPROM.C3);
  Serial.println(PPROM.C4);
  Serial.println(PPROM.C5);
  Serial.println(PPROM.C6);
  Serial.println(PPROM.A0);
  Serial.println(PPROM.A1);
  Serial.println(PPROM.A2);
}



void loop()
{
GetD1D2();
//Serial.println(D1);
//Serial.println(D2);

 PSenReadings Pread = readPSen(PPROM);
Serial.print("Temperature:"); Serial.println(Pread.T);
Serial.print("Pressure:"); Serial.println(Pread.P);
  
delay(1000);
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

   return Psen;
}
