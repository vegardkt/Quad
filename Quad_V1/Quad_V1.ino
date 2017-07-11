#include <Wire.h>
double g_x, g_y, g_z, g_x_ang;
double g_x_off, g_y_off, g_z_off;
float apitch, aroll;
float fpitch, froll;
double pitch, roll, yaw;
double temp;
long a_x, a_y, a_z, a_tot;
long timer, esc_timer;
bool firstcalc;
int esc1,esc2,esc3,esc4;
unsigned long current_time, t_ch1, t_ch2, t_ch3, t_ch4;
unsigned long t_esc1, t_esc2, t_esc3, t_esc4;
byte ps_ch1, ps_ch2, ps_ch3,ps_ch4;
volatile int ch1_input, ch2_input, ch3_input, ch4_input;


void setup() {
  //1. Start I2C
  Wire.begin();
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  //register_write();
  //get_offset();
  firstcalc = true;
  timer = micros();
  DDRD |= B11110000;//Configure digital port 4, 5, 6 and 7 as output.
  DDRB |= B00010000;//Configure digital port 12 as output.
  
  PCICR |= (1 << PCIE0);  //set PCIE0 to enable PCMSK0 scan.
  PCMSK0 |= (1 << PCINT0);//set PCINT0 (DI 8) to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT1);//set PCINT1 (DI 9)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT2);//set PCINT2 (DI 10)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT3);//set PCINT3 (DI 11)to trigger an interrupt on state change.

}

void loop() {
  //read_mpu();
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
 
  Serial.print(ch1_input);
  Serial.print("|---|");
  Serial.print(ch2_input);
  Serial.print("|---|");
  Serial.print(ch3_input);
  Serial.print("|---|");
  Serial.println(ch4_input);
  

  if(micros() - timer > 4000)Serial.print("WARNING EXCEEDING 4000us CALCULATIONS ARE OFF!");
  while(micros() - timer < 4000);
  timer = micros();
  esc1 = ch1_input; //For testing only!
  esc2 = ch2_input; //For testing only!
  esc3 = ch3_input; //For testing only!
  esc4 = ch4_input; //For testing only!
  
  PORTD |= B11110000; //set DO 4,5,6,7 high.
  t_esc1 = esc1 + timer; 
  t_esc2 = esc2 + timer;
  t_esc3 = esc3 + timer;
  t_esc4 = esc4 + timer;

  while(PORTD >= 16) //Stay here untill all pulses are done.
  {
    esc_timer = micros();
    if(t_esc1 <= esc_timer)PORTD &= B11101111;
    if(t_esc2 <= esc_timer)PORTD &= B11011111;
    if(t_esc3 <= esc_timer)PORTD &= B10111111;
    if(t_esc4 <= esc_timer)PORTD &= B01111111;
  }



  
} //| -----------  END OF MAIN LOOP ----------- |

void read_mpu()
{

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
  Wire.write(0x00);
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
}

void get_offset()
{
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

ISR(PCINT0_vect){
  current_time = micros();
  
  //Channel 8
  if(PINB & B00000001) //DI8 high
  {
    if(ps_ch1 == 0) //if the channel has changed state
    {
      ps_ch1 = 1;
      t_ch1 = current_time; 
    }
  }
  else if(ps_ch1 == 1)
  {
    ps_ch1 = 0;
    ch1_input = current_time - t_ch1;
  }

  //Channel 2
  if(PINB & B00000010) //DI9 high
  {
    if(ps_ch2 == 0)
    {
      ps_ch2 = 1;
      t_ch2 = current_time;
    }
  }  
  else if(ps_ch2 == 1)
  {
    ps_ch2 = 0;
    ch2_input = current_time - t_ch2;
  }
  

  //Channel3
  if(PINB & B00000100) //DI10 high
  {
    if(ps_ch3 == 0)
    {
      ps_ch3 = 1;
      t_ch3 = current_time;  
    }  
  }
  else if(ps_ch3 == 1)
  {
    ps_ch3 = 0;
    ch3_input = current_time - t_ch3;
  }
  

  //Channel 11
  if(PINB & B00001000) //DI10 high
  {
    if(ps_ch4 == 0)
    {
      ps_ch4 = 1;
      t_ch4 = current_time;  
    }
  }  
  else if(ps_ch4 == 1)
  {
    ps_ch4 = 0;
    ch4_input = current_time - t_ch4;
  }
  
}
