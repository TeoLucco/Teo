#define ANGRY_FORWARD_SP 38.0f
#define HAPPY_FORWARD_SP 34.0f
#define SCARED_FORWARD_SP 38.0f
#define SAD_FORWARD_SP  17.0f //13

#define ANGRY_ANGULAR_SP 4.0f
#define HAPPY_ANGULAR_SP 1.5f
#define SCARED_ANGULAR_SP 2.0f
#define SAD_ANGULAR_SP  0.8f  //0.6

int lost = 0;

unsigned long int lastround = 0;
void iMfollowingU() {
  follow2 = true;
  aut_mov = false;
  idle_mov = false;
  if (no_obstacle) {
    if (last_obstacle == none) {
      timedPlayS(9, 10000);
      triskar.run(0.0, 0.0);
    }
    if (last_obstacle == right) {
      triskar.run(0.0, -(2.0f * (float)PI) / 6.0f);
    }
    if (last_obstacle == left) {
      triskar.run(0.0, (2.0f * (float)PI) / 6.0f);
    }
    if (last_obstacle == front) {
      timedPlayS(10, 10000);
      triskar.run(25.0, 0.0);
    }
  } else {
    if (lost > 5000) {
      startMovement(make_sad0, yellowC, color_wipe, 11);
      lost = 0;
    }
    else if (veryclose_front_obstacle && triskar.isStopped() &&  previous_distance == actual_distance) lost++;
    if (veryclose_back_obstacle && millis() - lastround > 10000 && backI >= 100 && triskar.isStopped()) {
      lastround = millis();
      stopMovement();
      backI = 0;
      last_obstacle = none;
      alpha = PI;
      if (dir == 1)
        startMovement(turnAlphaL, greenC, color_wipe, 12);
      else
        startMovement(turnAlphaR, greenC, color_wipe, 12);
      dir = rand() % 2;
    } else {
      if (actual_obstacle == right) {
        if (!close_front_obstacle && !close_right_obstacle) {
          triskar.run(15.0, -(2.0f * (float)PI) / 20.0f);
        } else triskar.run(0.0, 0.0);
      } else if (actual_obstacle == left) {
        if (!close_front_obstacle && !close_left_obstacle)
          triskar.run(15.0f, (2.0f * (float)PI) / 20.0f);
        else triskar.run(0.0, 0.0);
      } else if (actual_obstacle == front) {
        if (close_front_obstacle || close_right_obstacle || close_left_obstacle)
          triskar.run(0.0, 0.0);
        else triskar.run(20.0, 0.0);
      }
    }
  }
}

void autonomousMovement() {
  aut_mov = true;
  follow2 = false;
  idle_mov = false;
  if (millis() - movStartTime >= randomTurnTime) {//Turn randomly left/right 90 degrees at random time
    alpha = PI / 2;
    stopMovement();
    if (dir == 1)
      startMovement(turnAlphaL, greenC, color_wipe, 13);
    else
      startMovement(turnAlphaR, greenC, color_wipe, 13);
    dir = rand() % 2;
    randomTurnTime = 15000 + rand() % (20000);
    obstacleCount = 0;
  } else {
    if (!close_front_obstacle && !close_right_obstacle && !close_left_obstacle) {//if no obstacles, go straight
      triskar.run(20.0f, 0.0f);
      if (millis() - lastObstacleTime > 1000)
        obstacleCount = 0;
    } else {                                                                    // if obstacles, define a new time for random turn and turn in the opposite direction to the closest obstacle
      randomTurnTime = 15000 + rand() % (20000);
      lastObstacleTime = millis();
      if ((f_right < f_front && f_right < f_left) || (f_front < f_right && f_right < f_left)) {//if closest obstacle on right(or at center, but right sonar reads a closest value than left one), turn left
        if (f_right > VERYCLOSE_DISTANCE)                 //if not veryclose, turn still going straight
          triskar.run(10.0f, (2.0f * (float)PI) / 8.0f);
        else {                                            //if robot is veryclose to an obstacle, stop to go straight and rotates only
          if (obstacleCount >= 4)  alpha = PI + PI / 6;
          else                  alpha = PI / 3;
          stopMovement();
          startMovement(turnAlphaL, greenC, color_wipe, 14);
          obstacleCount++;
        }
      }
      else if ((f_left < f_front && f_left < f_right) || (f_front < f_left && f_left < f_right)) {
        if (f_left > VERYCLOSE_DISTANCE)
          triskar.run(10.0f, -(2.0f * (float)PI) / 8.0f);
        else {
          if (obstacleCount >= 4)  alpha = PI + PI / 6;
          else                  alpha = PI / 3;
          stopMovement();
          startMovement(turnAlphaR, greenC, color_wipe, 14);
          obstacleCount++;
        }
      }
    }
  }
}


