void headCapacitiveLoop(){
     long sensorValue;
     boolean flag;
     for(int i=0,flag=false;i < bodyNumberSensors;i++){
     sensorValue = headSensor[i]->capacitiveSensor(30);
       if(sensorValue > headThreshold){
          pressedButton=i;
          flag=true;
       }
     }
     if(flag==false){
      pressedButton=-1;
     }
     flag=false;
}


