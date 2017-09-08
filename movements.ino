#define SCARED_FORWARD_SP 20.0f
#define SCARED_ANGULAR_SP 1.0f
#define SCARED_ANGULAR_SP2 1.5f //rad/s
#define HAPPY_FORWARD_SP 30.0f
#define HAPPY_ANGULAR_SP 2.0f
#define SAD_ANGULAR_SP 0.6f
#define SAD_FORWARD_SP  10.0f
#define DONTWONNA_ANGULAR_SP 1.5f
#define ANGRY_ANGULAR_SP 4.0f
#define ANGRY_FORWARD_SP 20.0f

unsigned long int lastround = 0;
void iMfollowingU() {
  follow2 = true;
  aut_mov = false;
  if (no_obstacle) {
    if (last_obstacle == none) triskar.run(0.0, 0.0);
    if (last_obstacle == right) triskar.run(0.0, -(2.0f * (float)PI) / 6.0f);
    if (last_obstacle == left) triskar.run(0.0, (2.0f * (float)PI) / 6.0f);
    if (last_obstacle == front) triskar.run(25.0, 0.0);
  } else {
    if (veryclose_back_obstacle && millis() - lastround > 10000 && backI >= 100 && triskar.isStopped()) {
      lastround = millis();
      stopMovement();
      backI = 0;
      last_obstacle = none;
      alpha = PI;
      if (dir == 1)
        startMovement(turnAlphaL);
      else
        startMovement(turnAlphaR);
      dir = rand() % 2;
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
  if (millis() - movStartTime >= randomTurnTime) {
    alpha = PI / 2;
    stopMovement();
    if (dir == 1)
      startMovement(turnAlphaL);
    else
      startMovement(turnAlphaR);
    dir = rand() % 2;
    randomTurnTime = 15000 + rand() % (20000);
    obstacleCount = 0;
  } else {
    if (!close_front_obstacle && !close_right_obstacle && !close_left_obstacle) {
      triskar.run(20.0f, 0.0f);
      if (millis() - lastObstacleTime > 1000)
        obstacleCount = 0;
    } else {
      randomTurnTime = 15000 + rand() % (20000);
      lastObstacleTime = millis();
      if ((f_right < f_front && f_right < f_left) || (f_front < f_right && f_right < f_left)) {
        if (f_right > VERYCLOSE_DISTANCE)
          triskar.run(10.0f, (2.0f * (float)PI) / 8.0f);
        else {
          if (obstacleCount >= 5)  alpha = PI + PI / 4;
          else                  alpha = PI / 3;
          stopMovement();
          startMovement(turnAlphaL);
          obstacleCount++;
        }
      }
      else if ((f_left < f_front && f_left < f_right) || (f_front < f_left && f_left < f_right)) {
        if (f_left > VERYCLOSE_DISTANCE)
          triskar.run(10.0f, -(2.0f * (float)PI) / 8.0f);
        else {
          if (obstacleCount >= 5)  alpha = PI;
          else                  alpha = PI / 3;
          stopMovement();
          startMovement(turnAlphaR);
          obstacleCount++;
        }
      }
    }
  }
}

void startMovement(byte movement, uint32_t color, ledStates ledState, byte audio) {
  triskar.resetIterm();
  movStartTime = millis();
  triskar.setPosTh(0);
  triskar.setPosX(0);
  triskar.setPosY(0);
  startPosTh = triskar.getPosTh();
  startPosX = triskar.getPosX();
  startPosY = triskar.getPosY();
  actual_movement = movement;
  headLedUpdate(color, ledState);
  playS(audio);
  movementI = 0;
  move = true;
}

void startMovement(byte movement, uint32_t color, ledStates ledState) {
  triskar.resetIterm();
  movStartTime = millis();
  triskar.setPosTh(0);
  triskar.setPosX(0);
  triskar.setPosY(0);
  startPosTh = triskar.getPosTh();
  startPosX = triskar.getPosX();
  startPosY = triskar.getPosY();
  actual_movement = movement;
  headLedUpdate(color, ledState);
  updateSong();
  movementI = 0;
  move = true;
}
void startMovement(byte movement, uint32_t color) {
  triskar.resetIterm();
  movStartTime = millis();
  triskar.setPosTh(0);
  triskar.setPosX(0);
  triskar.setPosY(0);
  startPosTh = triskar.getPosTh();
  startPosX = triskar.getPosX();
  startPosY = triskar.getPosY();
  actual_movement = movement;
  headLedSetColor(color);
  updateSong();
  movementI = 0;
  move = true;
}

void startMovement(byte movement, ledStates ledState) {
  triskar.resetIterm();
  movStartTime = millis();
  triskar.setPosTh(0);
  triskar.setPosX(0);
  triskar.setPosY(0);
  startPosTh = triskar.getPosTh();
  startPosX = triskar.getPosX();
  startPosY = triskar.getPosY();
  actual_movement = movement;
  headLedUpdate(ledState);
  updateSong();
  movementI = 0;
  move = true;
}

void startMovement(byte movement) {
  triskar.resetIterm();
  movStartTime = millis();
  triskar.setPosTh(0);
  triskar.setPosX(0);
  triskar.setPosY(0);
  startPosTh = triskar.getPosTh();
  startPosX = triskar.getPosX();
  startPosY = triskar.getPosY();
  actual_movement = movement;
  headLedUpdate(color_pulse);
  updateSong();
  movementI = 0;
  move = true;
}

void stopMovement() {
  triskar.stop();
  triskar.setPosTh(0);
  triskar.setPosX(0);
  triskar.setPosY(0);
  actual_movement = no_movement;
  move = false;
  head_strip.setBrightness(255);
  if (!aut_mov && !follow2) setHeadLedRainbow();
  //stopS();
  movementFinishTime = millis();
  if (gameState == mov) gameState = make_question;
}

void stopAutFollow() {
  aut_mov = false;
  follow2 = false;
  stopMovement();
  prec_movement = no_movement;
  last_obstacle = none;
}

void obstacle_stop_movement() {
  if (actual_movement != no_movement && actual_movement != dontwonna && actual_movement != follow && actual_movement != autonomous_movement) {
    if (triskar.getDirection() == 1) {
      if (veryclose_front_obstacle || veryclose_left_obstacle || veryclose_right_obstacle) {
        actual_movement = no_movement;
        //makeCircle=false;
        startMovement(dontwonna, red);
      }
    } else if (triskar.getDirection() == 2) {
      if (veryclose_back_obstacle) {
        actual_movement = no_movement;
        //makeCircle=false;
        startMovement(dontwonna, red);
      }
    }
  }
}
#define FORWARD_SP 20.0f
#define RADIO2 16.0f
#define PERIOD 2.0f*(float)PI*RADIO2/FORWARD_SP
#define PERIOD_MS PERIOD*1000
#define ANGULAR_SP FORWARD_SP/RADIO2

void make_Circle() {
  if (triskar.getPosTh() > startPosTh - 2.0 * PI)
    triskar.run(FORWARD_SP, ANGULAR_SP);
  else
    stopMovement();
}


void turn_alpha_left() {
  if (triskar.getPosTh() > startPosTh - alpha)
    triskar.run(0.0, ANGULAR_SP);
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
  }
}

void turn_alpha_left2() {
  if (triskar.getPosTh() > startPosTh - alpha && movementI == 0)
    triskar.run(0.0, ANGULAR_SP);
  else if (movementI == 0) movementI = 1;

  if (triskar.getPosX() > startPosX - 100.0 && movementI == 1)
    triskar.run(-SAD_FORWARD_SP, 0.0);
  else if (movementI == 1) movementI = 2;

  if (movementI == 2)
    stopMovement();
}


void turn_alpha_right() {
  if (triskar.getPosTh() < startPosTh + alpha)
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
  }
}

