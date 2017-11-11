#define SENDSTATETIME 1000
unsigned long int lastSendState = 0;
double prev_posX = 0;
double actual_posX = 0;
double prev_posY = 0;
double actual_posY = 0;
double prev_posTh = 0;
double actual_posTh = 0;

#define SCARED_KI 2.5
#define SCARED_KP 2.0
#define SCARED_SPD 36.0

#define HAPPY_KI 0.8
#define HAPPY_KP 0.35
#define HAPPY_SPD 30.0

#define SAD_KI 0.3
#define SAD_KP 0.2
#define SAD_SPD 15.0

void btInterpreter() {
  switch (interpreterState) {
    case choose_modality: chooseModality(); break;
    case choose_game:     chooseGame(); break;
    case choose_scenario: chooseScenario(); break;
    case sg_waiting:      sgWaiting(); break;
    case game_modality:   gameMod(); break;
    case fam_modality:    famMod(); break;
    case discharge:       disCharge(); break;
    case test_modality:   hardwareTests(); break;
  }
}

void chooseModality() {
  if (Serial3.available()) {
    b = Serial3.read();
    settings();
    switch (b) {
      case '7':
        interpreterState = choose_game;
        Serial3.println();
        Serial3.println("Selezionare Gioco");
        playS(02);
        break;
      case '8':
        interpreterState = fam_modality;
        CapacitivesUpdate(body);
        movementFinishTime = millis();
        break;
      default:
        break;
    }
  }
  sendState();
}

void famMod() {
  if (Serial3.available()) {
    b = Serial3.read();
    movementPanel();
    startMovementBT();
    settings();
    playAudio();
    headButtonsControl();
    switch (b) {
      case '.': interpreterState = choose_modality; break;
      case '7': interpreterState = choose_game; break;

    }
  }
  sendState();
}

void headButtonsControl() {
  if (b == '!') {
    buttonToTouch = Serial3.parseInt();
    CapacitivesUpdate(head);
    //Serial3.print("buttonToTouch: ");Serial3.println(buttonToTouch);
  }
}

void movementPanel() {
  switch (b) {
    case '0':
      //resetButtons();
      if (actual_movement == no_movement) {
        if (prev_movement != idle && prev_movement != follow && prev_movement != autonomous_movement) {
          triskar.stop2();
          move = false;
        } else if (prev_movement == follow) {
          actual_movement = follow;
          prev_movement = no_movement;
        }
        else if (prev_movement == autonomous_movement) {
          actual_movement = autonomous_movement;
          prev_movement = no_movement;
        }
        else if (prev_movement == idle) {
          //startMovement(idle, rainbow_cycle);
          actual_movement = idle;
          prev_movement = no_movement;
        }
      }
      //}
      break;

    case '1':
      //Serial3.println("1-SU");
      if (!veryclose_front_obstacle && actual_movement != dontwonna) {
        if (actual_movement == idle) {
          prev_movement = idle;
          actual_movement = no_movement;
        }
        move = true;
        movementFinishTime = millis();
        triskar.run(speed_trg, 0.0);
      } else if (veryclose_front_obstacle && actual_movement != dontwonna)  startMovement(dontwonna, yellowC, color_pulse, DONT_WONNA_AUDIO);
      break;
    case '2':
      //Serial3.println("2-GIU'");
      if (!veryclose_back_obstacle && actual_movement != dontwonna) {
        if (actual_movement == idle) {
          prev_movement = idle;
          actual_movement = no_movement;
        }
        move = true;
        triskar.run(-speed_trg, 0.0);
        movementFinishTime = millis();
      } else if (veryclose_back_obstacle && actual_movement != dontwonna)  startMovement(dontwonna, yellowC, color_pulse, DONT_WONNA_AUDIO);
      break;

    case '3':
      //Serial3.println("3-SINISTRA");
      move = true;
      if (actual_movement == no_movement) {
        triskar.run(0.0, speed_trg / robot_radius);
        movementFinishTime = millis();
      } else if (actual_movement == autonomous_movement || actual_movement == follow || actual_movement == idle) {
        prev_movement = actual_movement;
        actual_movement = no_movement;
        movementFinishTime = millis();
        triskar.run(0.0, speed_trg / robot_radius);
      }

      break;

    case '4':
      //Serial3.println("4-DESTRA");
      move = true;
      if (actual_movement == no_movement) {
        triskar.run(0.0, -speed_trg / robot_radius);
        movementFinishTime = millis();
      } else if (actual_movement == autonomous_movement || actual_movement == follow || actual_movement == idle) {
        prev_movement = actual_movement;
        actual_movement = no_movement;
        movementFinishTime = millis();
        triskar.run(0.0, -speed_trg / robot_radius);
      }
      break;
    //Code when RIGHT key is pressed

    case '5':
      speed_trg -= 3;
      if (speed_trg < 0)     speed_trg = 0;
      Serial3.print("*C" + String(speed_trg) + "*");
      break;

    case '6':
      speed_trg += 3;
      if (speed_trg > 45)   speed_trg = 45;
      Serial3.print("*C" + String(speed_trg) + "*");
      break;

    case 'f':
      triskar.setKi(triskar.getKi() + 0.02);
      Serial3.print("*I" + String(triskar.getKi()) + "*");
      break;
    case 'g':
      triskar.setKi(triskar.getKi() - 0.02);
      Serial3.print("*I" + String(triskar.getKi()) + "*");
      break;
    case 'h':
      triskar.setKp(triskar.getKp() + 0.02);
      Serial3.print("*P" + String(triskar.getKp()) + "*");
      break;
    case 'i':
      triskar.setKi(triskar.getKp() - 0.02);
      Serial3.print("*P" + String(triskar.getKp()) + "*");
      break;
  }
}

