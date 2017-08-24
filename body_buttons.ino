void bodyCapacitiveSetup() {
  bodySensor[0] = new CapacitiveSensor(CAPACITIVE_COMMONPIN, BODY_SX_PIN); //sx
  bodySensor[1] = new CapacitiveSensor(CAPACITIVE_COMMONPIN, BODY_DX_PIN); //dx
  bodySensor[2] = new CapacitiveSensor(CAPACITIVE_COMMONPIN, BODY_BEHIND_PIN); //behind
  bodySensor[3] = new CapacitiveSensor(CAPACITIVE_COMMONPIN, BODY_FRONT_PIN); //front
  for (int i = 0; i < N_SENSORS; i++) {
    bodySensor[i]->set_CS_AutocaL_Millis(SCALIBRATION_TIMER);
    capacitiveState[i] = no_touch;
  }
}

void bodyCapacitiveLoop() {

  updateCapacitiveFlags();
  reactions();
}

void updateCapacitiveFlags() {
  // catturo i valori di output di ogni sensore capacitivo
  for (int i = 0; i < N_SENSORS; i++) {
    bodySensorValue[i] = bodySensor[i]->capacitiveSensor(30);
    if (i == 2) updateBehindState(i);
    else updateNoBehindState(i);
  }
}

void updateNoBehindState(int i) {
  if (bodySensorValue[i] < lowBodyThreshold) {
    if (capacitiveState[i] != no_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = no_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
  else if (bodySensorValue[i] >= lowBodyThreshold && bodySensorValue[i] < middleBodyThreshold) {
    if (capacitiveState[i] != soft_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = soft_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
  else if (bodySensorValue[i] >= middleBodyThreshold && bodySensorValue[i] < highBodyThreshold) {
    if (capacitiveState[i] != middle_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = middle_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
  else if (bodySensorValue[i] >= highBodyThreshold) {
    if (capacitiveState[i] != strong_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = strong_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
}

void updateBehindState(int i) {
  if (bodySensorValue[i] < lowBehindBodyThreshold) {
    if (capacitiveState[i] != no_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = no_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
  else if (bodySensorValue[i] >= lowBehindBodyThreshold && bodySensorValue[i] < middleBehindBodyThreshold) {
    if (capacitiveState[i] != soft_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = soft_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
  else if (bodySensorValue[i] >= middleBehindBodyThreshold && bodySensorValue[i] < highBehindBodyThreshold) {
    if (capacitiveState[i] != middle_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = middle_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
  else if (bodySensorValue[i] >= highBehindBodyThreshold) {
    if (capacitiveState[i] != strong_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = strong_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
}

void reactions() {
  if ( (millis() - movementFinishTime > 2000) && (actual_movement == no_movement) && (gameState == no_game) && (interpreterState == fam_modality) && digitalRead(BUSY_PIN) == HIGH && triskar.isStopped()) {

    if (capacitiveState[3] == soft_touch && capacitiveState[0] == soft_touch && capacitiveState[1] == soft_touch) {
      //abbraccio completo
      setHeadLedPulse(green);
    }
    else if ((capacitiveState[3] == soft_touch && capacitiveState[0] == soft_touch)||(capacitiveState[3] == soft_touch &&  capacitiveState[1] == soft_touch)) {
      //abbraccio incompleto, stringi più forte, allarga l'abbraccio!
      setHeadLedWipe(green);
    }
    else if (capacitiveState[3] == soft_touch) {
      if(millis()-stateStartTime[3]<1000)
      setHeadLedWipe(yellow);
      else
      setHeadLedWipe(green);
      //colpo o carezza? dammi un abbraccio! stringimi!
    }

  }
}
