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
#include <LiquidCrystal_I2C.h>

MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2);

double total;           // Valor final de aceleração
double totalOffset;     // Valor do offset de instalação do MPU
double threshold = 10;  // Threshold para início da aquisição de pico
double max_hold=0;      // Variável para armazenamento do pico
int window=0;           // Variável para contador da janela de aquisição
int window_size=300;    // Tamanho da janela de aquisição

void setup() 
{
  Serial.begin(115200);
  lcd.begin();

  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
 lcd.backlight();
 
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

  lcd.print("Boxing 102");
  delay(1000);
  lcd.clear();
  
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

if(total>=threshold && window==0){
  max_hold=0;       // Reset max_hold
    do{
      Vector normAccel = mpu.readNormalizeAccel();
        total=abs((normAccel.XAxis+normAccel.YAxis+normAccel.ZAxis)-totalOffset);
          if(total>=max_hold){
          max_hold=total;
          }
      delay(10);
      window++;
  
    //Serial.print(total);          //the first variable for plotting
    //Serial.print(",");
    //Serial.print(max_hold);       //the second variable for plotting
    //Serial.print(",");//seperator
    //Serial.println(window);       //the third variable for plotting including line break
          
    }while (window <= window_size);
  
    window=0;
  }

lcd.clear();
lcd.print(max_hold);
  
delay(10);
}
