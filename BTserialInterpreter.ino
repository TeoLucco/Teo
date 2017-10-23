void resetButtons() {
  triangolo = false;
  quadrato = false;
  cerchio = false;
  croce = false;
  startbutton = false;
  select = false;
}

void FromRobotToApp() {
 
    //Serial3.write();
  
}

void famMod() {
  if (Serial3.available()) {
    b = Serial3.read();
    switch (b) {
      case '0':
        //Serial3.println("0");
//        if (noneB) {
//          noneB = false;
          resetButtons();
          if (actual_movement == no_movement && prev_movement != idle && prev_movement != follow && prev_movement != autonomous_movement) {
            triskar.stop2();
            move = false;
          } else if (actual_movement == no_movement && prev_movement == follow) {
            actual_movement = follow;
            prev_movement = no_movement;
          }
          else if (actual_movement == no_movement && prev_movement == autonomous_movement) {
            actual_movement = autonomous_movement;
            prev_movement = no_movement;
          }
          else if (actual_movement == no_movement && prev_movement == idle) {
            //startMovement(idle, rainbow_cycle);
            actual_movement = idle;
            prev_movement = no_movement;
          }
   //}     
        break;

      case '1':
        Serial3.println("1-SU");
        //noneB = true;
        if (!veryclose_front_obstacle && actual_movement != dontwonna) {
          if (actual_movement == idle) {
            prev_movement = idle;
            actual_movement = no_movement;
          }
          move = true;
          movementFinishTime = millis();
          triskar.run(speed_trg, 0.0);
        } else if (veryclose_front_obstacle && actual_movement != dontwonna)  startMovement(dontwonna);
        break;
      case '2':
        Serial3.println("2-GIU'");
        //noneB = true;
        if (!veryclose_back_obstacle && actual_movement != dontwonna) {
          if (actual_movement == idle) {
            prev_movement = idle;
            actual_movement = no_movement;
          }
          move = true;
          triskar.run(-speed_trg, 0.0);
          movementFinishTime = millis();
        } else if (veryclose_back_obstacle && actual_movement != dontwonna)  startMovement(dontwonna);
        break;

      case '3':
        Serial3.println("3-SINISTRA");
        //noneB = true;
        move = true;
        if (actual_movement == no_movement) {
          triskar.run(0.0, speed_trg / robot_radius);
          movementFinishTime = millis();
        } else if (actual_movement == autonomous_movement) {
          prev_movement = autonomous_movement;
          actual_movement = no_movement;
          movementFinishTime = millis();
          triskar.run(0.0, speed_trg / robot_radius);
        } else if (actual_movement == follow) {
          prev_movement = follow;
          actual_movement = no_movement;
          movementFinishTime = millis();
          triskar.run(0.0, speed_trg / robot_radius);
        } else if (actual_movement == idle) {
          prev_movement = idle;
          actual_movement = no_movement;
          movementFinishTime = millis();
          triskar.run(0.0, speed_trg / robot_radius);
        }

        break;

      case '4':
        Serial3.println("4-DESTRA");
        //noneB = true;
        move = true;
        if (actual_movement == no_movement) {
          triskar.run(0.0, -speed_trg / robot_radius);
          movementFinishTime = millis();
        } else if (actual_movement == autonomous_movement) {
          prev_movement = autonomous_movement;
          actual_movement = no_movement;
          movementFinishTime = millis();
          triskar.run(0.0, -speed_trg / robot_radius);
        } else if (actual_movement == follow) {
          prev_movement = follow;
          actual_movement = no_movement;
          movementFinishTime = millis();
          triskar.run(0.0, -speed_trg / robot_radius);
        } else if (actual_movement == idle) {
          prev_movement = idle;
          actual_movement = no_movement;
          movementFinishTime = millis();
          triskar.run(0.0, -speed_trg / robot_radius);
        }
        break;
      //Code when RIGHT key is pressed

      case '5':
        Serial3.println("5-SELECT");
        if (!select) {
          //noneB = true;
          select = true;
          speed_trg -= 3;
          if (speed_trg < 0)     speed_trg = 0;
          Serial3.print("Speed trg:  ");
          Serial3.println(speed_trg);
        }
        break;

      case '6':
        Serial3.println("6-START");
        if (!startbutton) {
          //noneB = true;
          startbutton = true;
          speed_trg += 3;
          if (speed_trg > 35)   speed_trg = 35;
          Serial3.print("Speed trg:  ");
          Serial3.println(speed_trg);
        }
        break;

      case '7':
        Serial3.println("7-TRIANGOLO");
        if (!triangolo) {
          //noneB = true;
          triangolo = true;
          //startMovement(make_happy[2]);

          //CODICE PER ATTIVARE FOLLOWING
          //startMovement(follow);
          //actual_obstacle=none;
          //last_obstacle=none;

          //CODICE PER ATTIVARE AUTONOMOUS MOVEMENT
          btMov = true; //mi segno che il comando è stato dato da bluetooth, quindi la fotoresistenza non deve intervenire.
          if (actual_movement == no_movement || actual_movement == follow || actual_movement == idle) {
            Serial3.println("start Autonomous Movement");
            startMovement(autonomous_movement);

          } else {
            Serial3.println("start Following");
            stopMovement();
            startMovement(follow);
            actual_obstacle = none;
            last_obstacle = none;
          }

        }
        break;
      case '8':
        Serial3.println("8-QUADRATO");
        if (!quadrato) {
          //noneB = true;
          quadrato = true;
          startMovement(next_movement);
          if (next_movement < make_sad2)
            next_movement++;
          else next_movement = make_circle;
        }
        break;

      case '9':
        Serial3.println("9-X");
        if (!croce) {
          //noneB = true;
          croce = true;
          startMovement(make_happy2);
          //startMovement(makeCircle);
          //startMovement(angrymov);
        }
        break;
      case 'A':
        Serial3.println("A-CERCHIO");
        if (!cerchio) {
          cerchio = true;
          //noneB = true;
          //startMovement(make_happy[3]);
          //CODICE PER DISATTIVARE FOLLOWING E AUTONOMOUS MOVEMENT
          btMov = false;
          stopAutFollow();

          //CODICE PER DISATTIVARE AUTONOMOUS MOVEMENT
          /* autonomous_movement=false;
            move=false;
            aut=false;*/
        }
        break;
      default:
        // default code (should never run)
        break;
    }

  }
  FromRobotToApp();
}
void chooseModality() {
  if (Serial3.available()) {
    b = Serial3.read();
    switch (b) {
      case '0':
        resetButtons();
        break;

      case '7':
        Serial3.println("7-TRIANGOLO");
        if (!triangolo) {
          triangolo = true;
          interpreterState = choose_game;
          playS(02);
        }
        break;
      case '8':
        Serial3.println("8-QUADRATO");
        if (!quadrato) {
          quadrato = true;
          interpreterState = fam_modality;
          previousWorkingCapacitives=workingCapacitives;
          workingCapacitives=body;
          movementFinishTime = millis();
        }
        break;
      case 'A':
        Serial3.println("A-CERCHIO");
        if (!cerchio) {
          cerchio = true;
          previousWorkingCapacitives=workingCapacitives;
          workingCapacitives=noOne;
          interpreterState = test_modality;
        }
        break;

      default:
        break;
    }

  }
  FromRobotToApp();
}
void chooseGame() {
  if (Serial3.available()) {
    b = Serial3.read();
    switch (b) {
      case '0':
        resetButtons();
        break;

      case '7':
        Serial3.println("7-TRIANGOLO");
        if (!triangolo) {
          triangolo = true;
          interpreterState = sg_waiting;
          gameNumber = colorGame;
          playS(03);
        }
        break;
      case '8':
        Serial3.println("8-QUADRATO");
        if (!quadrato) {
          quadrato = true;
          interpreterState = sg_waiting;
          gameNumber = animalGame;
          playS(03);
        }
        break;

      case '9':
        Serial3.println("9-X");
        if (!croce) {
          croce = true;
          interpreterState = sg_waiting;
        }
        break;
      case 'A':
        Serial3.println("A-CERCHIO");
        if (!cerchio) {
          cerchio = true;
          interpreterState = sg_waiting;
        }
        break;
      default:
        // default code (should never run)
        break;
    }

  }
  FromRobotToApp();
}

void sgWaiting() {
  if (Serial3.available()) {
    b = Serial3.read();
    switch (b) {
      case '0':
        resetButtons();
        break;

      case '9':
        Serial3.println("9-X");
        if (!croce) {
          croce = true;
          interpreterState = game_modality;
          gameState = setting;
          previousWorkingCapacitives=workingCapacitives;
          workingCapacitives=head;
        }
        break;

      default:
        // default code (should never run)
        break;
    }

  }
  FromRobotToApp();
}

void gameMod() {
  if (Serial3.available()) {
    b = Serial3.read();
    switch (b) {
      case '0':
        resetButtons();
        break;

      default:
        // default code (should never run)
        break;
    }

  }
  FromRobotToApp();
}

void disCharge() {
  FromRobotToApp();
}

void btInterpreter() {
  switch (interpreterState) {
    case choose_modality: chooseModality(); break;
    case choose_game:     chooseGame(); break;
    case sg_waiting:      sgWaiting(); break;
    case game_modality:   gameMod(); break;
    case fam_modality:    famMod(); break;
    case discharge:       disCharge(); break;
    case test_modality:   hardwareTests(); break;
  }
}


