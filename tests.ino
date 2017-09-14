void hardwareTests() {
  switch (testState) {
    case no_test:                   break;
    case choose_test:               chooseTest(); break;
    case wait_choose_test:          waitChooseTest(); break;
    case sonar_test_pre:            sonarTestPre(); break;
    case sonar_test_wait:           sonarTestWait(); break;
    case sonar_test:                sonarTest(); break;
    case head_capacitive_test_pre:  headCapacitiveTestPre(); break;
    case head_capacitive_test_wait: headCapacitiveTestWait(); break;
    case head_capacitive_test:      headCapacitiveTest(); break;
    case body_capacitive_test_pre:  bodyCapacitiveTestPre(); break;
    case body_capacitive_test_wait: bodyCapacitiveTestWait(); break;
    case body_capacitive_test:      bodyCapacitiveTest(); break;
    case fotores_test_pre:    fotoresTestPre(); break;
    case fotores_test_wait:   fotoresTestWait(); break;
    case fotores_test:        fotoresTest(); break;
    case speaker_test_pre:          speakerTestPre(); break;
    case speaker_test_wait:         speakerTestWait(); break;
    case speaker_test:              speakerTest(); break;
    case micro_test_pre:            microTestPre(); break;
    case micro_test_wait:           microTestWait(); break;
    case micro_test:                microTest(); break;
  }
}

void chooseTest() {
  Serial3.println("##########SESSIONE TEST##########");
  Serial3.println("Scegliere il test da eseguire:");
  Serial3.println("0 - test Sonar");
  Serial3.println("1 - test Capacitivi Testa");
  Serial3.println("2 - test Capacitivi Corpo");
  Serial3.println("3 - test Fotoresistenza");
  Serial3.println("4 - test Casse");
  Serial3.println("5 - test Microfono");
  testState = wait_choose_test;
}

void waitChooseTest() {
  if (Serial3.available()) {
    b = Serial3.read();
    switch (b) {
      case '0':
        testState = sonar_test_pre;
        break;

      case '1':
        testState = head_capacitive_test_pre;
        break;

      case '2':
        testState = body_capacitive_test_pre;
        break;

      case '3':
        testState = fotores_test_pre;
        break;

      case '4':
        testState = speaker_test_pre;
        break;

      case '5':
        testState = micro_test_pre;
        break;

      default:
        // default code (should never run)
        break;
    }

  }
}

void sonarTestPre() {
  Serial3.println("Verranno visualizzate le distanze captate dai sonar. Muovere una mano davanti ad un sonar alla volta per verificarne il corretto funzionamento.");
  Serial3.println("Premere START per iniziare, END per terminare il test");
  testState = sonar_test_wait;
}

void sonarTestWait() {
  if (Serial3.available()) {

    b = Serial3.read();
    if (b == 0) testState = sonar_test;
  }
}

void sonarTest() {

}
void headCapacitiveTestPre() {
  Serial3.println("Verrà visualizzato il numero[0-3] del capacitivo toccato. Toccare uno alla volta i capacitivi posti sulla testa del robot per verificarne il corretto funzionamento.");
  Serial3.println("Premere START per iniziare, END per terminare il test");
  testState = head_capacitive_test_wait;
}
void headCapacitiveTestWait() {
  if (Serial3.available()) {

    b = Serial3.read();
    if (b == 0) testState = head_capacitive_test;
  }
}
void headCapacitiveTest() {

}

void bodyCapacitiveTestPre() {
  Serial3.println("Verranno visualizzati i valori e lo stato captati dai tre capacitivi posti sulla testa. Toccare un capacitivo alla volta per verificarne il corretto funzionamento.");
  Serial3.println("Premere START per iniziare, END per terminare il test");
  testState = head_capacitive_test_wait;
}

void bodyCapacitiveTestWait() {
  if (Serial3.available()) {
    b = Serial3.read();
    if (b == 0) testState = body_capacitive_test;
  }
}

void bodyCapacitiveTest() {

}
void fotoresTestPre() {
  Serial3.println("Verrà visualizzato il valore captato dalla fotoresistenza posta in prossimità dei led ed il suo stato. Coprire il robot o la fotoresistenza con differenti materiali per verificarne il corretto funzionamento.");
  Serial3.println("Premere START per iniziare, END per terminare il test");
  testState = fotores_test_wait;

}
void fotoresTestWait() {
  if (Serial3.available()) {
    b = Serial3.read();
    if (b == 0) testState = fotores_test;
  }
}
void fotoresTest() {

}
void speakerTestPre() {
  Serial3.println("Verrà riprodotto un audio per verificare il corretto funzionamento delle casse.");
  Serial3.println("Premere START per avviare la riproduzione dell'audio");
  testState = speaker_test_wait;
}
void speakerTestWait() {
  if (Serial3.available()) {
    b = Serial3.read();
    if (b == 0) testState = speaker_test;
  }
}
void speakerTest() {

}
void microTestPre() {
  Serial3.println("Verrà visualizzato il valore captato dal microfono. Emettere suoni di diversa intensità per verificarne il corretto funzionamento.");
  Serial3.println("Premere START per iniziare, END per terminare il test");
  testState = micro_test_wait;

}
void microTestWait() {
  if (Serial3.available()) {
    b = Serial3.read();
    if (b == 0) testState = micro_test;
  }
}
void microTest() {

}