void idleMovement() {
  idle_mov = true;
  aut_mov = false;
  follow2 = false;
  if (millis() - movStartTime >= randomIdleTurnTime) {//Turn randomly left/right 90 degrees at random time
    alpha = PI / 6 + (rand() % (6));
    stopMovement();
    if (dir == 1)
      startMovement(turnAlphaL, rainbow_cycle, 13);
    else
      startMovement(turnAlphaR, rainbow_cycle, 13);
    dir = rand() % 2;
    randomIdleTurnTime = 3000 + rand() % (7000);
    obstacleCount = 0;
  } else {
    if (!close_front_obstacle && !close_right_obstacle && !close_left_obstacle) {//if no obstacles, go straight
      if (body_led_state != led_off) resetLeds();
      triskar.run(20.0f, 0.0f);
      if (millis() - lastObstacleTime > 1000)
        obstacleCount = 0;
    } else {                                                                    // if obstacles, define a new time for random turn and turn in the opposite direction to the closest obstacle
      randomIdleTurnTime = 3000 + rand() % (7000);
      lastObstacleTime = millis();
      if ((f_right < f_front && f_right < f_left) || (f_front < f_right && f_right < f_left)) {//if closest obstacle on right(or at center, but right sonar reads a closest value than left one), turn left
        if (f_right > VERYCLOSE_DISTANCE)                 //if not veryclose, turn still going straight
          triskar.run(10.0f, (2.0f * (float)PI) / 8.0f);
        else {                                            //if robot is veryclose to an obstacle, stop to go straight and rotates only
          if (obstacleCount >= 4)  alpha = PI + PI / 6;
          else                  alpha = PI / 3;
          stopMovement();
          startMovement(turnAlphaL, rainbow_cycle, 14);
          obstacleCount++;
        }
      }
      else if ((f_left < f_front && f_left < f_right) || (f_front < f_left && f_left < f_right)) {
        if (f_left > VERYCLOSE_DISTANCE)
          triskar.run(10.0f, -(2.0f * (float)PI) / 8.0f);
        else {
          if (obstacleCount >= 4)  alpha = PI + PI / 6;
          else                  alpha = PI / 3;
          stopMovement();
          startMovement(turnAlphaR, rainbow_cycle, 14);
          obstacleCount++;
        }
      }
    }
  }
}

void broke_ice() {
  if (no_obstacle) {
    timedPlayS(9, 10000); //AVVICINATI!
    triskar.run(0.0, 0.0);
  } else {
    if (actual_obstacle == right) {
      if (!close_front_obstacle && !close_right_obstacle)
        triskar.run(15.0, -(2.0f * (float)PI) / 20.0f);
      else triskar.run(0.0, 0.0);
    } else if (actual_obstacle == left) {
      if (!close_front_obstacle && !close_left_obstacle)
        triskar.run(15.0f, (2.0f * (float)PI) / 20.0f);
      else triskar.run(0.0, 0.0);
    } else if (actual_obstacle == front) {
      if (!close_front_obstacle)
        triskar.run(20.0, 0.0);
      else startMovement(dance_mov, blueC, color_wipe, 18);//inizia ballo e canta
    }
  }
}

