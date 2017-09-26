double averageLight;
double error=100.0;
#define nReadings 50
enum fotoresistorStates{clean, sheet, covered};
fotoresistorStates fotoresistorState=clean;

void fotoresSetup(){
  int sum=0;
  for(int I=0;I<nReadings;I++)
    sum+=analogRead(FOTORES_PIN);
  averageLight=sum/nReadings;
  
  Serial.print("Luce media: "); Serial.println(averageLight);
  Serial.print("Lenzuolo alto: "); Serial.println(averageLight-240+error);
  Serial.print("Lenzuolo basso: "); Serial.println(averageLight-240-error);
}


void fotoresLoop(){
  fotores_value=analogRead(FOTORES_PIN);
  if((fotores_value<averageLight-240-error || fotores_value<80)&&fotoresistorState!=covered){
    fotoresistorState=covered;
    startMovement(autonomous_movement); 
  }else if(fotores_value<averageLight-240+error && fotores_value>averageLight-240-error && fotoresistorState!=sheet){
    fotoresistorState=sheet;
    startMovement(follow);
  }else if(fotoresistorState!=clean && (actual_movement==autonomous_movement || actual_movement==follow)){
    fotoresistorState=clean;
    stopMovement();
  }
}