void settings() {
  switch (b) {
    case 'L': bodyCapacitor = true; CapacitivesUpdate(body); break;
    case 'M': CapacitivesUpdate(noOne); bodyCapacitor = false; break;
    case 'N': speakers = true; break;
    case 'O': speakers = false; break;
    case 'P': headLedSetColor(red); break;
    case 'Q': headLedSetColor(yellow); break;
    case 'R': headLedSetColor(blue); break;
    case 'S': headLedSetColor(green); break;
    case 'T': headLedUpdate(led_off); head_leds = false; break;
    case 'U': head_leds = true; headLedUpdate(rainbow_cycle); break;
    case 'V': head_leds = true; headLedUpdate(color_wipe);  break;
    case 'W': head_leds = true; headLedUpdate(color_pulse); break;
    case 'X': bodyLedUpdate(redC); break;
    case 'Y': bodyLedUpdate(yellowC); break;
    case 'Z': bodyLedUpdate(blueC); break;
    case '[': bodyLedUpdate(greenC); break;
    case ']': bodyLedUpdate(led_off); body_leds = false; break;
    case '^': body_leds = true; bodyLedUpdate(rainbow_cycle); break;
    case '_': body_leds = true; bodyLedUpdate(color_wipe); break;
    case 'a': body_leds = true; bodyLedUpdate(color_pulse); break;
    case 'b': fotoresistor = true; break;
    case 'c': fotoresistor = false; break;
    case 'd': micro = true; break;
    case 'e': micro = false; break;
    case 'l': /*triskar.setKi(SCARED_KI); triskar.setKp(SCARED_KP);*/ bodyLedUpdate(color_pulse, orangeC); speed_trg = SCARED_SPD;Serial3.print("*C" + String(speed_trg) + "*"); break;
    case 'm': /*triskar.setKi(HAPPY_KI); triskar.setKp(HAPPY_KP);*/ bodyLedUpdate(color_pulse, greenC); speed_trg = HAPPY_SPD; Serial3.print("*C" + String(speed_trg) + "*");break;
    case 'n': /*triskar.setKi(SAD_KI); triskar.setKp(SAD_KP);*/ bodyLedUpdate(color_pulse, blueC); speed_trg = SAD_SPD; Serial3.print("*C" + String(speed_trg) + "*");break;
    
  }
}