void autonomousMovementWithCapacitors() { 
  aut_mov = true;
  follow2 = false;
  idle_mov = false;
  if (touch_type == patT || touch_type == hitT) { //Turn randomly left/right 90 degrees at random time
    stopMovement();
    switch (touched) {
      case left: alpha = (2 * PI) / 3; startMovement(turnAlphaL, greenC, color_wipe, 13); break;
      case right: alpha = (2 * PI) / 3; startMovement(turnAlphaR, greenC, color_wipe, 13); break;
      case front: 
//        alpha = PI / 2;
//        if (dir == 1)
//          startMovement(turnAlphaL, greenC, color_wipe, 13);
//        else
//          startMovement(turnAlphaR, greenC, color_wipe, 13);
//        dir = rand() % 2; 
        break;
    }
    randomTurnTime = 20000 + rand() % (20000);
  } else if (millis() - movStartTime >= randomTurnTime) { //Turn randomly left/right 90 degrees at random time
    alpha = PI / 2;
    stopMovement();
    if (dir == 1)
      startMovement(turnAlphaL, greenC, color_wipe, 13);
    else
      startMovement(turnAlphaR, greenC, color_wipe, 13);
    dir = rand() % 2;
    randomTurnTime = 15000 + rand() % (20000);
    obstacleCount = 0;
  } else {
    if (!close_front_obstacle && !close_right_obstacle && !close_left_obstacle) {//if no obstacles, go straight
      triskar.run(20.0f, 0.0f);
      if (millis() - lastObstacleTime > 1000)
        obstacleCount = 0;
    } else {                                                                    // if obstacles, define a new time for random turn and turn in the opposite direction to the closest obstacle
      randomTurnTime = 15000 + rand() % (20000);
      lastObstacleTime = millis();
      if ((f_right < f_front && f_right < f_left) || (f_front < f_right && f_right < f_left)) {//if closest obstacle on right(or at center, but right sonar reads a closest value than left one), turn left
        if (f_right > VERYCLOSE_DISTANCE)                 //if not veryclose, turn still going straight
          triskar.run(10.0f, (2.0f * (float)PI) / 8.0f);
        else {                                            //if robot is veryclose to an obstacle, stop to go straight and rotates only
          if (obstacleCount >= 4)  alpha = PI + PI / 6;
          else                  alpha = PI / 3;
          stopMovement();
          startMovement(turnAlphaL, greenC, color_wipe, 14);
          obstacleCount++;
        }
      }
      else if ((f_left < f_front && f_left < f_right) || (f_front < f_left && f_left < f_right)) {
        if (f_left > VERYCLOSE_DISTANCE)
          triskar.run(10.0f, -(2.0f * (float)PI) / 8.0f);
        else {
          if (obstacleCount >= 4)  alpha = PI + PI / 6;
          else                  alpha = PI / 3;
          stopMovement();
          startMovement(turnAlphaR, greenC, color_wipe, 14);
          obstacleCount++;
        }
      }
    }
  }
}


void startMovement(byte movement, colors color, ledStates ledState, byte audio) {
  startMovementBase(movement);
  playS(audio);
  if (body_leds) bodyLedUpdate(ledState, color);
}

void startMovement(byte movement, ledStates ledState, byte audio) {
  startMovementBase(movement);
  playS(audio);
  if (body_leds) bodyLedUpdate(ledState);
}

void startMovement(byte movement, colors color, ledStates ledState) {
  startMovementBase(movement);
  if (body_leds) bodyLedUpdate(ledState, color);
}
void startMovement(byte movement, colors color) {
  startMovementBase(movement);
  if (body_leds) bodyLedUpdate(color);
}

void startMovement(byte movement, ledStates ledState) {
  startMovementBase(movement);
  if (body_leds) bodyLedUpdate(ledState);
}

void startMovement(byte movement) {
  startMovementBase(movement);
  //if (body_leds) bodyLedUpdate(color_pulse);
}

void startMovementBase(byte movement){
  triskar.resetIterm();
  movStartTime = millis();
  triskar.setPosTh(0);
  triskar.setPosX(0);
  triskar.setPosY(0);
//  startPosTh = triskar.getPosTh();
//  startPosX = triskar.getPosX();
//  startPosY = triskar.getPosY();
  prev_movement = actual_movement;
  actual_movement = movement;
  updateSong();
  movementI = 0;
  move = true;
}

void stopMovement() {
  triskar.stop();
//  triskar.setPosTh(0);
//  triskar.setPosX(0);
//  triskar.setPosY(0);
  prev_movement = actual_movement;
  actual_movement = no_movement;
  move = false;
  resetLeds();
  //stopS();
  movementFinishTime = millis();
  if (gameState == mov) {
    gameState = wait_answer;
    CapacitivesUpdate(head);
  }
}

