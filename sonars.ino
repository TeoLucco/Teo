void sonarLoop() {

  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1)  // Sensor ping cycle complete, do something with the results.
        sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = MAX_DISTANCE;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  check_obstacle();

}
void check_obstacle() { // Sensor ping cycle complete, do something with the results.

  frontMedian.addValue(cm[0]);
  rightMedian.addValue(cm[1]);
  leftMedian.addValue(cm[2]);
  backMedian.addValue(cm[3]);

  //  f_front = frontMedian.getMedian();
  //  f_right = rightMedian.getMedian();
  //  f_left  =  leftMedian.getMedian();
  //  f_back = backMedian.getMedian();


  float mf_front = frontMedian.getMedian();
  float mf_right = rightMedian.getMedian();
  float mf_left  =  leftMedian.getMedian();
  float mf_back = backMedian.getMedian();

  front_sonar_f.input(mf_front);
  right_sonar_f.input(mf_right);
  left_sonar_f.input(mf_left);
  back_sonar_f.input(mf_back);

  f_front = front_sonar_f.output();
  f_right = right_sonar_f.output();
  f_left = left_sonar_f.output();
  f_back = back_sonar_f.output();


  previous_distance = actual_distance;


  if (f_front > FAR_DISTANCE && f_right > FAR_DISTANCE && f_left > FAR_DISTANCE) {
    //serial.println("NO obstacle");
    right_obstacle = farOb;
    front_obstacle = farOb;
    left_obstacle = farOb;
    back_obstacle=farOb;
   
    actual_distance = 400;
    if(actual_distance>f_front)
    actual_distance=f_front;
    if(actual_distance>f_right)
    actual_distance=f_right;
    if(actual_distance>f_left)
    actual_distance=f_left;
    no_obstacle = true;
    targetPos = 0;
    last_obstacle = actual_obstacle;

  } else {
    no_obstacle = false;

    if (f_right > FAR_DISTANCE) 
      right_obstacle = farOb;
    else if(f_right <= FAR_DISTANCE && f_right > VERYCLOSE_DISTANCE)
      right_obstacle = closeOb;
    else if(f_right <= VERYCLOSE_DISTANCE)
      right_obstacle = veryCloseOb;

    if (f_left > FAR_DISTANCE) 
      left_obstacle = farOb;
    else if(f_left <= FAR_DISTANCE && f_left > VERYCLOSE_DISTANCE)
      left_obstacle = closeOb;
    else if(f_left <= VERYCLOSE_DISTANCE)
      left_obstacle = veryCloseOb;

    if (f_front > FAR_DISTANCE) 
      front_obstacle = farOb;
    else if(f_front <= FAR_DISTANCE && f_front > VERYCLOSE_DISTANCE)
      front_obstacle = closeOb;
    else if(f_front <= VERYCLOSE_DISTANCE)
      front_obstacle = veryCloseOb;

    if (f_back > FAR_DISTANCE){
      back_obstacle = farOb;
      backI=0;
    }
    else if(f_back <= FAR_DISTANCE && f_back > VERYCLOSE_DISTANCE){
      backI++;
      back_obstacle = closeOb;
    }
    else if(f_back <= VERYCLOSE_DISTANCE){
      backI++;
      back_obstacle = veryCloseOb;
    }
    
    //double error2=3.0f;

    if ((f_front <= f_left && f_front <= f_right)
        || (f_front <= f_right + error && f_front >= f_right - error && f_front <= f_left + error && f_front >= f_left - error) ) {
      actual_distance = f_front;
      targetPos = 0;

    } else {
      if ( (f_right < f_front && f_right < f_left)) {
        targetPos++;
        actual_distance = f_right;
      }
      if ( (f_left < f_front && f_left < f_right)) {
        targetPos--;
        actual_distance = f_left;
      }

    }

    if (targetPos >= MAX_COUNTER) targetPos = MAX_COUNTER;
    else if (targetPos <= -MAX_COUNTER) targetPos = -MAX_COUNTER;
    
    if (targetPos > COUNTER)
      actual_obstacle = right;
    else if (targetPos < -COUNTER)
        actual_obstacle = left;
    else actual_obstacle = front;
    
  }

}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void sonarSetup() {
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

