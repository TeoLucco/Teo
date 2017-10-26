#define REDPIN 7
#define GREENPIN 8
#define BLUEPIN 6

#define FADESPEED 20     // make this higher to slow down


unsigned long int lastBodyLedLoop = 0;
int bodyI = 0;

void bodyLedSetup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  setBodyLedRainbow();
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
    switch (led_state) {
      case led_off: break;
      case rainbow_cycle: idleLoop(); break;
      case color_wipe: wipeLoop(head_color); break;
      case color_pulse: pulseLoop(head_color); break;
    }
    bodyShow();
  }
}

void wipeLoop(uint32_t color) {
  if (color == head_strip.Color(0, 0, 255)) {
    if (bl < 255) bl++;
  } else if (color == head_strip.Color(255, 0, 0)) {
    if (r < 255) r++;
  } else if (color == head_strip.Color(0, 255, 0)) {
    if (g < 255) g++;
  } else if (color == head_strip.Color(0, 255, 255)) {
    if (g < 255) g++;
    if (bl < 255) bl++;
  } else if (color == head_strip.Color(255, 0, 255)) {
    if (r < 255) r++;
    if (bl < 255) bl++;
  } else if (color == head_strip.Color(255, 170, 0)) {
    if (r < 250) r=r+2;
    if (g < 125) g++;
  } else if (color == head_strip.Color(255, 100, 0)) {
    if (r < 255) r++;
    if (g < 100) g++;
  }
}

void pulseLoop(uint32_t color) {
  if (color == head_strip.Color(0, 0, 255)) {
    if (bl < 255 && bodyI == 0) bl++;
    else if (bodyI == 0) bodyI = 1;
    if (bl > 0 && bodyI == 1) bl--;
    else if (bodyI == 1) bodyI = 0;
  } else if (color == head_strip.Color(255, 0, 0)) {
    if (r < 255 && bodyI == 0) r++;
    else if (bodyI == 0) bodyI = 1;
    if (r > 0 && bodyI == 1) r--;
    else if (bodyI == 1) bodyI = 0;
  } else if (color == head_strip.Color(0, 255, 0)) {
    if (g < 255 && bodyI == 0) g++;
    else if (bodyI == 0) bodyI = 1;
    if (g > 0 && bodyI == 1) g--;
    else if (bodyI == 1) bodyI = 0;
  } else if (color == head_strip.Color(0, 255, 255)) {
    if (bodyI == 0) {
      if (g < 255) g++;
      if (bl < 255) bl++;
      if (bl == 255) bodyI = 1;
    } else if (bodyI == 1) {
      if (g > 0) g--;
      if (bl > 0) bl--;
      if (bl == 0) bodyI = 0;
    }
  } else if (color == head_strip.Color(255, 0, 255)) {
    if (bodyI == 0) {
      if (r < 255) r++;
      if (bl < 255) bl++;
      if (bl == 255) bodyI = 1;
    } else if (bodyI == 1) {
      if (r > 0) r--;
      if (bl > 0) bl--;
      if (bl == 0) bodyI = 0;
    }
  } else if (color == head_strip.Color(255, 170, 0)) {
    if (bodyI == 0) {
      if (r <= 248) r=r+2;
      if (g < 125) g++;
      if (r == 250) bodyI = 1;
    } else if (bodyI == 1) {
      if (r > 0) r=r-2;
      if (g > 0) g--;
      if (r == 0) bodyI = 0;
    }
  } else if (color == head_strip.Color(255, 100, 0)) {
    if (bodyI == 0) {
      if (r < 255) r++;
      if (g < 100) g++;
      if (r == 255) bodyI = 1;
    } else if (bodyI == 1) {
      if (r > 0) r--;
      if (g > 0) g--;
      if (r == 0) bodyI = 0;
    }
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
//void bodyLedUpdate(ledStates ledState) {
//    switch(ledState){
//        case color_pulse:   setBodyLedPulse(); break;
//        case color_wipe:    setBodyLedWipe(); break;
//        case rainbow_cycle: setBodyLedRainbow();break;
//        case led_off:       setBodyLedOff();break;
//    }
//
//}

void setBodyLedRainbow() {
  r = 0; g = 0; bl = 255;
}

void setBodyLedOff() {
  r = 0; g = 0; bl = 0;
  bodyShow();
}

//
//void setBodyLedPulse(uint32_t color){
//
//}
//
//void setBodyLedWipe(uint32_t color){
//
//}

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
