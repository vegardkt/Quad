void setup() {
  // put your setup code here, to run once:

}

void loop() {
  //1. verify that clock is 16MHz
  //2. Verify that I2C clock speed is set to 400khz.
  //3. Verify that all 4 receiver signals are between 990 and 2100
  //4. Store center positions
  //5. Check throttle inversion
  //6. Check roll inversion
  //7. Check pitch inversion
  //8. Check yaw inversion
  //9. Register min & max of all sticks
  //   Checks which MPY is used (i dont need this)
  //10.Store gyro offset by averaging (should i do this every time anyway? (lets test)
  //11. Check Gyro Roll inversion
  //12. Check Gyro Pitch inverison
  //13. Check Gyro Yaw inversion
  //14. Store information in EEPROM!

  //Following variables are to be stored:
  //c_ch_1
  //c_ch_2
  //c_ch_3
  //c_ch_4
  //h_ch_1
  //h_ch_2
  //h_ch_3
  //h_ch_4
  //l_ch_1
  //l_ch_2
  //l_ch_3
  //l_ch_4
  //ch_1_ass
  //ch_2_ass
  //ch_3_ass
  //ch_4_ass
  
}
