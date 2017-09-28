void headCapacitiveSetup() {
  headSensor[0] = new CapacitiveSensor(HEAD_BUTTON_0S, HEAD_BUTTON_0R);
  headSensor[1] = new CapacitiveSensor(HEAD_BUTTON_1S, HEAD_BUTTON_1R);
  headSensor[2] = new CapacitiveSensor(HEAD_BUTTON_2S, HEAD_BUTTON_2R);
  headSensor[3] = new CapacitiveSensor(HEAD_BUTTON_3S, HEAD_BUTTON_3R);
  for (int i = 0; i < N_HEAD_SENSORS; i++)                          //TODO cambiare 2 con N_SENSORS
    headSensor[i]->set_CS_AutocaL_Millis(SCALIBRATION_TIMER);
}

unsigned long int startTime = 0;
boolean overTheLimit = false;

int FindMax(long ARRAY[], byte START, byte END)
{
  int MAXIMUM, LOCATION;

  MAXIMUM = ARRAY[START];
  LOCATION = START;
  for (byte i = START + 1; i < END; i++)
  {
    if (ARRAY[i] > MAXIMUM)
    {
      MAXIMUM = ARRAY[i];
      LOCATION = i;
    }
  }

  return LOCATION;
}
void checkBT() {
  if (millis() - firstSoundTime >= WAIT_BT_CONN && !Serial3.available()) {
    capacitive_commands = true;
    playS(17);
  } else if (Serial3.available()) firstSoundTime = millis();
}
void headCapacitiveLoop() {
  boolean checkMax = false;
  checkBT();
  if (gameState == wait_answer || capacitive_commands) {
    for (int i = 0; i < N_HEAD_SENSORS; i++) {                          //TODO cambiare 2 con N_SENSORS
      headSensorValue[i] = headSensor[i]->capacitiveSensor(5);
      if (headSensorValue[i] > headThreshold && checkMax == false)
        checkMax = true;
      if (headSensorValue[i] >= SCALIBRATION_VALUE && !overTheLimit) {
        startTime = millis();
        overTheLimit = true;
      }
      if (headSensorValue[i] < SCALIBRATION_VALUE && overTheLimit)
        overTheLimit = false;
      if (overTheLimit && millis() - startTime > SCALIBRATION_TIMER) {
        Serial.println("AUTOCALIBRAZIONE");
        headSensor[i]->reset_CS_AutoCal();
        overTheLimit = false;
      }
    }
    if (checkMax) pressedButton = FindMax(headSensorValue, 0, N_HEAD_SENSORS);
    else pressedButton = -1;
    checkMax = false;
    headCapacitiveInterpreter();
  }

}

void headCapacitiveInterpreter() {
  if (capacitive_commands && gameState != wait_answer) {
    switch (interpreterState) {
      case choose_modality: chooseModCap(); break;
      case choose_game:     chooseGameCap(); break;
      case sg_waiting:      startGameWaitCap(); break;
      case game_modality:   break;
      case fam_modality:    break;
      case test_modality:   break;
      case discharge:       break;
    }
  }
}

void chooseModCap() {
  switch (pressedButton) {
    case -1: break;
    case 0: interpreterState = choose_game; break;
    case 1: interpreterState = fam_modality; break;
    case 2: interpreterState = test_modality; break;
    case 3: break;
  }
}
void chooseGameCap() {
  switch (pressedButton) {
    case -1: break;
    case 0:
      if (gameNumber > 1) gameNumber--;
      else gameNumber = 1;
      playS(19 + gameNumber);
      break;

    case 1:
      if (gameNumber < N_GAMES) gameNumber++;
      else gameNumber = N_GAMES;
      playS(19 + gameNumber);
      break;

    case 2:
      interpreterState = sg_waiting;
      playS(24 + gameNumber); //come posizionare patch
      break;

    case 3:
      interpreterState = choose_modality;
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

