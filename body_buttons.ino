void bodyCapacitiveSetup() {
  bodySensor[0] = new CapacitiveSensor(CAPACITIVE_COMMONPIN_BODY, BODY_SX_PIN); //sx
  bodySensor[1] = new CapacitiveSensor(CAPACITIVE_COMMONPIN_BODY, BODY_DX_PIN); //dx
  bodySensor[2] = new CapacitiveSensor(CAPACITIVE_COMMONPIN_BODY, BODY_BEHIND_PIN); //behind
  bodySensor[3] = new CapacitiveSensor(CAPACITIVE_COMMONPIN_BODY, BODY_FRONT_PIN); //front
  for (int i = 0; i < N_SENSORS; i++) {
    bodySensor[i]->set_CS_AutocaL_Millis(SCALIBRATION_TIMER);
    capacitiveState[i] = no_touch;
  }
}

void bodyCapacitiveLoop() {

  updateCapacitiveFlags();
  reactions();
}

unsigned long int startTimeBody=0;
boolean overTheLimitBody=false;

void updateCapacitiveFlags() {
  // catturo i valori di output di ogni sensore capacitivo
  for (int i = 0; i < N_SENSORS; i++) {
    bodySensorValue[i] = bodySensor[i]->capacitiveSensorRaw(2);
    if (bodySensorValue[i] >= SCALIBRATION_VALUE && !overTheLimitBody) {
      startTimeBody = millis();
      overTheLimitBody = true;
    }
    if (bodySensorValue[i] < SCALIBRATION_VALUE && overTheLimitBody)
      overTheLimitBody = false;
    if (overTheLimitBody && millis() - startTimeBody > SCALIBRATION_TIMER) {
      Serial.println("AUTOCALIBRAZIONE");
      bodySensor[i]->reset_CS_AutoCal();
      overTheLimitBody = false;
    }
    updateBodyState(i);
  }
}

void updateBodyState(int i) {
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

boolean go_backward_when_finish_rotation=false;
void reactions() {
  if ( (millis() - movementFinishTime > 2000) && (actual_movement == no_movement) && (gameState == no_game) && (interpreterState == fam_modality) && digitalRead(BUSY_PIN) == HIGH && triskar.isStopped()) {

    if (capacitiveState[3] == soft_touch && capacitiveState[0] == soft_touch && capacitiveState[1] == soft_touch) {
      //abbraccio completo
      setHeadLedPulse(green);
    }
    else if ((capacitiveState[3] == soft_touch && capacitiveState[0] == soft_touch) || (capacitiveState[3] == soft_touch &&  capacitiveState[1] == soft_touch)) {
      //abbraccio incompleto, stringi più forte, allarga l'abbraccio!
      setHeadLedWipe(green);
    }
    else if (capacitiveState[3] == soft_touch) {
      if (millis() - stateStartTime[3] < 1000)
        setHeadLedWipe(yellow);
      else
        setHeadLedWipe(green);
      //colpo o carezza? dammi un abbraccio! stringimi!
    }
    else if(capacitiveState[2] == strong_touch){
      startMovement(makeOnemF,red,color_pulse, 33); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
    }
    else if(capacitiveState[3] == strong_touch){
      startMovement(makeOnemB,red,color_pulse, 34);
    }
    else if(capacitiveState[0] == strong_touch){
      alpha=PI/2;
      startMovement(turnAlphaL,red,color_pulse, 35);
      go_backward_when_finish_rotation=true;  
    }
    else if(capacitiveState[1] == strong_touch){
      alpha=PI/2;
      startMovement(turnAlphaR,red,color_pulse, 35);
      go_backward_when_finish_rotation=true;  
    }

  }
  else if(go_backward_when_finish_rotation){
    startMovement(makeOnemB,red,color_pulse);
    go_backward_when_finish_rotation=false;
  }
}
