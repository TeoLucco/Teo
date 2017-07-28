void bodyCapacitiveSetup(){
  bodySensor[3] = new CapacitiveSensor(BODY_CAPACITIVE_COMMONPIN,47);
  bodySensor[0] = new CapacitiveSensor(BODY_CAPACITIVE_COMMONPIN,45);
  bodySensor[2] = new CapacitiveSensor(BODY_CAPACITIVE_COMMONPIN,43);
  bodySensor[1] = new CapacitiveSensor(BODY_CAPACITIVE_COMMONPIN,41);
//  bodySensor[0] = new CapacitiveSensor(BODY_CAPACITIVE_COMMONPIN,BODY_SX_PIN);//sx
//  bodySensor[1] = new CapacitiveSensor(BODY_CAPACITIVE_COMMONPIN,BODY_DX_PIN);//dx
//  bodySensor[2] = new CapacitiveSensor(BODY_CAPACITIVE_COMMONPIN,BODY_BEHIND_PIN);//behind
//  bodySensor[3] = new CapacitiveSensor(BODY_CAPACITIVE_COMMONPIN,BODY_FRONT_PIN);//front  
  for(int i=0;i<bodyNumberSensors;i++)
  bodySensor[i]->set_CS_AutocaL_Millis(10000);
}



void bodyCapacitiveLoop(){
  for(int i=0;i < bodyNumberSensors;i++){
     // catturo i valori di output di ogni sensore capacitivo
     bodySensorValue[i] = bodySensor[i]->capacitiveSensor(30);
     if(bodySensorValue[i] > bodyThreshold){
      switch(i){
        case '0': leftCapacitive=true; break;
        case '1': rightCapacitive=true;break;
        case '2': if(bodySensorValue[2] > behindBodyThreshold)behindCapacitive=true;else behindCapacitive=false; break;
        case '3': frontCapacitive=true;break;
      }
    }else{
      switch(i){
        case '0': leftCapacitive=false; break;
        case '1': rightCapacitive=false;break;
        case '2': behindCapacitive=false; break;
        case '3': frontCapacitive=false;break;        
      }
    }   
  }
}

void bodyCapacitiveLoop2(){
     if(!move){
       long sensorValue;
       boolean flag=false;
       for(int i=0;i < bodyNumberSensors;i++){
       sensorValue = bodySensor[i]->capacitiveSensor(30);
       Serial3.print(sensorValue);Serial3.print(" ");
         if(sensorValue > bodyThreshold){
            pressedButton=i;
            flag=true;
         }
       }Serial3.println("");
       if(flag==false){
        pressedButton=-1;
       }
       flag=false;
     }
}

