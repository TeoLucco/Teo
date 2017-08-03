double averageLight;
double error=100.0;
#define nReadings 50

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
  if(fotores_value<averageLight-240-error || fotores_value<80)
    Serial.println("Coperto"); 
  else if(fotores_value<averageLight-240+error && fotores_value>averageLight-240-error)
    Serial.println("Lenzuolo");
}