void startMovementBT() {
  switch (b) {
    case '#': stopMovement(); break;
    case '9': startMovement(make_circle); break;
    case ':': startMovement(scared_round); break;
    case ';': startMovement(dontwonna); break;
    case '<': startMovement(scared_behind); break;
    case '=': startMovement(make_happy0); break;
    case '>': startMovement(make_happy1); break;
    case '?': startMovement(make_happy2); break;
    case '@': startMovement(make_happy3); break;
    case 'A': startMovement(make_sad0); break;
    case 'B': startMovement(make_sad1); break;
    case 'C': startMovement(scared_hit); break;
    case 'D': startMovement(make_sad2); break;
    case 'E': startMovement(angrymov); break;
    case 'F': stopMovement(); startMovement(follow); actual_obstacle = none; last_obstacle = none; break;
    case 'G': stopMovement(); startMovement(autonomous_movement); break;
    case 'H': stopMovement(); startMovement(autonomous_capa); break; //START DI MOSCACIECA
    case 'I': stopMovement(); startMovement(brokeIce); break;
    case 'J': stopMovement(); startMovement(dance_mov); break;
    case 'K': stopMovement(); startMovement(colorGame); break;
  }
}


void playAudio() {
  //    switch (b) {
  //      case 'b': playS(9); break;
  //      case 'c': playS(10); break;
  //      case 'd': playS(11); break;
  //      case 'e': playS(12); break;
  //      case 'f': playS(13); break;
  //      case 'g': playS(14); break;
  //      case 'h': playS(15); break;
  //      case 'i': playS(16); break;
  //    }
  if (b == '%') {
    playS(Serial.parseInt());
  }
}
//  Serial3.println("7-TRIANGOLO");
//        if (!triangolo) {
//          startMovementBT();
//          triangolo = true;
//          //startMovement(make_happy[2]);
//
//          //CODICE PER ATTIVARE FOLLOWING
//          //startMovement(follow);
//          //actual_obstacle=none;
//          //last_obstacle=none;
//
//          //CODICE PER ATTIVARE AUTONOMOUS MOVEMENT
//          btMov = true; //mi segno che il comando è stato dato da bluetooth, quindi la fotoresistenza non deve intervenire.
//          if (actual_movement == no_movement || actual_movement == follow || actual_movement == idle) {
//            Serial3.println("start Autonomous Movement");
//            startMovement(autonomous_movement);
//
//          } else {
//            Serial3.println("start Following");
//            stopMovement();
//            startMovement(follow);
//            actual_obstacle = none;
//            last_obstacle = none;
//          }
//
//        }
//}
//
//        Serial3.println("8-QUADRATO");
//        if (!quadrato) {
//
//          quadrato = true;
//          startMovement(next_movement);
//          if (next_movement < make_sad2)
//            next_movement++;
//          else next_movement = make_circle;
//        }

//
//      case '9':
//        Serial3.println("9-X");
//        if (!croce) {
//
//          croce = true;
//          startMovement(make_happy2);
//          //startMovement(makeCircle);
//          //startMovement(angrymov);
//        }
//        break;
//      case 'A':
//        Serial3.println("A-CERCHIO");
//        if (!cerchio) {
//          cerchio = true;
//
//          //startMovement(make_happy[3]);
//          //CODICE PER DISATTIVARE FOLLOWING E AUTONOMOUS MOVEMENT
//          btMov = false;
//          stopAutFollow();
//
//          //CODICE PER DISATTIVARE AUTONOMOUS MOVEMENT
//          /* autonomous_movement=false;
//            move=false;
//            aut=false;*/
//        }
//        break;
//      default:
//        // default code (should never run)
//        break;

