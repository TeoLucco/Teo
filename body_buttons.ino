void bodyCapacitiveSetup() {
  bodySensor[0] = new CapacitiveSensor(BODY_SX_S, BODY_SX_R); //sx
  bodySensor[1] = new CapacitiveSensor(BODY_DX_S, BODY_DX_R); //dx
  bodySensor[2] = new CapacitiveSensor(BODY_FRONT_S, BODY_FRONT_R); //front
  for (int i = 0; i < N_BODY_SENSORS; i++) {
    bodySensor[i]->set_CS_AutocaL_Millis(SCALIBRATION_TIMER);
    capacitiveState[i] = no_touch;
  }
}

void bodyCapacitiveLoop() {
  if ((interpreterState == fam_modality || (testState == test_exe && testType==body_capacitives_t))) { //&& millis()-lastCapacitiveLoopTime>CAPACITIVE_LOOP_TIME
  updateCapacitiveFlags();
  if(!(testState == test_exe && testType==body_capacitives_t))
  reactions();
  lastCapacitiveLoopTime = millis();
  }
}

unsigned long int startTimeBody[N_BODY_SENSORS] = {0, 0, 0};
boolean overTheLimitBody[N_BODY_SENSORS] = {false, false, false};

void updateCapacitiveFlags() {
  pats = 0;
  hits = 0;
  // catturo i valori di output di ogni sensore capacitivo
  for (int i = 0; i < N_BODY_SENSORS; i++) {
    bodySensorValue_nf[i] = bodySensor[i]->capacitiveSensorRaw(10);
  }
  left_body_f.input(bodySensorValue_nf[0]);
  right_body_f.input(bodySensorValue_nf[1]);
  front_body_f.input(bodySensorValue_nf[2]);
  bodySensorValue[0]=left_body_f.output();
  bodySensorValue[1]=right_body_f.output();
  bodySensorValue[2]=front_body_f.output();
  updateBodyState(0);
  updateBodyState(1);
  updateBodyState(2);
  for (int i = 0; i < N_BODY_SENSORS; i++) {
    pats += pat[i];
    hits += hit[i];
  }
}
void resetCapacitive() {
  for (int i = 0; i < N_BODY_SENSORS; i++) {
    bodySensor[i]->reset_CS_AutoCal();
  }
}

