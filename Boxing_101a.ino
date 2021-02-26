/*

    MPU Library https://github.com/jarzebski/Arduino-MPU6050
    I2C LCD display library https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

    Arduino     MPU6050   I2Cdisplay
      VCC ....... VCC ...... VCC
      GND ....... GND ...... GND
      A4  ....... SDA ...... SDA
      A5  ....... SCL ...... SCL
*/

/*----------------------------------------------------*/
/*Bibliotecas*/
/*----------------------------------------------------*/
#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>

/*----------------------------------------------------*/
/*Bibliotecas*/
/*----------------------------------------------------*/
MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2);

/*----------------------------------------------------*/
/*Variáveis*/
/*----------------------------------------------------*/
double total;             // Variável que recebe o valor de aceleração
double totalOffset;       // Offset de montagem

double max_hold=0;        // Variável de armazenamento do valor máximo
double max_hold_anterior=0;
int i=0;
double total_Array[300];
int max_hold_index=0;

/*----------------------------------------------------*/
/*Setup*/
/*----------------------------------------------------*/
void setup() 
{
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  //lcd.print("Boxing 101");
  lcd.print("Boxing 101a"); //101a
  delay(1000);
  lcd.clear();

/*----------------------------------------------------*/
/*Debug*/
/*----------------------------------------------------*/
/*
  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
*/

/*----------------------------------------------------*/
/*Faz leitura do valor de offset*/
/*----------------------------------------------------*/  
  Vector normAccel = mpu.readNormalizeAccel();
  totalOffset=normAccel.XAxis+normAccel.YAxis+normAccel.ZAxis;
  Serial.print("Offset = ");
  Serial.println(totalOffset);

}

/*----------------------------------------------------*/
/*Loop principal*/
/*----------------------------------------------------*/  
void loop()
{

/*----------------------------------------------------*/
/*Faz leitura do valor de aceleração*/
/*----------------------------------------------------*/    
  delay(10);
  Vector normAccel = mpu.readNormalizeAccel();
  total=abs((normAccel.XAxis+normAccel.YAxis+normAccel.ZAxis)-totalOffset);
 
/*----------------------------------------------------*/
/*Se aceleração maior que threshold, grava valores no vetor*/
/*----------------------------------------------------*/
  //double threshold = 10;
  double threshold = 30; //101a

  if(total>=threshold){
    while (i < 300) {
        Vector normAccel = mpu.readNormalizeAccel();
        total=abs((normAccel.XAxis+normAccel.YAxis+normAccel.ZAxis)-totalOffset);
        total_Array[i]=total;
    delay(10);
    i++;
    }

    i=0;
    max_hold=0;

/*----------------------------------------------------*/
/*Se aceleração maior que threshold, grava valores no vetor*/
/*----------------------------------------------------*/
    while (i < 300) {
        if(total_Array[i]>=max_hold){
          max_hold=total_Array[i];
          max_hold_index=i;
        }
    i++;
    }
    
    Serial.print("    Max = ");
    Serial.print(max_hold);
    Serial.println();
    Serial.print("    Index = ");
    Serial.print(max_hold_index);
    Serial.println();
    
    lcd.clear();
    lcd.print(max_hold);
    
  }
    

}
