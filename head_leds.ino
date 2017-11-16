unsigned long int lastMilliLed = 0;
unsigned long int lastMilliLed2 = 0;
uint16_t ledI = 0;
uint16_t ledJ = 0;
uint16_t ledQ = 0;
uint16_t ledZ = 0;

void setHeadLedPulse(uint32_t color) {
  led_state = color_pulse;
  resetCounters();
  head_strip.setBrightness(255);
  headLedSetColor(color);
}
//funzione che usa i colori prefissati per i vari dei movimenti
void setHeadLedPulse() {
  led_state = color_pulse;
  resetCounters();
  head_strip.setBrightness(255);
  headLedSetColor();
}

void setHeadLedWipe(uint32_t color) {
  led_state = color_wipe;
  resetCounters();
  head_strip.setBrightness(255);
  headLedSetColor(color);
}

void setHeadLedWipe() {
  led_state = color_wipe;
  resetCounters();
  head_strip.setBrightness(255);
  headLedSetColor();
}

void setHeadLedRainbow() {
  led_state = rainbow_cycle;
  resetCounters();
  head_strip.setBrightness(255);
}

void setHeadLedOff() {
  resetLed();
  led_state = led_off;
}

//Loop e Setup
void headLedLoop() {
  switch (led_state) {
    case led_off: break;
    case rainbow_cycle: rainbowCycle7(20); break;
    case color_wipe: colorWipe(head_color, 65); break;
    case color_pulse: colorPulse(head_color, 65); break;
  }
  if (colorByButton && millis() - lastPressedButtonTime > WIPE_DURATION) {
    colorByButton = false;
    headLedUpdate(rainbow_cycle);
  }
}

void headLedSetup() {
  head_strip.begin();
  head_strip.show();
  setHeadLedRainbow();
}

//funzioni "private" che gestiscono i vari stati
void resetLed() {
  for (uint16_t i = 0; i < head_strip.numPixels(); i++)
    head_strip.setPixelColor(i, 0);
  head_strip.show();
}

