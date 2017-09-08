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
  if (interpreterState == fam_modality) {
    updateCapacitiveFlags();
    reactions();
  }
}

unsigned long int startTimeBody[N_BODY_SENSORS] = {0, 0, 0};
boolean overTheLimitBody[N_BODY_SENSORS] = {false, false, false};

void updateCapacitiveFlags() {
  // catturo i valori di output di ogni sensore capacitivo
  for (int i = 0; i < N_BODY_SENSORS; i++) {
    bodySensorValue[i] = bodySensor[i]->capacitiveSensor(10);
    if (bodySensorValue[i] >= SCALIBRATION_VALUE && !overTheLimitBody) {
      startTimeBody[i] = millis();
      overTheLimitBody[i] = true;
    }
    if (bodySensorValue[i] < SCALIBRATION_VALUE && overTheLimitBody[i])
      overTheLimitBody[i] = false;
    if (overTheLimitBody[i] && millis() - startTimeBody[i] > SCALIBRATION_TIMER) {
      Serial.println("AUTOCALIBRAZIONE");
      bodySensor[i]->reset_CS_AutoCal();
      overTheLimitBody[i] = false;
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

#define REACTION_TIME 100
int angryCount = 0;
int happyCount = 0;

void reactions() {
  if ( (millis() - movementFinishTime > 2000) && (actual_movement == no_movement) && (gameState == no_game) && (interpreterState == fam_modality) && digitalRead(BUSY_PIN) == HIGH && triskar.isStopped()) {
    if (happyCount < 3) {
      if (capacitiveState[2] == soft_touch && capacitiveState[0] == soft_touch && capacitiveState[1] == soft_touch && stateStartTime[0] - millis() > REACTION_TIME && stateStartTime[1] - millis() > REACTION_TIME && stateStartTime[2] - millis() > REACTION_TIME) {
        //abbraccio completo
        setHeadLedPulse(green);
        playS(19);
        happyCount++;
      }
      else if ((capacitiveState[2] == soft_touch && capacitiveState[0] == soft_touch && stateStartTime[2] - millis() > REACTION_TIME && stateStartTime[0] - millis() > REACTION_TIME) ||
               (capacitiveState[3] == soft_touch && capacitiveState[1] == soft_touch && stateStartTime[3] - millis() > REACTION_TIME && stateStartTime[1] - millis() > REACTION_TIME)) {
        //abbraccio incompleto, stringi più forte, allarga l'abbraccio!
        setHeadLedWipe(green);
        playS(20);
        happyCount++;
      }
      else if (capacitiveState[2] == soft_touch && stateStartTime[2] - millis() > REACTION_TIME) {
        happyCount++;
        if (millis() - stateStartTime[2] < 1000)
          setHeadLedWipe(yellow);
        else
          setHeadLedWipe(green);
        //colpo o carezza? dammi un abbraccio! stringimi!
      }
    } else {
      happyCount = 0;
      startMovement(make_happy0, green, color_pulse, 50);
    }
    if (angryCount < 3) {
      if (capacitiveState[2] == strong_touch) {
        angryCount++;
        startMovement(scared_hit, red, color_pulse, 34); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      }

      else if (capacitiveState[0] == strong_touch) {
        angryCount++;
        alpha = PI / 3;
        startMovement(scared_hitL, red, color_pulse, 34);
      }
      else if (capacitiveState[1] == strong_touch) {
        angryCount++;
        alpha = PI / 3;
        startMovement(scared_hitR, red, color_pulse, 34);
      }

    } else {

      if (capacitiveState[2] == strong_touch) {
        angryCount = 0;
        startMovement(make_sad2, red, color_pulse, 33); //13 è l'esempio di audio che deve eseguire TODO VA CAMBIATO
      }
      else if (capacitiveState[0] == strong_touch) {
        angryCount = 0;
        alpha = PI / 3;
        startMovement(make_sad2L, red, color_pulse, 33);
      }
      else if (capacitiveState[1] == strong_touch) {
        angryCount = 0;
        alpha = PI / 3;
        startMovement(make_sad2R, red, color_pulse, 33);
      }

    }
  }
}
