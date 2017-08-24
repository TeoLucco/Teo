void headCapacitiveSetup(){
  headSensor[0] = new CapacitiveSensor(CAPACITIVE_COMMONPIN,HEAD_BUTTON_0);//sx
  headSensor[1] = new CapacitiveSensor(CAPACITIVE_COMMONPIN,HEAD_BUTTON_1);//dx
  headSensor[2] = new CapacitiveSensor(CAPACITIVE_COMMONPIN,HEAD_BUTTON_2);//behind
  headSensor[3] = new CapacitiveSensor(CAPACITIVE_COMMONPIN,HEAD_BUTTON_3);//front  
  for(int i=0;i<N_SENSORS;i++)
  headSensor[i]->set_CS_AutocaL_Millis(SCALIBRATION_TIMER);
}

unsigned long int startTime=0;
boolean overTheLimit=false;

void headCapacitiveLoop(){
 if(gameState==wait_answer){
   long sensorValue;
   boolean flag=false;
   for(int i=0;i < N_SENSORS;i++){
     sensorValue = headSensor[i]->capacitiveSensor(30);
     Serial3.print(sensorValue);Serial3.print(" ");
     if(sensorValue > headThreshold){
       pressedButton=i;
       flag=true;
     }
     if(sensorValue>=SCALIBRATION_VALUE && !overTheLimit){
      startTime=millis();
      overTheLimit=true;
     }
     if(sensorValue<SCALIBRATION_VALUE && overTheLimit)
      overTheLimit=false;
     if(overTheLimit && millis()-startTime>SCALIBRATION_TIMER){
      Serial.println("AUTOCALIBRAZIONE");
      headSensor[i]->reset_CS_AutoCal();
      overTheLimit=false;
     }
   }
   Serial3.println("");
   if(flag==false){
    pressedButton=-1;
   }
   flag=false;
 }
 
}