void stopAutFollow() {
  aut_mov = false;
  follow2 = false;
  idle_mov = false;
  stopMovement();
  prev_movement = no_movement;
  last_obstacle = none;
}


void obstacle_stop_movement() {
  if (actual_movement != dontwonna && actual_movement != idle && actual_movement != follow && actual_movement != autonomous_movement) {
    if ( (triskar.getDirection() == 1) && (veryclose_front_obstacle || veryclose_left_obstacle || veryclose_right_obstacle) ){
      actual_movement = no_movement;
      startMovement(dontwonna, redC, color_pulse, 15);
    }
    else if ( (triskar.getDirection() == 2 && veryclose_back_obstacle) ) {
     actual_movement = no_movement;
     startMovement(makeOnemF, redC, color_pulse, 15);
    }
  }
}



//#############################################################################DEFINIRE AUDIO E COLORI LEDDD#################################################################
#define FORWARD_SP 20.0f
#define RADIO2 16.0f
#define PERIOD 2.0f*(float)PI*RADIO2/FORWARD_SP
#define PERIOD_MS PERIOD*1000
#define ANGULAR_SP FORWARD_SP/RADIO2

void make_Circle() {
  rotateRobot(- 2.0 * PI , -ANGULAR_SP, FORWARD_SP, 0);
  stopMovement(1);
}


void turn_alpha_left() {
  rotateRobot(- alpha , ANGULAR_SP, 0.0, 0);
  stopMovement(1);
  if(actual_movement==no_movement){
    switch(prev_movement){
      case autonomous_movement:startMovement(autonomous_movement);break;
      case follow:startMovement(follow);break;
      case idle:startMovement(idle);break;
    }
  }
}
//
//void turn_alpha_left2() {
//  rotateRobot(-alpha , -ANGULAR_SP, 0.0, 0);
//  traslateRobot(-100.0 ,-SAD_FORWARD_SP,0.0, 1);
//  stopMovement(2);
//}


void turn_alpha_right() {
  if (triskar.getPosTh() <  alpha)
    triskar.run(0.0, -ANGULAR_SP);
  else {
    stopMovement();
    if (aut_mov) {
      aut_mov = false;
      startMovement(autonomous_movement);
    }
    if (follow2) {
      follow2 = false;
      startMovement(follow);
      actual_obstacle = none;
      last_obstacle = none;
    }
    if (idle_mov) {
      idle_mov = false;
      startMovement(idle, rainbow_cycle);
    }
  }
}

//void turn_alpha_right2() {
//  rotateRobot(alpha , ANGULAR_SP, 0.0, 0);
//  traslateRobot(-100.0 ,-SAD_FORWARD_SP,0.0, 1);
//  stopMovement(2);
//}

void turn180_left() {
  if (triskar.getPosTh() > - PI)
    triskar.run(FORWARD_SP, ANGULAR_SP);
  else
    stopMovement();
}

void turn180_right() {
  if (triskar.getPosTh() < + PI)
    triskar.run(FORWARD_SP, -ANGULAR_SP);
  else
    stopMovement();
}



void makeAnEight() {
  if (actual_movement == make_eight) {
    float t = millis() - movStartTime; //msec
    if (t < 2 * PERIOD_MS / 3) {
      triskar.run(FORWARD_SP, ANGULAR_SP);
    } else if (t > 2 * PERIOD_MS / 3 && t < 2 * PERIOD_MS) {
      triskar.run(FORWARD_SP, -ANGULAR_SP);
    } else if (t > 2 * PERIOD_MS && t < 8 * PERIOD_MS / 3)
      triskar.run(FORWARD_SP, ANGULAR_SP);
    else {
      stopMovement();
    }
  }
}
//funzionante, movimento espresso in funzione di t. accellera troppo ed il periodo risulta quindi troppo lungo
void makeAnEight2() {
  float t = millis() - movStartTime; //msec
  if (t < 2 * PERIOD_MS / 3) {
    if (!front_obstacle && !left_obstacle) {
      triskar.run(FORWARD_SP, ANGULAR_SP);
    } else {
      triskar.stop();
      move = false;
    }
  } else if (t > 2 * PERIOD_MS / 3 && t < 4 * PERIOD_MS / 3) {
    if (!front_obstacle && !right_obstacle) {
      triskar.run(FORWARD_SP, -ANGULAR_SP);
    } else {
      triskar.stop();
      move = false;
    }
  } else {
    triskar.stop();
    move = false;
    actual_movement = no_movement;
  }
}