void turn_alpha_right2() {
  if (triskar.getPosTh() < startPosTh + alpha && movementI == 0)
    triskar.run(0.0, -ANGULAR_SP);
  else if (movementI == 0) movementI = 1;

  if (triskar.getPosX() > startPosX - 100.0 && movementI == 1)
    triskar.run(-SAD_FORWARD_SP, 0.0);
  else if (movementI == 1) movementI = 2;

  if (movementI == 2)
    stopMovement();

}

void turn180_left() {
  if (triskar.getPosTh() > startPosTh - PI)
    triskar.run(FORWARD_SP, ANGULAR_SP);
  else
    stopMovement();
}

void turn180_right() {
  if (triskar.getPosTh() < startPosTh + PI)
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

  float forward_speed = SCARED_FORWARD_SP;
  float angular_speed = SCARED_ANGULAR_SP;

  if ((triskar.getPosTh() < startPosTh + PI / 12.0) && movementI == 0)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 0) movementI = 1;

  if ((triskar.getPosTh() > startPosTh - PI / 12.0) && movementI == 1)
    triskar.run(-forward_speed, +angular_speed);
  else if (movementI == 1) movementI = 2;

  if ((triskar.getPosTh() < startPosTh) && movementI == 2)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 2) movementI = 3;

  if (movementI == 3) {
    stopMovement();
  }
}

