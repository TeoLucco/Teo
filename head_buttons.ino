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


void headCapacitiveLoop() {
  boolean checkMax = false;
  if (gameState == wait_answer) {
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
  }

}


