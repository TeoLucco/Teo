//double averageLight;
//double clearTresh = 0;
//double sheetTresh = 0;
//double error = 100.0;
//#define nReadings 50.0d
#define ACTIVATION_TIME 10000
enum fotoresistorStates {clean, covered};
fotoresistorStates fotoresistorState = clean;
unsigned long int stateStartTime = 0;

//void fotoresSetup() {
//  double sum = 0.0;
//  for (int I = 0; I < nReadings; I++) {
//    sum += analogRead(FOTORES_PIN);
//    Serial3.println(analogRead(FOTORES_PIN));
//  }
//  averageLight = (double)sum / (double)nReadings;
//  clearTresh = averageLight - 240 - error;
//  sheetTresh = averageLight - 240 + error;
//
//  Serial3.print("Luce media: "); Serial3.println(averageLight);
//  Serial3.print("Lenzuolo alto: "); Serial3.println(sheetTresh);
//  Serial3.print("Lenzuolo basso: "); Serial3.println(clearTresh);
//}


void fotoresLoop() {
  if (fotoresistor) {
    setFotoresistorState();
    //startMovements();
  }
}

void setFotoresistorState() {
  if (interpreterState == fam_modality) {
    fotores_value = analogRead(FOTORES_PIN);
    if ((fotores_value <= 200) && fotoresistorState != covered) {
      fotoresistorState = covered;
      stateStartTime = millis();
    } else if (fotores_value > 200 && fotoresistorState != clean) {
      fotoresistorState = clean;
      stateStartTime = millis();
    }
  } else {
    fotores_value = 0;
    fotoresistorState = clean;
  }
}

void startMovements() {
  if (fotoresistorState == covered && millis() - stateStartTime > ACTIVATION_TIME && actual_movement == no_movement) {
    startMovement(autonomous_capa);
  } else if (fotoresistorState == clean && millis() - stateStartTime > ACTIVATION_TIME && actual_movement == autonomous_capa) {
    startMovement(no_movement);
  }
}