void makeRunScaredBehind() {
  scaredRunBasicBR(0);
  stopMovement(5);
}

void makeRunScaredForward(int i) {
  rotateRobot(+ PI / 12.0 , SCARED_ANGULAR_SP, SCARED_FORWARD_SP, i);
  rotateRobot(- PI / 12.0 , -SCARED_ANGULAR_SP, SCARED_FORWARD_SP, i+1); 
  rotateRobot(+ PI / 12.0 , SCARED_ANGULAR_SP, SCARED_FORWARD_SP, i+2); 
  rotateRobot(- PI / 12.0 , -SCARED_ANGULAR_SP, SCARED_FORWARD_SP, i+3); 
  rotateRobot(0.0 , SCARED_ANGULAR_SP, SCARED_FORWARD_SP, i+4); 
}

void makeRunScaredHit() { //at the end of the scared movement the robot will go in the opposite direction in line way like makeonemF
  scaredRunBasicBL(0); 
  stopRobot(5);
  traslateRobot( 0.0, SAD_FORWARD_SP, 0.0, 6);
  stopMovement(7);
}

void makeRunScaredHitL() { //at the end of the scared movement the robot will go in the opposite direction in line way like makeonemF
  rotateRobot(- alpha , -SCARED_ANGULAR_SP, 0.0, 0);
  stopRobot2(1);
  scaredRunBasicBL(2); 
  stopRobot(7);
  traslateRobot( 0.0, SAD_FORWARD_SP, 0.0, 8);
  stopMovement(9);
  
}

void makeRunScaredHitR() { //at the end of the scared movement the robot will go in the opposite direction in line way like makeonemF

  rotateRobot(+ alpha , SCARED_ANGULAR_SP, 0.0, 0);
  stopRobot2(1);
  scaredRunBasicBR(2);
  stopRobot(7);
  traslateRobot( 0.0, SAD_FORWARD_SP, 0.0, 8);
  stopMovement(9);
}

void scaredRunBasicBR(int i){
  rotateRobot(- PI / 12.0 , -SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i); 
  rotateRobot(+ PI / 12.0 , SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i+1);
  rotateRobot(- PI / 12.0 , -SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i+2); 
  rotateRobot(+ PI / 12.0 , SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i+3); 
  rotateRobot( 0.0 ,  -SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i+4); 
}

void scaredRunBasicBL(int i){
  rotateRobot(+ PI / 12.0 , SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i);
  rotateRobot(- PI / 12.0 , -SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i+1); 
  rotateRobot(+ PI / 12.0 , SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i+2); 
  rotateRobot(- PI / 12.0 , -SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i+3); 
  rotateRobot( 0.0 ,  SCARED_ANGULAR_SP, -SCARED_FORWARD_SP, i+4); 
}


void makeHappy0() { 
  
    scaredRunBasicBR(0);
    stopRobot(5);
    makeRunScaredForward(6);
    stopMovement(11);
    
//  rotateRobot( PI / 12.0 ,  HAPPY_ANGULAR_SP, -HAPPY_FORWARD_SP, 0);
//  rotateRobot( - PI / 12.0 ,  -HAPPY_ANGULAR_SP, -HAPPY_FORWARD_SP, 1);
//  rotateRobot( 0.0 ,  HAPPY_ANGULAR_SP, -HAPPY_FORWARD_SP, 2);
//  stopRobot(3);
//  rotateRobot( PI / 12.0 ,  HAPPY_ANGULAR_SP, HAPPY_FORWARD_SP, 4);
//  rotateRobot( - PI / 12.0 ,  -HAPPY_ANGULAR_SP, HAPPY_FORWARD_SP, 5);
//  rotateRobot( 0.0 ,  HAPPY_ANGULAR_SP, HAPPY_FORWARD_SP, 6);
//  stopMovement(7);
}


