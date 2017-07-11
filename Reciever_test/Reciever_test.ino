#include <EEPROM.h> 

void setup() 
{
  // Initialize output ports
  DDRD |= B11110000; //DP 4,5,6, & 7 = DO
  DDRB |= B00010000; //DP 12 = DO

  //Configure interrupts
  PCICR |= (1 << PCIE0); //Setting PCIE0 will enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0); //PCINT0, DO8 as interrupt
  PCMSK0 |= (1 << PCINT1); //PCINT1, DO9 as interrupt
  PCMSK0 |= (1 << PCINT2); //PCINT2, DO10 as interrupt
  PCMSK0 |= (1 << PCINT3); //PCINT3, DO11 as interrupt
}

void loop() 
{
  Serial.print("Main");
}

ISR(PCINT0_vect)
{
  Serial.print("Interrupt");
}
