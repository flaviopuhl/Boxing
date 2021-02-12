/*


    MPU Library https://github.com/jarzebski/Arduino-MPU6050
    I2C LCD display library https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

    Arduino     MPU6050
      VCC ....... VCC
      GND ....... GND
      A4  ....... SDA
      A5  ....... SCL
*/

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

double total;
double totalOffset;
double threshold = 10;
double max_hold=0;

void setup() 
{
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  // If you want, you can set accelerometer offsets
  // mpu.setAccelOffsetX();
  // mpu.setAccelOffsetY();
  // mpu.setAccelOffsetZ();
  
  Serial.println();
  
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
  
  Serial.println();

  Vector normAccel = mpu.readNormalizeAccel();

  totalOffset=normAccel.XAxis+normAccel.YAxis+normAccel.ZAxis;
  Serial.print("Offset = ");
  Serial.println(totalOffset);
  
}


void loop()
{
  //Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();

  //Serial.print(" Xraw = ");
  //Serial.print(rawAccel.XAxis);
  //Serial.print(" Yraw = ");
  //Serial.print(rawAccel.YAxis);
  //Serial.print(" Zraw = ");
  //Serial.println(rawAccel.ZAxis);

  //Serial.print(" Xnorm = ");
  //Serial.print(normAccel.XAxis);
  //Serial.print(" Ynorm = ");
  //Serial.print(normAccel.YAxis);
  //Serial.print(" Znorm = ");
  //Serial.println(normAccel.ZAxis);

  total=abs((normAccel.XAxis+normAccel.YAxis+normAccel.ZAxis)-totalOffset);

  if(total>=threshold){
    max_hold=total;
    Serial.print("    Max = ");
    Serial.print(max_hold);
    Serial.println();
    delay(1000);
  }
    
  //Serial.print(" Total = ");
  //Serial.print(total);
  //Serial.print("    Max = ");
  //Serial.print(max_hold);
  //Serial.println();
  delay(10);
}
