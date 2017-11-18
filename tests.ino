void hardwareTests() {
  switch (testState) {
    case tests_descr: testDescr(); break;
    case choose_test: chooseTest(); break;
    case start_test:  startTest(); break;
    case test_exe:    testExecution(); break;
  }
}
int numSonarTest = 0;
int numBodyTest = 0;

void testDescr() {
  Serial3.println("##########SESSIONE TEST##########");
  Serial3.println("Scegliere il test da eseguire:");
  Serial3.println("1 - Test Sonar");
  Serial3.println("2 - Test Capacitivi Testa");
  Serial3.println("3 - Test Capacitivi Corpo");
  Serial3.println("4 - Test Fotoresistenza");
  Serial3.println("5 - Test Casse");
  Serial3.println("6 - Test Microfono");
  Serial3.println("0 - Esci da Sessione Test");
  testState = choose_test;
}

void chooseTest() {
  if (Serial3.available()) {
    b = Serial3.read();
    switch (b) {
      case '1':
        testType = sonar_t;
        Serial3.println("Viene visualizzata la distanza captata dal Sonar Frontale. Muovere una mano o un oggetto davanti al sonar per verificarne il corretto funzionamento.");
        Serial3.println("Premere START per iniziare, END per terminare il test");
        testState = start_test;
        break;

      case '2':
        //CapacitivesUpdate(head);
        testType = head_capacitives_t;
        Serial3.println("Viene visualizzato il numero[0-3] del capacitivo toccato. Toccare uno alla volta i capacitivi posti sulla testa del robot per verificarne il corretto funzionamento.");
        Serial3.println("Premere START per iniziare, END per terminare il test");
        testState = start_test;
        break;

      case '3':
        //CapacitivesUpdate(body);
        testType = body_capacitives_t;
        Serial3.println("Vengono visualizzati il valore e lo stato captato dal capacitivo posto nella parte posteriore sinistra(del robot). Toccare il capacitivo più o meno forte per verificarne il corretto funzionamento.");
        Serial3.println("Premere START per iniziare, END per terminare il test");
        testState = start_test;
        break;

      case '4':
        testType = fotores_t;
        Serial3.println("Viene visualizzato il valore captato dalla fotoresistenza posta in prossimità dei led ed il suo stato. Coprire il robot o la fotoresistenza con differenti materiali per verificarne il corretto funzionamento.");
        Serial3.println("Premere START per iniziare, END per terminare il test");
        testState = start_test;
        break;

      case '5':
        testType = speaker_t;
        Serial3.println("Viene riprodotto un audio per verificare il corretto funzionamento delle casse.");
        Serial3.println("Premere START per avviare la riproduzione dell'audio");
        testState = start_test;
        break;

      case '6':
        testType = micro_t;
        Serial3.println("Viene visualizzato il valore captato dal microfono. Emettere suoni di diversa intensità per verificarne il corretto funzionamento.");
        Serial3.println("Premere START(1) per iniziare, END(2) per terminare il test");
        testState = start_test;
        break;

      case '0':
        interpreterState = choose_modality;
        loopStartTime=millis();
        testState = tests_descr;
        testType = no_one;
        break;

      default:
        // default code (should never run)
        break;
    }

  }
}

void startTest() {
  if (Serial3.available()) {
    b = Serial3.read();
    switch (b) {
      case '1':
        testState = test_exe;
        if (testType == speaker_t) playS(1);
        break;
      case '2':
        testState = tests_descr;
        testType = no_one;
        if (testType == speaker_t) {
          Serial3.println("Test Casse terminato");
        }
        break;

      default:
        // default code (should never run)
        break;
    }

  }
}


void testExecution() {
  switch (testType) {
    case sonar_t:             sonarTest(); break;
    case head_capacitives_t:  headCapacitiveTest(); break;
    case body_capacitives_t:  /*bodyCapacitiveTest();*/ break;
    case fotores_t :          fotoresistorTest(); break;
    case speaker_t :          speakerTest();      break;
    case micro_t:             microTest();      break;
  }
}

void sonarTest() {
  printSonarDetail();
  if (Serial3.available()) {
    b = Serial3.read();
    if (b == '1') {
      if (numSonarTest < SONAR_NUM - 1) {
        numSonarTest++;
        testState = start_test;
        switch (numSonarTest) {
          case 1:
            Serial3.println("Verrà visualizzata la distanza captata dal Sonar Frontale Destro(destra del robot). Muovere una mano o un oggetto davanti al sonar per verificarne il corretto funzionamento.");
            break;
          case 2:
            Serial3.println("Verrà visualizzata la distanza captata dal Sonar Frontale Sinistro(sinistra del robot). Muovere una mano o un oggetto davanti al sonar per verificarne il corretto funzionamento.");
            break;
          case 3:
            Serial3.println("Verrà visualizzata la distanza captata dal Sonar Posteriore. Muovere una mano o un oggetto davanti al sonar per verificarne il corretto funzionamento.");
            break;
        }
        Serial3.println("Premere START per iniziare, END per terminare il test");
      }
      else {
        numSonarTest = 0;
        testState = tests_descr;
        testType = no_one;
        Serial3.println("Test dei Sonar terminato.");
      }
    }
  }
}