void makeHappy1() {
  rotateRobot( PI/2.0 ,SAD_ANGULAR_SP, -HAPPY_FORWARD_SP, 0);
  stopRobot(1);
  rotateRobot( PI , SAD_ANGULAR_SP, HAPPY_FORWARD_SP, 2);
  stopRobot(3);
  rotateRobot( 3.0 * PI / 2.0 , SAD_ANGULAR_SP, -HAPPY_FORWARD_SP, 4);
  stopRobot(5);
  rotateRobot( 2.0 * PI , SAD_ANGULAR_SP, HAPPY_FORWARD_SP, 6);
  stopMovement(7);
  
//  if ((triskar.getPosTh() < + PI / 2.0)) {
//    triskar.run(-HAPPY_FORWARD_SP, -SAD_ANGULAR_SP);
//  }
//  else if ((triskar.getPosTh() >= PI / 2.0) && (triskar.getPosTh() < PI)) {
//    triskar.run(HAPPY_FORWARD_SP, -SAD_ANGULAR_SP);
//  }
//  else if ((triskar.getPosTh() >= PI) && (triskar.getPosTh() < 3.0 * PI / 2.0)) {
//    triskar.run(-HAPPY_FORWARD_SP, -SAD_ANGULAR_SP);
//  }
//
//  else if ((triskar.getPosTh() >= 3.0 * PI / 2.0) && (triskar.getPosTh() < 2.0 * PI)) {
//    triskar.run(HAPPY_FORWARD_SP, -SAD_ANGULAR_SP);
//  }
//  else {
//    stopMovement();
//  }

}

void makeHappy2() {
  rotateRobot( 2.0 * PI , HAPPY_ANGULAR_SP, 0.0f, 0);
  rotateRobot( 0.0 * PI , -HAPPY_ANGULAR_SP, 0.0f, 1);
  rotateRobot( 2.0 * PI , HAPPY_ANGULAR_SP, 0.0f, 2);
  stopMovement(3);
}

void makeHappy3() {
  rotateRobot( 2.0 * PI , HAPPY_ANGULAR_SP, 0.0f, 0);
  rotateRobot( 0.0 , -HAPPY_ANGULAR_SP, 0.0f, 1);
  stopMovement(2);
}

void makeSad0() {
  rotateRobot( PI , SAD_ANGULAR_SP, 0.0f, 0);
  stopRobot(1);
  traslateRobot(100.0f , SAD_FORWARD_SP, 0.0f, 2);
  stopMovement(3);
}


void makeSad1() {

  rotateRobot( PI / 40.0, SAD_ANGULAR_SP, 0.0f, 0);
  rotateRobot( - PI / 40.0, -SAD_ANGULAR_SP, 0.0f, 1);
  rotateRobot( PI / 40.0, SAD_ANGULAR_SP, 0.0f, 2);
  rotateRobot( - PI / 40.0, -SAD_ANGULAR_SP, 0.0f, 3);
  rotateRobot( 0.0f, SAD_ANGULAR_SP, 0.0f, 4);
  stopMovement(5);
}



void makeSad2() {
  traslateRobot(-100.f, -SCARED_FORWARD_SP, 0.0f, 0);
  rotateRobot( PI / 40.0, SAD_ANGULAR_SP, 0.0f, 1);
  rotateRobot(- PI / 40.0, -SAD_ANGULAR_SP, 0.0f, 2);
  rotateRobot( PI / 40.0, SAD_ANGULAR_SP, 0.0f, 3);
  rotateRobot(- PI / 40.0, -SAD_ANGULAR_SP, 0.0f, 4);
  rotateRobot( PI / 40.0, SAD_ANGULAR_SP, 0.0f, 5);
  stopRobot(6);
  traslateRobot(100.0, SAD_FORWARD_SP, 0.0f, 7);
  stopMovement(8);
}

