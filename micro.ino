
void microLoop() {
  if (micro) {
    int wave = analogRead(soundPin);//read the value of A0. range 0-1024
    int value = abs(wave - 512); //range 0-512
    microLowpassFilter.input(value);
    micro_f=microLowpassFilter.output();
    if ( (millis() - movementFinishTime > 2000) && (actual_movement == no_movement) && (gameState == no_game) && (interpreterState == fam_modality) && digitalRead(BUSY_PIN) == HIGH && triskar.isStopped()) {
      if (micro_f > microSoglia) { //if the value is greater than 384 on 512
        microI++;
        lastadd = millis();
      }
      if (microI >= microISequence) { //if the value is greater than 384 on 512
        startMovement(scared_behind,redCrazy,color_wipe,AUDIO_SCARED_BEHIND); //audio: mi stavo per schiantare
        microLowpassFilter.setToNewValue(0.0);
        microI = 0;
      } else if (micro_f < microSoglia && (microI >= microISequenceShortMin && microI <= microISequenceShortMax)) {
        startMovement(scared_round,orangeC,color_wipe,AUDIO_SCARED_ROUND);
        microLowpassFilter.setToNewValue(0.0);
        microI = 0;
      }
    } else if (interpreterState != test_modality) microLowpassFilter.setToNewValue(0.0);
    if (millis() - lastadd > 500) {
      microI = 0;
    }
  }else micro_f=0;
}