void headCapacitiveTest() {
  printCapacitiveHeadPressed();
  if (Serial3.available()) {
    b = Serial3.read();
    if (b == '1') {
      CapacitivesUpdate(noOne);
      testState = tests_descr;
      testType = no_one;
      Serial3.println("Test dei Capacitivi Testa terminato.");
    }
  }
}

void bodyCapacitiveTest() {
  printTouches();
  if (Serial3.available()) {
    b = Serial3.read();
    if (b == '1') {
      CapacitivesUpdate(noOne);
      testState = tests_descr;
      testType = no_one;
      Serial3.println("Test Capacitivi Corpo terminato.");
    }
  }
}

void printTouches(){
  switch(touch_type){
    case nothing:break;
    case hugT: Serial3.print("Abbraccio durata ");Serial3.println(abbraccioN);
    case patT: printPat();break;
    case hitT: printHit();break;
  }
}

void printPat(){
  switch(touched){
    case noWhere: break;
    case leftT:   Serial3.println("Left Pat!");break;
    case rightT:  Serial3.println("Right Pat!");break;
    case frontT:  Serial3.println("Front Pat!");break;
  }
}

void printHit(){
  switch(touched){
    case noWhere: break;
    case leftT:   Serial3.println("Left Hit!");break;
    case rightT:  Serial3.println("Right Hit!");break;
    case frontT:  Serial3.println("Front Hit!");break;
  } 
}
//
//void bodyCapacitiveTest() {
//  printCapacitiveBodyValueAndStatus();
//  if (Serial3.available()) {
//    b = Serial3.read();
//    if (b == '1') {
//      if (numBodyTest < N_BODY_SENSORS - 1) {
//        numBodyTest++;
//        testState = start_test;
//        switch (numBodyTest) {
//          case 1:
//            Serial3.println("Verranno visualizzati il valore e lo stato captato dal capacitivo posto nella parte posteriore destra(del robot). Toccare il capacitivo più o meno forte per verificarne il corretto funzionamento.");
//            break;
//          case 2:
//            Serial3.println("Verranno visualizzati il valore e lo stato captato dal capacitivo posto nella parte anteriore del robot. Toccare il capacitivo più o meno forte per verificarne il corretto funzionamento.");
//            break;
//        }
//        Serial3.println("Premere START per iniziare, END per terminare il test");
//      }
//      else {
//        numBodyTest = 0;
//        testState = tests_descr;
//        testType = no_one;
//        Serial3.println("Test dei Capacitivi Corpo terminato.");
//      }
//    }
//  }
//}

void fotoresistorTest() {
  printFotoresValueAndState();
  if (Serial3.available()) {
    b = Serial3.read();
    if (b == '2') {
      testState = tests_descr;
      testType = no_one;
      Serial3.println("Test Fotoresistenza terminato.");
    }
  }
}

void speakerTest() {
  if (millis() - startPlayTime >= 5000) {
    Serial3.println("L'audio è stato riprodotto?");
    Serial3.println("Premere (0) per ripetere, (1) per uscire");
    testState = start_test;
  }
}

void microTest() {
  Serial3.println(microLowpassFilter.output());
  if (Serial3.available()) {
    b = Serial3.read();
    if (b == '2') {
      testState = tests_descr;
      testType = no_one;
      Serial3.println("Test Microfono terminato.");
    }
  }
}

void printFotoresValueAndState() {
  Serial3.print(fotores_value); Serial3.print("   ");
  switch (fotoresistorState) {
    case 0: Serial3.println("Libero"); break;
    case 1: Serial3.println("Lenzuolo"); break;
    case 2: Serial3.println("Coperto"); break;
  }
}
//
//void printCapacitiveBodyValueAndStatus() {
//  Serial3.print(bodySensorValue[numBodyTest]); Serial3.print("  ");
//  switch (capacitiveState[numBodyTest]) {
//    case 0: Serial3.println("Nessun tocco"); break;
//    case 1: Serial3.println("Tocco Leggero"); break;
//    case 2: Serial3.println("Tocco Forte"); break;
//  }
//
//}

void printSonarDetail() {
  switch (numSonarTest) {
    case 0: Serial3.println(f_front); break;
    case 1: Serial3.println(f_right); break; //Robot's right
    case 2: Serial3.println(f_left); break; //Robot's left
    case 3: Serial3.println(f_back); break;
  }
}

void printCapacitiveHeadPressed() {
  switch (pressedButton) {
    case -1: Serial3.println("Nessuno"); break;
    case 0: Serial3.println("Bottone 0"); break;
    case 1: Serial3.println("Bottone 1"); break;
    case 2: Serial3.println("Bottone 2"); break;
    case 3: Serial3.println("Bottone 3"); break;
  }
}