void makeRunScaredBehind2() { //at the end of the scared movement the robot will go in the opposite direction in line way like makeonemF

  float forward_speed = SCARED_FORWARD_SP;
  float forward_speed2 = SAD_FORWARD_SP;
  float angular_speed = SCARED_ANGULAR_SP;

  if ((triskar.getPosTh() < startPosTh + PI / 12.0) && movementI == 0)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 0) movementI = 1;

  if ((triskar.getPosTh() > startPosTh - PI / 12.0) && movementI == 1)
    triskar.run(-forward_speed, +angular_speed);
  else if (movementI == 1) movementI = 2;

  if ((triskar.getPosTh() < startPosTh) && movementI == 2)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 2) movementI = 3;

  if (movementI == 3) {
    triskar.stop();
    movementI = 4;
  }
  if (triskar.getPosX() < startPosX + 100.0 && movementI == 4)
    triskar.run(forward_speed2, 0.0f);
  else if (movementI == 4) movementI = 5;

  if (movementI == 5)
    stopMovement();
}

void makeRunScaredBehind2L() { //at the end of the scared movement the robot will go in the opposite direction in line way like makeonemF

  float forward_speed = SCARED_FORWARD_SP;
  float forward_speed2 = SAD_FORWARD_SP;
  float angular_speed = SCARED_ANGULAR_SP;

  if (triskar.getPosTh() > startPosTh - alpha && movementI == 0)
    triskar.run(0.0, angular_speed);
  else if (movementI == 0) movementI = 1;

  if (movementI == 1)
    triskar.stop();
  else if (movementI == 1) movementI = 2;

  if ((triskar.getPosTh() < startPosTh + PI / 12.0) && movementI == 2)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 2) movementI = 3;

  if ((triskar.getPosTh() > startPosTh - PI / 12.0) && movementI == 3)
    triskar.run(-forward_speed, +angular_speed);
  else if (movementI == 3) movementI = 4;

  if ((triskar.getPosTh() < startPosTh) && movementI == 4)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 4) movementI = 5;

  if (movementI == 5) {
    triskar.stop();
    movementI = 6;
  }
  if (triskar.getPosX() < startPosX + 100.0 && movementI == 6)
    triskar.run(forward_speed2, 0.0f);
  else if (movementI == 6) movementI = 7;

  if (movementI == 7)
    stopMovement();
}

