 int battery_voltage;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(57600);

  //Load the battery voltage to the battery_voltage variable.
  //65 is the voltage compensation for the diode.
  //12.6V equals ~5V @ Analog 0.
  //12.6V equals 1023 analogRead(0).
  //1260 / 1023 = 1.2317.
  //The variable battery_voltage holds 1050 if the battery voltage is 10.5V.
  battery_voltage = (analogRead(0) + 65) * 1.2317;
  pinMode(12, OUTPUT);
  Serial.println("Setup completed");
  digitalWrite(12,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  battery_voltage = (analogRead(0) + 65) * 1.2317;
  battery_voltage = 1001;
  if(battery_voltage < 1000)digitalWrite(12,HIGH);
  
 Serial.println(battery_voltage);

}
