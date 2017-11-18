#define REDPIN 7
#define GREENPIN 8
#define BLUEPIN 6

#define FADESPEED 20     // make this higher to slow down


unsigned long int lastBodyLedLoop = 0;
int bodyI = 0;
unsigned long int resetTime=0;
unsigned long int startLedTime=0;

void bodyLedSetup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  //setBodyLedRainbow();
}

int r, g, bl;
void simpleproof() {
  if (r < 255) {
    analogWrite(8, r);
    r++;
  }
  else r = 0;
}

void bodyLedLoop() {
  if (millis() - lastBodyLedLoop > FADESPEED) {
    switch (body_led_state) {
      case led_off: break;
      case rainbow_cycle: idleLoop(); break;
      case color_wipe: wipeLoop(body_color); break;
      case color_pulse: pulseLoop(body_color); break;
    }
    bodyShow();
 }
 
}

void resetLeds(){
  r = 0; g = 0; bl = 0;
  bodyShow();
}

void setBodyLedOff() {
  body_led_state = led_off;
  resetLeds();
}

void setBodyLedRainbow() {
  body_led_state = rainbow_cycle;
  r = 0; g = 0; bl = 255;
  bodyShow();
}

void setBodyLedWipe(colors color) {
  body_led_state = color_wipe;
  body_color=color;
  resetLeds();
}
void setBodyLedWipe() {
  body_led_state = color_wipe;
 resetLeds();
}

void setBodyLedPulse(colors color) {
  body_led_state = color_pulse;
  body_color=color;
  resetLeds();
}

void setBodyLedPulse() {
  body_led_state = color_pulse;
  resetLeds();
}

void bodyLedUpdate(ledStates ledState) {
  if (body_leds && body_led_state != ledState) {
    bodyI=0;
    switch (ledState) {
      case color_pulse:   setBodyLedPulse(); break;
      case color_wipe:    setBodyLedWipe(); break;
      case rainbow_cycle: setBodyLedRainbow(); break;
      case led_off:       setBodyLedOff(); break;
    }
  }
}

void bodyLedUpdate(colors color) {
  if(body_leds && body_color!=color){
    body_color=color;
    bodyI=0;
    resetLeds();
  }
}

void bodyLedUpdate(ledStates state, colors color) {
  if (body_leds && (body_led_state != state || body_color!=color)) {
    bodyI=0;
    switch (state) {
      case color_pulse:   setBodyLedPulse(color); break;
      case color_wipe:    setBodyLedWipe(color); break;
      case rainbow_cycle: setBodyLedRainbow(); break;
      case led_off:       setBodyLedOff(); break;
    }
  }
}

void bodyLedUpdate(ledStates state, colors color, unsigned long int timer) {
  if (body_leds){
    setLedTimer(timer); 
    if(body_led_state != state || body_color!=color) {
      bodyI=0;
      switch (state) {
        case color_pulse:   setBodyLedPulse(color); break;
        case color_wipe:    setBodyLedWipe(color); break;
        case rainbow_cycle: setBodyLedRainbow(); break;
        case led_off:       setBodyLedOff(); break;
      }
    }
  }
}

boolean ledTimer=false;
unsigned long int fullColorTime=0;


void wipeLoop(colors color) {
  if (color == blueC) {
    if (bl < 255) bl++;
  } else if (color == redC) {
    if (r < 255) r++;
  } else if (color == greenC) {
    if (g < 255) g++;
  } else if (color == yellowC) {
    if (r < 254) r = r + 2;
    if (g < 125) g++;
  } else if(color == lightBlueC){
    if (bl < 254) bl=bl+2; 
    if (g < 254) g=g+2;
    if (r<100) r++;
  }else if(color == orangeC){
   if (r < 250) r =r+5;
   if (g < 50)  g++;
  }else if(color == redCrazy){
   if(r<=235) r=r+20;
   else r=55;
//    int switchM=millis()-startLedTime%10;
//   if(switchM<5){
//    r=250;
//   }else{
//    r=0;
//   }
  }
  if(ledTimer && millis()-fullColorTime>resetTime){
    ledTimer=false;
    resetLeds();
    bodyLedUpdate(led_off);
  }
  //  else if (color == head_strip.Color(255, 100, 0)) {
  //    if (r < 255) r++;
  //    if (g < 100) g++;
  //  }} else if (color == head_strip.Color(0, 255, 255)) {
  //    if (g < 255) g++;
  //    if (bl < 255) bl++;
  //  } else if (color == head_strip.Color(255, 0, 255)) {
  //    if (r < 255) r++;
  //    if (bl < 255) bl++;
}