void makeRunScaredBehind2R() { //at the end of the scared movement the robot will go in the opposite direction in line way like makeonemF

  float forward_speed = SCARED_FORWARD_SP;
  float forward_speed2 = SAD_FORWARD_SP;
  float angular_speed = SCARED_ANGULAR_SP;

  if (triskar.getPosTh() < startPosTh + alpha && movementI == 0)
    triskar.run(0.0, -angular_speed);
  else if (movementI == 0) movementI = 1;

  if (movementI == 1)
    triskar.stop();
  else if (movementI == 1) movementI = 2;

  if ((triskar.getPosTh() < startPosTh + PI / 12.0) && movementI == 2)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 2) movementI = 3;

  if ((triskar.getPosTh() > startPosTh - PI / 12.0) && movementI == 3)
    triskar.run(-forward_speed, +angular_speed);
  else if (movementI == 3) movementI = 4;

  if ((triskar.getPosTh() < startPosTh) && movementI == 4)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 4) movementI = 5;

  if (movementI == 5) {
    triskar.stop();
    movementI = 6;
  }
  if (triskar.getPosX() < startPosX + 100.0 && movementI == 6)
    triskar.run(forward_speed2, 0.0f);
  else if (movementI == 6) movementI = 7;

  if (movementI == 7)
    stopMovement();
}



void makeHappy0() {
  float forward_speed = HAPPY_FORWARD_SP;
  float angular_speed = HAPPY_ANGULAR_SP;

  if ((triskar.getPosTh() < startPosTh + PI / 12.0) && movementI == 0)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 0) movementI = 1;

  if ((triskar.getPosTh() > startPosTh - PI / 12.0) && movementI == 1)
    triskar.run(-forward_speed, +angular_speed);
  else if (movementI == 1) movementI = 2;

  if ((triskar.getPosTh() < startPosTh) && movementI == 2)
    triskar.run(-forward_speed, -angular_speed);
  else if (movementI == 2) movementI = 3;

  if (movementI == 3) {
    triskar.stop();
    movementI = 4;
  }

  if ((triskar.getPosTh() < startPosTh + PI / 12.0) && movementI == 4)
    triskar.run(+forward_speed, -angular_speed);
  else if (movementI == 4) movementI = 5;

  if ((triskar.getPosTh() > startPosTh - PI / 12.0) && movementI == 5)
    triskar.run(+forward_speed, +angular_speed);
  else if (movementI == 5) movementI = 6;

  if ((triskar.getPosTh() < startPosTh) && movementI == 6)
    triskar.run(+forward_speed, -angular_speed);
  else if (movementI == 6) movementI = 7;

  if (movementI == 7) {
    stopMovement();
  }
}


void makeHappy1() {

  if ((triskar.getPosTh() < startPosTh + PI / 2.0)) {
    triskar.run(-25.0f, -1.0f);
  }
  else if ((triskar.getPosTh() >= startPosTh + PI / 2.0) && (triskar.getPosTh() < startPosTh + PI)) {
    triskar.run(25.0f, -1.0f);
  }
  else if ((triskar.getPosTh() >= startPosTh + PI) && (triskar.getPosTh() < startPosTh + 3.0 * PI / 2.0)) {
    triskar.run(-25.0f, -1.0f);
  }
  else if ((triskar.getPosTh() >= startPosTh + 3.0 * PI / 2.0) && (triskar.getPosTh() < startPosTh + 2.0 * PI)) {
    triskar.run(25.0f, -1.0f);
  }
  else {
    Serial.println("LOOP5");
    stopMovement();
  }

}

void makeHappy2() {
  if (triskar.getPosTh() < startPosTh + PI / 2.0 && movementI == 0) {
    triskar.run(0.0f, -2.0f);
  } else if (movementI == 0) movementI = 1;

  if (triskar.getPosTh() > startPosTh - PI / 2.0 && movementI == 1) {
    triskar.run(0.0f, 2.0f);
  } else if (movementI == 1) movementI = 2;

  if (triskar.getPosTh() < startPosTh + 2.0 * PI && movementI == 2) {
    triskar.run(0.0f, -2.0f);
  } else if (movementI == 2) movementI = 3;

  if (movementI == 3) {
    stopMovement();
  }

}