void makeSad2L() {
  rotateRobot( -alpha, -SCARED_ANGULAR_SP, 0.0f, 0);
  stopRobot2(1);
  traslateRobot(-100.f, -SCARED_FORWARD_SP, 0.0f, 2);
  rotateRobot( PI / 40.0, SAD_ANGULAR_SP, 0.0f, 3);
  rotateRobot(- PI / 40.0, -SAD_ANGULAR_SP, 0.0f, 4);
  rotateRobot( PI / 40.0, SAD_ANGULAR_SP, 0.0f, 5);
  rotateRobot(- PI / 40.0, -SAD_ANGULAR_SP, 0.0f, 6);
  rotateRobot(PI, SAD_ANGULAR_SP, 0.0f, 7);
  stopRobot(8);
  traslateRobot(0.0, SAD_FORWARD_SP, 0.0f, 9);
  stopMovement(10);
}

void makeSad2R() {
  rotateRobot(alpha, SCARED_ANGULAR_SP, 0.0f, 0);
  stopRobot2(1);
  traslateRobot(-100.f, -SCARED_FORWARD_SP, 0.0f, 2);
  rotateRobot(PI / 40.0f, SAD_ANGULAR_SP, 0.0f, 3);
  rotateRobot(-PI / 40.0f, -SAD_ANGULAR_SP, 0.0f, 4);
  rotateRobot(PI / 40.0f, SAD_ANGULAR_SP, 0.0f, 5);
  rotateRobot(-PI / 40.0f, -SAD_ANGULAR_SP, 0.0f, 6);
  rotateRobot(PI, SAD_ANGULAR_SP, 0.0f, 7);
  stopRobot(8);
  traslateRobot(0.0, SAD_FORWARD_SP, 0.0f, 9);
  stopMovement(10);
}

void dance(){
   rotateRobot(PI / 40.0f, HAPPY_ANGULAR_SP, HAPPY_FORWARD_SP, 0);
   rotateRobot(-PI / 10.0f, -HAPPY_ANGULAR_SP, HAPPY_FORWARD_SP, 1);
   if(triskar.getPosTh() >= 2*PI ) stopMovement();
   else if(movementI==2) movementI=0;
}



void makeScaredRound() {
  if (dir == 1) {
    if ((triskar.getPosTh() < 2 * PI && !veryclose_front_obstacle) && movementI == 0)
      triskar.run(0.0f, -SCARED_ANGULAR_SP);
    else if (movementI == 0) movementI = 1;

    stopRobot(1);
    rotateRobot( - PI / 6,  -SCARED_ANGULAR_SP, 0.0f, 2);
    rotateRobot( + PI / 6,   SCARED_ANGULAR_SP, 0.0f, 3);
    rotateRobot(      0.0,  -SCARED_ANGULAR_SP, 0.0f, 4);
    if (movementI == 5) {
      stopMovement();
      dir = rand() % 2;
    }
  } else {
    if ((triskar.getPosTh() > - 2 * PI && !veryclose_front_obstacle) && movementI == 0)
      triskar.run(0.0f, SCARED_ANGULAR_SP);
    else if (movementI == 0) movementI = 1;
    stopRobot(1);
    rotateRobot( PI / 6, SCARED_ANGULAR_SP, 0.0f, 2);
    rotateRobot( - PI / 6, -SCARED_ANGULAR_SP, 0.0f, 3);
    rotateRobot( 0.0, SCARED_ANGULAR_SP, 0.0f, 4);
    if (movementI == 5) {
      stopMovement();
      dir = rand() % 2;
    }
  }
}


void makeOneMeterForward() {
  if (triskar.getPosX() < startPosX + 100.0)
    triskar.run(SCARED_FORWARD_SP, 0.0f);
  else
    stopMovement();
}

void makeOneMeterBackward() {
  if (triskar.getPosX() > startPosX - 100.0)
    triskar.run(-SCARED_FORWARD_SP, 0.0f);
  else
    stopMovement();
}


void makeDontWonna() {
  rotateRobot( PI / 75.0,  SCARED_ANGULAR_SP, 0.0f, 0);
  rotateRobot(- PI / 75.0,  - SCARED_ANGULAR_SP, 0.0f, 1);
  rotateRobot( PI / 75.0,  SCARED_ANGULAR_SP, 0.0f, 2);
  rotateRobot(- PI / 75.0,  - SCARED_ANGULAR_SP, 0.0f, 3);
  rotateRobot( 0.0f, SCARED_ANGULAR_SP, 0.0f, 4);
  stopMovement(5);
}

