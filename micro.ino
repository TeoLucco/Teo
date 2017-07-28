void getDownMicroFilter(){
  for(int i=0;i<1500;i++) //metto 0 per n volte al fine di abbassare il valore del filtro
      microLowpassFilter.input(0);
}
unsigned long int lastadd=0;
unsigned long int movementFinishTime=TIME_TO_SETUP+1000;
void microLoop(){
  if( (millis()-movementFinishTime>1000) && (actual_movement==no_movement) && myDFPlayer.available() && gameState==no_game && triskar.isStopped()){
    int wave = analogRead(soundPin);//read the value of A0. range 0-1024
    int value= abs(wave-512); //range 0-512
    microLowpassFilter.input(value);

    if(microLowpassFilter.output() > microSoglia){ //if the value is greater than 384 on 512
      microI++;
      lastadd=millis();
    }
    if(microI>=microISequence){ //if the value is greater than 384 on 512
      startMovement(scared_behind);
      microLowpassFilter.setToNewValue(0.0);
      //getDownMicroFilter();
      microI=0;
    }else
    if(microLowpassFilter.output() < microSoglia && (microI>10 && microI<microISequence)){
      startMovement(scared_round);
      microLowpassFilter.setToNewValue(0.0);
      microI=0;
    } 
  }
  if(millis()-lastadd>500){
      microI=0;
  }
}