void makeHappy3() {

  if (triskar.getPosTh() < startPosTh + 2.0 * PI && movementI == 0) {
    triskar.run(0.0f, -2.0f);
  } else if (movementI == 0) movementI = 1;

  if (triskar.getPosTh() > startPosTh && movementI == 1) {
    triskar.run(0.0f, 2.0f);
  } else if (movementI == 1) movementI = 2;

  if (movementI == 2) {
    stopMovement();
  }

}

void makeSad0() {

  if (triskar.getPosTh() < startPosTh + PI && movementI == 0) {
    triskar.run(0.0f, -0.5f);
  } else if (movementI == 0) movementI = 1;

  if (triskar.getPosX() < startPosX + 100.0f && movementI == 1) {
    triskar.run(10.0f, 0.0f);
  } else if (movementI == 1) movementI = 2;

  if (movementI == 2) {
    stopMovement();
  }

}


void makeSad1() {
  float angular_speed = SAD_ANGULAR_SP;
  if ((triskar.getPosTh() < startPosTh + PI / 20.0) && movementI == 0)
    triskar.run(0.0f, -angular_speed);
  else if (movementI == 0) movementI = 1;

  if ((triskar.getPosTh() > startPosTh - PI / 20.0) && movementI == 1)
    triskar.run(0.0f, +angular_speed);
  else if (movementI == 1) movementI = 2;

  if ((triskar.getPosTh() < startPosTh + PI / 20.0) && movementI == 2)
    triskar.run(0.0f, -angular_speed);
  else if (movementI == 2) movementI = 3;

  if ((triskar.getPosTh() > startPosTh - PI / 20.0) && movementI == 3)
    triskar.run(0.0f, +angular_speed);
  else if (movementI == 3) movementI = 4;

  if ((triskar.getPosTh() < startPosTh) && movementI == 4)
    triskar.run(0.0f, -angular_speed);
  else if (movementI == 4) movementI = 5;

  if (movementI == 5)
    stopMovement();
}


void makeSad2() {
  float angular_speed = SAD_ANGULAR_SP;
  

  if (triskar.getPosX() > startPosX - 100.0f && movementI == 0) {
    triskar.run(-10.0f, 0.0f);
  } else if (movementI == 0) movementI = 1;

  if ((triskar.getPosTh() < startPosTh + PI / 20.0) && movementI == 1)
    triskar.run(0.0f, -SAD_ANGULAR_SP);
  else if (movementI == 1) movementI = 2;

  if ((triskar.getPosTh() > startPosTh - PI / 20.0) && movementI == 2)
    triskar.run(0.0f, +SAD_ANGULAR_SP);
  else if (movementI == 2) movementI = 3;

  if ((triskar.getPosTh() < startPosTh + PI / 20.0) && movementI == 3)
    triskar.run(0.0f, -SAD_ANGULAR_SP);
  else if (movementI == 3) movementI = 4;

  if ((triskar.getPosTh() > startPosTh - PI / 20.0) && movementI == 4)
    triskar.run(0.0f, +SAD_ANGULAR_SP);
  else if (movementI == 4) movementI = 5;

  if ((triskar.getPosTh() < startPosTh + PI) && movementI == 5)
    triskar.run(0.0f, -SAD_ANGULAR_SP);
  else if (movementI == 5) movementI = 6;

  if (movementI == 6) {
    triskar.stop();
    movementI = 7;
  }

  if (triskar.getPosX() < startPosX + 100.0f && movementI == 7) {
    triskar.run(SAD_FORWARD_SP, 0.0f);
  } else if (movementI == 7) movementI = 8;

  if (movementI == 8)
    stopMovement();
}

