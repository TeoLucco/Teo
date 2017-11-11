double averageLight;
double clearTresh = 0;
double sheetTresh = 0;
double error = 100.0;
#define nReadings 50.0d
enum fotoresistorStates {clean, sheet, covered};
fotoresistorStates fotoresistorState = clean;

void fotoresSetup() {
  double sum = 0.0;
  for (int I = 0; I < nReadings; I++) {
    sum += analogRead(FOTORES_PIN);
    Serial3.println(analogRead(FOTORES_PIN));
  }
  averageLight = (double)sum / (double)nReadings;
  clearTresh = averageLight - 240 - error;
  sheetTresh = averageLight - 240 + error;

  Serial3.print("Luce media: "); Serial3.println(averageLight);
  Serial3.print("Lenzuolo alto: "); Serial3.println(sheetTresh);
  Serial3.print("Lenzuolo basso: "); Serial3.println(clearTresh);
}


void fotoresLoop() {
  if(fotoresistor){
  if ((interpreterState == fam_modality || interpreterState == test_modality) && !btMov ){
    fotores_value = analogRead(FOTORES_PIN);
    if ((fotores_value < averageLight - 240 - error || fotores_value < 80) && fotoresistorState != covered) {
      fotoresistorState = covered;
      if (actual_movement != autonomous_movement && interpreterState != test_modality)
        startMovement(autonomous_movement);
    } else if (fotores_value < averageLight - 240 + error && fotores_value > averageLight - 240 - error && fotoresistorState != sheet) {
      fotoresistorState = sheet;
      if (actual_movement != follow && interpreterState != test_modality)
        startMovement(follow);
    } else if (fotores_value > averageLight - 240 + error && fotoresistorState != clean) {
      fotoresistorState = clean;
      if ((actual_movement == follow || actual_movement == autonomous_movement) && interpreterState != test_modality)
        stopAutFollow();
    }
  }
  }else {
    fotores_value=0;
    fotoresistorState=clean;
  }
}