void sendState() {//send data from arduino to App
  if (millis() - lastSendState > SENDSTATETIME) {
    Serial3.print("*A" + String(interpreterState) + "*");
    lastSendState = millis();
  }
  prev_posX = actual_posX;
  actual_posX = triskar.getPosX();
  prev_posY = actual_posY;
  actual_posY = triskar.getPosY();
  prev_posTh = actual_posTh;
  actual_posTh = triskar.getPosTh();
  if (actual_posX != prev_posX) {
    Serial3.print("*X" + String(actual_posX) + "*");
  }
  if (actual_posY != prev_posY) {
    Serial3.print("*Y" + String(actual_posY) + "*");
  }
  if (actual_posTh != prev_posTh) {
    Serial3.print("*T" + String(actual_posTh) + "*");
  }
  if (millis() - lastBatteryUpdate > BATTERY_UPDATE_TIME) {
    battery_indicator = constrain(mapfloat(voltage, MIN_INDICATOR_VOLTAGE, MAX_INDICATOR_VOLTAGE, MIN_INDICATOR_VALUE, MAX_INDICATOR_VALUE), MIN_INDICATOR_VALUE, MAX_INDICATOR_VALUE);
    Serial3.print("*B" + String(battery_indicator) + "*");
    lastBatteryUpdate = millis();
  }
  Serial3.print("*C" + String(speed_trg) + "*");
  Serial3.print("*Q" + String(triskar.getSpeedX()) + "*");
  Serial3.print("*N" + String(actual_movement) + "*");
  Serial3.print("*O" + String(prev_movement) + "*");
  Serial3.print("*D" + String(f_front) + "*");
  Serial3.print("*L" + String(f_left) + "*");
  Serial3.print("*R" + String(f_right) + "*");
  Serial3.print("*S" + String(f_back) + "*");
  Serial3.print("*F" + String(fotores_value) + "*");
  Serial3.print("*M" + String(micro_f) + "*");
}


void chooseGame() {
  if (Serial3.available()) {
    b = Serial3.read();
    settings();
    switch (b) {
      case '0':
        interpreterState = choose_scenario;
        currentGameI = 0;
        playS(firstGameAudioNumber + currentGameI);
        Serial3.print("Selezionato Gioco: "); Serial3.println(currentGameI);
        Serial3.println("Selezionare Scenario");
        //        }
        break;
      case '1':
        interpreterState = choose_scenario;
        currentGameI = 1;
        playS(firstGameAudioNumber + currentGameI);
        //        }
        break;

      case '2':

        break;
      case '3':

        break;
      case '8':
        interpreterState = fam_modality;
        CapacitivesUpdate(body);
        movementFinishTime = millis();
        break;
      case '.':
        interpreterState = choose_modality;
        CapacitivesUpdate(noOne);
        break;

    }

  }
  sendState();
}

void chooseScenario() {
  if (Serial3.available()) {
    b = Serial3.read();
    switch (b) {
      case '0':
        interpreterState = sg_waiting;
        currentScenarioI = 0;
        playS(firstScenarioAudioNumber + currentScenarioI);
        Serial3.print("Selezionato Scenario:  "); Serial3.println(currentScenarioI);
        Serial3.println("Premere '1' per iniziare Gioco");
        //        }
        break;
      case '1':
        interpreterState = sg_waiting;
        currentScenarioI = 1;
        playS(firstScenarioAudioNumber + currentGameI);
        //        }
        break;

      case '2':
        //        Serial3.println("9-X");
        //        if (!croce) {
        //          croce = true;
        interpreterState = sg_waiting;
        //        }
        break;
      case '3':
        //        Serial3.println("A-CERCHIO");
        //        if (!cerchio) {
        //          cerchio = true;
        interpreterState = sg_waiting;
        //        }
        break;
      case '7': interpreterState = choose_game; break;
      default:
        // default code (should never run)
        break;
    }
  }
  sendState();
}

void sgWaiting() {
  if (Serial3.available()) {
    b = Serial3.read();
    settings();
    switch (b) {
      //      case '0':
      //        resetButtons();
      //        break;

      case '1':
        //        Serial3.println("9-X");
        //        if (!croce) {
        //          croce = true;
        interpreterState = game_modality;
        gameState = setting;
        CapacitivesUpdate(head);
        //        }
        break;

      default:
        // default code (should never run)
        break;
    }

  }
  sendState();
}

void gameMod() {
  if (Serial3.available()) {
    b = Serial3.read();
    settings();
    switch (b) {
      case '.':
        interpreterState = choose_modality;
        break;
    }

  }
  sendState();
}

void disCharge() {
  sendState();
}

//void resetButtons() {
//  triangolo = false;
//  quadrato = false;
//  cerchio = false;
//  croce = false;
//  startbutton = false;
//  select = false;
//}
//