void makeSad2L() {

  if (triskar.getPosTh() > startPosTh - alpha && movementI == 0)
    triskar.run(0.0, SAD_ANGULAR_SP);
  else if (movementI == 0) movementI = 1;

  if (movementI == 1)
    triskar.stop();
  else if (movementI == 1) movementI = 2;

  if (triskar.getPosX() > startPosX - 100.0f && movementI == 2) {
    triskar.run(-10.0f, 0.0f);
  } else if (movementI == 2) movementI = 3;

  if ((triskar.getPosTh() < startPosTh + PI / 20.0) && movementI == 3)
    triskar.run(0.0f, -SAD_ANGULAR_SP);
  else if (movementI == 3) movementI = 4;

  if ((triskar.getPosTh() > startPosTh - PI / 20.0) && movementI == 4)
    triskar.run(0.0f, +SAD_ANGULAR_SP);
  else if (movementI == 4) movementI = 5;

  if ((triskar.getPosTh() < startPosTh + PI / 20.0) && movementI == 5)
    triskar.run(0.0f, -SAD_ANGULAR_SP);
  else if (movementI == 5) movementI = 6;

  if ((triskar.getPosTh() > startPosTh - PI / 20.0) && movementI == 6)
    triskar.run(0.0f, +SAD_ANGULAR_SP);
  else if (movementI == 6) movementI = 7;

  if ((triskar.getPosTh() < startPosTh + PI) && movementI == 7)
    triskar.run(0.0f, -SAD_ANGULAR_SP);
  else if (movementI == 7) movementI = 8;

  if (movementI == 8) {
    triskar.stop();
    movementI = 9;
  }

  if (triskar.getPosX() < startPosX + 100.0f && movementI == 9) {
    triskar.run(SAD_FORWARD_SP, 0.0f);
  } else if (movementI == 9) movementI = 10;

  if (movementI == 10)
    stopMovement();
}

void makeSad2R() {
  float angular_speed = 0.6f;

  if (triskar.getPosTh() < startPosTh + alpha && movementI == 0)
    triskar.run(0.0, -angular_speed);
  else if (movementI == 0) movementI = 1;

  if (movementI == 1)
    triskar.stop();
  else if (movementI == 1) movementI = 2;

  if (triskar.getPosX() > startPosX - 100.0f && movementI == 2) {
    triskar.run(-10.0f, 0.0f);
  } else if (movementI == 2) movementI = 3;

  if ((triskar.getPosTh() < startPosTh + PI / 20.0) && movementI == 3)
    triskar.run(0.0f, -angular_speed);
  else if (movementI == 3) movementI = 4;

  if ((triskar.getPosTh() > startPosTh - PI / 20.0) && movementI == 4)
    triskar.run(0.0f, +angular_speed);
  else if (movementI == 4) movementI = 5;

  if ((triskar.getPosTh() < startPosTh + PI / 20.0) && movementI == 5)
    triskar.run(0.0f, -angular_speed);
  else if (movementI == 5) movementI = 6;

  if ((triskar.getPosTh() > startPosTh - PI / 20.0) && movementI == 6)
    triskar.run(0.0f, +angular_speed);
  else if (movementI == 6) movementI = 7;

  if ((triskar.getPosTh() < startPosTh + PI) && movementI == 7)
    triskar.run(0.0f, -angular_speed);
  else if (movementI == 7) movementI = 8;

  if (movementI == 8) {
    triskar.stop();
    movementI = 9;
  }

  if (triskar.getPosX() < startPosX + 100.0f && movementI == 9) {
    triskar.run(SAD_FORWARD_SP, 0.0f);
  } else if (movementI == 9) movementI = 10;

  if (movementI == 10)
    stopMovement();
}



