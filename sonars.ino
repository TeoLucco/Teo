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

  float mf_front = frontMedian.getMedian();
  float mf_right = rightMedian.getMedian();
  float mf_left  =  leftMedian.getMedian();
  float mf_back = backMedian.getMedian();

  front_sonar_f.input(mf_front);
  right_sonar_f.input(mf_right);
  left_sonar_f.input(mf_left);
  back_sonar_f.input(mf_back);

  f_front=front_sonar_f.output();
  f_right=right_sonar_f.output();
  f_left = left_sonar_f.output();
  f_back = back_sonar_f.output();

  previous_distance=actual_distance;
  
  /*
  if(abs(f_front-prec_f_front)>150.00)
       f_front=(frontMedian.getMedian() + prec_f_front + prec_f_front2) / 3;
  else f_front=frontMedian.getMedian();

  if(abs(f_right-prec_f_right)>150.00)
    f_right=(rightMedian.getMedian() + prec_f_right + prec_f_right2) / 3;
  else f_right=rightMedian.getMedian();

  if(abs(f_left-prec_f_left)>150.00)
    f_left =( leftMedian.getMedian() + prec_f_left  + prec_f_left2 ) / 3;
  else f_left = leftMedian.getMedian();
*/


  /*
  right_sonar_f.input(right_distance);
  left_sonar_f.input(left_distance);
  front_sonar_f.input(front_distance);

  f_front=front_sonar_f.output();
  f_right=right_sonar_f.output();
  f_left = left_sonar_f.output();



  if(abs(front_sonar_f.output()-prec_f_front)>150.00)
     f_front=(front_sonar_f.output() + prec_f_front + prec_f_front2) / 3;
  else f_front=front_sonar_f.output();

  if(abs(right_sonar_f.output()-prec_f_right)>150.00)
     f_right=(right_sonar_f.output() + prec_f_right + prec_f_right2) / 3;
  else f_right=right_sonar_f.output();

  if(abs(left_sonar_f.output()-prec_f_left)>150.00)
     f_left =( left_sonar_f.output() + prec_f_left  + prec_f_left2 ) / 3;
  else f_left = left_sonar_f.output();
*/


/*
 *f_front e tutti gli altri devono essere inizializzati al nuovo valore sparato fuori dal filtro che,
 *f_front aggiunto ai precedenti N e diviso per N+1 mi da un valore smooth
 * if(abs(front_sonar_f.output()-prec_f_front)>150.00){
 *    for(int i=0;i<sizeof(prec_front)/sizeof(*prec_front);i++)
 *      f_front+=prec_front[i];
 *    f_front=f_front/(sizeof(prec_front)/sizeof(*prec_front)+1);
 * }
 *
 * if(abs(right_sonar_f.output()-prec_f_right)>150.00){
 *    for(int i=0;i<sizeof(prec_right)/sizeof(*prec_right);i++)
 *      f_right+=prec_right[i];
 *    f_right=f_right/(sizeof(prec_right)/sizeof(*prec_right)+1);
 * }
 *
 * if(abs(left_sonar_f.output()-prec_f_left)>150.00){
 *    for(int i=0;i<sizeof(prec_left)/sizeof(*prec_left);i++)
 *      f_left+=prec_left[i];
 *    f_left=f_left/(sizeof(prec_left)/sizeof(*prec_left)+1);
 * }
 *
 * for(int i=sizeof(prec_front)/sizeof(*prec_front)-1; i>0 ; i++){
 *   prec_front[i] = prec_front[i-1];
 *   prec_right[i] = prec_right[i-1];
 *   prec_left[i]  = prec_left[i-1];
 * }
*
*  prec_front[0] = f_front;
*  prec_right[0] = f_right;
*  prec_left[0] = f_left;




  prec_f_front2 = prec_f_front;
  prec_f_right2 = prec_f_right;
  prec_f_left2  = prec_f_left;

  prec_f_front = f_front;
  prec_f_right = f_right;
  prec_f_left  = f_left;

*/

  if(f_front>FAR_DISTANCE && f_right>FAR_DISTANCE && f_left>FAR_DISTANCE) {
    //serial.println("NO obstacle");
    right_obstacle=false;
    front_obstacle=false;
    left_obstacle=false;
    //back_obstacle=false;
  
    close_right_obstacle=false;
    close_front_obstacle=false;
    close_left_obstacle=false;
    //close_back_obstacle=false;
  
    veryclose_right_obstacle=false;
    veryclose_front_obstacle=false;
    veryclose_left_obstacle=false;
    //veryclose_back_obstacle=false;
    actual_distance=400;
    no_obstacle=true;
    targetPos=0;
    last_obstacle=actual_obstacle;

  }else{
    no_obstacle=false;

    if(f_right<=FAR_DISTANCE) {
      right_obstacle=true;
      if(f_right<=CLOSE_DISTANCE){
         close_right_obstacle=true;
         if(f_right<=VERYCLOSE_DISTANCE)
           veryclose_right_obstacle=true;
         else veryclose_right_obstacle=false;
      }else close_right_obstacle=false;
      //serial.println("OSTACOLODX");
    }else{
      right_obstacle=false;
    }

    if(f_left<=FAR_DISTANCE) {
      left_obstacle=true;
      if(f_left<=CLOSE_DISTANCE){
       close_left_obstacle=true;
       if(f_left<=VERYCLOSE_DISTANCE)
         veryclose_left_obstacle=true;
       else veryclose_left_obstacle=false;
      }else close_left_obstacle=false;
      //serial.println("OSTACOLOSX");
    }else{
      left_obstacle=false;
    }

    if(f_front<=FAR_DISTANCE) {
      front_obstacle=true;
      if(f_front<CLOSE_DISTANCE){
         close_front_obstacle=true;
         if(f_front<=VERYCLOSE_DISTANCE)
           veryclose_front_obstacle=true;
         else veryclose_front_obstacle=false;
      }else close_front_obstacle=false;
        //serial.println("OSTACOLO");
    }else{
      front_obstacle=false;
    }

    if(f_back<=FAR_DISTANCE) {
      backI++;
      back_obstacle=true;
      if(f_back<CLOSE_DISTANCE){
         close_back_obstacle=true;
         if(f_back<=VERYCLOSE_DISTANCE)
           veryclose_back_obstacle=true;
         else veryclose_back_obstacle=false;
      }else close_back_obstacle=false;
        //serial.println("OSTACOLO");
    }else{
      back_obstacle=false;
    }

    double error=5.0f;
    //double error2=3.0f;
    
    if((f_front<=f_left && f_front<=f_right)
        || (f_front<=f_right+error && f_front>=f_right-error && f_front<=f_left+error && f_front>=f_left-error) ){
      actual_distance=f_front;
      targetPos=0;
    
    }else{
      if( (f_right<f_front && f_right<f_left)){
          targetPos++;
          actual_distance=f_right;
      }
      if( (f_left<f_front && f_left<f_right)){
          targetPos--;
          actual_distance=f_left;
      }
    
    }
    
    if(targetPos>=MAX_COUNTER) targetPos=MAX_COUNTER;
    else if (targetPos<=-MAX_COUNTER) targetPos=-MAX_COUNTER;
    if(targetPos>COUNTER)
      actual_obstacle=right;
    else{ 
      if(targetPos<-COUNTER)
        actual_obstacle=left;
      else actual_obstacle=front;
    }
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

