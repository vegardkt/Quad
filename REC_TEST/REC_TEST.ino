#include <Wire.h>   
#include <EEPROM.h> 

unsigned long current_time, t_ch1, t_ch2, t_ch3, t_ch4;
byte ps_ch1, ps_ch2, ps_ch3,ps_ch4;
volatile int ch1_input, ch2_input, ch3_input, ch4_input;

void setup() {
  Serial.begin(57600); //Turn off after debugging
  Serial.println("Starting setup");
  DDRD |= B11110000;//Configure digital poort 4, 5, 6 and 7 as output.
  DDRB |= B00010000;//Configure digital poort 12 as output.
  
  PCICR |= (1 << PCIE0);  //set PCIE0 to enable PCMSK0 scan.
  PCMSK0 |= (1 << PCINT0);//set PCINT0 (DI 8) to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT1);//set PCINT1 (DI 9)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT2);//set PCINT2 (DI 10)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT3);//set PCINT3 (DI 11)to trigger an interrupt on state change.
  Serial.println("Setup completed");
}

void loop() {
  delay(100); 
  Serial.print(ch1_input);
  Serial.print("|---|");
  Serial.print(ch2_input);
  Serial.print("|---|");
  Serial.print(ch3_input);
  Serial.print("|---|");
  Serial.println(ch4_input);
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