void makeScaredRound() {

  if (movementI == 0) {
    if (startPosTh < 0) {
      if (triskar.getPosTh() < startPosTh + PI)
        triskar.run(0.0f, -SCARED_ANGULAR_SP2);
      else movementI = 1;
    } else {
      if (triskar.getPosTh() > startPosTh - PI)
        triskar.run(0.0f, SCARED_ANGULAR_SP2);
      else movementI = 1;
    }

  }
  if (movementI == 1) {
    if (startPosTh < 0) {
      if (triskar.getPosTh() > startPosTh + 5 * PI / 6)
        triskar.run(0.0f, SCARED_ANGULAR_SP2);
      else movementI = 2;
    } else {
      if (triskar.getPosTh() < startPosTh - 5 * PI / 6)
        triskar.run(0.0f, -SCARED_ANGULAR_SP2);
      else movementI = 2;
    }
  }
  if (movementI == 2) {
    if (startPosTh < 0) {
      if (triskar.getPosTh() < startPosTh + PI + PI / 6)
        triskar.run(0.0f, -SCARED_ANGULAR_SP2);
      else movementI = 3;
    } else {
      if (triskar.getPosTh() > startPosTh - PI - PI / 6)
        triskar.run(0.0f, +SCARED_ANGULAR_SP2);
      else movementI = 3;
    }
  }
  if (movementI == 3) {
    if (startPosTh < 0) {
      if (triskar.getPosTh() >= startPosTh + PI)
        triskar.run(0.0f, SCARED_ANGULAR_SP2);
      else stopMovement();
    } else {
      if (triskar.getPosTh() <= startPosTh - PI)
        triskar.run(0.0f, -SCARED_ANGULAR_SP2);
      else stopMovement();
    }
  }


}


void makeOneMeterForward() {
  if (triskar.getPosX() < startPosX + 100.0)
    triskar.run(20.0f, 0.0f);
  else
    stopMovement();
}

void makeOneMeterBackward() {
  if (triskar.getPosX() > startPosX - 100.0)
    triskar.run(-20.0f, 0.0f);
  else
    stopMovement();
}


void makeDontWonna() {
  float angular_speed = DONTWONNA_ANGULAR_SP;
  if ((triskar.getPosTh() < startPosTh + PI / 75.0) && movementI == 0)
    triskar.run(0.0f, -angular_speed);
  else if (movementI == 0) movementI = 1;

  if ((triskar.getPosTh() > startPosTh - PI / 75.0) && movementI == 1)
    triskar.run(0.0f, +angular_speed);
  else if (movementI == 1) movementI = 2;

  if ((triskar.getPosTh() < startPosTh + PI / 75.0) && movementI == 2)
    triskar.run(0.0f, -angular_speed);
  else if (movementI == 2) movementI = 3;

  if ((triskar.getPosTh() > startPosTh - PI / 75.0) && movementI == 3)
    triskar.run(0.0f, +angular_speed);
  else if (movementI == 3) movementI = 4;

  if ((triskar.getPosTh() < startPosTh) && movementI == 4)
    triskar.run(0.0f, -angular_speed);
  else if (movementI == 4) movementI = 5;

  if (movementI == 5)
    stopMovement();
  //  float tMax=2000;
  //  float t=millis()-movStartTime; //msec
  //  if(triskar.getPosTh() < startPosTh+2.0f*PI/200.0)
  //    triskar.run(0.0,-angularSpeed);
  //  else
  //    triskar.run(0.0,angularSpeed);
  //
  //  if(t>tMax && triskar.getPosTh()>=startPosTh-0.01 && triskar.getPosTh()<=startPosTh+0.01)
  //    stopMovement(dontwonna);
}

double error2 = 0.1f;
void makeBeAngry() {
  triskar.setKi(2.0);
  triskar.setKp(1.5);
  int t = millis() - movStartTime; //msec
  int moduleT = movementI % 200;
  if (moduleT < 100) //3 gradi
    triskar.run(0.0, -ANGRY_ANGULAR_SP);
  else
    triskar.run(0.0, ANGRY_ANGULAR_SP);
  movementI++;
  if (t > 2400 && triskar.getPosTh() < startPosTh + error2 && triskar.getPosTh() > startPosTh - error2) {
    triskar.resetKi();
    triskar.resetKp();
    stopMovement();
  }
}


void makeMovement() {
  if (move) {
    switch (actual_movement) {
      case no_movement:         break;
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
      case scared_hit:          makeRunScaredBehind2(); break;
      case scared_hitL:         makeRunScaredBehind2L(); break;
      case scared_hitR:         makeRunScaredBehind2R(); break;
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
  }
}