double error2 = 0.2f;
void makeBeAngry() {
  if(movementI==0){
    triskar.setKi(2.0);
    triskar.setKp(1.5);
  }
  rotateRobot( 5*PI/3,  ANGRY_ANGULAR_SP, 0.0f, 0);
  rotateRobot(-4*PI/5,  - ANGRY_ANGULAR_SP, 0.0f, 1);
  rotateRobot(-2*PI/5,  ANGRY_ANGULAR_SP, 0.0f, 2);
  rotateRobot(-8*PI/5,  - ANGRY_ANGULAR_SP, 0.0f, 3);
  rotateRobot( PI,  ANGRY_ANGULAR_SP, 0.0f, 4);
  rotateRobot(-PI/5,  - ANGRY_ANGULAR_SP, 0.0f, 5);
  rotateRobot(0.0,  ANGRY_ANGULAR_SP, 0.0f, 6);
  if(movementI==7){
    triskar.resetKi();
    triskar.resetKp();
  }
  stopMovement(7);
}



void traslateRobot(float dX, float speedX, float speedTh, int I) {
  if (speedX > 0) {
    if (triskar.getPosX() < dX && movementI == I) {
      triskar.run(speedX, speedTh);
    } else if (movementI == I) movementI = I + 1;
  } else if (speedX < 0) {
    if (triskar.getPosX() > dX && movementI == I) {
      triskar.run(speedX, speedTh);
    } else if (movementI == I) movementI = I + 1;
  }
}

void rotateRobot(float dTh, float speedTh, float speedX, int I) {
  if (speedTh > 0) {
    if ((triskar.getPosTh() < dTh) && movementI == I)
      triskar.run(speedX, -speedTh);
    else if (movementI == I) movementI = I + 1;

  } else if (speedTh < 0) {
    if ((triskar.getPosTh() > dTh) && movementI == I)
      triskar.run(speedX, -speedTh);
    else if (movementI == I) movementI = I + 1;
  }
}

void stopRobot(int i) {
  if (movementI == i) {
    triskar.stop3();
    movementI = i + 1;
  }
}

void stopRobot2(int i) {
  if (movementI == i) {
    triskar.stop();
    movementI = i + 1;
  }
}

void stopMovement(int i) {
  if (movementI == i)
    stopMovement();
}


void switchToIdle() {
  if (millis() - movementFinishTime > switchToIdleTime && interpreterState == fam_modality) {
    startMovement(idle, rainbow_cycle);
  }
}

void makeMovement() {
  if (move) {
    switch (actual_movement) {
      case idle:                idleMovement(); break;
      case follow:              iMfollowingU(); break;
      case make_eight:          break;
      case make_circle:         make_Circle(); break;
      case turn180r:            turn180_right(); break;
      //case turn180r2:           turn180_right2();break;
      case turn180l:            turn180_left(); break;
      case turnAlphaR:          turn_alpha_right(); break;
      //case turnAlphaR2:          turn_alpha_right2();break;
      case turnAlphaL:          turn_alpha_left(); break;
      //case turnAlphaL2:         turn_alpha_left2();break;
      case makeOnemF:           makeOneMeterForward(); break;
      case makeOnemB:           makeOneMeterBackward(); break;
      case scared_round:        makeScaredRound(); break;
      case dontwonna:           makeDontWonna(); break;
      case angrymov:            makeBeAngry(); break;
      case scared_behind:       makeRunScaredBehind(); break;
      case scared_hit:          makeRunScaredHit(); break;
      case scared_hitL:         makeRunScaredHitL(); break;
      case scared_hitR:         makeRunScaredHitR(); break;
      case autonomous_movement: autonomousMovement(); break;
      case make_happy0:         makeHappy0(); break;
      case make_happy1:         makeHappy1(); break;
      case make_happy2:         makeHappy2(); break;
      case make_happy3:         makeHappy3(); break;
      case make_sad0:           makeSad0(); break;
      case make_sad1:           makeSad1(); break;
      case make_sad2:           makeSad2(); break;
      case make_sad2L:          makeSad2L(); break;
      case make_sad2R:          makeSad2R(); break;
    }
    obstacle_stop_movement();
  }//else switchToIdle();
}