void updateBodyState(int i) {
  previousDynamicCapacitiveState[i] = capacitiveState[i];
  if (bodySensorValue[i] < lowBodyThreshold[i]) {
    if (capacitiveState[i] != no_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = no_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
  else if (bodySensorValue[i] >= lowBodyThreshold[i] && bodySensorValue[i] < highBodyThreshold[i]) {
    if (capacitiveState[i] != soft_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = soft_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }

  }
  //  else if (bodySensorValue[i] >= middleBodyThreshold && bodySensorValue[i] < highBodyThreshold) {
  //    if (capacitiveState[i] != middle_touch) {
  //      previousCapacitiveState[i] = capacitiveState[i];
  //      capacitiveState[i] = middle_touch;
  //      previousStateStartTime[i] = stateStartTime[i];
  //      stateStartTime[i] = millis();
  //    }
  //  }
  else if (bodySensorValue[i] >= highBodyThreshold[i]) {
    if (capacitiveState[i] != strong_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = strong_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }

  }
}

void reactions() {
  if (actual_movement == no_movement || actual_movement == idle) {
    switch (touchState) {
      case nothing: waitTouch(); break;
      case hug:     checkHug(); break;
      case pat0:    checkPat0(); break;
      case pat1:    checkPat1(); break;
      case pat2:    checkPat2(); break;
      case hit0:    checkHit0(); break;
      case hit1:    checkHit1(); break;
      case hit2:    checkHit2(); break;
    }
  }
}

void waitTouch() {
  resetLedTimer();
  resetHitCountTimer();
  resetPatCountTimer();
  hugsCount = 0;
  if ((bodySensorValue[2] >= lowBodyThreshold[2] && bodySensorValue[1] < lowBodyThreshold[1] && bodySensorValue[0] >= lowBodyThreshold[0]) ||
      (bodySensorValue[2] >= lowBodyThreshold[2] && bodySensorValue[1] >= lowBodyThreshold[1] && bodySensorValue[0] <= lowBodyThreshold[0]) ||
      (bodySensorValue[2] >= lowBodyThreshold[2] && bodySensorValue[1] >= lowBodyThreshold[1] && bodySensorValue[0] >= lowBodyThreshold[0])) {
    touchState = hug;
  }
  else if (capacitiveState[0] == soft_touch && capacitiveState[1] == no_touch && capacitiveState[2] == no_touch) {
    touchState = pat0;
  }
  else if (capacitiveState[0] == no_touch && capacitiveState[1] == soft_touch && capacitiveState[2] == no_touch) {
    touchState = pat1;
  }
  else if (capacitiveState[0] == no_touch && capacitiveState[1] == no_touch && capacitiveState[2] == soft_touch) {
    touchState = pat2;
  }
  else if (capacitiveState[0] == strong_touch && capacitiveState[1] == no_touch && capacitiveState[2] == no_touch) {
    touchState = hit0;
  }
  else if (capacitiveState[1] == strong_touch && capacitiveState[0] == no_touch && capacitiveState[2] == no_touch) {
    touchState = hit1;
  }
  else if (capacitiveState[2] == strong_touch && capacitiveState[1] == no_touch && capacitiveState[0] == no_touch) {
    touchState = hit2;
  }
}

void resetPatCountTimer() {
  for (int i = 0; i < N_BODY_SENSORS; i++) {
    if (millis() - stateStartTime[i] > RESET_PAT_TIME && capacitiveState[i] == no_touch) {
      pat[i] = 0;
    }
  }
}

void resetHitCountTimer() {
  for (int i = 0; i < N_BODY_SENSORS; i++) {
    if (millis() - stateStartTime[i] > RESET_HIT_TIME && capacitiveState[i] == no_touch) {
      hit[i] = 0;
    }
  }
}
void resetLedTimer() {
  if (millis() - stateStartTime[0] > RESET_PAT_TIME && capacitiveState[0] == no_touch &&
      millis() - stateStartTime[1] > RESET_PAT_TIME && capacitiveState[1] == no_touch &&
      millis() - stateStartTime[2] > RESET_PAT_TIME && capacitiveState[2] == no_touch && led_state != rainbow_cycle) {
    head_strip.setBrightness(255);
    headLedUpdate(rainbow_cycle);
  }
}
void checkHug() {
  if ((millis() - stateStartTime[2] >= HUGTIME && bodySensorValue[2] >= lowBodyThreshold[2]) &&
      ((millis() - stateStartTime[0] >= HUGTIME && bodySensorValue[0] >= lowBodyThreshold[0]) ||
       (millis() - stateStartTime[1] >= HUGTIME && bodySensorValue[1] >= lowBodyThreshold[1]) )) {
    playS(23);
    headLedUpdate(green, color_pulse);
    for (int j = 0; j < N_BODY_SENSORS; j++) {
      pat[j] = 0;
      hit[j] = 0;
      stateStartTime[j] = millis();
    }

    hugsCount++;
    //abbraccio = true;
    if (hugsCount == 1) {
      Serial3.println("ABBRACCIO!");

    } else {
      Serial3.print("ABBRACCIO durata "); Serial3.println(hugsCount);
    }
  }
  if (capacitiveState[2] == no_touch && capacitiveState[1] == no_touch && capacitiveState[0] == no_touch)
    touchState = nothing;
}
void checkPat0() {
  if (previousDynamicCapacitiveState[0] == soft_touch && previousDynamicCapacitiveState[1] == no_touch && previousDynamicCapacitiveState[2] == no_touch &&
      capacitiveState[0] == no_touch && capacitiveState[1] == no_touch && capacitiveState[2] == no_touch &&
      stateStartTime[0] - previousStateStartTime[0] >= MIN_PAT_TIME && stateStartTime[0] - previousStateStartTime[0] <= MAX_PAT_TIME) {
    touchState = nothing;
    for (int j = 0; j < N_BODY_SENSORS; j++) hit[j] = 0;
    if (pat[0] == N_PATS) {
      for (int j = 0; j < N_BODY_SENSORS; j++) pat[j] = 0;
      playS(23);
      headLedUpdate(green, color_wipe);
      Serial3.println("N CAREZZE!");
      resetCapacitive();
    } else {
      pat[0]++;
      playS(23);
      headLedUpdate(green, color_pulse);
      Serial3.println("UNA CAREZZA!");
      resetCapacitive();
    }
  }
  else if (capacitiveState[0] == strong_touch && capacitiveState[1] == no_touch && capacitiveState[2] == no_touch) {
    touchState = hit0;
  } else patHitStatusExitCond();
}
void checkPat1() {
  if (previousDynamicCapacitiveState[1] == soft_touch && previousDynamicCapacitiveState[0] == no_touch && previousDynamicCapacitiveState[2] == no_touch &&
      capacitiveState[1] == no_touch && capacitiveState[0] == no_touch && capacitiveState[2] == no_touch &&
      stateStartTime[1] - previousStateStartTime[1] >= MIN_PAT_TIME && stateStartTime[1] - previousStateStartTime[1] <= MAX_PAT_TIME) {
    touchState = nothing;
    for (int j = 0; j < N_BODY_SENSORS; j++) hit[j] = 0;
    if (pat[1] == N_PATS) {
      for (int j = 0; j < N_BODY_SENSORS; j++) pat[j] = 0;
      playS(23);
      headLedUpdate(green, color_wipe);
      Serial3.println("N CAREZZE!");
      resetCapacitive();
    } else {
      pat[1]++;
      playS(23);
      headLedUpdate(green, color_pulse);
      Serial3.println("UNA CAREZZA!");
      resetCapacitive();
    }
  }
  else if (capacitiveState[1] == strong_touch && capacitiveState[0] == no_touch && capacitiveState[2] == no_touch) {
    touchState = hit1;
  } else patHitStatusExitCond();
}
void checkPat2() {
  if (previousDynamicCapacitiveState[2] == soft_touch && previousDynamicCapacitiveState[0] == no_touch && previousDynamicCapacitiveState[1] == no_touch &&
      capacitiveState[2] == no_touch && capacitiveState[0] == no_touch && capacitiveState[1] == no_touch &&
      stateStartTime[2] - previousStateStartTime[2] >= MIN_PAT_TIME && stateStartTime[2] - previousStateStartTime[2] <= MAX_PAT_TIME) {
    touchState = nothing;
    for (int j = 0; j < N_BODY_SENSORS; j++) hit[j] = 0;
    if (pat[2] == N_PATS) {
      for (int j = 0; j < N_BODY_SENSORS; j++) pat[j] = 0;
      playS(23);
      headLedUpdate(green, color_wipe);
      Serial3.println("N CAREZZE!");
      resetCapacitive();
    } else {
      pat[2]++;
      playS(23);
      headLedUpdate(green, color_pulse);
      Serial3.println("UNA CAREZZA!");
      resetCapacitive();
    }
  }
  else if (capacitiveState[2] == strong_touch && capacitiveState[1] == no_touch && capacitiveState[0] == no_touch) {
    touchState = hit2;
  } else patHitStatusExitCond();
}

void checkHit0() {
  if ((previousDynamicCapacitiveState[0] == strong_touch || previousDynamicCapacitiveState[0] == soft_touch) && previousDynamicCapacitiveState[1] == no_touch && previousDynamicCapacitiveState[2] == no_touch &&
      capacitiveState[0] == no_touch && capacitiveState[1] == no_touch && capacitiveState[2] == no_touch &&
      stateStartTime[0] - previousStateStartTime[0] >= MIN_HIT_TIME && stateStartTime[0] - previousStateStartTime[0] <= MAX_HIT_TIME) {
    touchState = nothing;
    for (int j = 0; j < N_BODY_SENSORS; j++) pat[j] = 0;
    if (hit[0] == N_HITS) {
      for (int j = 0; j < N_BODY_SENSORS; j++) hit[j] = 0;
      startMovement(make_sad2L, red, color_pulse, 33); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      Serial3.println("N COLPI A SX!");
      resetCapacitive();
    } else {
      hit[0]++;
      startMovement(scared_hitL, red, color_pulse, 34); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      Serial3.println("UN COLPO A SX!");
      resetCapacitive();
    }
  } else patHitStatusExitCond();
}

void checkHit1() {
  if ((previousDynamicCapacitiveState[1] == strong_touch || previousDynamicCapacitiveState[1] == soft_touch) && previousDynamicCapacitiveState[0] == no_touch && previousDynamicCapacitiveState[2] == no_touch &&
      capacitiveState[0] == no_touch && capacitiveState[1] == no_touch && capacitiveState[2] == no_touch &&
      stateStartTime[1] - previousStateStartTime[1] >= MIN_HIT_TIME && stateStartTime[1] - previousStateStartTime[1] <= MAX_HIT_TIME) {
    touchState = nothing;
    for (int j = 0; j < N_BODY_SENSORS; j++) pat[j] = 0;
    if (hit[1] == N_HITS) {
      for (int j = 0; j < N_BODY_SENSORS; j++) hit[j] = 0;
      startMovement(make_sad2R, red, color_pulse, 33); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      Serial3.println("N COLPI A DX!");
      resetCapacitive();
    } else {
      hit[1]++;
      startMovement(scared_hitR, red, color_pulse, 34); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      Serial3.println("UN COLPO A DX!");
      resetCapacitive();
    }
  } else patHitStatusExitCond();
}
void checkHit2() {
  if ((previousDynamicCapacitiveState[2] == strong_touch || previousDynamicCapacitiveState[2] == soft_touch) && previousDynamicCapacitiveState[0] == no_touch && previousDynamicCapacitiveState[1] == no_touch &&
      capacitiveState[0] == no_touch && capacitiveState[1] == no_touch && capacitiveState[2] == no_touch &&
      stateStartTime[2] - previousStateStartTime[2] >= MIN_HIT_TIME && stateStartTime[2] - previousStateStartTime[2] <= MAX_HIT_TIME) {
    touchState = nothing;
    for (int j = 0; j < N_BODY_SENSORS; j++) pat[j] = 0;
    if (hit[2] == N_HITS) {
      for (int j = 0; j < N_BODY_SENSORS; j++) hit[j] = 0;
      startMovement(make_sad2, red, color_pulse, 33); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      Serial3.println("N COLPI AL CENTRO!");
      resetCapacitive();
    } else {
      hit[2]++;
      startMovement(scared_hit, red, color_pulse, 34); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      Serial3.println("UN COLPO AL CENTRO!");
      resetCapacitive();
    }
  } else
    patHitStatusExitCond();

}

void patHitStatusExitCond() {
  if ((bodySensorValue[2] >= lowBodyThreshold[2] && bodySensorValue[1] < lowBodyThreshold[1] && bodySensorValue[0] >= lowBodyThreshold[0]) ||
      (bodySensorValue[2] >= lowBodyThreshold[2] && bodySensorValue[1] >= lowBodyThreshold[1] && bodySensorValue[0] <= lowBodyThreshold[0]) ||
      (bodySensorValue[2] >= lowBodyThreshold[2] && bodySensorValue[1] >= lowBodyThreshold[1] && bodySensorValue[0] >= lowBodyThreshold[0])) {
    touchState = hug;
  } else if (capacitiveState[2] == no_touch && capacitiveState[1] == no_touch && capacitiveState[0] == no_touch)
    touchState = nothing;

}