void setLedTimer(){
  fullColorTime=millis(); 
  ledTimer=true;
}

void setLedTimer(unsigned long int reset_time){
  resetTime=reset_time;
  fullColorTime=millis(); 
  ledTimer=true;
}

void pulseLoop(colors color) {
  if (color == blueC) {
    if (bl < 255 && bodyI == 0) bl++;
    else if (bodyI == 0) bodyI = 1;
    if (bl > 0 && bodyI == 1) bl--;
    else if (bodyI == 1) bodyI = 0;
  } 
  else if (color == redC) {
    if (r < 255 && bodyI == 0) r++;
    else if (bodyI == 0) bodyI = 1;
    if (r > 0 && bodyI == 1) r--;
    else if (bodyI == 1) bodyI = 0;
  } 
  else if (color == greenC) {
    if (g < 255 && bodyI == 0) g++;
    else if (bodyI == 0) bodyI = 1;
    if (g > 0 && bodyI == 1) g--;
    else if (bodyI == 1) bodyI = 0;
  } 
  else if (color == yellowC) {
    if (bodyI == 0) {
      if (r <= 248) r = r + 2;
      if (g < 125) g++;
      if (r == 250) bodyI = 1;
    } else if (bodyI == 1) {
      if (r > 0) r = r - 2;
      if (g > 0) g--;
      if (r == 0) bodyI = 0;
    }
  }else if(color == orangeC){
   if(bodyI==0){
      if (r < 250) r =r+5;
      if (g < 50) g++;
      if(r==250) bodyI = 1;
   }else if (bodyI == 1) {
      if (r > 0) r = r - 5;
      if (g > 0) g--;
      if (r == 0) bodyI = 0;
    } 
  } 
  if(ledTimer && millis()-fullColorTime>resetTime){
    ledTimer=false;
    bodyLedUpdate(led_off);
    headLedUpdate(rainbow_cycle);
  }
}


//
//void bodyLedUpdate(uint32_t color) {
//  if(color==head_strip.Color(0, 0, 255)){
//   r = 0; g = 0; bl = 255;
//  }else if(color == head_strip.Color(255, 0, 0)){
//   r = 255; g = 0; bl = 0;
//  }else if(color == head_strip.Color(0, 255, 0)){
//   r = 0; g = 255; bl = 0;
//  }else if(color == head_strip.Color(0, 255, 255)){
//   r = 0; g = 255; bl = 255;
//  }else if(color == head_strip.Color(255, 0, 255)){
//   r = 255; g = 0; bl = 255;
//  }else if(color == head_strip.Color(255, 170, 0)){
//   r = 255; g = 170; bl = 0;
//  }else if(color == head_strip.Color(255, 100, 0)){
//   r = 255; g = 100; bl = 0;
//  }
//}
//





void idleLoop() {
  if (bodyI == 0) {
    r++;
    analogWrite(REDPIN, r);
    lastBodyLedLoop = millis();
    if (r == 255) bodyI = 1;
  }
  if (bodyI == 1) {
    bl--;
    analogWrite(BLUEPIN, bl);
    lastBodyLedLoop = millis();
    if (bl == 1) bodyI = 2;
  }
  if (bodyI == 2) {
    g++;
    analogWrite(GREENPIN, g);
    lastBodyLedLoop = millis();
    if (g == 255) bodyI = 3;
  }
  if (bodyI == 3) {
    r--;
    analogWrite(REDPIN, r);
    lastBodyLedLoop = millis();
    if (r == 1) bodyI = 4;
  }
  if (bodyI == 4) {
    bl++;
    analogWrite(BLUEPIN, bl);
    lastBodyLedLoop = millis();
    if (bl == 255) bodyI = 5;
  }
  if (bodyI == 5) {
    g--;
    analogWrite(GREENPIN, g);
    lastBodyLedLoop = millis();
    if (g == 1) bodyI = 0;
  }
}

void bodyShow() {
  analogWrite(REDPIN, r);
  analogWrite(GREENPIN, g);
  analogWrite(BLUEPIN, bl);
  lastBodyLedLoop = millis();
}
