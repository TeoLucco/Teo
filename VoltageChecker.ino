/*--------------------------------------------------------------
  Program:      volt_measure

  Description:  Reads value on analog input A2 and calculates
                the voltage assuming that a voltage divider
                network on the pin divides by 11.
  
  Hardware:     Arduino Uno with voltage divider on A2.
                
  Software:     Developed using Arduino 1.0.5 software
                Should be compatible with Arduino 1.0 +

  Date:         22 May 2013
 
  Author:       W.A. Smith, https://startingelectronics.org/articles/arduino/measuring-voltage-with-arduino/
--------------------------------------------------------------*/

void voltageCheckSetup()
{
    // take a number of analog samples and add them up
    while (sample_count < NUM_SAMPLES) {
        sum += analogRead(VOLT_CHECKER_PIN);
        sample_count++;
    }
    // calculate the voltage
    // use 5.0 for a 5.0V ADC reference voltage
    // 5.015V is the calibrated reference voltage
    voltage = (((float)sum / (float)NUM_SAMPLES * 5.005) / 1024.0)*4.013;
    // send voltage for display on Serial Monitor
    // voltage multiplied by 11 when using voltage divider that
    // divides by 11. 11.132 is the calibrated voltage divide
    // value
    sample_count = 0;
    sum = 0;
    if(voltage<=11.5){
      playS(2);
      interpreterState=discharge;
      lastWarning=millis();
      firstSound=true;
    }
}




void voltageCheckloop()
{
    // take a number of analog samples and add them up
    while (sample_count < NUM_SAMPLES) {
        sum += analogRead(VOLT_CHECKER_PIN);
        sample_count++;
    }
    // calculate the voltage
    // use 5.0 for a 5.0V ADC reference voltage
    // 5.015V is the calibrated reference voltage
    voltage = (((float)sum / (float)NUM_SAMPLES * 5.005) / 1024.0)*4.013;
    // send voltage for display on Serial Monitor
    // voltage multiplied by 11 when using voltage divider that
    // divides by 11. 11.132 is the calibrated voltage divide
    // value
    sample_count = 0;
    sum = 0;
    if(voltage<=11.5 && millis()-lastWarning>15000){
      playS(2);
      lastWarning=millis();
    }
}