void colorWipe(uint32_t c, uint8_t wait) {
  if (color_wipe) {
    if ((millis() - lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if (ledI == head_strip.numPixels()) ledI = 0;
      head_strip.setPixelColor(ledI, c);
      head_strip.show();
      ledI++;
    }
  }
}

boolean verse = true;
void colorPulse(uint32_t c, uint8_t wait) {
  if (color_pulse) {
    if ((millis() - lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      for (uint16_t i = 0; i < head_strip.numPixels(); i++)
        head_strip.setPixelColor(i, c);
      head_strip.setBrightness((ledI));
      head_strip.show();
      if (ledI == 252) verse = false;
      if (ledI == 0)   verse = true;
      if (verse == true)ledI += 6;
      else ledI -= 6;
    }
  }
}



void rainbow(uint8_t wait) {
  if (rainbow) {
    if ((millis() - lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if (ledJ == 256) ledJ = 0;
      if (ledI == head_strip.numPixels()) ledI = 0;
      head_strip.setPixelColor(ledI, Wheel((ledI + ledJ) & 255));
      ledI++;
      if (ledI == head_strip.numPixels()) {
        head_strip.show();
        ledJ++;
      }
    }
  }
}


void rainbowCycle(uint8_t wait) {
  if ((millis() - lastMilliLed) >= wait)   {
    lastMilliLed = millis();
    if (ledJ >= 256 * 5) ledJ = 0;
    else { // 5 cycles of all colors on wheel
      if (ledI >= head_strip.numPixels()) ledI = 0;
      else {
        head_strip.setPixelColor(ledI, Wheel(((ledI * 256 / head_strip.numPixels()) + ledJ) & 255));
        ledI++;
      }
      ledJ++;
    }
  }
  head_strip.show();
}


void rainbowCycle2(uint8_t wait) {

  if ((millis() - lastMilliLed) >= wait)   {
    lastMilliLed = millis();
    if (ledJ < 256 * 5) { // 5 cycles of all colors on wheel
      if (ledI < head_strip.numPixels()) {
        head_strip.setPixelColor(ledI, Wheel(((ledI * 256 / head_strip.numPixels()) + ledJ) & 255));
        ledI++;
      } else ledI = 0;
      head_strip.show();
      ledJ++;
    } else ledJ = 0;
  }

}

void rainbowCycle3(uint8_t wait) {
  if (rainbow_cycle) {
    if ((millis() - lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if (ledJ < 256 * 5) { // 5 cycles of all colors on wheel
        if (ledI < head_strip.numPixels()) {
          head_strip.setPixelColor(ledI, Wheel(((ledI * 256 / head_strip.numPixels()) + ledJ) & 255));
          ledI++;
        }

        if (ledI == head_strip.numPixels()) {
          ledJ++;
          ledI = 0;
        }
        head_strip.show();
      } else ledJ = 0;
    }
  }
}

void rainbowCycle4(uint8_t wait) {
  if (rainbow_cycle) {
    if ((millis() - lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      if (ledJ == 256 * 5) ledJ = 0;
      if (ledI == head_strip.numPixels()) ledI = 0;
      head_strip.setPixelColor(ledI, Wheel(((ledI * 256 / head_strip.numPixels()) + ledJ) & 255));
      ledI++;
      if (ledI == head_strip.numPixels()) {
        head_strip.show();
        ledJ++;
      }
    }
  }
}

void rainbowCycle5(uint8_t wait) {
  if (ledJ < 256 * 5) { // 5 cycles of all colors on wheel
    if (ledI < head_strip.numPixels()) {
      head_strip.setPixelColor(ledI, Wheel(((ledI * 256 / head_strip.numPixels()) + ledJ) & 255));
      ledI++;
    } else if ((millis() - lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      ledI = 0;
      head_strip.show();
      ledJ++;
    }
  } else ledJ = 0;
}
void rainbowCycle6(uint8_t wait) {
  if (ledJ < 256 * 5) { // 5 cycles of all colors on wheel
    for (int i = 0; i < head_strip.numPixels(); i++) {
      head_strip.setPixelColor(i, Wheel(((i * 256 / head_strip.numPixels()) + ledJ) & 255));
    }
    if ((millis() - lastMilliLed) >= wait)   {
      lastMilliLed = millis();
      head_strip.show();
      ledJ++;
    }
  } else ledJ = 0;
}

void rainbowCycle7(uint8_t wait) {
  if (ledJ < 256) { // 5 cycles of all colors on wheel
    if ((millis() - lastMilliLed) >= wait)   {
      for (int i = 0; i < head_strip.numPixels(); i++) {
        head_strip.setPixelColor(i, Wheel(((i * 256 / head_strip.numPixels()) + ledJ) & 255));
      }
      lastMilliLed = millis();
      head_strip.show();
      ledJ++;
    }
  } else ledJ = 0;
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return head_strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return head_strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return head_strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//FUNZIONI DI CAMBIO STATO(funzioni "pubbliche") E COLORE
void headLedUpdate(uint32_t color, ledStates ledState) {
  if (head_leds && (head_color != color || led_state != ledState)) {
    //setBodyLedOff();
    //bodyI=0;
    switch (ledState) {
      case color_pulse:   setHeadLedPulse(color); break;
      case color_wipe:    setHeadLedWipe(color); break;
      case rainbow_cycle: setHeadLedRainbow(); break;
      case led_off:       setHeadLedOff(); break;
    }
  }
}

void headLedUpdate(ledStates ledState) {

  if (head_leds && led_state != ledState) {
    //setBodyLedOff();
    //bodyI=0;
    switch (ledState) {
      case color_pulse:   setHeadLedPulse(); break;
      case color_wipe:    setHeadLedWipe(); break;
      case rainbow_cycle: setHeadLedRainbow(); break;
      case led_off:       setHeadLedOff(); break;
    }
  }
}
void headLedSetColor(uint32_t color) {
  if (head_leds && head_color != color) {
    head_color = color;
    //setBodyLedOff();
    resetCounters();
  }
}

void headLedSetColor() { //colori predefiniti per i movimenti
  switch (actual_movement) {
    case no_movement:         break;
    case follow:              head_color = green; break;
    case make_eight:          break;
    case make_circle:         head_color = green; break;
    case turn180r:            head_color = yellow; break;
    case turn180l:            head_color = yellow; break;
    case makeOnemF:            head_color = green; break;
    case makeOnemB:            head_color = green; break;
    case scared_round:        head_color = yellow; break;
    case dontwonna:           head_color = red; break;
    case angrymov:            head_color = red; break;
    case scared_behind:       head_color = yellow; break;
    case autonomous_movement: head_color = green; break;
    case make_happy0:         head_color = green; break;
    case make_happy1:         head_color = green; break;
    case make_happy2:         head_color = green; break;
    case make_happy3:         head_color = green; break;
    //      case make_happy4:         /*head_color=green;*/break;
    case make_sad0:           head_color = red; break;
    case make_sad1:           head_color = red; break;
    case make_sad2:           head_color = red; break;
  }
}

void resetCounters() {
  ledI = 0;
  ledJ = 0;
  ledQ = 0;
  ledZ = 0;
}

uint32_t chooseColor(colors color){
  switch(color){
    case blueC: return blue;break;
    case redC:  return red;break;
    case greenC: return green;break;
    case lightBlueC: return light_blue;break; 
    //case violetC:return violet;break;
    case orangeC: return orange;break;
    case redCrazy: return red;break;
  }
}

