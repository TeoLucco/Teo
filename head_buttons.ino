void checkBT() {
  if (millis() - firstSoundTime >= WAIT_BT_CONN && !Serial3.available() && interpreterState == choose_modality) {
    CapacitivesUpdate(head);
    headInterpreter = true;
    //Serial3.println("ATTIVAZIONE CAPACITIVI TESTA");
    playS(17);
  } else if (Serial3.available()) {
    firstSoundTime = millis(); headInterpreter = false;
  }
}
unsigned long int lastPressedButtonTime = 0;
boolean colorByButton = false;
#define WIPE_DURATION 2000
void headCapacitiveLoop() {
  if (pressedButton != -1) {
    lastPressedButtonTime = millis();
    colorByButton = true;
    //if (interpreterState != game_modality) {
//      switch (pressedButton) {
//        case 0: headLedUpdate(red, color_wipe); break;
//        case 1: headLedUpdate(green, color_wipe); break;
//        case 2: headLedUpdate(blue, color_wipe); break;
//        case 3: headLedUpdate(yellow, color_wipe); break;
//      }
    //}
    if (buttonToTouch != -1) {
      if (pressedButton == buttonToTouch) {
        rightAnswer();
        buttonToTouch = -1;
      } else {
        wrongAnswer();
      }
      if (color_Game)color_Game = false;
      pressedButton = -1;
    }
  }
}

void headCapacitiveInterpreter() {
  if (headInterpreter && gameState != wait_answer) {
    switch (interpreterState) {
      case choose_modality: chooseModCap(); break;
      case choose_game:     chooseGameCap(); break;
      case sg_waiting:      startGameWaitCap(); break;
      case choose_scenario: chooseScenarioCap(); break;
      case game_modality:   break;
      case fam_modality:    break;
      case test_modality:   break;
      case discharge:       break;
    }
  } else checkBT();
}

void chooseModCap() {
  switch (pressedButton) {
    case -1: break;
    case 0: interpreterState = fam_modality; break;
    case 1: interpreterState = fam_modality; break;
    case 2: interpreterState = choose_game; break;
    case 3: interpreterState = choose_game; break;
  }
}
void chooseGameCap() {
  switch (pressedButton) {
    case -1: break;
    case 0:
      if (currentGameI > 0) currentGameI--;
      else currentGameI = 0;
      playS(firstGameAudioNumber + currentGameI);
      break;

    case 1:
      interpreterState = choose_scenario;
      playS(28); //scegli scenario
      break;

    case 2:

      if (currentGameI < N_GAMES) currentGameI++;
      else currentGameI = N_GAMES;
      playS(firstGameAudioNumber + currentGameI);
      break;

    case 3:
      interpreterState = choose_modality;
      break;
  }
}


void chooseScenarioCap() {
  switch (pressedButton) {
    case -1: break;
    case 0:
      if (currentScenarioI > 1) currentScenarioI--;
      else currentScenarioI = 1;
      playS(firstScenarioAudioNumber + currentScenarioI);
      break;

    case 1:
      interpreterState = sg_waiting;
      playS(28); //scegli scenario
      break;

    case 2:

      if (currentScenarioI < N_GAMES) currentScenarioI++;
      else currentScenarioI = N_GAMES;
      playS(firstScenarioAudioNumber + currentScenarioI);
      break;

    case 3:
      interpreterState = choose_game;
      break;
  }
}
void startGameWaitCap() {
  switch (pressedButton) {
    case -1:  break;
    case 0:   break;
    case 1:   break;
    case 2:   interpreterState = game_modality; break;
    case 3:   interpreterState = choose_game; break;
  }

}

