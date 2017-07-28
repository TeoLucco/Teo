unsigned long int lastMilliLed=0;
unsigned long int lastMilliLed2=0;
uint16_t ledI=0; 
uint16_t ledJ=0;
uint16_t ledQ=0;
uint16_t ledZ=0;

void headLedPulse(uint32_t color){
   led_state=color_pulse;
   resetCounters();
   headLedUpdate(color);
}
//funzione che usa i colori prefissati per i vari dei movimenti
void headLedPulse(){
   led_state=color_pulse;
   resetCounters();
   headLedUpdate();
}

void headLedWipe(uint32_t color){
   led_state=color_wipe;
   resetCounters();
   headLedUpdate(color);
}

void headLedWipe(){
   led_state=color_wipe;
   resetCounters();
   headLedUpdate();
}

void headLedRainbow(){
   led_state=rainbow_cycle;
   resetCounters();
}

void headLedOff(){
  led_state=led_off;
}

//Loop e Setup
void headLedLoop(){
  switch(led_state){
    case led_off: resetCounters();break;
    case rainbow_cycle: rainbowCycle(5); break;
    case color_wipe: colorWipe(head_color, 50);break;
    case color_pulse: colorPulse(head_color, 50); break;
  }  
}

void ledSetup(){
  head_strip.begin();
  head_strip.show(); 
}

//funzioni "private" che gestiscono i vari stati
void resetLed() {
  for(uint16_t i=0; i<head_strip.numPixels(); i++)
    head_strip.setPixelColor(i, 0);
    head_strip.show();
}

void colorWipe(uint32_t c, uint8_t wait) {
  if(color_wipe){
    if((millis()-lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if(ledI==head_strip.numPixels()) ledI=0;
      head_strip.setPixelColor(ledI, c);
      head_strip.show();
      ledI++;
    }
  }
}
 boolean verse=true;
void colorPulse(uint32_t c, uint8_t wait) {
  if(color_pulse){
    if((millis()-lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      for(uint16_t i=0; i<head_strip.numPixels(); i++)
      head_strip.setPixelColor(i, c);
      head_strip.setBrightness((ledI));
      head_strip.show();
      if(ledI==252) verse=false;
      if(ledI==0)   verse=true;
      if(verse==true)ledI+=6;
      else ledI-=6;
    }
  }
}



void rainbow(uint8_t wait) {
  if(rainbow){
    if((millis()-lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if(ledJ==256) ledJ=0;
      if(ledI==head_strip.numPixels()) ledI=0;
      head_strip.setPixelColor(ledI, Wheel((ledI+ledJ) & 255));
      ledI++;
      if(ledI==head_strip.numPixels()){
        head_strip.show();
        ledJ++;
      }
    }
  }
}


void rainbowCycle(uint8_t wait) {
  if(rainbow_cycle){
    if((millis()-lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if(ledJ>=256*5) ledJ=0;
      else{ // 5 cycles of all colors on wheel
        if(ledI>=head_strip.numPixels()) ledI=0;
        else{
          head_strip.setPixelColor(ledI, Wheel(((ledI * 256 / head_strip.numPixels()) + ledJ) & 255));
          ledI++;
        }
        head_strip.show();
        ledJ++;
      }
    }
  }
}


void rainbowCycle2(uint8_t wait) {
  if(rainbow_cycle){
    if((millis()-lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if(ledJ<256*5) { // 5 cycles of all colors on wheel
        if(ledI< head_strip.numPixels()) {
          head_strip.setPixelColor(ledI, Wheel(((ledI * 256 / head_strip.numPixels()) + ledJ) & 255));
          ledI++;
        }else ledI=0;
        head_strip.show();
        ledJ++;
      }else ledJ=0;
    }
  }
}

void rainbowCycle3(uint8_t wait) {
  if(rainbow_cycle){
    if((millis()-lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if(ledJ<256*5) { // 5 cycles of all colors on wheel
        if(ledI< head_strip.numPixels()) {
          head_strip.setPixelColor(ledI, Wheel(((ledI * 256 / head_strip.numPixels()) + ledJ) & 255));
          ledI++;
        }
        
        if(ledI==head_strip.numPixels()){ledJ++; ledI=0;}
        head_strip.show();
      }else ledJ=0;
    }
  }
}

void rainbowCycle4(uint8_t wait) {
  if(rainbow_cycle){
    if((millis()-lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if(ledJ==256*5) ledJ=0;
      if(ledI==head_strip.numPixels()) ledI=0;
      head_strip.setPixelColor(ledI, Wheel(((ledI * 256 / head_strip.numPixels()) + ledJ) & 255));
      ledI++;
      if(ledI==head_strip.numPixels()){
        head_strip.show();
        ledJ++;
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return head_strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return head_strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return head_strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//FUNZIONI DI CAMBIO STATO(funzioni "pubbliche") E COLORE
void headLedUpdate(uint32_t color,ledStates ledState){
  if(head_color!=color || led_state!=ledState){ 
    switch(ledState){
      case color_pulse:   headLedPulse(color); break;
      case color_wipe:    headLedWipe(color); break;
      case rainbow_cycle: headLedRainbow();break;
      case led_off:       headLedOff();break;
    } 
  }
}

void headLedUpdate(ledStates ledState){
  if(led_state!=ledState){
    switch(ledState){
        case color_pulse:   headLedPulse(); break;
        case color_wipe:    headLedWipe(); break;
        case rainbow_cycle: headLedRainbow();break;
        case led_off:       headLedOff();break;
      }    
  } 
}
void headLedUpdate(uint32_t color){
  head_color=color;
}

void headLedUpdate(){
  switch(actual_movement){
      case no_movement:         break;
      case follow:              head_color=green;break;
      case make_eight:          break;
      case make_circle:         head_color=green;break;
      case turn180r:            head_color=yellow;break;
      case turn180l:            head_color=yellow;break;
      case makeOnem:            head_color=green;break;
      case scared_round:        head_color=yellow;break;  
      case dontwonna:           head_color=red;break;
      case angrymov:            head_color=red;break;
      case scared_behind:       head_color=yellow;break;
      case autonomous_movement: head_color=green;break;
      case make_happy0:         head_color=green;break;
      case make_happy1:         head_color=green;break;
      case make_happy2:         head_color=green;break;
      case make_happy3:         head_color=green;break;
//      case make_happy4:         /*head_color=green;*/break;
      case make_sad0:           head_color=red;break;
      case make_sad1:           head_color=red;break;
      case make_sad2:           head_color=red;break;
    }
}

void resetCounters(){
 ledI=0; 
 ledJ=0;
 ledQ=0;
 ledZ=0;
}
