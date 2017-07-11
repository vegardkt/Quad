//#include <Wire.h>
#include <WSWire.h>
double g_x, g_y, g_z, g_x_ang;
double g_x_off, g_y_off, g_z_off;
float apitch, aroll;
float fpitch, froll;
double pitch, roll, yaw;
double temp;
long a_x, a_y, a_z, a_tot;
long timer;
bool firstcalc;


void setup() {
  //1. Start I2C
  Wire.begin();
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  register_write();
  get_offset();
  firstcalc = true;
  timer = micros();

}

void loop() {
  Serial.print("Starting main loop");
  read_mpu();
  //Raw values return in range [-32768,32768]
  //Actual measurements are in [-500,500] deg/s
  // (1/ 250Hz) / (32768 / 500) = 0.00006103515

a_tot = sqrt((a_x*a_x)+(a_y*a_y)+(a_z*a_z));

if(abs(a_y) < a_tot)
{
  apitch = asin((float)a_y/a_tot) * 57.296;
}
if(abs(a_x) < a_tot)
{
  aroll = asin((float)a_x/a_tot) * -57.296;
}
  Serial.print(apitch);
  //substrace gyro offset
  g_x = g_x - g_x_off;
  g_y = g_y - g_y_off;
  g_z = g_z - g_z_off;

  //convert gyro raw-> degrees
  if(firstcalc)
  {
    pitch = apitch;
    roll = aroll; 
    firstcalc = false; 
  }
  else
  {
    pitch += g_x * 0.0000611;
    roll += g_y * 0.0000611;
  }
  //Need to take YAW into acount for roll & pitch values!
  pitch -= roll * (sin(g_z * 0.000001066)); //Arduino sin is in rad! 1deg = 0.0174533 rad.
  roll += pitch * (sin(g_z * 0.000001066)); //And 0.0000611 * 0.0174533 = 0.00000106639
  Serial.print(";");
  Serial.print(pitch);
  pitch = (pitch * 0.98) + (apitch * 0.02);
  roll  = (roll  * 0.98 + aroll  * 0.02);
  Serial.print(";");
  Serial.println(pitch);
 
  
  

  if(micros() - timer > 4000){Serial.print("WARNING EXCEEDING 4000us CALCULATIONS ARE OFF!");}
  while(micros() - timer < 4000);
  timer = micros();
}

void read_mpu()
{
  Serial.println("Reading mpu");
  //SDA = A4, SCL = A5
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68,14); //Request 14 bytes 2*7 || Dont i only need 12?
  while(Wire.available() < 14);
  a_x = Wire.read()<<8|Wire.read();
  a_y = Wire.read()<<8|Wire.read();
  a_z = Wire.read()<<8|Wire.read();
  temp = Wire.read()<<8|Wire.read(); // we can skip this by readying two times, but this is probably faster
  g_x = Wire.read()<<8|Wire.read();
  g_y = Wire.read()<<8|Wire.read();
  g_z = Wire.read()<<8|Wire.read();
  Serial.println("Done Reading mpu");
}

void register_write()
{
  Serial.println("|-------------------------|");
  Serial.print("|   CPU Speed: ");
  Serial.print(F_CPU);
  Serial.println("   |");
  Serial.println("|-------------------------|");
  Serial.println("|  Writing to registers!  |");
  
  Serial.println("|-------------------------|");
  Serial.println("|     Activating MPU      |");
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);                                                   
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Serial.println("|     MPU Activated       |");
 
  Serial.println("|-------------------------|");
  Serial.println("|Configuring Accelerometer|");
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);     
  Wire.endTransmission();
  Serial.println("| Accelerometer Configured|");

  Serial.println("|-------------------------|");
  Serial.println("|  Configuring Gyroscope  |");
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();
  Serial.println("|  Gyroscope Configured   |");
  Serial.println("|-------------------------|");
  Serial.println("register_write complete");
}

void get_offset()
{
  Serial.println("Starting get_offset");
  for(int i = 0; i < 2000;i++)
  {
    read_mpu();
    g_x_off += g_x;
    g_y_off += g_y;
    g_z_off += g_z;
  }
  g_x_off /= 2000;
  g_y_off /= 2000;
  g_z_off /= 2000;
  Serial.println("Gyro Offset values");
  Serial.print(g_x_off);
  Serial.print("   |   ");
  Serial.print(g_y_off);
  Serial.print("   |   ");
  Serial.println(g_z_off);
  delay(1000);
}
