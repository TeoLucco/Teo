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
  if (interpreterState == fam_modality && triskar.isStopped()) {
    updateCapacitiveFlags();
    reactions();
  }
}

unsigned long int startTimeBody[N_BODY_SENSORS] = {0, 0, 0};
boolean overTheLimitBody[N_BODY_SENSORS] = {false, false, false};

void updateCapacitiveFlags() {
  pats=0;
  hits=0;
  // catturo i valori di output di ogni sensore capacitivo
  for (int i = 0; i < N_BODY_SENSORS; i++) {
    bodySensorValue[i] = bodySensor[i]->capacitiveSensor(10);
    //    if (bodySensorValue[i] >= SCALIBRATION_VALUE && !overTheLimitBody) {
    //      startTimeBody[i] = millis();
    //      overTheLimitBody[i] = true;
    //    }
    //    if (bodySensorValue[i] < SCALIBRATION_VALUE && overTheLimitBody[i])
    //      overTheLimitBody[i] = false;
    //    if (overTheLimitBody[i] && millis() - startTimeBody[i] > SCALIBRATION_TIMER) {
    //      Serial.println("AUTOCALIBRAZIONE");
    //      bodySensor[i]->reset_CS_AutoCal();
    //      overTheLimitBody[i] = false;
    //    }
    updateBodyState(i);
  }
  for (int i = 0; i < N_BODY_SENSORS; i++) {
    pats+=pat[i];
    hits+=hit[i];
  }
}

void updateBodyState(int i) {
  previousDynamicCapacitiveState[i] = capacitiveState[i];
  if (bodySensorValue[i] < lowBodyThreshold) {
    if (capacitiveState[i] != no_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = no_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }
  }
  else if (bodySensorValue[i] >= lowBodyThreshold && bodySensorValue[i] < highBodyThreshold) {
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
  else if (bodySensorValue[i] >= highBodyThreshold) {
    if (capacitiveState[i] != strong_touch) {
      previousCapacitiveState[i] = capacitiveState[i];
      capacitiveState[i] = strong_touch;
      previousStateStartTime[i] = stateStartTime[i];
      stateStartTime[i] = millis();
    }

  }
}

void reactions() {
  if ( (actual_movement == no_movement)) {
    // abbraccio se in stato soft touch sul sensore centale ed uno laterale per almeno tot secondi
    if (touchState == nothing) {
      resetLedTimer();
      resetHitCountTimer();
      resetPatCountTimer();
      hugsCount = 0;
      if ((bodySensorValue[2] >= lowBodyThreshold && bodySensorValue[1] < lowBodyThreshold && bodySensorValue[0] >= lowBodyThreshold) ||
          (bodySensorValue[2] >= lowBodyThreshold && bodySensorValue[1] >= lowBodyThreshold && bodySensorValue[0] <= lowBodyThreshold) ||
          (bodySensorValue[2] >= lowBodyThreshold && bodySensorValue[1] >= lowBodyThreshold && bodySensorValue[0] >= lowBodyThreshold)) {
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
    } else switch (touchState) {
        case hug:  checkHug(); break;
        case pat0: checkPat0(); break;
        case pat1: checkPat1(); break;
        case pat2: checkPat2(); break;
        case hit0: checkHit0(); break;
        case hit1: checkHit1(); break;
        case hit2: checkHit2(); break;
      }
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
  if ((millis() - stateStartTime[2] >= HUGTIME && bodySensorValue[2] >= lowBodyThreshold) &&
      ((millis() - stateStartTime[0] >= HUGTIME && bodySensorValue[0] >= lowBodyThreshold) ||
       (millis() - stateStartTime[1] >= HUGTIME && bodySensorValue[1] >= lowBodyThreshold) )) {
    playS(23);
    setHeadLedPulse(green);
    for (int j = 0; j < N_BODY_SENSORS; j++) {
      pat[j] = 0;
      hit[j] = 0;
      stateStartTime[j] = millis();
    }
    
    hugsCount++;
    //abbraccio = true;
    if (hugsCount == 1)
      Serial3.println("ABBRACCIO!");
    else {
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
      setHeadLedPulse(green);
      Serial3.println("N CAREZZE!");
    } else {
      pat[0]++;
      playS(23);
      setHeadLedPulse(green);
      Serial3.println("UNA CAREZZA!");
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
      setHeadLedPulse(green);
      Serial3.println("N CAREZZE!");
    } else {
      pat[1]++;
      playS(23);
      setHeadLedPulse(green);
      Serial3.println("UNA CAREZZA!");
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
      setHeadLedPulse(green);
      Serial3.println("N CAREZZE!");
    } else {
      pat[2]++;
      playS(23);
      setHeadLedPulse(green);
      Serial3.println("UNA CAREZZA!");
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
    } else {
      hit[0]++;
      startMovement(scared_hitL, red, color_pulse, 34); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      Serial3.println("UN COLPO A SX!");
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
    } else {
      hit[1]++;
      startMovement(scared_hitR, red, color_pulse, 34); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      Serial3.println("UN COLPO A DX!");
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
    } else {
      hit[2]++;
      startMovement(scared_hit, red, color_pulse, 34); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      Serial3.println("UN COLPO AL CENTRO!");
    }
  } else
    patHitStatusExitCond();
  
}

void patHitStatusExitCond() {
  if ((bodySensorValue[2] >= lowBodyThreshold && bodySensorValue[1] < lowBodyThreshold && bodySensorValue[0] >= lowBodyThreshold) ||
      (bodySensorValue[2] >= lowBodyThreshold && bodySensorValue[1] >= lowBodyThreshold && bodySensorValue[0] <= lowBodyThreshold) ||
      (bodySensorValue[2] >= lowBodyThreshold && bodySensorValue[1] >= lowBodyThreshold && bodySensorValue[0] >= lowBodyThreshold)) {
    touchState = hug;
  } else if (capacitiveState[2] == no_touch && capacitiveState[1] == no_touch && capacitiveState[0] == no_touch)
    touchState = nothing;

}


