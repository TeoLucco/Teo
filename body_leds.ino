/*
#define REDPIN 6
#define GREENPIN 7
#define BLUEPIN 8

#define FADESPEED 5     // make this higher to slow down

void bodyLedSetup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
}

int r, g, bl;

void bodyLedLoop() {
  switch (led_state) {
    case led_off: break;
    case rainbow_cycle: idleLoop();break;
    case color_wipe:
    case color_pulse: break;
  }
}

void bodyLedUpdate(uint32_t color) {
  if (head_color != color) {
    switch (color) {
      case blue: r = 0; g = 0; bl = 255; break;
      case red: r = 255; g = 0; bl = 0; break;
      case green: r = 0; g = 255; bl = 0; break;
      case light_blue: r = 0; g = 255; bl = 255; break;
      case violet: r = 255; g = 0; bl = 255; break;
      case orange: r = 255; g = 100; bl = 0; break;
    }
  }
}

void bodyLedUpdate(ledStates ledstate) {
  if(led_state!=ledState){
    switch(ledState){
        case color_pulse:   setBodyLedColor(); break;
        case color_wipe:    setBodyLedColor(); break;
        case rainbow_cycle: setBodyLedRainbow();break;
        case led_off:       setBodyLedOff();break;
    }    
  } 
}

void setBodyLedRainbow(){
  r=0;g=0;bl=255;
  analogWrite(REDPIN, r);
  analogWrite(GREENPIN, g);
  analogWrite(BLUEPIN, bl); 
}

void setBodyLedOff(){
  r=0;g=0;bl=0;
  analogWrite(REDPIN, r);
  analogWrite(GREENPIN, g);
  analogWrite(BLUEPIN, bl);
}

void idleLoop() {
  if (millis() - lastBodyLedLoop > FADESPEED) {
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
}
*/
